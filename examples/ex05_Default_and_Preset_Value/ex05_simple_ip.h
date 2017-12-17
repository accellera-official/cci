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
 *  @brief  A testbench that demonstrates how to use default values and init values 
 *          for an integer parameter
 *  @author Vishal Goel, Texas Instruments
 */

#ifndef EXAMPLES_EX05_DEFAULT_AND_PRESET_VALUE_EX05_SIMPLE_IP_H_
#define EXAMPLES_EX05_DEFAULT_AND_PRESET_VALUE_EX05_SIMPLE_IP_H_

#include <cci_configuration>
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
    SC_THREAD(execute);
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

  static bool cfg_ip_preset_cci_value_predicate(
          const std::pair<std::string, cci::cci_value>& preset_cci_value)
  {
    return (preset_cci_value.first.substr(0, 7) == "cfg_ip.");
  }

  static bool preset_cci_value_2_predicate(
          const std::pair<std::string, cci::cci_value>& preset_cci_value)
  {
    return (preset_cci_value.second == cci::cci_value(2));
  }

  void execute() {
      cci::cci_broker_handle broker = cci::cci_get_broker();

      XREPORT("Setting up param_implicit_1's preset value to 1");
      broker.set_preset_cci_value("param_implicit_1",
                                   cci::cci_value(1));

      XREPORT("Setting up foo.bar.baz.param_implicit_2's preset value to 2");
      broker.set_preset_cci_value("foo.bar.baz.param_implicit_2",
                                   cci::cci_value(2));

      XREPORT("Unconsumed preset cci values:");
      std::vector<std::pair<std::string, cci::cci_value> >
              unconsumed_preset_cci_values =
              broker.get_unconsumed_preset_values();
      for(std::vector<std::pair<std::string, cci::cci_value> >::iterator it =
              unconsumed_preset_cci_values.begin();
          it != unconsumed_preset_cci_values.end(); ++it) {
          std::cout << it->first << std::endl;
      }

      XREPORT("Ignore unconsumed values cfg_ip.*");
      broker.ignore_unconsumed_preset_values(
              &ex05_simple_ip::cfg_ip_preset_cci_value_predicate);

      XREPORT("Unconsumed preset cci values result:");
      std::vector<std::pair<std::string, cci::cci_value> >
              unconsumed_preset_cci_values_without_cfg_ip =
              broker.get_unconsumed_preset_values();
      for(std::vector<std::pair<std::string, cci::cci_value> >::iterator it =
              unconsumed_preset_cci_values_without_cfg_ip.begin();
          it != unconsumed_preset_cci_values_without_cfg_ip.end(); ++it) {
          std::cout << it->first << std::endl;
      }

      XREPORT("Unconsumed preset cci values with value = 2:");
      cci::cci_preset_value_predicate pred_val(
              &ex05_simple_ip::preset_cci_value_2_predicate);
      cci::cci_preset_value_range init_filtered_range =
              broker.get_unconsumed_preset_values(pred_val);
      for(cci::cci_preset_value_range::iterator it =
              init_filtered_range.begin();
          it != init_filtered_range.end();
          ++it) {
          std::cout << (*it).first << std::endl;
      }
  }

 private:
  cci::cci_param<int> param_1; ///< CCI param
  cci::cci_param<int> param_2; ///< CCI param
};
// ex05_simple_ip

#endif  // EXAMPLES_EX05_DEFAULT_AND_PRESET_VALUE_EX05_SIMPLE_IP_H_
