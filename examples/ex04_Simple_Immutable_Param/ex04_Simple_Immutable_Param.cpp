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
 *  @file   testbench.cpp
 *  @brief  A testbench that demonstrates immutable parameters
 *  @author Vishal Goel, TI
 */

#include "ex04_simple_ip.h"
#include "ex04_config_ip.h"
#include <cci_utils/broker.h>
using sc_core::sc_report_handler;
using sc_core::SC_LOG;
using sc_core::SC_DISPLAY;
using sc_core::SC_ERROR;

/**
 *  @fn     int sc_main(int argc, char* argv[])
 *  @brief  Testbench for the CCI simple immutable parameters example
 *  @param  argc  An integer containing the number of input arguments
 *  @param  argv  A list of the input arguments
 *  @return An integer representing the status of the function
 */
/// Testbench for the CCI simple immutable parameters example
int sc_main(int argc, char *argv[]) {
  cci::cci_register_broker(new cci_utils::broker("My Global Broker"));
  ex04_config_ip cfg_ip("cfg_ip");
  ex04_simple_ip sim_ip("sim_ip");

  // Inhibit simulation abort for expected errors
  sc_report_handler::set_actions("/Accellera/CCI/SET_PARAM_FAILED", SC_ERROR, SC_LOG | SC_DISPLAY);

  SC_REPORT_INFO("sc_main", "Begin Simulation");
  sc_core::sc_start();
  SC_REPORT_INFO("sc_main", "End Simulation");

  return EXIT_SUCCESS;
}
