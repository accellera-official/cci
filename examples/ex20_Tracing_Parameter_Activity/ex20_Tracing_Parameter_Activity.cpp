/*****************************************************************************
  Copyright 2006-2014 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2010-2015 CircuitSutra Technologies Pvt. Ltd.
  All rights reserved.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 *****************************************************************************/

/**
 *  @file    main.cpp
 *  @brief   The observer class and the parameter_owner module are instantiated here
 *  @author  P V S Phaneendra, CircuitSutra Technologies <pvs@circuitsutra.com>
 *  @date    11th October, 2011
 */

#include "ex20_observer.h"
#include "ex20_parameter_owner.h"

/**
 *  @fn     int sc_main(int sc_argc, char* sc_argv[])
 *  @brief  Main testbench function which instantiates an observer and parameter owner.
 *  @param  sc_argc The number of input arguments
 *  @param  sc_argv The list of input arguments
 *  @return void
 */
int sc_main(int sc_argc, char* sc_argv[]) {
  // Instantiate the observer class
  ex20_observer observer_inst;

  // Instantiate the parameter_owner module
  ex20_parameter_owner param_owner("param_owner");

  // Simulation starts
  sc_core::sc_start(15.0, sc_core::SC_NS);

  return EXIT_SUCCESS;
}
// sc_main
