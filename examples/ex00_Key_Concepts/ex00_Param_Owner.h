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
 *  @file   ex00_Param_Owner.h
 *  @brief  A module that owns an integer parameter.
 *  @author Sonal Poddar, Intel
 */

#ifndef EXAMPLES_EX00_KEY_CONCEPTS_EX00_PARAM_OWNER_H_
#define EXAMPLES_EX00_KEY_CONCEPTS_EX00_PARAM_OWNER_H_

#include <cci_configuration>
#include "xreport.hpp"

/**
 *  @class  ex00_Param_Owner
 *  @brief  The parameter owner initializes CCI-based integer parameter.
 */ 

SC_MODULE(ex00_Param_Owner) {
 public:
 /**
   *  @fn     ex00_Param_Owner
   *  @brief  The class constructor
   *  @return void
   */

  SC_CTOR(ex00_Param_Owner)
  // Initialize int_param with 0
      : int_param("int_param", 0) {
    SC_THREAD(execute);
    XREPORT("Ctor: Default value of " << int_param.get_name() << " is "
            << int_param);
  }

  /**
   *  @fn     void ~ex00_Param_Owner()
   *  @brief  The destructor for the class.
   *  @return void
   */
  ~ex00_Param_Owner() {
    XREPORT_PLAIN("Dtor: Current value of " << int_param.get_name() << " is "
                  << int_param);
  }

  /**
   *  @fn     void execute()
   *  @brief  The main execution block (no real functionality)
   *  @return void
   */
  void execute() {
    // Wait for 10ns to allow Param_User to update int_param value
    wait(10, sc_core::SC_NS);

    // Increment value of the parameter
    XREPORT("execute: Set value of int_param to 1");
    int_param =1;
    XREPORT("execute: Current value of int_param is "<< int_param);
  }

 private:
  cci::cci_param<int> int_param; ///< CCI param to hold buffer size
};
// ex00_Param_Owner

#endif  // EXAMPLES_EX00_KEY_CONCEPTS_EX00_PARAM_OWNER_H_

