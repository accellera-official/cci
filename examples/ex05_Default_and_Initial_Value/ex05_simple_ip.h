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
 *  @brief  A testbench that demonstrates how to use default values and init values 
 *          for an integer parameter
 *  @author Vishal Goel, Texas Instruments
 */

#ifndef EXAMPLES_EX05_DEFAULT_AND_INITIAL_VALUE_EX05_SIMPLE_IP_H_
#define EXAMPLES_EX05_DEFAULT_AND_INITIAL_VALUE_EX05_SIMPLE_IP_H_

#include <cci>
#include "xreport.hpp"

/// The SimpleIP owns a CCI param
SC_MODULE(ex05_simple_ip) {
 public:
  /// Ctor
  SC_CTOR(ex05_simple_ip)
  // Initialize param_1 with 1, and param_2 with 2
      : param_1("param_1", 1),
        param_2("param_2", 2) {
    XREPORT("@Ctor: Value of " << param_1.get_name() << " is " << param_1
            << " (Default value Specified: 1)");
    XREPORT("@Ctor: Value of " << param_2.get_name() << " is " << param_2
            << " (Default value Specified: 2)");
  }

  /// Dtor
  ~ex05_simple_ip() {
    XREPORT_PLAIN("@Dtor: Value of " << param_1.get_name() << " is "
                  << param_1);
    XREPORT_PLAIN("@Dtor: Value of " << param_2.get_name() << " is "
                  << param_2);
  }

 private:
  /// CCI params
  cci::cnf::cci_param<int> param_1;
  cci::cnf::cci_param<int> param_2;
};
// ex05_simple_ip

#endif  // EXAMPLES_EX05_DEFAULT_AND_INITIAL_VALUE_EX05_SIMPLE_IP_H_
