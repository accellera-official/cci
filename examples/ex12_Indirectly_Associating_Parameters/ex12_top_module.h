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
 *  @file     top_module.h
 *  @brief    A brief on TOP_MODULE.
 *            This header declares and defines the top module which instantiates
 *            the two owner modules
 *  @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date     9th June, 2011 (Thursday)
 */

#ifndef EXAMPLES_EX12_INDIRECTLY_ASSOCIATING_PARAMETERS_EX12_TOP_MODULE_H_
#define EXAMPLES_EX12_INDIRECTLY_ASSOCIATING_PARAMETERS_EX12_TOP_MODULE_H_

#include <cci_configuration>
#include <vector>
#include <sstream>
#include <string>

#include "ex12_parameter_owner.h"
#include "ex12_param_value_sync_with_cf.h"
#include "xreport.hpp"

/**
 *  @class  ex12_top_module
 *  @brief  This module instantiates the rest of the platform (configurator and owners)
 */
SC_MODULE(ex12_top_module) {
 public:
  // Pointers to the owner module
  ex12_parameter_owner* param_owner1;  ///< Owner module #1
  ex12_parameter_owner* param_owner2;  ///< Owner module #2

  // Create an instance of the 'param_value_sync_with_cf' class
  ex12_param_value_sync_with_cf *param_sync;  ///< 'param_value_sync' instance

  /**
   *  @fn     ex12_top_module
   *  @brief  The class constructor
   */
  SC_CTOR(ex12_top_module):
            m_broker(cci::cci_get_broker())
  {
    // Strings to store the names of the owner's parameters
    std::string str1, str2;

    str1 = "clk_freq_Hz";
    str2 = "clock_speed_KHz";

    param_owner1 = new ex12_parameter_owner("param_owner1", str1, 1);
    param_owner2 = new ex12_parameter_owner("param_owner2", str2, 2);

    // Check for existence of the owner cci_parameter using name-based look up
    // access and then assign their reference to respective cci_base_param
    std::string param1_str = "top_mod.param_owner1.clk_freq_Hz";
    std::string param2_str = "top_mod.param_owner2.clock_speed_KHz";

    cci::cci_param_handle temp_handle = m_broker.get_param_handle(param1_str);
    if (temp_handle.is_valid()) {
      selectedBaseParamList.push_back(temp_handle);

      XREPORT("[TOP_MODULE C_TOR] : Parameter Name : "
              << temp_handle.get_name() << "\tParameter Value : "
              << temp_handle.get_cci_value().to_json());
    } else {
      XREPORT("[TOP_MODULE C_TOR] : Parameter Name : " << param1_str
              << "\tnot found.");
    }

    // Check for existence of the owner cci_parameter using name-based look up
    // access and then assign their reference to respective cci_param_handle
    temp_handle = m_broker.get_param_handle(param2_str); 
    if (temp_handle.is_valid()) {
      selectedBaseParamList.push_back(temp_handle);

      XREPORT("[TOP_MODULE C_TOR] : Parameter Name : "
              << temp_handle.get_name() << "\tParameter Value : "
              << temp_handle.get_cci_value().to_json());
    } else {
      XREPORT("[TOP_MODULE C_TOR] : Parameter Name : " << param2_str
              << "\tnot found.");
    }

    // Pass the selected base parameters list to the 'param_value_sync_with_cf'
    // class synchronization related activity
    param_sync = new ex12_param_value_sync_with_cf("param_value_sync_with_cf",
                                                   selectedBaseParamList);
  }

 private:
  cci::cci_broker_handle m_broker;  ///< Declaring a CCI configuration broker handle
  std::vector<cci::cci_param_handle> selectedBaseParamList; ///< Selected cci_param_handle list (selection done by top_module)
};
// ex12_top_module

#endif  // EXAMPLES_EX12_INDIRECTLY_ASSOCIATING_PARAMETERS_EX12_TOP_MODULE_H_
