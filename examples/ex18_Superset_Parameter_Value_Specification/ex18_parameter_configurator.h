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
 *  @file     parameter_configurator.h
 *  @brief    This header declares and defines configurator class.  The configurator
 *            class tries to find the list of unconsumed parameters with the model
 *  @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date     21st July, 2011 (Thursday)
 */
#ifndef EXAMPLES_EX18_SUPERSET_PARAMETER_VALUE_SPECIFICATION_EX18_PARAMETER_CONFIGURATOR_H_
#define EXAMPLES_EX18_SUPERSET_PARAMETER_VALUE_SPECIFICATION_EX18_PARAMETER_CONFIGURATOR_H_

#include <cci_configuration>
#include <vector>
#include <string>

#include "xreport.hpp"

/**
 *  @class  ex18_parameter_configurator
 *  @brief  The configurator class illustrates way of retrieving cci-parameters
 *          list within a cci-model by using the 'get_param_handles()' API.
 */
SC_MODULE(ex18_parameter_configurator) {
 public:
  /**
   *  @fn     ex18_parameter_configurator()
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex18_parameter_configurator):
            m_broker(cci::cci_get_broker())
  {
    // Retrieve the list of all cci-parameters within a model.
    complete_parameter_list = m_broker.get_param_handles();
  }

  /**
   *  @fn     void end_of_elaboration(void)
   *  @brief  This callback function demonstrates ways for a tool developer to
   *          retrieve the list of unconsumed parameters in a model.
   *  @return void
   */
  void end_of_elaboration(void) {
    XREPORT("[CFGR within EOE] : List of all parameters in the model");
    
    for (unsigned int i = 0; i < complete_parameter_list.size(); i++) {
        XREPORT("[CFGR within EOE] : Parameter Name : "
                << complete_parameter_list[i].get_name());
    }

    XREPORT("@ " << sc_time_stamp());
    XREPORT("[CFGR] : List of parameter names with unconsumed preset values");

    unconsumed_parameter_list = m_broker.get_unconsumed_preset_values();
    for (unsigned int i = 0; i < unconsumed_parameter_list.size(); i++) {
      XREPORT("[CFGR] : Unconsumed Parameter Name : "
              << (unconsumed_parameter_list[i]).first);
    }
  }

 private:
  cci::cci_broker_handle m_broker;  ///< Declaring a CCI configuration broker handle

  // std::vector to store the list of the unconsumed parameters
  std::vector<cci::cci_param_untyped_handle> complete_parameter_list; ///< List of all parameters
  std::vector<cci::cci_name_value_pair> unconsumed_parameter_list; ///< List of parameters unconsumed
};
// ex18_parameter_configurator

#endif  // EXAMPLES_EX18_SUPERSET_PARAMETER_VALUE_SPECIFICATION_EX18_PARAMETER_CONFIGURATOR_H_
