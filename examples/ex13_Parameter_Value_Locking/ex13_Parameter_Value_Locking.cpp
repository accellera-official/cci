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
 * @file      main.cpp
 * @brief     This file instantiates the owner and configurator modules and
 *            sets the simulation time
 * @author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * @date      10th May, 2011
 */

#include "ex13_parameter_owner.h"
#include "ex13_parameter_configurator.h"

/// Testbench that instantiates an owner and configurator module
int sc_main(int sc_argc, char* sc_argv[]) {
  ex13_parameter_owner param_owner("param_owner");
  ex13_parameter_configurator param_cfgr("param_cfgr");

  // Setting simulation time and starting the simulation
  sc_core::sc_start(25.0, sc_core::SC_NS);

  return EXIT_SUCCESS;
}
// sc_main
