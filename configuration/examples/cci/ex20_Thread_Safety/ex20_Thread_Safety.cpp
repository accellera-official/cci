/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 ****************************************************************************/

/**
 *  @file   ex20_Thread_Safety.cpp
 *  @brief  Demonstrates CCI thread-safety issues when accessing the broker
 *          from multiple threads concurrently.
 *
 *  This test creates a broker with preset values, then spawns worker threads
 *  that read presets and param handles concurrently. Without thread-safety
 *  mechanisms in the broker, this produces data races (detectable with TSan).
 *
 *  It also demonstrates using a custom originator to access the broker
 *  from non-SystemC threads, avoiding the sc_get_current_object() issue.
 */

#include <cci_configuration>
#include <systemc>

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

/* Simple module that creates a CCI param */
SC_MODULE(my_module) {
    cci::cci_param<int> p_value;

    SC_CTOR(my_module)
        : p_value("value", 42, "a test parameter")
    {
        SC_REPORT_INFO("my_module", "Constructed");
    }
};

/* Worker thread function: reads broker state concurrently.
 * Uses a pre-created broker handle with a custom originator
 * (not dependent on sc_get_current_object). */
static void worker_read(cci::cci_broker_handle broker, int id,
                        std::atomic<int>& errors) {
    try {
        for (int i = 0; i < 100; i++) {
            /* Read preset values — concurrent with other readers and writers */
            auto val = broker.get_preset_cci_value("mod.value");

            /* Get param handles — concurrent access to param registry */
            auto handles = broker.get_param_handles();

            /* Get unconsumed presets — iterates internal maps */
            auto presets = broker.get_unconsumed_preset_values();
        }
    } catch (const std::exception& e) {
        std::cerr << "Thread " << id << " exception: " << e.what() << "\n";
        errors++;
    }
}

/* Worker thread function: writes preset values concurrently */
static void worker_write(cci::cci_broker_handle broker, int id,
                         std::atomic<int>& errors) {
    try {
        for (int i = 0; i < 100; i++) {
            /* Write preset values — concurrent with readers and other writers */
            broker.set_preset_cci_value(
                "dynamic_param_" + std::to_string(id) + "_" + std::to_string(i),
                cci::cci_value(i));

            /* Also read while writing */
            auto val = broker.get_preset_cci_value("mod.value");
        }
    } catch (const std::exception& e) {
        std::cerr << "Thread " << id << " exception: " << e.what() << "\n";
        errors++;
    }
}

