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
 *  @file     configurator.h
 *  @brief    This header implements the functionality of the configurator
 *  @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date     12th July, 2011 (Tuesday)
 */

#ifndef EXAMPLES_EX15_INTER_PARAMETER_VALUE_CONSTRAINTS_EX15_CONFIGURATOR_H_
#define EXAMPLES_EX15_INTER_PARAMETER_VALUE_CONSTRAINTS_EX15_CONFIGURATOR_H_

#include <cci_configuration>
#include <string>

#include "xreport.hpp"

/**
 *  @class  ex15_configurator
 *  @brief  This module gets the references to all the cci_parameters iwthin the
 *          model and writes (valid) values to them
 */
SC_MODULE(ex15_configurator) {
 public:
  /**
   *  @fn     ex15_configurator
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex15_configurator):
      m_broker(cci::cci_get_broker())
  {
    // Hierarchical names for the cci_parameters of the owner modules
    std::string cfgr_param_str1 = "processor.addr_lines_mod.curr_addr_lines";
    std::string cfgr_param_str2 = "processor.memory_block.mem_size";

    // Check for the existence of 'curr_addr_lines' cci_parameter
    // of ADDRESS_LINES_REGISTER
    child_base_param_handle = m_broker.get_param_handle(cfgr_param_str1); 
    if (!child_base_param_handle.is_valid()) {
      XREPORT("[CFGR C_TOR] : Parameter " << cfgr_param_str1
              << " doesn't exists");
    }

    // Check for the existence of 'mem_size' cci_parameter of MEMORY_STACK
    mem_size_base_handle = m_broker.get_param_handle(cfgr_param_str2);
    if (!mem_size_base_handle.is_valid()) {
      XREPORT("[CFGR C_TOR] : Parameter " << cfgr_param_str2
              << " doesn't exists");
    }

    // Registering SC_THREAD with the SystemC kernel
    SC_THREAD(run_cfgr);
  }

  /**
   *  @fn     void run_cfgr(void)
   *  @brief  Function to configure the parameters
   *  @return void
   */
  void run_cfgr(void) {
    while (1) {
      XREPORT("@ " << sc_core::sc_time_stamp());

      XREPORT("[CFGR] : Changing the 'mem_size' to 640");
      mem_size_base_handle.set_cci_value(cci::cci_value(640));

      wait(5.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());

      XREPORT("[CFGR] : Modify the 'curr_addr_lines' to 10");
      child_base_param_handle.set_cci_value(cci::cci_value(10));

      wait(5.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());

      XREPORT("[CFGR] : Changing the 'mem_size' to 800");
      mem_size_base_handle.set_cci_value(cci::cci_value(800));

      wait(5.0, sc_core::SC_NS);
    }
  }

 private:
  cci::cci_broker_handle m_broker;  ///< Declaring a CCI configuration broker handle

  /// CCI base parameters
  cci::cci_param_handle child_base_param_handle;  ///< Handle to the base of the address lines
  cci::cci_param_handle mem_size_base_handle;  ///< Handle to the base mem size
};
// ex15_configurator

#endif  // EXAMPLES_EX15_INTER_PARAMETER_VALUE_CONSTRAINTS_EX15_CONFIGURATOR_H_
