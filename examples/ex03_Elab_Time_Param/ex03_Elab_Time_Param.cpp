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
 *  @brief  A testbench that demonstrates possible updates to a elaboration-time 
 *          integer parameter, which are subsequently locked.
 *  @detail sim_ip:
 *          1. Instantiates a structural param with a default value.
 *                 (A structural param is a normal mutable param which is
 *                  locked at the end of elaboration)
 *             a. Updates are made to this param at these simulation points:
 *                - Before end of elaboration (valid)
 *                - After start of simulation (invalid)
 *          2. Creates a struc_param_post_eoe, after end of elaboration with a default value.
 *             a. Updates are made to this param at these simulation points:
 *                - After start of simulation (invalid)        
 *          cfg_ip:
 *          1. Updates parameters viz. structure_param and struc_param_post_eoe, both updations are invalid.
 *
 *  @author Udaya Ranga, TI
 */

#include "ex03_simple_ip.h"
#include "ex03_config_ip.h"
#include <cci_utils/broker.h>
using sc_core::sc_report_handler;
using sc_core::SC_LOG;
using sc_core::SC_DISPLAY;
using sc_core::SC_ERROR;

/**
 *  @fn     int sc_main(int argc, char* argv[])
 *  @brief  Testbench for the CCI elab_time_param example
 *  @param  argc  An integer representing the number of input arguments
 *  @param  argv  A list of the input arguments
 *  @return An integer representing the status of the execution of the function
 */
int sc_main(int argc, char *argv[]) {
  cci::cci_register_broker(new cci_utils::broker("My Global Broker"));
  ex03_simple_ip sim_ip("sim_ip");
  ex03_config_ip cfg_ip("cfg_ip");

  // Inhibit simulation abort for expected errors
  sc_report_handler::set_actions("/Accellera/CCI/SET_PARAM_FAILED", SC_ERROR, SC_LOG | SC_DISPLAY);

  SC_REPORT_INFO("sc_main", "Begin Simulation.");
  sc_core::sc_start();
  SC_REPORT_INFO("sc_main", "End Simulation.");

  return EXIT_SUCCESS;
}
