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
#include <cassert>
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
            m_cci(cci::cci_broker_manager::get_broker())
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

    // Check for existance of int_param
    if (m_cci.param_exists(param_name)) {
      // Get handle to the param
      cci::cci_param_handle int_param = m_cci.get_param_handle(param_name);
      assert(int_param.is_valid());

      cci::cci_data_type partype = cci::CCI_UNAVAILABLE_DATA;
      partype = int_param.get_basic_type();
      if(partype == cci::CCI_NUMBER_DATA) {
        XREPORT("@execute: Type of " << param_name << " is a number.");
      } else {
        XREPORT_ERROR("@execute: Type of " << param_name << " is not a number.");
      }

      // Typecast the param to an 'int' type
      cci::cci_param_typed_handle<int> int_param_typed =
          cci::cci_param_typed_handle<int>(int_param);
      if (!int_param_typed.is_valid()) {
        XREPORT_WARNING("@execute: Typecast of " << int_param.get_name()
                        << " to 'cci::cci_param_typed_handle<int> *' type failed.");
      } else {
        XREPORT("@execute: Typecast of " << param_name
                << " to 'cci::cci_param_typed_handle<int> *' succeeded");
        XREPORT("@execute: Current value of " << int_param_typed.get_name()
                << " is " << int_param_typed.get_value());
      }

      // Typecast the param to 'unsigned int' type
      cci::cci_param_typed_handle<unsigned int> uint_param_typed =
          cci::cci_param_typed_handle<unsigned int>(int_param);
      if (!uint_param_typed.is_valid()) {
        XREPORT_WARNING("@execute: Typecast of " << param_name
                        << " to 'cci::cci_param_typed_handle<unsigned int> *'"
                        " type failed.");
      } else {
        XREPORT("@execute: Current value of " << uint_param_typed.get_name()
                << " is " << uint_param_typed.get_value());
      }

      // Typecast the param to 'std::string' type
      cci::cci_param_typed_handle<std::string> string_param_typed =
          cci::cci_param_typed_handle<std::string>(int_param);
      if (!string_param_typed.is_valid()) {
        XREPORT_WARNING("@execute: Typecast of " << param_name
                        << " to 'cci::cci_param_typed_handle<std::string> *'"
                        " type failed.");
      } else {
        XREPORT("@execute: Current value of " << string_param_typed.get_name()
                << " is " << string_param_typed.get_value());
      }

    } else {
      XREPORT_ERROR("execute: Param (" << param_name << ") is not found!");
    }
  }

 private:
  cci::cci_broker_if& m_cci; ///< CCI configuration handle
};
// ex02_config_ip

#endif  // EXAMPLES_EX02_FULLY_SUPPORTED_DATA_TYPE_PARAM_EX02_CONFIG_IP_H_
