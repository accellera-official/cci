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
 *  @file      main.cpp
 *  @brief     This file instantiates the TOP_MODULE and the PARAMETER_CONFIGURATOR
 *  @author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date      9th June, 2011 (Thursday)
 */

#include "ex11_top_module.h"
#include "ex11_parameter_configurator.h"

#include <cci_configuration>

/**
 *  @fn     int sc_main(int argc, char* argv[])
 *  @brief  Testbench that instantiates the ex11_top_module, which then
 *          instantiates the remained of the platform
 */
int sc_main(int sc_argc, char* sc_argv[]) {
  cci::cci_register_broker(new cci_utils::broker("My Global Broker"));
  // Instantiation of the owner and configurator sc_modules
  ex11_top_module top_mod("top_mod");
  ex11_parameter_configurator param_cfgr("param_cfgr");

  // Start and run the simulation
  SC_REPORT_INFO("sc_main", "Begin Simulation.");
  sc_core::sc_start(30.0, sc_core::SC_NS);
  SC_REPORT_INFO("sc_main", "End Simulation.");

  return EXIT_SUCCESS;
}
// sc_main
