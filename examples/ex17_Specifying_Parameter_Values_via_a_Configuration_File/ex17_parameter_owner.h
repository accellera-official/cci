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
 *  @file      parameter_owner.h
 *  @brief     This sc_module is the owner class of cci-parameters
 *  @author    P V S Phaneendra, CircuitSutra Technologies    <pvs@circuitsutra.com>
 *             Girish Verman, CircuitSutra Technologies   <girish@circuitsutra.com>
 *  @date      18th May, 2011 (Monday)
 */

#ifndef EXAMPLES_EX17_SPECIFYING_PARAMETER_VALUES_VIA_A_CONFIGURATION_FILE_EX17_PARAMETER_OWNER_H_
#define EXAMPLES_EX17_SPECIFYING_PARAMETER_VALUES_VIA_A_CONFIGURATION_FILE_EX17_PARAMETER_OWNER_H_

#include <cci_configuration>
#include <string>

#include "xreport.hpp"

class ex17_parameter_configurator;

/**
 *  @class  ex17_parameter_owner
 *  @brief  This sc_module gives default values to all its cci-parameters.
 *          The parameters that are overridden with their preset values
 *          appear with their preset values rather than default values.
 */
SC_MODULE(ex17_parameter_owner) {
 public:
  friend class ex17_parameter_configurator;

  /**
   *  @fn     ex17_parameter_owner()
   *  @brief  The class constructor.
   *  @return void
   */
  SC_CTOR(ex17_parameter_owner)
      : int_param("mutable_int_param", 5),
        float_param("mutable_float_param", 123.45),
        string_param("mutable_string_param", "default") {
    XREPORT("[OWNER C_TOR] : Int Parameter Value : " << int_param.get_value());
    XREPORT("[OWNER C_TOR] : Float Parameter Value : " << float_param.get_value());
    XREPORT("[OWNER C_TOR] : String Parameter Value : " << string_param.get_value());

    // SC_THREAD declaration
    SC_THREAD(run_owner);
  }

  /**
   *  @fn     void run_owner(void)
   *  @brief  Reads and prints the value of the cci parameters.
   *  @return void
   */
  void run_owner(void) {
    while (1) {
      wait(5.0, sc_core::SC_NS);
      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[OWNER] : Int Parameter Value : " << int_param.get_value());
      XREPORT("[OWNER] : Float Parameter Value : " << float_param.get_value());
      XREPORT("[OWNER] : String Parameter Value : "
              << string_param.get_value());

      wait(50.0, sc_core::SC_NS);
    }
  }

 private:
  cci::cci_param<int> int_param; ///< CCI int parameter
  cci::cci_param<float> float_param; ///< CCI float parameter
  cci::cci_param<std::string> string_param;  ///< CCI string parameter
};
// ex17_parameter_owner

#endif  // EXAMPLES_EX17_SPECIFYING_PARAMETER_VALUES_VIA_A_CONFIGURATION_FILE_EX17_PARAMETER_OWNER_H_
