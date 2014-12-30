/*******************************************************************************
 * The following code is derived, directly or indirectly, from the SystemC
 * source code Copyright (c) 1996-2010 by all Contributors.
 * All Rights reserved.
 * 
 * The contents of this file are subject to the restrictions and limitations
 * set forth in the SystemC Open Source License Version 2.2.0 (the "License");
 * One may not use this file except in compliance with such restrictions and
 * limitations.  One may obtain instructions on how to receive a copy of the
 * License at http://www.systemc.org/.  Software distributed by Contributors
 * under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 * ANY KIND, either express or implied. See the License for the specific
 * language governing rights and limitations under the License.
 * *******************************************************************************/

/**
 * @file     configurator.h
 * @brief    This header implements the functionality of the configurator
 * @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * @date     12th July, 2011 (Tuesday)
 */

#ifndef EXAMPLES_EX15_INTER_PARAMETER_VALUE_CONSTRAINTS_EX15_CONFIGURATOR_H_
#define EXAMPLES_EX15_INTER_PARAMETER_VALUE_CONSTRAINTS_EX15_CONFIGURATOR_H_

#include <cci>
#include <cassert>
#include <string>

#include "xreport.hpp"

/// This module gets the references to all the cci_parameters iwthin the model
/// and writes (valid) values to them
SC_MODULE(ex15_configurator) {
 public:
  SC_CTOR(ex15_configurator) {
    // Get handle of the broker responsible for the class/module
    myCfgrBrokerIF = &cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));

    // Report if handle returned is NULL
    assert(myCfgrBrokerIF != NULL && "Configuration Broker handle is NULL");

    /// Hierarchical names for the cci_parameters of the owner modules
    std::string cfgr_param_str1 = "processor.addr_lines_mod.curr_addr_lines";
    std::string cfgr_param_str2 = "processor.memory_block.mem_size";

    /// Check for the existence of 'curr_addr_lines' cci_parameter
    /// of ADDRESS_LINES_REGISTER
    if (myCfgrBrokerIF->param_exists(cfgr_param_str1)) {
      addr_lines_base_ptr = myCfgrBrokerIF->get_param(cfgr_param_str1);

      assert(addr_lines_base_ptr != NULL
             && "Handle of 'curr_addr_lines' parameter returned is NULL");
    } else {
      XREPORT("[CFGR C_TOR] : Parameter " << cfgr_param_str1
              << "\tdoesn't exists");
    }

    /// Check for the existence of 'mem_size' cci_parameter of MEMORY_STACK
    if (myCfgrBrokerIF->param_exists(cfgr_param_str2)) {
      mem_size_base_ptr = myCfgrBrokerIF->get_param(cfgr_param_str2);

      assert(mem_size_base_ptr != NULL
             && "Handle of 'mem_size' parameter returned is NULL");
    } else {
      XREPORT("[CFGR C_TOR] : Parameter " << cfgr_param_str2
              << "\tdoesn't exists");
    }

    /// Registering SC_THREAD with the SystemC kernel
    SC_THREAD(run_cfgr);
  }

  void run_cfgr(void) {
    while (1) {
      XREPORT("@ " << sc_core::sc_time_stamp());

      XREPORT("[CFGR] : Changing the 'mem_size' to 640");
      mem_size_base_ptr->json_deserialize("640");

      wait(5.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());

      XREPORT("[CFGR] : Modify the 'curr_addr_lines' to 10");
      addr_lines_base_ptr->json_deserialize("10");

      wait(5.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());

      XREPORT("[CFGR] : Changing the 'mem_size' to 800");
      mem_size_base_ptr->json_deserialize("800");

      wait(5.0, sc_core::SC_NS);
    }
  }

 private:
  /// Declaring a CCI configuration broker interface instance
  cci::cnf::cci_cnf_broker_if* myCfgrBrokerIF;

  /// CCI base parameters
  cci::cnf::cci_base_param* addr_lines_base_ptr;
  cci::cnf::cci_base_param* mem_size_base_ptr;
};
/// ex15_configurator

#endif  // EXAMPLES_EX15_INTER_PARAMETER_VALUE_CONSTRAINTS_EX15_CONFIGURATOR_H_
