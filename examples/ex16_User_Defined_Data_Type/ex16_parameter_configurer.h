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
 *  @file    parameter_configurer.h
 *  @brief   This class demonstrates means to access and set various
 *           attributes of a cci parameter
 *  @author  P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *           Girish Verma, CircuitSutra Technologies   <girish@circuitsutra.com>
 *  @date    12th September, 2011 (Monday)
 */

#ifndef EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_PARAMETER_CONFIGURER_H_
#define EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_PARAMETER_CONFIGURER_H_

#include <cci_configuration>
#include <string>

#include "xreport.hpp"

/**
 *  @class  ex16_parameter_configurer
 *  @brief  This module instantiates a cci configuration broker which accessess
 *          and sets thevarious attributes of a cci_parameter
 */
SC_MODULE(ex16_parameter_configurer) {
 public:
  /**
   *  @fn     ex16_parameter_configurer
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex16_parameter_configurer):
      m_broker(cci::cci_get_broker()),
      udt_param_handle(cci::cci_originator(*this))
  {
    // Check for the broker type (default or private) using
    // 'is_global_broker()' API
    if (!m_broker.is_global_broker()) {
      // Access broker's name using 'name()'
      XREPORT("[CFGR C_TOR] : Broker Type : " << m_broker.name());
    } else {
      XREPORT("[CFGR C_TOR] : Broker Type : "
              << m_broker.name() << " - is not a private broker.");
    }

    udt_param_str = "param_owner.User_data_type_param";

    // Check the existence of the user-defined data type cci-parameter
    udt_param_handle = m_broker.get_param_handle(udt_param_str);
    if (!udt_param_handle.is_valid()) {
      XREPORT("[CFGR C_TOR] : User define datatype  parameter does not exist");
    }

    // Registering SC_THREAD process
    SC_THREAD(execute);
  }

  /**
   *  @fn     void execute(void)
   *  @brief  Main function to access the CCI parameters
   *  @return void
   */
  void execute(void) {
    while (1) {
      wait(4.0, SC_NS);

      XREPORT("@ " << sc_time_stamp());

      // Access parameter's name using 'get_name()' API
      XREPORT("[CFGR -> Retrieve] : Parameter name : "
              << udt_param_handle.get_name());

      // Access parameter's value using 'json_serialize' API
      XREPORT("[CFGR -> Retrieve] : Parameter value: "
              << udt_param_handle.get_cci_value().to_json());

      // Access parameter's description using 'get_description()' API
      XREPORT("[CFGR -> Retrieve] : Parameter desc: "
              << udt_param_handle.get_description());

      wait(2.0, SC_NS);

      XREPORT("@ " << sc_time_stamp());
      XREPORT("[CFGR -> Set] : Value - 's_address:1024,d_address:1280,index:3");

      std::string set_string("{\"s_address\":1024,\"d_address"
                             "\":1280,\"index\":3}");
      udt_param_handle.set_cci_value(cci::cci_value::from_json(set_string));

      wait(2.0, SC_NS);
      XREPORT("@ " << sc_time_stamp());

      // Access parameter's name using 'get_name()' API
      XREPORT("[CFGR -> Retrieve] : Parameter name : "
              << udt_param_handle.get_name());

      // Access parameter's value using 'json_serialize' API
      XREPORT("[CFGR -> Retrieve] : Parameter value: "
              << udt_param_handle.get_cci_value().to_json());

      wait(20.0, SC_NS);
    }
  }

 private:
  cci::cci_broker_handle m_broker; ///< CCI configuration broker handle instance

  std::string udt_param_str;  ///< std::string types for storing parameters hierarchical paths

  // Declaring CCI parameter handles
  cci::cci_param_handle udt_param_handle;  ///< CCI parameter handle
};
// ex16_parameter_configurer

#endif  // EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_PARAMETER_CONFIGURER_H_
