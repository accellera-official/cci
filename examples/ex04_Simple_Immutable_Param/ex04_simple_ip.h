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
 *  @brief  A Simple IP that holds 2 immutable params
 *  @author Vishal Goel, TI
 */

#ifndef EXAMPLES_EX04_SIMPLE_IMMUTABLE_PARAM_EX04_SIMPLE_IP_H_
#define EXAMPLES_EX04_SIMPLE_IMMUTABLE_PARAM_EX04_SIMPLE_IP_H_

#include <cci_configuration>
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
    XREPORT("@Ctor: Default value specified for " << param_1.get_name()
            << " is 1");
    XREPORT("@Ctor: Default value specified for " << param_2.get_name()
            << " is 2");

    XREPORT("@Ctor: param_1 mutable type is "
                    << mutable_type_to_string(param_1.get_mutable_type()));
    XREPORT("@Ctor: param_2 mutable type is "
                    << mutable_type_to_string(param_2.get_mutable_type()));

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
      param_1.set_cci_value(cci::cci_value::from_json("10"));
    } catch (std::exception &x) {
      XREPORT_WARNING(x.what());
    }
    expect("@Run: ", "param_1", static_cast<int>(param_1), 100);

    // Attempt to set new value (20) to param_2
    try {
      XREPORT("@Run: Assign new value (20) to " << param_2.get_name());
      param_2.set_cci_value(cci::cci_value::from_json("20"));
    } catch (std::exception &x) {
      XREPORT_WARNING(x.what());
    }
    expect("@Run: ", "param_2", static_cast<int>(param_2), 2);
  }

  const std::string mutable_type_to_string(
      cci::param_mutable_type mutable_type) {
     switch(mutable_type) {
         case cci::mutable_param:
             return "mutable";
            break;
         case cci::immutable_param:
             return "immutable";
             break;
         case cci::elaboration_time_param:
             return "elaboration_time_param";
             break;
         case cci::other_param:
             return "other_param";
            break;
         default:
             return "unknow";
            break;
     }
 }

 private:
  cci::cci_param<int, cci::immutable_param> param_1;  ///< CCI immutable param
  cci::cci_param<int, cci::immutable_param> param_2;  ///< CCI immutable param
};
// ex04_simple_ip

#endif  // EXAMPLES_EX04_SIMPLE_IMMUTABLE_PARAM_EX04_SIMPLE_IP_H_
