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
 *  @file   config_ip.h
 *  @brief  Configure simple_ip's param_1 and param_2 values
 *  @author Sheshadri Chakravarthy, Texas Instruments
 */

#ifndef EXAMPLES_EX08_PARAMETER_VALUE_CHANGE_CALLBACKS_EX08_CONFIG_IP_H_
#define EXAMPLES_EX08_PARAMETER_VALUE_CHANGE_CALLBACKS_EX08_CONFIG_IP_H_

#include <sstream>
#include <string>
#include "cci_configuration"
#include "xreport.hpp"

/**
 *  @class  ex08_config_ip
 *  @brief  The config_ip configures sim_ip's parameters
 */
SC_MODULE(ex08_config_ip) {
 public:
  /**
   *  @fn     ex08_config_ip
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex08_config_ip):
            m_broker(cci::cci_get_broker())
  {}

  /**
   *  @fn     void end_of_elaboration()
   *  @brief  Function to perform tasks at the end of elaboration
   *  @return void
   */
  void end_of_elaboration() {
    std::ostringstream ss;
    const std::string param_1_name = "sim_ip.param_1";
    const std::string param_2_name = "sim_ip.param_2";

    // Get handle to param_1
    cci::cci_param_handle param_1_handle =
        m_broker.get_param_handle(param_1_name);

    // Check its validity
    if (param_1_handle.is_valid()) {
      // Update the param's value
      XREPORT("config_ip::end_of_elaboration: [EXTERNAL] Set value of "
              << param_1_name << " to 10");
      param_1_handle.set_cci_value(cci::cci_value(10));

      // Display new value
      std::string new_value = param_1_handle.get_cci_value().to_json();
      XREPORT("config_ip::end_of_elaboration: [EXTERNAL] Current value of "
              << param_1_handle.get_name() << " is " << new_value);
    } else {
      XREPORT_ERROR("end_of_elaboration: Param (" << param_1_name
                    << ") is not found!");
    }

    // Get handle to the param_2
    cci::cci_param_handle param_2_handle =
        m_broker.get_param_handle(param_2_name);
    // Check its validity
    if (param_2_handle.is_valid()) {
      // Update the param's value
      XREPORT("config_ip::end_of_elaboration: [EXTERNAL] Set value of "
              << param_2_name << " to 20");
      try {
        /// @note This should be rejected
        param_2_handle.set_cci_value(cci::cci_value(20));
      } catch (sc_core::sc_report const &e) {
        // Catch exception, if value-change is rejected and handle it
        cci::cci_handle_exception(cci::CCI_SET_PARAM_FAILURE);
        XREPORT_WARNING("Caught exception: " << e.what());
      }

      // Display new value
      std::string new_value = param_2_handle.get_cci_value().to_json();
      XREPORT("config_ip::end_of_elaboration: [EXTERNAL] Current value of "
              << param_2_handle.get_name() << " is " << new_value);
    } else {
      XREPORT_ERROR("config_ip::end_of_elaboration: Param ("
                    << param_2_name << ") is not found!");
    }
  }

 private:
  cci::cci_broker_handle m_broker; ///< CCI configuration handle
};
// ex08_config_ip

#endif  // EXAMPLES_EX08_PARAMETER_VALUE_CHANGE_CALLBACKS_EX08_CONFIG_IP_H_
