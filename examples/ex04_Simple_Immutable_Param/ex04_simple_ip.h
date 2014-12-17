/*******************************************************************************
 The following code is derived, directly or indirectly, from the SystemC
 source code Copyright (c) 1996-2010 by all Contributors.
 All Rights reserved.

 The contents of this file are subject to the restrictions and limitations
 set forth in the SystemC Open Source License Version 3.0 (the "License");
 You may not use this file except in compliance with such restrictions and
 limitations. You may obtain instructions on how to receive a copy of the
 License at http://www.systemc.org/. Software distributed by Contributors
 under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 ANY KIND, either express or implied. See the License for the specific
 language governing rights and limitations under the License.
 ******************************************************************************/

/**
 *  @file   simple_ip.h
 *  @brief  A Simple IP that holds 2 immutable params
 *  @author Vishal Goel, TI
 */

#ifndef EXAMPLES_EX04_SIMPLE_IMMUTABLE_PARAM_EX04_SIMPLE_IP_H_
#define EXAMPLES_EX04_SIMPLE_IMMUTABLE_PARAM_EX04_SIMPLE_IP_H_

#include <cci>
#include "xreport.hpp"

/// The SimpleIP owns CCI immutable params
SC_MODULE(ex04_simple_ip) {
 public:
  /// Ctor
  SC_CTOR(ex04_simple_ip)
      : param_1("param_1", 1),
        param_2("param_2", 2) {
    SC_THREAD(execute);
    XREPORT("@Ctor: Default vale specified for " << param_1.get_name()
            << " is 1");
    XREPORT("@Ctor: Default vale specified for " << param_2.get_name()
            << " is 2");

    expect("Ctor: immutable param:", "param_1", static_cast<int>(param_1), 100);
    expect("Ctor: immutable param:", "param_2", static_cast<int>(param_2), 2);
  }

  //! @note remove this function after the CCI implementation is in place
  void expect(const char* phase, const char *key, const int val,
              const int exp) {
    if (val != exp) {
      XREPORT_PLAIN("Warning: " << phase << key << " Expected to have value = "
                    << exp << " but has " << val << ". TBD?");
    } else {
      XREPORT_PLAIN("Info: " << phase << key << " = " << val << " (expected)");
    }
  }

  /// Main execution block
  void execute() {
    wait(20, sc_core::SC_NS);

    // Display current value of bufsiz
    expect("@Run: ", "param_1", static_cast<int>(param_1), 100);
    expect("@Run: ", "param_2", static_cast<int>(param_2), 2);

    // Attempt to set new value (10) to param_1
    try {
      XREPORT("@Run: Assign new value (10) to " << param_1.get_name());
      param_1.json_deserialize("10");
    } catch (std::exception &x) {
      XREPORT_WARNING(x.what());
    }
    expect("@Run: ", "param_1", static_cast<int>(param_1), 100);

    // Attempt to set new value (20) to param_2
    try {
      XREPORT("@Run: Assign new value (20) to " << param_2.get_name());
      param_2.json_deserialize("20");
    } catch (std::exception &x) {
      XREPORT_WARNING(x.what());
    }
    expect("@Run: ", "param_2", static_cast<int>(param_2), 2);
  }

 private:
  /// CCI immutable param to hold buffer size
  cci::cnf::cci_param<int, cci::cnf::immutable_param> param_1;
  cci::cnf::cci_param<int, cci::cnf::immutable_param> param_2;
};
// ex04_simple_ip

#endif  // EXAMPLES_EX04_SIMPLE_IMMUTABLE_PARAM_EX04_SIMPLE_IP_H_
