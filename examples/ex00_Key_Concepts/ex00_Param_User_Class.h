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
 *  @file   ex00_Param_User_Class.h
 *  @brief  This file declares and defines a class outside SystemC hierarchy which demonstrates the
 *          means to access the cci-parameters.
 *  @author Sonal Poddar, Intel
 */

#ifndef EXAMPLES_EX00_KEY_CONCEPTS_EX00_PARAM_USER_CLASS_H_
#define EXAMPLES_EX00_KEY_CONCEPTS_EX00_PARAM_USER_CLASS_H_

#include <cci_configuration>
#include <vector>
#include "xreport.hpp"

/**
 *  @class  ex00_Param_User_Class
 *  @brief  The Param_User_Class updates Param_Owner's parameter.
 */
class ex00_Param_User_Class {
 public:

  /**
   *  @fn     ex00_Param_User_Class()
   *  @brief  The class constructor.
   *  @return void
   */
  ex00_Param_User_Class():
      m_cci(cci::cci_broker_manager::get_broker(
              cci::cci_originator("User_Class")))
  {
  }

  /**
   *  @fn     const char *name()
   *  @brief  This functiongives the name of the class to the originator.
   *  @return name of the class.
   */
const char* name() const
{
	return("User_Class");
}

  /**
   *  @fn     void update_param()
   *  @brief  Updates the value of "top.subsystem1.param_owner.int_param" parameter
   *  @return void
   */

void update_param() {
    std::ostringstream ss;
    const std::string int_param_name = "top.subsystem1.param_owner.int_param";

    // Check for existance of the param
    if (m_cci.param_exists(int_param_name)) {
      // Get handle to the param
      cci::cci_param_handle int_param_handle = m_cci.get_param_handle(int_param_name);
      sc_assert(int_param_handle.is_valid());

	  // Update the param's value to 3
      XREPORT("update_param: [EXTERNAL] Set value of " << int_param_name << " to 3");
      int_param_handle.set_cci_value(cci::cci_value(3));
	  }
	  else {
      XREPORT_ERROR("update_param: Param (" << int_param_name<< ") is not found!");
      }
	 }

 private:
  cci::cci_broker_if& m_cci; ///< Declaring a CCI configuration broker interface instance

};
// ex00_Param_User_Class

#endif  // EXAMPLES_EX00_KEY_CONCEPTS_EX00_PARAM_USER_CLASS_H_
