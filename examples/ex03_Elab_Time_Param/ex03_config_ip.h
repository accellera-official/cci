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
 *  @brief  Configure simple_ip's structure_param (simple integer) parameter
 *  @author Udaya Ranga, TI
 *          Lei Liang, Ericsson
 */

#ifndef EXAMPLES_EX03_ELAB_TIME_PARAM_EX03_CONFIG_IP_H_
#define EXAMPLES_EX03_ELAB_TIME_PARAM_EX03_CONFIG_IP_H_

#include <cci_configuration>
#include <string>
#include "xreport.hpp"

/**
 *  @class  ex03_config_ip
 *  @brief  The config_ip class configures sim_ip's parameters
 */
SC_MODULE(ex03_config_ip) {
 public:
  /**
   *  @fn     void ex03_config_ip()
   *  @brief  The class constructor
   *  @return void
   */
 SC_CTOR(ex03_config_ip):
    // Get CCI configuration handle specific for this module
    m_broker(cci::cci_get_broker())
  {
    SC_THREAD(execute);
  }

  /**
   *  @fn     void execute()
   *  @brief  Configure the value of "sim_ip.struc_param" parameter
   *  @return void
   */
  void execute() {
    std::ostringstream ss;
    const std::string struc_param_name = "sim_ip.struc_param";

    // Wait for a while to update param value
    wait(20, sc_core::SC_NS);

    // Check for existence of the structure_param
    cci::cci_param_handle struc_param_handle =
        m_broker.get_param_handle(struc_param_name);
    if (struc_param_handle.is_valid()) {
      // Update the structure_param value to 3 (invalid)
	  XREPORT("execute: [EXTERNAL] Set value of " << struc_param_name << " to 3");
      struc_param_handle.set_cci_value(cci::cci_value(3));

      // Display new value
      std::string new_value = struc_param_handle.get_cci_value().to_json();
      // XREPORT("execute: [EXTERNAL] Current value of " <<
      // struc_param.get_name()<< " is " << new_value);
      if ("1" == new_value) {
        XREPORT("execute: [EXTERNAL] Value of " << struc_param_handle.get_name()
                << " remains unchanged " << new_value);
      } else if ("3" == new_value) {
        XREPORT_WARNING("execute: [EXTERNAL] Value of "
                        << struc_param_handle.get_name() << " changed to "
                        << new_value);
      } else {
        XREPORT_ERROR("execute: [EXTERNAL] Invalid update to "
                      << struc_param_handle.get_name() << " value changed to "
                      << new_value);
        sc_assert(0);
      }
    } else {
      XREPORT_ERROR("execute: Param (" << struc_param_name
                    << ") is not found!");
    }
  }

 private:
  cci::cci_broker_handle m_broker; ///< CCI configuration handle
};
// ex03_config_ip

#endif  // EXAMPLES_EX03_ELAB_TIME_PARAM_EX03_CONFIG_IP_H_
