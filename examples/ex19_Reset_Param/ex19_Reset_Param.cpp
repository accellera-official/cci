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
 *  @file   ex19_Reset_Param.cpp
 *  @brief  An example demonstrating parameter reset.
 *  @author Trevor Wieman, Intel
 */

#include "ex19_simple_ip.h"
#include "ex19_config_ip.h"
#include <cci_utils/broker.h>
#include "param_tracker.h"

/**
 *  @fn     int sc_main(int argc, char* argv[])
 *  @brief  The testbench for the CCI ex19_Reset_Param example
 *  @param  argc  An integer for the number of arguments; not used
 *  @param  argv  An array with the input arguments; not used
 *  @return and integer of successful execution
 */
int sc_main(int argc, char *argv[]) {
  // Initialize and register a global broker
  cci::cci_register_broker(new cci_utils::broker("My Global Broker"));

  // Instantiate the modules
  ex19_config_ip cfg_ip("cfg_ip");
  ex19_simple_ip sim_ip("sim_ip");

  // Attach trackers to the parameters
  cci::cci_originator originator("sc_main");
  cci::cci_broker_handle broker = cci::cci_get_global_broker(originator);
  param_tracker ptrkA(broker.get_param_handle("sim_ip.paramA"));
  param_tracker ptrkB(broker.get_param_handle("sim_ip.paramB"));
  param_tracker ptrkC(broker.get_param_handle("sim_ip.paramC"));
  param_tracker ptrkD(broker.get_param_handle("sim_ip.paramD"));

  // Start the simulation
  SC_REPORT_INFO("sc_main", "Begin Simulation.");
  sc_core::sc_start();
  SC_REPORT_INFO("sc_main", "End Simulation.");

  return EXIT_SUCCESS;
}
