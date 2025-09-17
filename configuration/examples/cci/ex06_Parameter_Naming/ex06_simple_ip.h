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
 *  @file   ex06_simple_ip.h
 *  @brief  A simple IP that has a mutable integer parameter
 *  @author R. Swaminathan, TI
 */

#ifndef EXAMPLES_EX06_PARAMETER_NAMING_EX06_SIMPLE_IP_H_
#define EXAMPLES_EX06_PARAMETER_NAMING_EX06_SIMPLE_IP_H_

#include <cci_configuration>
#include "xreport.hpp"

/**
 *  @class  ex06_simple_sub_ip
 *  @brief  A simple sub IP class to register a name in SystemC hierarchy
 */
SC_MODULE(ex06_simple_sub_ip) {
        SC_CTOR(ex06_simple_sub_ip) {}
};

/**
 *  @class  ex06_simple_ip
 *  @brief  The simple ip owns a CCI param
 */
SC_MODULE(ex06_simple_ip) {
 public:
  /**
   *  @fn     ex06_simple_ip
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex06_simple_ip)
  // Initialize int_param with 0
  // Param name - In line with SystemC Hierarchy
      : sub_ip("sub_ip"),
        bool_param("sub_ip", true),
        int_param_ip("int_param_ip", 1),
        // Param name (Duplicate)
        int_param_ip_dup(
            "sim_ip.int_param_ip",
            2,
	    "An int parameter given a duplicate absolute name.",
            cci::CCI_ABSOLUTE_NAME),
        // Param name - Absolute
        int_param_top(
            "int_param_top",
            3,
	    "An int parameter given an absolute name.",
            cci::CCI_ABSOLUTE_NAME),
        // Param name - Custom
        int_param_custom(
            "top.sub.int_param_custom",
            4,
	    "An int parameter with name outside of module hierarchy.",
            cci::CCI_ABSOLUTE_NAME)
  {
    SC_THREAD(execute);
    XREPORT("Ctor: Name of bool_param is " << bool_param.name());
    XREPORT("Ctor: Default value of " << int_param_ip.name() << " is "
            << int_param_ip);
    XREPORT("Ctor: Default value of " << int_param_ip_dup.name() << " is "
            << int_param_ip_dup);
    XREPORT("Ctor: Default value of " << int_param_top.name() << " is "
            << int_param_top);
    XREPORT("Ctor: Default value of " << int_param_custom.name() << " is "
            << int_param_custom);
  }

  /**
   *  @fn     ~ex06_simple_ip()
   *  @brief  The class destructor
   *  @return void
   */
  ~ex06_simple_ip() {
    XREPORT_PLAIN("Dtor: Current value of " << int_param_ip.name()
                  << " is " << int_param_ip);
    XREPORT_PLAIN("Dtor: Current value of " << int_param_ip_dup.name()
                  << " is " << int_param_ip_dup);
    XREPORT_PLAIN("Dtor: Current value of " << int_param_top.name()
                  << " is " << int_param_top);
    XREPORT_PLAIN("Dtor: Current value of " << int_param_custom.name()
                  << " is " << int_param_custom);
  }

  /**
   *  @fn     void execute()
   *  @brief  The main execution block (no functionality)
   *  @return void
   */
  void execute() {
    // Wait for 10ns to allow config_ip to update int_param value
    wait(10, sc_core::SC_NS);

    XREPORT("execute: Current value of " << int_param_ip.name()
            << " is " << int_param_ip);
    XREPORT("execute: Current value of " << int_param_ip_dup.name()
            << " is " << int_param_ip_dup);
    XREPORT("execute: Current value of " << int_param_top.name()
            << " is " << int_param_top);
    XREPORT("execute: Current value of " << int_param_custom.name()
            << " is " << int_param_custom);

    // Update param values
    XREPORT("execute: Set int_param_ip = 10");
    XREPORT("execute: Set int_param_ip_dup = 20");
    XREPORT("execute: Set int_param_top = 30");
    XREPORT("execute: Set int_param_custom = 40");
    int_param_ip = 10;
    int_param_ip_dup = 20;
    int_param_top = 30;
    int_param_custom = 40;
    XREPORT("execute: Current value of " << int_param_ip.name()
            << " is " << int_param_ip);
    XREPORT("execute: Current value of " << int_param_ip_dup.name()
            << " is " << int_param_ip_dup);
    XREPORT("execute: Current value of " << int_param_top.name()
            << " is " << int_param_top);
    XREPORT("execute: Current value of " << int_param_custom.name()
            << " is " << int_param_custom);
  }

  ex06_simple_sub_ip sub_ip;
 private:
  cci::cci_param<bool> bool_param; ///< Duplicate name of sub_ip
  cci::cci_param<int> int_param_ip;  ///< CCI param to hold buffer size
  cci::cci_param<int> int_param_ip_dup;  ///< Duplicate of int_param_ip
  cci::cci_param<int> int_param_top; ///< CCI param
  cci::cci_param<int> int_param_custom;  ///< CCI param
};
// ex06_simple_ip

#endif  // EXAMPLES_EX06_PARAMETER_NAMING_EX06_SIMPLE_IP_H_
