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
 *  @brief  A testbench that demonstrates Requesting and responding to value change notifications 
 *          for an integer parameter
 *  @author Sheshadri Chakravarthy, Texas Instruments
 */

#include "ex08_simple_ip.h"
#include "ex08_config_ip.h"
#include <cci_utils/broker.h>

/**
 *  @fn     int sc_main(int argc, char* argv[])
 *  @brief  The testbench for the CCI simple_int_param example
 *  @param  argc  The number of input arguments
 *  @param  argv  The list of input arguments
 *  @return An interger representing the execution status
 */
int sc_main(int argc, char *argv[]) {
  cci::cci_register_broker(new cci_utils::broker("My Global Broker"));
  ex08_config_ip cfg_ip("cfg_ip");
  ex08_simple_ip sim_ip("sim_ip");

  SC_REPORT_INFO("sc_main", "Begin Simulation.");
  sc_core::sc_start();
  SC_REPORT_INFO("sc_main", "End Simulation.");

  return EXIT_SUCCESS;
}
