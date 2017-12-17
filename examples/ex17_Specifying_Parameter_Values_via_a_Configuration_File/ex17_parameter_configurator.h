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
 *  @brief    This header declares and defines configurator
 *  @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *            Girish Verma, CircuitSutra Technologies   <girish@circuitsutra.com>
 *  @date     18th May, 2011 (Monday)
 */

#ifndef EXAMPLES_EX17_SPECIFYING_PARAMETER_VALUES_VIA_A_CONFIGURATION_FILE_EX17_PARAMETER_CONFIGURATOR_H_
#define EXAMPLES_EX17_SPECIFYING_PARAMETER_VALUES_VIA_A_CONFIGURATION_FILE_EX17_PARAMETER_CONFIGURATOR_H_

#include <cci_configuration>
#include "xreport.hpp"

/**
 *  @class  ex17_parameter_configurator
 *  @brief  This class either reads or sets values to the parameter-owner's parameters initialization.
 */
SC_MODULE(ex17_parameter_configurator) {
 public:
  /**
   *  @fn     ex17_parameter_configurator()
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex17_parameter_configurator):
      m_broker(cci::cci_get_broker()),
      int_param_handle(cci::cci_originator(*this)),
      float_param(cci::cci_originator(*this)),
      str_param(cci::cci_originator(*this))
  {
    // Get handle of the 'int_param' cci-parameter
    int_param_handle =
        m_broker.get_param_handle("param_owner.mutable_int_param");
    if (int_param_handle.is_valid()) {
      XREPORT("[CFGR C_TOR] : Integer parameter exists");
    } else {
      XREPORT("[CFGR C_TOR] : Integer parameter doesn't exists.");
    }

    // Get handle of the 'float_param' cci-parameter
    float_param =
        m_broker.get_param_handle("param_owner.mutable_float_param");
    if (float_param.is_valid()) {
      XREPORT("[CFGR C_TOR] : Float parameter exists");
    } else {
      XREPORT("[CFGR C_TOR] : Float parameter doesn't exists.");
    }

    // Get handle of the 'string_param' cci-parameter
    str_param =
        m_broker.get_param_handle("param_owner.mutable_string_param");
    if (str_param.is_valid()) {
      XREPORT("[CFGR C_TOR] : String parameter exists");
    } else {
      XREPORT("[CFGR C_TOR] : String parameter doesn't exists.");
    }

    // Registering SC_THREAD with the SystemC kernel
    SC_THREAD(run_mutable_cfgr);
  }

  /**
   *  @fn     void run_mutable_cfgr(void)
   *  @brief  The implementation of the thread process, which will configure the parameter.
   *  @return void
   */
  void run_mutable_cfgr(void) {
    while (1) {
      // Set value to the 'integer' parameter of the owner module
      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[CFGR] : Set integer parameter value to '20'");
      int_param_handle.set_cci_value(cci::cci_value(20));

      // Set value to the 'string' parameter of the owner module
      XREPORT("[CFGR] : Set string  parameter value to 'configure'");
      str_param.set_cci_value(cci::cci_value::from_json("\"configure\""));

      wait(50.0, sc_core::SC_NS);
    }
  }

 private:
  cci::cci_broker_handle m_broker;  ///< Declare a configuration broker

  // Declare cci_base_param for each of the owner's cci-parameters
  cci::cci_param_handle int_param_handle;  ///< Parameter handle to an int parameter
  cci::cci_param_handle float_param;  ///< Parameter handle to a float parameter
  cci::cci_param_handle str_param;  ///< Parameter handle to a string parameter
};
// ex17_parameter_configurator

#endif  // EXAMPLES_EX17_SPECIFYING_PARAMETER_VALUES_VIA_A_CONFIGURATION_FILE_EX17_PARAMETER_CONFIGURATOR_H_
