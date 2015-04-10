/*****************************************************************************
  Copyright 2006-2014 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2010-2015 Texas Instruments Inc.
  All rights reserved.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 *****************************************************************************/

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

/**
 *  @class  ex05_simple_ip
 *  @brief  The simple ip owns the CCI param
 */
SC_MODULE(ex05_simple_ip) {
 public:
  /**
   *  @fn     ex05_simple_ip
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex05_simple_ip)
  // Initialize param_1 with 1, and param_2 with 2
      : param_1("param_1", 1),
        param_2("param_2", 2) {
    XREPORT("@Ctor: Value of " << param_1.get_name() << " is " << param_1
            << " (Default value Specified: 1)");
    XREPORT("@Ctor: Value of " << param_2.get_name() << " is " << param_2
            << " (Default value Specified: 2)");
  }

  /**
   *  @fn     ~ex05_simple_ip()
   *  @brief  The class destructor
   *  @return void
   */
  ~ex05_simple_ip() {
    XREPORT_PLAIN("@Dtor: Value of " << param_1.get_name() << " is "
                  << param_1);
    XREPORT_PLAIN("@Dtor: Value of " << param_2.get_name() << " is "
                  << param_2);
  }

 private:
  cci::cnf::cci_param<int> param_1; ///< CCI param
  cci::cnf::cci_param<int> param_2; ///< CCI param
};
// ex05_simple_ip

#endif  // EXAMPLES_EX05_DEFAULT_AND_INITIAL_VALUE_EX05_SIMPLE_IP_H_
