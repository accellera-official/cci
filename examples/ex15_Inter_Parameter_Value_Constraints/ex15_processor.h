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
 *  @file      processor.h
 *  @brief     This header implements 'processor' module validates the consistency of the system
 *  @author    P V S Phaneendra, CircuitSutra Technologies, Inc.  <pvs@circuitsutra.com>
 *  @date      4th August, 2011 (Thursday)
 */

#ifndef EXAMPLES_EX15_INTER_PARAMETER_VALUE_CONSTRAINTS_EX15_PROCESSOR_H_
#define EXAMPLES_EX15_INTER_PARAMETER_VALUE_CONSTRAINTS_EX15_PROCESSOR_H_

#include <math.h>
#include <cci_configuration>
#include <string>

#include "ex15_address_lines_register.h"
#include "ex15_memory_block.h"
#include "xreport.hpp"

/**
 *  @class  ex15_processor
 *  @brief  This module instantiates a processor module which register's callbacks
 *          on references of the cci_parameter of the two register modules and
 *          the memory stack module. It also does a few checks and comparisons
 *          in order to validate whether or not the system configuration meets
 *          the requirements
 */
SC_MODULE(ex15_processor) {
 public:
  /**
   *  @fn     ex15_processor
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex15_processor)
      : addr_lines_module("addr_lines_mod"),
        memory_block_module("memory_block"),
        m_broker(cci::cci_get_broker()),
        addr_lines_base_handle(cci::cci_originator(*this)),
        mem_size_base_handle(cci::cci_originator(*this))
  {
    // Get handle of the 'no_of_addr_lines' cci-parameter of
    // 'address_lines_register'
    std::string param_path(name());
    param_path.append(".addr_lines_mod.curr_addr_lines");

    addr_lines_base_handle = m_broker.get_param_handle(param_path); 
    if (!addr_lines_base_handle.is_valid()) {
      XREPORT("[PROCESSOR C_TOR] : Parameter " << param_path
              << "\tdoesn't not exists");
    }

    // Get handle of the 'mem_size' cci-parameter of 'memory_block'
    param_path = name();
    param_path.append(".memory_block.mem_size");

    mem_size_base_handle = m_broker.get_param_handle(param_path);
    if (!mem_size_base_handle.is_valid()) {
      XREPORT("[PROCESSOR C_TOR] : Parameter " << param_path
              << "\tdoesn't not exists");
    }

    // Checks for the condition whether the default total number of the
    // address lines can address the default address location
    total_addr_lines =
            atoi(addr_lines_base_handle.get_cci_value().to_json().c_str());
    mem_block_size =
            atoi(mem_size_base_handle.get_cci_value().to_json().c_str());
    TestCondition(total_addr_lines, mem_block_size);

    // Registering 'POST_WRITE' callbacks on the cci-parameters of the
    // two register modules
    addr_lines_post_wr_cb = addr_lines_base_handle.register_post_write_callback(
        sc_bind(&ex15_processor::addr_lines_post_wr_cb_func,
        this, sc_unnamed::_1, mem_size_base_handle));

    mem_block_post_wr_cb = mem_size_base_handle.register_post_write_callback(
        sc_bind(&ex15_processor::mem_block_post_wr_cb_func,
        this, sc_unnamed::_1, addr_lines_base_handle));
  }

  /**
   *  @fn     void typed_post_write_callback(const cci::cci_param_write_event<int> & ev)
   *  @brief  Post Callback function for address line
   *  @return void
   */
  void addr_lines_post_wr_cb_func(const cci::cci_param_write_event<> & ev ,
                                   cci::cci_param_handle mem_size_handle)
  {
    XREPORT("[PROCESSOR addr_lines_post_wr_cb] : Parameter Name : "
            << ev.param_handle.get_name() << "\tParameter Value : "
            << ev.new_value);

    XREPORT("[PROCESSOR addr_lines_post_wr_cb] : Parameter Name : "
            << mem_size_handle.get_name() << "\tParameter Value : "
            << mem_size_handle.get_cci_value());

    total_addr_lines = atoi(ev.new_value.to_json().c_str());
    mem_block_size = atoi(mem_size_handle.get_cci_value().to_json().c_str());

    // Test condition : X < 2^n - 1
    TestCondition(total_addr_lines, mem_block_size);
  }

  /**
   *  @fn     void typed_post_write_callback(const cci::cci_param_write_event<int> & ev)
   *  @brief  Post Callback function for memory block
   *  @return void
   */
  void mem_block_post_wr_cb_func(const cci::cci_param_write_event<> & ev ,
                                   cci::cci_param_handle addr_lines_handle)
  {
    XREPORT("[PROCESSOR mem_block_post_wr_cb] : Parameter Name : "
            << ev.param_handle.get_name() << "\tParameter Value : "
            << ev.new_value);

    XREPORT("[PROCESSOR mem_block_post_wr_cb] : Parameter Name : "
            << addr_lines_handle.get_name() << "\tParameter Value : "
            << addr_lines_handle.get_cci_value());

    mem_block_size = atoi(ev.new_value.to_json().c_str());
    total_addr_lines = atoi(addr_lines_handle.get_cci_value().to_json().c_str());

    TestCondition(total_addr_lines, mem_block_size);
  }

  /**
   *  @fn     void TestCondition(int lines, int memory_size)
   *  @brief  The function validates the consistency of the system base on the two input fed to it
   *  @param  lines The number of address lines
   *  @param  memory_size The size of the memory
   *  @return void
   */
  void TestCondition(int lines, int memory_size) {
    static int check = 0;

    if (memory_size < (1 << lines) - 1) {
      if (check == 0) {
        XREPORT("[PROCESSOR fn] : User may proceed with the present"
                " configuration");
      } else {
        XREPORT("[PROCESSOR fn] : The number of address lines can now address"
                " the current memory size");
      }

      check = 0;
    } else {
      XREPORT("[PROCESSOR fn] : The address lines cannot address the current"
              " memory size");

      check = 1;
    }
  }

 private:
  // Declare parameter_owner instance
  ex15_address_lines_register addr_lines_module;  ///< Declare address line register
  ex15_memory_block memory_block_module;  ///< Memory block module

  cci::cci_broker_handle m_broker;  ///< CCI configuration broker handle

  int total_addr_lines; ///< The total number of address lines
  int mem_block_size; ///< The size of the memory block

  // CCI Base parameter pointer
  cci::cci_param_handle addr_lines_base_handle;  ///< Handle to the address lines
  cci::cci_param_handle mem_size_base_handle;  ///< Handle to the base of the memory size

  // Callback Adaptor Objects
  cci::cci_callback_untyped_handle addr_lines_post_wr_cb; ///< Address lines callback adapter object
  cci::cci_callback_untyped_handle mem_block_post_wr_cb;  ///< Memory block callback adapter object
};
// ex15_processor

#endif  // EXAMPLES_EX15_INTER_PARAMETER_VALUE_CONSTRAINTS_EX15_PROCESSOR_H_
