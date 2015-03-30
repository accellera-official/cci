/*******************************************************************************
 The following code is derived, directly or indirectly, from the SystemC
 source code Copyright (c) 1996-2013 by all Contributors.
 All Rights reserved.

 The contents of this file are subject to the restrictions and limitations
 set forth in the SystemC Open Source License Version 3.1 (the "License");
 You may not use this file except in compliance with such restrictions and
 limitations. You may obtain instructions on how to receive a copy of the
 License at http://www.accellera.org/. Software distributed by Contributors
 under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 ANY KIND, either express or implied. See the License for the specific
 language governing rights and limitations under the License.
 ******************************************************************************/

/**
 *  @file   ex06_simple_ip.h
 *  @brief  A simple IP that has a mutable integer parameter
 *  @author R. Swaminathan, TI
 */

#ifndef EXAMPLES_EX06_PARAMETER_NAMING_EX06_SIMPLE_IP_H_
#define EXAMPLES_EX06_PARAMETER_NAMING_EX06_SIMPLE_IP_H_

#include <cci>
#include "xreport.hpp"

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
      : int_param_ip("int_param_ip", 1),
        // Param name (Duplicate)
        int_param_ip_dup(
            "sim_ip.int_param_ip",
            2,
            cci::cnf::CCI_TOP_LEVEL_NAME,
            cci::cnf::cci_broker_manager::get_current_broker(
                cci::cnf::cci_originator(*this))),
        // Param name - Absolute
        int_param_top(
            "int_param_top",
            3,
            cci::cnf::CCI_TOP_LEVEL_NAME,
            cci::cnf::cci_broker_manager::get_current_broker(
                cci::cnf::cci_originator(*this))),
        // Param name - Custom
        int_param_custom(
            "top.sub.int_param_custom",
            4,
            cci::cnf::CCI_TOP_LEVEL_NAME,
            cci::cnf::cci_broker_manager::get_current_broker(
                cci::cnf::cci_originator(*this))) {
    SC_THREAD(execute);
    XREPORT("Ctor: Default value of " << int_param_ip.get_name() << " is "
            << int_param_ip);
    XREPORT("Ctor: Default value of " << int_param_ip_dup.get_name() << " is "
            << int_param_ip_dup);
    XREPORT("Ctor: Default value of " << int_param_top.get_name() << " is "
            << int_param_top);
    XREPORT("Ctor: Default value of " << int_param_custom.get_name() << " is "
            << int_param_custom);
  }

  /**
   *  @fn     ~ex06_simple_ip()
   *  @brief  The class destructor
   *  @return void
   */
  ~ex06_simple_ip() {
    XREPORT_PLAIN("Dtor: Current value of " << int_param_ip.get_name()
                  << " is " << int_param_ip);
    XREPORT_PLAIN("Dtor: Current value of " << int_param_ip_dup.get_name()
                  << " is " << int_param_ip_dup);
    XREPORT_PLAIN("Dtor: Current value of " << int_param_top.get_name()
                  << " is " << int_param_top);
    XREPORT_PLAIN("Dtor: Current value of " << int_param_custom.get_name()
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

    XREPORT("execute: Current value of " << int_param_ip.get_name()
            << " is " << int_param_ip);
    XREPORT("execute: Current value of " << int_param_ip_dup.get_name()
            << " is " << int_param_ip_dup);
    XREPORT("execute: Current value of " << int_param_top.get_name()
            << " is " << int_param_top);
    XREPORT("execute: Current value of " << int_param_custom.get_name()
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
    XREPORT("execute: Current value of " << int_param_ip.get_name()
            << " is " << int_param_ip);
    XREPORT("execute: Current value of " << int_param_ip_dup.get_name()
            << " is " << int_param_ip_dup);
    XREPORT("execute: Current value of " << int_param_top.get_name()
            << " is " << int_param_top);
    XREPORT("execute: Current value of " << int_param_custom.get_name()
            << " is " << int_param_custom);
  }

 private:
  cci::cnf::cci_param<int> int_param_ip;  ///< CCI param to hold buffer size
  cci::cnf::cci_param<int> int_param_ip_dup;  ///< Duplicate of int_param_ip
  cci::cnf::cci_param<int> int_param_top; ///< CCI param
  cci::cnf::cci_param<int> int_param_custom;  ///< CCI param
};
// ex06_simple_ip

#endif  // EXAMPLES_EX06_PARAMETER_NAMING_EX06_SIMPLE_IP_H_
