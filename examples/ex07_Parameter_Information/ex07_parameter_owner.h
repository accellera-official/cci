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
 *  @file    parameter_owner.h
 *  @brief   This file defines the OWNER module's functionality.
 *           This file defines a 'PARAMETER_OWNER' sc_module which, in turn,
 *           'int-type' & 'string-type'cci-parameters with default values
 *  @author  P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date    12th September, 2011 (Monday)
 */
#ifndef EXAMPLES_EX07_PARAMETER_INFORMATION_EX07_PARAMETER_OWNER_H_
#define EXAMPLES_EX07_PARAMETER_INFORMATION_EX07_PARAMETER_OWNER_H_

#include <cci_configuration>
#include <string>
#include "xreport.hpp"

/**
 *  @class  ex07_parameter_owner
 *  @brief  The parameter owner initializes various CCI-based parameters
 */
SC_MODULE(ex07_parameter_owner) {
 public:
  /**
   *  @fn     ex07_parameter_owner
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex07_parameter_owner)
  // Assign preset value to mutable_int_param
      : int_param("mutable_int_param", 0),
        // Assign a name to the string param (Default is not assigned)
        string_param("mutable_string_param", "Default_Value",
                     "This is a mutable type string parameter") {
    SC_THREAD(run_thread);

    XREPORT("Prior to " << sc_core::sc_time_stamp()
            << "\tdemonstrating 'is_preset_value' API");

    XREPORT("[OWNER] : Parameter : " << int_param.get_name() << "\tValue : "
            << int_param.get_value());

    XREPORT("Is preset value ?\t" << std::boolalpha
            << int_param.is_preset_value());

    XREPORT("Prior to " << sc_core::sc_time_stamp()
            << "\tdemonstrating 'get_default_value()'");
    XREPORT("[OWNER -> Retrieve] : Parameter name  : " << int_param.get_name());

    // Setting parameter value using 'cci_value' not implemented
    // Used something like : int_param = 1; (see code within the SC_THREAD)

    // Query default value of a parameter using 'get_default_type()' API
    XREPORT("[OWNER -> Retrieve] : Using 'get_default_value()' : "
            << int_param.get_default_value());

    // Set Description - This gives the meta-data about the parameter
    XREPORT("[OWNER -> Set] : Param description - 'This is a mutable"
            " type integer parameter'");
    const std::string init_desc = "This is a mutable type integer parameter";
    int_param.set_description(init_desc);
    int_param.add_metadata("min_value", cci::cci_value("0"),
                           "Minimum value");
    int_param.add_metadata("max_value", cci::cci_value("100"),
                           "Maximum value");
    int_param.add_metadata("unit", cci::cci_value("F"),
                           "Unit of the parameter (Farad)");
  }

  /**
   *  @fn     void run_thread()
   *  @brief  Main thread to set parameter values
   *  @return void
   */
  void run_thread() {
    while (1) {
      wait(1.0, sc_core::SC_NS);

      // Override the default value
      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[OWNER -> Set] : Overriding default value of "
              << int_param.get_name() << " to 1");

      // Setting value to the integer type parameter
      int_param = 1;

      wait(25.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp()
              << "\tdemonstrating setting values by OWNER using cci_value");
      XREPORT("[OWNER -> Set] : New String Value : 'String_Value_Set_by_OWNER");

      cci::cci_value str_value("String_Value_Set_by_OWNER");
      string_param.set_cci_value(str_value);

      wait(2.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp()
              << "\tdemonstrating retrieving values by OWNER using cci_value");

      cci::cci_value rec_str_value = string_param.get_cci_value();
      std::string recv_str = rec_str_value.get_string();

      XREPORT("[OWNER -> Retrieve] : Receive str_value using 'cci_value' : "
              << recv_str);

      wait(10.0, sc_core::SC_NS);
    }
  }

 private:
  cci::cci_param<int> int_param; ///< integer CCI parameter
  cci::cci_param<std::string> string_param;  ///< string CCI parameter
};
// ex07_parameter_owner

#endif  // EXAMPLES_EX07_PARAMETER_INFORMATION_EX07_PARAMETER_OWNER_H_
