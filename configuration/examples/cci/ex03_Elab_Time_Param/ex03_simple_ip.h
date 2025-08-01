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
 *  @file   simple_ip.h
 *  @brief  A simple IP that demonstartes the use of elaboration_time_param
 *  @author Udaya Ranga, TI
 *          Lei Liang, Ericsson
 */

#ifndef EXAMPLES_EX03_ELAB_TIME_PARAM_EX03_SIMPLE_IP_H_
#define EXAMPLES_EX03_ELAB_TIME_PARAM_EX03_SIMPLE_IP_H_

#include <cci_configuration>
#include "xreport.hpp"

/**
 *  @class  ex03_simple_ip
 *  @brief  The simple_ip class owns a CCI param
 */
SC_MODULE(ex03_simple_ip) {
 public:
  /**
   *  @fn     void ex03_simple_ip()
   *  @brief  the class constructor
   *  @return void
   */
  SC_CTOR(ex03_simple_ip)
  // Initialize struc_param with 0
      : struc_param("struc_param", 0) {
    SC_THREAD(execute);
    XREPORT("Ctor: Default value of " << struc_param.name() << " is "
            << struc_param);
  }

  /**
   *  @fn     void ~ex03_simple_ip()
   *  @brief  The destructor for the class
   */
  ~ex03_simple_ip() {
    XREPORT_PLAIN("Dtor: Current value of " << struc_param.name() << " is "
                  << struc_param);
  }

  /**
   *  @fn     void before_end_of_elaboration()
   *  @brief  Before end of elaboration function to update the param value.
   *  @return void
   */
  void before_end_of_elaboration() {
    // Update struc_param value to 1 (valid)
    XREPORT("Before_End_Elab: Set value of struc_param to 1");
    struc_param = 1;
    if (0 == struc_param) {
      XREPORT_ERROR("Before_End_Elab: Value of struc_param"
                    " remains unchanged " << struc_param);
    } else if (1 == struc_param) {
      XREPORT("Before_End_Elab: Value of struc_param changed to "
              << struc_param);
    } else {
      XREPORT_ERROR("Before_End_Elab: Invalid update to struc_param,"
                    " value changed to " << struc_param);
      sc_assert(0);
    }
  }

  /**
   *  @fn     void execute()
   *  @brief  The main execution block (no real functionality)
   *  @return void
   */
  void execute() {
    // Wait for 10ns to allow config_ip to update int_param value
    wait(10, sc_core::SC_NS);

    // Update struc_param value to 2 (invalid)
	XREPORT("execute: Set value of struc_param to 2");
	struc_param = 2;


    if (1 == struc_param) {
      XREPORT("execute: Value of struc_param unchanged " << struc_param);
    } else if (2 == struc_param) {
      XREPORT_WARNING("execute: Value of struc_param changed to "
                      << struc_param);
    } else {
      XREPORT_ERROR("execute: Invalid update to struc_param,value changed to "
                    << struc_param);
      sc_assert(0);
    }
  }

 private:
  cci::cci_param<int, cci::CCI_MUTABLE_PARAM> struc_param; ///< CCI param
  /**
   *  @fn     void end_of_elaboration()
   *  @brief  end of elaboration function to lock structural param
   *  @return void
   */
  void end_of_elaboration() {
    struc_param.lock();
  }

};
// ex03_simple_ip

#endif  // EXAMPLES_EX03_ELAB_TIME_PARAM_EX03_SIMPLE_IP_H_
