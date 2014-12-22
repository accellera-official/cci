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
 * @file     top_module.h
 * @brief    A brief on TOP_MODULE.
 *           This header declares and defines the top module which instantiates
 *           the two owner modules 
 * @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * @date     9th June, 2011 (Thursday)
 */

#ifndef EXAMPLES_EX12_INDIRECTLY_ASSOCIATING_PARAMETERS_EX12_TOP_MODULE_H_
#define EXAMPLES_EX12_INDIRECTLY_ASSOCIATING_PARAMETERS_EX12_TOP_MODULE_H_

#include <cci>
#include <cassert>
#include <vector>
#include <sstream>
#include <string>

#include "ex12_parameter_owner.h"
#include "ex12_param_value_sync_with_cf.h"
#include "xreport.hpp"

/// This module instantiates the rest of the platform (configurator and owners)
SC_MODULE(ex12_top_module) {
 public:
  // Pointers to the owner module
  ex12_parameter_owner* param_owner1;  //!< Owner module #1
  ex12_parameter_owner* param_owner2;  //!< Owner module #2

  // Create an instance of the 'param_value_sync_with_cf' class
  ex12_param_value_sync_with_cf *param_sync;  //!< 'param_value_sync' instance

  SC_CTOR(ex12_top_module) {
    /// Get handle of the broker responsible for the class/module
    myTopModBrokerIF =
        &cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));

    // Strings to store the names of the owner's parameters
    std::string str1, str2;

    str1 = "clk_freq_Hz";
    str2 = "clock_speed_KHz";

    param_owner1 = new ex12_parameter_owner("param_owner1", str1, 1);
    param_owner2 = new ex12_parameter_owner("param_owner2", str2, 2);

    // Report if handle returned is NULL
    assert(myTopModBrokerIF != NULL && "Configuration Broker handle is NULL");

    /// Check for existence of the owner cci_parameter using name-based look up
    /// access and then assign their reference to respective cci_base_param
    std::string param1_str = "top_mod.param_owner1.clk_freq_Hz";
    std::string param2_str = "top_mod.param_owner2.clock_speed_KHz";

    if (myTopModBrokerIF->param_exists(param1_str)) {
      cci::cnf::cci_base_param *temp = myTopModBrokerIF->get_param(param1_str);
      selectedBaseParamList.push_back(temp);

      XREPORT("[TOP_MODULE C_TOR] : Parameter Name : "
              << temp->get_name() << "\tParameter Value : "
              << temp->json_serialize());
    } else {
      XREPORT("[TOP_MODULE C_TOR] : Parameter Name : " << param1_str
              << "\tnot found.");
    }

    /// Check for existence of the owner cci_parameter using name-based look up
    /// access and then assign their reference to respective cci_base_param
    if (myTopModBrokerIF->param_exists(param2_str)) {
      cci::cnf::cci_base_param *temp = myTopModBrokerIF->get_param(param2_str);
      selectedBaseParamList.push_back(temp);

      XREPORT("[TOP_MODULE C_TOR] : Parameter Name : "
              << temp->get_name() << "\tParameter Value : "
              << temp->json_serialize());
    } else {
      XREPORT("[TOP_MODULE C_TOR] : Parameter Name : " << param2_str
              << "\tnot found.");
    }

    /// Pass the selected base parameters list to the 'param_value_sync_with_cf'
    /// class synchronization related activity
    param_sync = new ex12_param_value_sync_with_cf("param_value_sync_with_cf",
                                                   selectedBaseParamList);
  }

 private:
  // Declaring a CCI configuration broker interface instance
  cci::cnf::cci_cnf_broker_if* myTopModBrokerIF;

  // Selected cci_base_parameter list (selection done by top_module)
  std::vector<cci::cnf::cci_base_param*> selectedBaseParamList;
};
// ex12_top_module

#endif  // EXAMPLES_EX12_INDIRECTLY_ASSOCIATING_PARAMETERS_EX12_TOP_MODULE_H_
