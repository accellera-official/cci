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
 *  @brief  A Simple IP that holds 2 immutable params
 *  @author Vishal Goel, TI
 */

#ifndef EXAMPLES_EX04_SIMPLE_IMMUTABLE_PARAM_EX04_SIMPLE_IP_H_
#define EXAMPLES_EX04_SIMPLE_IMMUTABLE_PARAM_EX04_SIMPLE_IP_H_

#include <cci>
#include "xreport.hpp"

/**
 *  @class  ex04_simple_ip
 *  @brief  The simple ip class owns the CCI immutable params
 */
SC_MODULE(ex04_simple_ip) {
 public:
  /**
   *  @fn     void ex04_simple_ip()
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex04_simple_ip)
      : param_1("param_1", 1),
        param_2("param_2", 2) {
    SC_THREAD(execute);
    XREPORT("@Ctor: Default vale specified for " << param_1.get_name()
            << " is 1");
    XREPORT("@Ctor: Default vale specified for " << param_2.get_name()
            << " is 2");

    expect("Ctor: immutable param:", "param_1", static_cast<int>(param_1), 100);
    expect("Ctor: immutable param:", "param_2", static_cast<int>(param_2), 2);
  }

  /**
   *  @fn     void expect(const char* phase, const char* key, const int val, const int exp)
   *  @brief  Compares the expected value with the actual value
   *  @param  phase Current phase
   *  @param  key Parameter key used to lookup the parameter
   *  @param  val The value of the parameter
   *  @param  exp The expected value of the parameter
   *  @return void
   */
  //! @note remove this function after the CCI implementation is in place
  void expect(const char* phase, const char *key, const int val,
              const int exp) {
    if (val != exp) {
      XREPORT_PLAIN("Warning: " << phase << key << " Expected to have value = "
                    << exp << " but has " << val << ". TBD?");
    } else {
      XREPORT_PLAIN("Info: " << phase << key << " = " << val << " (expected)");
    }
  }

  /**
   *  @fn     void execute()
   *  @brief  The main execution block
   *  @return void
   */
  void execute() {
    wait(20, sc_core::SC_NS);

    // Display current value of bufsiz
    expect("@Run: ", "param_1", static_cast<int>(param_1), 100);
    expect("@Run: ", "param_2", static_cast<int>(param_2), 2);

    // Attempt to set new value (10) to param_1
    try {
      XREPORT("@Run: Assign new value (10) to " << param_1.get_name());
      param_1.json_deserialize("10");
    } catch (std::exception &x) {
      XREPORT_WARNING(x.what());
    }
    expect("@Run: ", "param_1", static_cast<int>(param_1), 100);

    // Attempt to set new value (20) to param_2
    try {
      XREPORT("@Run: Assign new value (20) to " << param_2.get_name());
      param_2.json_deserialize("20");
    } catch (std::exception &x) {
      XREPORT_WARNING(x.what());
    }
    expect("@Run: ", "param_2", static_cast<int>(param_2), 2);
  }

 private:
  cci::cnf::cci_param<int, cci::cnf::immutable_param> param_1;  ///< CCI immutable param
  cci::cnf::cci_param<int, cci::cnf::immutable_param> param_2;  ///< CCI immutable param
};
// ex04_simple_ip

#endif  // EXAMPLES_EX04_SIMPLE_IMMUTABLE_PARAM_EX04_SIMPLE_IP_H_