int sc_main(int argc, char *argv[]) {
    /* Create broker and register it */
    cci_utils::consuming_broker global_broker("Global Broker");
    cci::cci_register_broker(global_broker);

    /* Create a custom originator for non-SystemC threads */
    cci::cci_originator worker_orig("worker_thread");
    auto worker_broker = global_broker.create_broker_handle(worker_orig);

    /* Set some initial preset values */
    cci::cci_originator main_orig("sc_main");
    auto main_broker = global_broker.create_broker_handle(main_orig);
    main_broker.set_preset_cci_value("mod.value", cci::cci_value(100));
    main_broker.set_preset_cci_value("mod.other", cci::cci_value(200));

    /* Create a module (consumes the preset) */
    my_module mod("mod");

    SC_REPORT_INFO("sc_main", "Starting concurrent access test...");

    std::atomic<int> errors{0};
    const int NUM_READERS = 4;
    const int NUM_WRITERS = 2;

    /* Spawn reader and writer threads — all using the worker_broker handle
     * with the custom originator (no sc_get_current_object dependency) */
    std::vector<std::thread> threads;

    for (int i = 0; i < NUM_READERS; i++) {
        threads.emplace_back(worker_read, worker_broker, i, std::ref(errors));
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        threads.emplace_back(worker_write, worker_broker,
                             NUM_READERS + i, std::ref(errors));
    }

    /* Join all threads */
    for (auto& t : threads) {
        t.join();
    }

    if (errors == 0) {
        SC_REPORT_INFO("sc_main",
            "All threads completed without exceptions. "
            "NOTE: absence of crashes does NOT prove thread safety. "
            "Run with ThreadSanitizer (TSan) to detect data races.");
    } else {
        SC_REPORT_ERROR("sc_main",
            ("Threads reported " + std::to_string(errors.load()) +
             " error(s)").c_str());
    }

    /* Also demonstrate: reading params from a non-SystemC thread works
     * with the custom originator — no sc_get_current_object needed */
    std::thread param_reader([&worker_broker]() {
        auto h = worker_broker.get_param_handle("mod.value");
        if (h.is_valid()) {
            int val = h.get_cci_value().get_int();
            std::cout << "Worker thread read mod.value = " << val << "\n";
        }
    });
    param_reader.join();

    /* Performance measurement: read-heavy workload */
    SC_REPORT_INFO("sc_main", "Starting performance measurement...");

    const int ITERATIONS = 100000;
    const int NUM_THREADS = 4;

    /* Single-threaded baseline */
    auto t0 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; i++) {
        auto h = main_broker.get_param_handle("mod.value");
        if (h.is_valid()) h.get_cci_value();
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    auto single_us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

    /* Multi-threaded: same total work split across threads */
    auto t2 = std::chrono::high_resolution_clock::now();
    {
        std::vector<std::thread> perf_threads;
        for (int t = 0; t < NUM_THREADS; t++) {
            perf_threads.emplace_back([&worker_broker, ITERATIONS, NUM_THREADS]() {
                for (int i = 0; i < ITERATIONS / NUM_THREADS; i++) {
                    auto h = worker_broker.get_param_handle("mod.value");
                    if (h.is_valid()) h.get_cci_value();
                }
            });
        }
        for (auto& t : perf_threads) t.join();
    }
    auto t3 = std::chrono::high_resolution_clock::now();
    auto multi_us = std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count();

    std::cout << "Performance: " << ITERATIONS << " get_param_handle + get_cci_value\n";
    std::cout << "  Single-threaded: " << single_us << " us ("
              << (single_us * 1000 / ITERATIONS) << " ns/op)\n";
    std::cout << "  Multi-threaded (" << NUM_THREADS << " threads): "
              << multi_us << " us ("
              << (multi_us * 1000 / ITERATIONS) << " ns/op)\n";
    std::cout << "  Overhead: " << (multi_us * 100 / std::max(single_us, (decltype(single_us))1) - 100)
              << "%\n";

    /* Cached handle benchmark — more realistic (handle created once, reused) */
    auto t4 = std::chrono::high_resolution_clock::now();
    {
        auto h = main_broker.get_param_handle("mod.value");
        for (int i = 0; i < ITERATIONS; i++) {
            if (h.is_valid()) h.get_cci_value();
        }
    }
    auto t5 = std::chrono::high_resolution_clock::now();
    auto cached_single_us = std::chrono::duration_cast<std::chrono::microseconds>(t5 - t4).count();

    auto t6 = std::chrono::high_resolution_clock::now();
    {
        std::vector<std::thread> perf_threads;
        for (int t = 0; t < NUM_THREADS; t++) {
            perf_threads.emplace_back([&worker_broker, ITERATIONS, NUM_THREADS]() {
                auto h = worker_broker.get_param_handle("mod.value");
                for (int i = 0; i < ITERATIONS / NUM_THREADS; i++) {
                    if (h.is_valid()) h.get_cci_value();
                }
            });
        }
        for (auto& t : perf_threads) t.join();
    }
    auto t7 = std::chrono::high_resolution_clock::now();
    auto cached_multi_us = std::chrono::duration_cast<std::chrono::microseconds>(t7 - t6).count();

    std::cout << "\nCached handle: " << ITERATIONS << " get_cci_value (handle reused)\n";
    std::cout << "  Single-threaded: " << cached_single_us << " us ("
              << (cached_single_us * 1000 / ITERATIONS) << " ns/op)\n";
    std::cout << "  Multi-threaded (" << NUM_THREADS << " threads): "
              << cached_multi_us << " us ("
              << (cached_multi_us * 1000 / ITERATIONS) << " ns/op)\n";
    std::cout << "  Overhead: " << (cached_multi_us * 100 / std::max(cached_single_us, (decltype(cached_single_us))1) - 100)
              << "%\n";

    SC_REPORT_INFO("sc_main", "Test complete.");
    return (errors == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
