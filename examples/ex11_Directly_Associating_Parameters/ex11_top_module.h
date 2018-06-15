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
 *  @brief    This header declares and defines the top module which instantiates
 *            two owner modules
 *  @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date     9th June, 2011 (Thursday)
 */

#ifndef EXAMPLES_EX11_SEARCHING_AND_DIRECTLY_ASSOCIATING_PARAMETERS_EX11_TOP_MODULE_H_
#define EXAMPLES_EX11_SEARCHING_AND_DIRECTLY_ASSOCIATING_PARAMETERS_EX11_TOP_MODULE_H_

#include <cci_configuration>
#include <vector>
#include <string>

#include "ex11_parameter_owner.h"
#include "ex11_param_value_sync.h"
#include "xreport.hpp"

/**
 *  @class ex11_top_module
 *  @brief Instantiates the platform (param_value_sync and param owners)
 */
SC_MODULE(ex11_top_module) {
 public:
  ex11_param_value_sync* param_sync;

  // Pointers to the owner module
  ex11_parameter_owner* param_owner1;
  ex11_parameter_owner* param_owner2;

  /**
   *  @fn     ex11_top_module
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex11_top_module):
            m_broker(cci::cci_get_broker())
  {
    // Strings to store the names of the owner's parameters
    std::string str1, str2;

    str1 = "clk_freq_Hz";
    str2 = "clock_speed_Hz";

    param_owner1 = new ex11_parameter_owner("param_owner1", str1, 1000);
    param_owner2 = new ex11_parameter_owner("param_owner2", str2, 2000);

    std::string param1_str = "top_mod.param_owner1.clk_freq_Hz";
    std::string param2_str = "top_mod.param_owner2.clock_speed_Hz";

    // Check for existence of the owner cci_parameter using name-based look up
    // access and then assign their reference to respective cci_base_param
    cci::cci_param_handle temp_handle =
        m_broker.get_param_handle(param1_str);
    if (temp_handle.is_valid()) {
      selected_base_param_list.push_back(temp_handle);

      XREPORT("[TOP_MODULE C_TOR] : Parameter Name : " << temp_handle.name()
              << ", Value : "
              << temp_handle.get_cci_value().to_json());
    } else {
      XREPORT("[TOP_MODULE C_TOR] : Parameter Name : " << param1_str
              << " not found.");
    }

    // Check for existence of the owner cci_parameter using name-based look up
    // access and then assign their reference to respective cci_base_param
    temp_handle = m_broker.get_param_handle(param2_str);
    if (temp_handle.is_valid()) {
      selected_base_param_list.push_back(temp_handle);

      XREPORT("[TOP_MODULE C_TOR] : Parameter Name : " << temp_handle.name()
              << ", Value : "
              << temp_handle.get_cci_value().to_json());
    } else {
      XREPORT("[TOP_MODULE C_TOR] : Parameter Name : " << param2_str
              << " not found.");
    }

    // Pass on the list of cci_base_parameters to the 'param_value_sync'
    // that looks at the synchronization related activity b/w the owner modules
    param_sync = new ex11_param_value_sync("param_value_sync",
                                           selected_base_param_list);
  }

 private:
  cci::cci_broker_handle m_broker;  ///< Declare a CCI configuration broker handle
  std::vector<cci::cci_param_handle> selected_base_param_list;  ///< vector storing the searched owner parameters references to CCI parameter handles
};
// ex11_top_module

#endif  // EXAMPLES_EX11_SEARCHING_AND_DIRECTLY_ASSOCIATING_PARAMETERS_EX11_TOP_MODULE_H_

