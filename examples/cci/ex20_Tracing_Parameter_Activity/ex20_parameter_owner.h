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

#ifndef EXAMPLES_EX20_TRACING_PARAMETER_ACTIVITY_EX20_PARAMETER_OWNER_H_
#define EXAMPLES_EX20_TRACING_PARAMETER_ACTIVITY_EX20_PARAMETER_OWNER_H_

#include <cci_configuration>
#include <systemc.h>

#include "xreport.hpp"

/**
 *  @class  ex20_parameter_owner
 *  @brief  The class declares and defines 'int' and 'float' type cci parameters
 */
SC_MODULE(ex20_parameter_owner) {
 public:
  /**
   *  @fn     ex20_parameter_owner()
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex20_parameter_owner) {
    XREPORT("Prior to " << sc_time_stamp());

    XREPORT("[OWNER C_TOR] : Creating new integer type cci-parameter with"
            " default value 10");
    int_param = new cci::cci_param<int>("int_param", 10);

    SC_THREAD(run_owner);
  }

  /**
   *  @fn     void run_owner(void)
   *  @brief  Implementation of SC_THREAD. Sets new value to the int type cci-parameter
   *          and sets a default value to the float type cci-parameter
   *  @return void
   */
  void run_owner(void) {
    while (1) {
      XREPORT("@ " << sc_time_stamp());
      XREPORT("[OWNER] : Setting new value to the 'int' type param to '15'");
      *int_param = 15;

      wait(5.0, SC_NS);
      XREPORT("@ " << sc_time_stamp());

      XREPORT("[OWNER] : Creating new 'double' type cci-parameter with default"
              " value : 12.345");
      dbl_param = new cci::cci_param<double>("double_param", 12.345);

      wait(15.0, SC_NS);
    }
  }

 private:
  // Integer-type cci-parameter
  cci::cci_param<int>* int_param;  ///< An integer cci parameter
  // Float-type cci-parameter
  cci::cci_param<double>* dbl_param; ///< A double cci parameter
};
// ex20_parameter_owner

#endif  // EXAMPLES_EX20_TRACING_PARAMETER_ACTIVITY_EX20_PARAMETER_OWNER_H_
