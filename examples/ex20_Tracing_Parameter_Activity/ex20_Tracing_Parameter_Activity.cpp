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
 *  @file    main.cpp
 *  @brief   The observer class and the parameter_owner module are instantiated here
 *  @author  P V S Phaneendra, CircuitSutra Technologies <pvs@circuitsutra.com>
 *  @date    11th October, 2011
 */

#include "ex20_observer.h"
#include "ex20_parameter_owner.h"
#include <cci_utils/broker.h>

/**
 *  @fn     int sc_main(int sc_argc, char* sc_argv[])
 *  @brief  Main testbench function which instantiates an observer and parameter owner.
 *  @param  sc_argc The number of input arguments
 *  @param  sc_argv The list of input arguments
 *  @return void
 */
int sc_main(int sc_argc, char* sc_argv[]) {
  cci::cci_register_broker(new cci_utils::broker("My Global Broker"));
  // Instantiate the observer class
  ex20_observer observer_inst;

  // Instantiate the parameter_owner module
  ex20_parameter_owner param_owner("param_owner");

  // Simulation starts
  SC_REPORT_INFO("sc_main", "Begin Simulation.");
  sc_core::sc_start(15.0, sc_core::SC_NS);
  SC_REPORT_INFO("sc_main", "End Simulation.");

  return EXIT_SUCCESS;
}
// sc_main
