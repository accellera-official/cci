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
 *  @file   ex19_simple_ip.h
 *  @brief  A simple module demonstrating parameter reset
 *  @author Trevor Wieman, Intel
 */

#ifndef EXAMPLES_EX19_RESET_PARAM_EX19_SIMPLE_IP_H_
#define EXAMPLES_EX19_RESET_PARAM_EX19_SIMPLE_IP_H_

#include <cci_configuration>
#include "xreport.hpp"

/**
 *  @class  ex19_simple_ip
 *  @brief  A simple IP that declares, updates, then resets parameters.
 */
SC_MODULE(ex19_simple_ip) {
 public:

  /**
   *  @fn     void ex19_simple_ip()
   *  @brief  The constructor for the class.
   *  @return void
   */
  SC_CTOR(ex19_simple_ip)
  // Initialize int_param with 0
      : paramA("paramA", 1, "mutable param without preset value") 
      , paramB("paramB", 2, "mutable param with preset value")
      , paramC("paramC", 3, "mutable locked param")
      , paramD("paramD", 4, "immutable param")

  {
    SC_THREAD(execute);

    XREPORT("Parameter construction complete.");

    // Lock a param to show it prevents reset().
    paramC.lock();
  }

  /**
   *  @fn     void execute()
   *  @brief  The main execution block
   *  @return void
   */
  void execute() {
    // Pass some time then reset the parameters
    wait(20, sc_core::SC_NS);
    XREPORT("Resetting parameters.");
    paramA.reset();
    paramB.reset();
    paramC.reset(); // no effect since paramC is locked
    paramD.reset();

    // Wait for config_ip to update preset values
    wait(20, sc_core::SC_NS);
    XREPORT("Resetting parameters.");
    paramA.reset();
    paramB.reset();
    paramC.reset(); // no effect since paramC is locked
    paramD.reset();

    // Pass more time then unlock paramC and reset it
    wait(10, sc_core::SC_NS);
    XREPORT("Unlocking and resetting paramC.");
    paramC.unlock();
    paramC.reset();
    paramC.lock();
  }

 private:
     cci::cci_param<int> paramA
         , paramB
         , paramC;
     cci::cci_param<int, cci::CCI_IMMUTABLE_PARAM> paramD;
};
// ex19_simple_ip

#endif  // EXAMPLES_EX19_RESET_PARAM_EX19_SIMPLE_IP_H_

