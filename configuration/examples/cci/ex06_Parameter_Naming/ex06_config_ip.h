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
 *  @file   ex06_config_ip.h
 *  @brief  Get handles to sim_ip/sc_main's params and update them
 *  @author Asif Mondal, TI
 *          Lei Liang, Ericsson AB
 */

#ifndef EXAMPLES_EX06_PARAMETER_NAMING_EX06_CONFIG_IP_H_
#define EXAMPLES_EX06_PARAMETER_NAMING_EX06_CONFIG_IP_H_

#include <cci_configuration>
#include <string>
#include "xreport.hpp"

/**
 *  @class  ex06_config_ip
 *  @brief  The ex06_config_ip configures sim_ip's parameter
 */
SC_MODULE(ex06_config_ip) {
 public:
  /**
   *  @fn     ex06_config_ip
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex06_config_ip):
            m_broker(cci::cci_get_broker())
  {
    SC_THREAD(execute);
  }

  /**
   *  @fn     void execute()
   *  @brief  Update the value of params
   *  @return void
   */
  void execute() {
    std::ostringstream ss;
    const std::string sim_ip_int_param_ip_name = "sim_ip.int_param_ip";
    const std::string sim_ip_int_param_ip_name_0 = "sim_ip.int_param_ip_0";
    const std::string sc_main_int_param_top_name = "int_param_top";
    const std::string int_param_custom_name = "top.sub.int_param_custom";

    // Wait for a while to update param value
    wait(20, sc_core::SC_NS);

    // Get handle to the param
    cci::cci_param_handle int_param_handle =
        m_broker.get_param_handle(sim_ip_int_param_ip_name);

    // Check its validity
    if (int_param_handle.is_valid()) {

      // Update the param's value
      XREPORT("execute: [EXTERNAL] Current value of "
              << sim_ip_int_param_ip_name << " is "
              << int_param_handle.get_cci_value().to_json());
      XREPORT("execute: [EXTERNAL] Set value of " << sim_ip_int_param_ip_name
              << " to 50");
      int_param_handle.set_cci_value(cci::cci_value(50));

      // Display new value
      std::string new_value = int_param_handle.get_cci_value().to_json();
      XREPORT("execute: [EXTERNAL] Current value of "
              << int_param_handle.name() << " is " << new_value);
    } else {
      XREPORT_ERROR("execute: Param (" << sim_ip_int_param_ip_name
                    << ") is not found!");
    }

    // Check the auto generated parameter name due to name collision
    // Get handle to the param
    cci::cci_param_handle int_param_ip_handle =
        m_broker.get_param_handle(sim_ip_int_param_ip_name_0);

    // Check its validity
    if (int_param_ip_handle.is_valid()) {

      // Update the param's value
      XREPORT("execute: [EXTERNAL] Current value of "
              << sim_ip_int_param_ip_name_0 << " is "
              << int_param_ip_handle.get_cci_value().to_json());
      XREPORT("execute: [EXTERNAL] Set value of " << sim_ip_int_param_ip_name_0
              << " to 60");
      int_param_ip_handle.set_cci_value(cci::cci_value(60));

      // Display new value
      std::string new_value = int_param_ip_handle.get_cci_value().to_json();
      XREPORT("execute: [EXTERNAL] Current value of "
              << int_param_ip_handle.name() << " is " << new_value);
    } else {
      XREPORT_ERROR("execute: Param (" << sim_ip_int_param_ip_name_0
                    << ") is not found!");
    }

    // Get handle to the param
    cci::cci_param_handle sc_main_int_param_top_handle =
        m_broker.get_param_handle(sc_main_int_param_top_name);

    // Check its validity
    if (sc_main_int_param_top_handle.is_valid()) {

      // Update the param's value
      XREPORT("execute: [EXTERNAL] Current value of "
              << sc_main_int_param_top_name << " is "
              << sc_main_int_param_top_handle.get_cci_value().to_json());
      XREPORT("execute: [EXTERNAL] Set value of "
              << sc_main_int_param_top_name
              << " to 70");
      sc_main_int_param_top_handle.set_cci_value(cci::cci_value(70));

      // Display new value
      std::string new_value = sc_main_int_param_top_handle.get_cci_value().to_json();
      XREPORT("execute: [EXTERNAL] Current value of "
              << sc_main_int_param_top_handle.name() << " is " << new_value);
    } else {
      XREPORT_ERROR("execute: Param (" << sc_main_int_param_top_name
                    << ") is not found!");
    }


    // Get handle to the param
    cci::cci_param_handle int_param_custom_handle =
        m_broker.get_param_handle(int_param_custom_name);
    
    // Check its validity
    if (int_param_custom_handle.is_valid()) {

      // Update the param's value
      XREPORT("execute: [EXTERNAL] Current value of " << int_param_custom_name
              << " is " << int_param_custom_handle.get_cci_value().to_json());
      XREPORT("execute: [EXTERNAL] Set value of " << int_param_custom_name
              << " to 80");
      int_param_custom_handle.set_cci_value(cci::cci_value(80));

      // Display new value
      std::string new_value = int_param_custom_handle.get_cci_value().to_json();
      XREPORT("execute: [EXTERNAL] Current value of "
              << int_param_custom_handle.name() << " is " << new_value);
    } else {
      XREPORT_ERROR("execute: Param (" << int_param_custom_name
                    << ") is not found!");
    }
  }

 private:
  cci::cci_broker_handle m_broker; ///< CCI configuration handle
};
// ex06_config_ip

#endif  // EXAMPLES_EX06_PARAMETER_NAMING_EX06_CONFIG_IP_H_
