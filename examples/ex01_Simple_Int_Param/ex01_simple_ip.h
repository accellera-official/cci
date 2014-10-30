/*******************************************************************************
 The following code is derived, directly or indirectly, from the SystemC
 source code Copyright (c) 1996-2010 by all Contributors.
 All Rights reserved.

 The contents of this file are subject to the restrictions and limitations
 set forth in the SystemC Open Source License Version 3.0 (the "License");
 You may not use this file except in compliance with such restrictions and
 limitations. You may obtain instructions on how to receive a copy of the
 License at http://www.systemc.org/. Software distributed by Contributors
 under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 ANY KIND, either express or implied. See the License for the specific
 language governing rights and limitations under the License.
 ******************************************************************************/

/**
 *  @file   simple_ip.h
 *  @brief  A simple IP that has a mutable integer parameter
 *  @author R. Swaminathan, TI
 */

#ifndef EXAMPLES_EX01_SIMPLE_INT_PARAM_EX01_SIMPLE_IP_H_
#define EXAMPLES_EX01_SIMPLE_INT_PARAM_EX01_SIMPLE_IP_H_

#include <cci>
#include "xreport.hpp"

/// The SimpleIP owns a CCI param
SC_MODULE(ex01_simple_ip) {
 public:
  /// Ctor
  SC_CTOR(ex01_simple_ip)
  // Initialize int_param with 0
      : int_param("int_param", 0) {
    SC_THREAD(execute);
    XREPORT("Ctor: Default value of " << int_param.get_name() << " is "
            << int_param);
  }

  /// Dtor
  ~ex01_simple_ip() {
    XREPORT_PLAIN("Dtor: Current value of " << int_param.get_name() << " is "
                  << int_param);
  }

  /// Main execution block (no functionality)
  void execute() {
    // Wait for 10ns to allow config_ip to update int_param value
    wait(10, sc_core::SC_NS);

    // Update int_param value to 1
    XREPORT("execute: Set value of int_param to 1");
    int_param = 1;
    XREPORT("execute: Current value of int_param is "<< int_param);
  }

 private:
  /// CCI param to hold buffer size
  cci::cnf::cci_param<int> int_param;
};
// ex01_simple_ip

#endif  // EXAMPLES_EX01_SIMPLE_INT_PARAM_EX01_SIMPLE_IP_H_

