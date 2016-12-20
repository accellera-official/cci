
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
 *  @file   ex00_Param_User.h
 *  @brief  This file declares and defines a sc_module which demonstrates the
 *          means to access the cci-parameters.
 *  @author Sonal Poddar, Intel
 */

#ifndef EXAMPLES_EX00_KEY_CONCEPTS_EX00_PARAM_USER_H_
#define EXAMPLES_EX00_KEY_CONCEPTS_EX00_PARAM_USER_H_

#include <cci_configuration>
#include <string>
#include "xreport.hpp"

/**
 *  @class  ex00_Param_User
 *  @brief  The Param_User uses Param_Owner's parameter.
 */
SC_MODULE(ex00_Param_User) {
 public:
  /**
   *  @fn     void ex00_Param_User()
   *  @brief  The constructor for the class.
   *  @return void
   */
  SC_CTOR(ex00_Param_User):
            m_cci(cci::cci_broker_manager::get_broker())
  {
    SC_THREAD(execute);
  }

  /**
   *  @fn     void execute()
   *  @brief  Configure the value of "top.subsystem1.param_owner.int_param" parameter
   *  @return void
   */
  void execute() {
    std::ostringstream ss;
    const std::string int_param_name = "top.subsystem1.param_owner.int_param";

    // Wait for a while to update param value
    wait(20, sc_core::SC_NS);

    // Check for existance of the param
    if (m_cci.param_exists(int_param_name)) {
      // Get handle to the param
      cci::cci_param_handle int_param_handle = m_cci.get_param_handle(int_param_name);
      sc_assert(int_param_handle.is_valid());

      // Update the param's value to 2
      XREPORT("execute: [EXTERNAL] Set value of " << int_param_name << " to 2");
      int_param_handle.set_cci_value(cci::cci_value(2));

    } else {
      XREPORT_ERROR("execute: Param (" << int_param_name<< ") is not found!");
    }
  }

 private:
  cci::cci_broker_if& m_cci; ///< CCI configuration handle
};
// ex00_Param_User

#endif  // EXAMPLES_EX00_KEY_CONCEPTS_EX00_PARAM_USER_H_
