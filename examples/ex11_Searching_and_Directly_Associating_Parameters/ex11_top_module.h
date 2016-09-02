/*****************************************************************************
  Copyright 2006-2014 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2010-2015 CircuitSutra Technologies Pvt. Ltd.
  All rights reserved.

  Copyright 2016 Ericsson
  All rights reserved.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 *****************************************************************************/

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
#include <cassert>
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
  SC_CTOR(ex11_top_module) {
    // Get handle of the broker responsible for the class/module
    myTopModBrokerIF =
        &cci::cci_broker_manager::get_current_broker(cci::cci_originator(*this));

    // Strings to store the names of the owner's parameters
    std::string str1, str2;

    str1 = "clk_freq_Hz";
    str2 = "clock_speed_Hz";

    param_owner1 = new ex11_parameter_owner("param_owner1", str1, 1000);
    param_owner2 = new ex11_parameter_owner("param_owner2", str2, 2000);

    // Report if handle returned is NULL
    assert(myTopModBrokerIF != NULL && "Configuration Broker handle is NULL");

    std::string param1_str = "top_mod.param_owner1.clk_freq_Hz";
    std::string param2_str = "top_mod.param_owner2.clock_speed_Hz";

    // Check for existence of the owner cci_parameter using name-based look up
    // access and then assign their reference to respective cci_base_param
    if (myTopModBrokerIF->param_exists(param1_str)) {
      cci::cci_param_handle temp =
              myTopModBrokerIF->get_param_handle(param1_str);
      selected_base_param_list.push_back(temp);

      XREPORT("[TOP_MODULE C_TOR] : Parameter Name : " << temp.get_name()
              << "\tParameter Value : "
              << temp.get_cci_value().to_json());
    } else {
      XREPORT("[TOP_MODULE C_TOR] : Parameter Name : " << param1_str
              << "\tnot found.");
    }

    // Check for existence of the owner cci_parameter using name-based look up
    // access and then assign their reference to respective cci_base_param
    if (myTopModBrokerIF->param_exists(param2_str)) {
      cci::cci_param_handle temp =
              myTopModBrokerIF->get_param_handle(param2_str);
      selected_base_param_list.push_back(temp);

      XREPORT("[TOP_MODULE C_TOR] : Parameter Name : " << temp.get_name()
              << "\tParameter Value : "
              << temp.get_cci_value().to_json());
    } else {
      XREPORT("[TOP_MODULE C_TOR] : Parameter Name : " << param2_str
              << "\tnot found.");
    }

    // Pass on the list of cci_base_parameters to the 'param_value_sync'
    // that looks at the synchronization related activity b/w the owner modules
    param_sync = new ex11_param_value_sync("param_value_sync",
                                           selected_base_param_list);
  }

 private:
  cci::cci_broker_if* myTopModBrokerIF;  ///< Declare a CCI configuration broker interface instance
  std::vector<cci::cci_param_handle> selected_base_param_list;  ///< vector storing the searched owner parameters references to CCI parameter handles
};
// ex11_top_module

#endif  // EXAMPLES_EX11_SEARCHING_AND_DIRECTLY_ASSOCIATING_PARAMETERS_EX11_TOP_MODULE_H_

