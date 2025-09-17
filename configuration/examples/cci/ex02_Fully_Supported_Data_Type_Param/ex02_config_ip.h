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
 *  @brief  Try to get sim_ip.int_param handle and typecast it to differnt
 *          data-types
 *  @author A. Nizamudheen, TI
 */

#ifndef EXAMPLES_EX02_FULLY_SUPPORTED_DATA_TYPE_PARAM_EX02_CONFIG_IP_H_
#define EXAMPLES_EX02_FULLY_SUPPORTED_DATA_TYPE_PARAM_EX02_CONFIG_IP_H_

#include <cci_configuration>
#include <string>
#include "xreport.hpp"

/**
 *  @class  ex02_config_ip
 *  @brief  The config_ip class, which configures sim_ip's parameter
 */
SC_MODULE(ex02_config_ip) {
 public:
  /**
   *  @fn     void ex02_config_ip
   *  @brief  The constructor for the class
   *  @return void
   */
 SC_CTOR(ex02_config_ip):
    // Get CCI configuration handle
    m_broker(cci::cci_get_broker())
  {
    SC_THREAD(execute);
  }

  /**
   *  @fn     void execute()
   *  @brief  The main function which updates the parameter of sim_ip
   *  @return void
   */
  void execute() {
    std::ostringstream ss;
    const std::string param_name = "sim_ip.int_param";

    // Wait for 10 ns to update the values of the param
    wait(10, sc_core::SC_NS);

    // Check for existence of int_param
    cci::cci_param_handle int_param_handle = m_broker.get_param_handle(param_name);
    if (int_param_handle.is_valid()) {      
      cci::cci_param_data_category partype = cci::CCI_OTHER_PARAM;
      partype = int_param_handle.get_data_category();
      if(partype == cci::CCI_INTEGRAL_PARAM) {
        XREPORT("@execute: Type of " << param_name << " is an integer.");
      } else {
        XREPORT_ERROR("@execute: Type of " << param_name << " is not an integer.");
      }

      // Convert the untyped handle of the param to a typed handle 'int' 
      cci::cci_param_typed_handle<int> int_param_typed_handle =
          cci::cci_param_typed_handle<int>(int_param_handle);
      if (!int_param_typed_handle.is_valid()) {
        XREPORT_WARNING("@execute: Typecast of " << int_param_handle.name()
                        << " to cci::cci_param_typed_handle<int> failed.");
      } else {
        XREPORT("@execute: Typecast of " << param_name
                << " to cci::cci_param_typed_handle<int> succeeded");
        XREPORT("@execute: Current value of " << int_param_typed_handle.name()
                << " is " << int_param_typed_handle.get_value());
      }

      // Convert the untyped handle of the param to a typed handle 'unsigned int'
      cci::cci_param_typed_handle<unsigned int> uint_param_typed_handle =
          cci::cci_param_typed_handle<unsigned int>(int_param_handle);
      if (!uint_param_typed_handle.is_valid()) {
        XREPORT_WARNING("@execute: Typecast of " << param_name
                        << " to cci::cci_param_typed_handle<unsigned int> failed.");
      } else {
        XREPORT("@execute: Current value of " << uint_param_typed_handle.name()
                << " is " << uint_param_typed_handle.get_value());
      }

      // Convert the untyped handle of the param to a typed handle 'std::string'
      cci::cci_param_typed_handle<std::string> string_param_typed_handle =
          cci::cci_param_typed_handle<std::string>(int_param_handle);
      if (!string_param_typed_handle.is_valid()) {
        XREPORT_WARNING("@execute: Typecast of " << param_name
                        << " to cci::cci_param_typed_handle<std::string> failed.");
      } else {
        XREPORT("@execute: Current value of " << string_param_typed_handle.name()
                << " is " << string_param_typed_handle.get_value());
      }

    } else {
      XREPORT_ERROR("execute: Param (" << param_name << ") is not found!");
    }
  }

 private:
  cci::cci_broker_handle m_broker; ///< CCI configuration handle
};
// ex02_config_ip

#endif  // EXAMPLES_EX02_FULLY_SUPPORTED_DATA_TYPE_PARAM_EX02_CONFIG_IP_H_
