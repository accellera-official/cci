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
 *  @brief   This header instantiates various CCI parameters with default values
 *  @author  Girish Verma, CircuitSutra Technologies   <girish@circuitsutra.com>
 *           P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date    12th September, 2011 (Monday)
 */

#ifndef EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_PARAMETER_OWNER_H_
#define EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_PARAMETER_OWNER_H_

#include <cci_configuration>
#include <string>

#include "ex16_user_datatype.h"
#include "xreport.hpp"

/**
 *  @class  ex16_parameter_owner
 *  @brief  This module instantiates an object of user-defined data type of
 *          cci_parameter
 */
SC_MODULE(ex16_parameter_owner) {
 public:
  /**
   *  @fn     ex16_parameter_owner
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex16_parameter_owner)
        // Assign name and value to the cci 'user-defined data type' parameter
      : udt_param("User_data_type_param", route_table_ut(0x200, 0x300, 1)) {
    // Registering SC_THREAD process
    SC_THREAD(run_thread);

    XREPORT("Prior to " << sc_time_stamp()
            << "\tdemonstrating 'get_default_value()'");
    XREPORT("[OWNER -> Retrieve] : Parameter name  : " << udt_param.name());

    // Query default value of a parameter using 'get_default_type()' API
    XREPORT("[OWNER -> Retrieve] : Using 'get_default_value()' : "
            << udt_param.get_default_value());
    XREPORT("[OWNER -> Retrieve] : Parameter Value' : " << udt_param.get_value());

    // Set description for the user-defined data type
    XREPORT("[OWNER -> Set] : Param desc - 'This is user-defined data type");
    const std::string init_desc = "This is user-defined data type";
    udt_param.set_description(init_desc);
  }

  /**
   *  @fn     void run_thread(void)
   *  @brief  The SC_THREAD process for modifying the cci parameter
   *  @return void
   */
  void run_thread(void) {
    while (1) {
      XREPORT("@ " << sc_time_stamp());
      XREPORT("[OWNER -> Retrieve] : Description : "
              << udt_param.get_description());

      // Override the default value
      XREPORT("[OWNER -> Set] : New Value " << udt_param.name()
              << "\t's_address:768,d_address:1024,index:2'");
      udt_param.set_cci_value(cci::cci_value::from_json(
              "{\"s_address\":768,\"d_address\":1024,\"index\":2}"));

      wait(2.0, SC_NS);
      XREPORT("@ " << sc_time_stamp());

      // Access parameter's value
      XREPORT("[OWNER -> Retrieve] : UDT Value : " << udt_param.get_value());

      wait(28.0, SC_NS);
    }
  }

 private:
  cci::cci_param<route_table_ut, cci::CCI_MUTABLE_PARAM> udt_param; ///< CCI mutable parameter
};
// ex16_parameter_owner

#endif  // EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_PARAMETER_OWNER_H_
