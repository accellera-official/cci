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
 *  @brief  Configure simple_ip's int_param (simple integer) parameter
 *  @author R. Swaminathan, TI
 */

#ifndef EXAMPLES_EX01_SIMPLE_INT_PARAM_EX01_CONFIG_IP_H_
#define EXAMPLES_EX01_SIMPLE_INT_PARAM_EX01_CONFIG_IP_H_

#include <cci_configuration>
#include <string>
#include "xreport.hpp"

/**
 *  @class  ex01_config_ip
 *  @brief  The config_ip configures simple_ip's parameter.
 */
SC_MODULE(ex01_config_ip) {
 public:
  /**
   *  @fn     void ex01_config_ip()
   *  @brief  The constructor for the class.
   *  @return void
   */

SC_CTOR(ex01_config_ip):
    // Get CCI configuration handle specific for this module
   m_broker(cci::cci_get_broker())
  {
    SC_THREAD(execute);
  }

  /**
   *  @fn     void execute()
   *  @brief  Configure the value of "sim_ip.int_param" parameter
   *  @return void
   */
  void execute() {
    std::ostringstream ss;
    const std::string int_param_name = "sim_ip.int_param";

    // Wait for a while to update param value
    wait(20, sc_core::SC_NS);

    // Check for existence of the param
    cci::cci_param_handle int_param_handle = m_broker.get_param_handle(int_param_name);
    if (int_param_handle.is_valid()) {

      // Update the param's value to 2
      XREPORT("execute: [EXTERNAL] Set value of " << int_param_name << " to 2");
      int_param_handle.set_cci_value(cci::cci_value(2));

      // Display new value
      std::string new_value = int_param_handle.get_cci_value().to_json();
      XREPORT("execute: [EXTERNAL] Current value of "
              << int_param_handle.name() << " is " << new_value);
    } else {
      XREPORT_ERROR("execute: Param (" << int_param_name<< ") is not found!");
    }
  }

 private:
  cci::cci_broker_handle m_broker; ///< CCI configuration handle
};
// ex01_config_ip

#endif  // EXAMPLES_EX01_SIMPLE_INT_PARAM_EX01_CONFIG_IP_H_
