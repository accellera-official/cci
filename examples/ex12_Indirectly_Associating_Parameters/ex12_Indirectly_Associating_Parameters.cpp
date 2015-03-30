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
 * @brief     This file instantiates the TOP_MODULE and the PARAMETER_CONFIGURATOR 
 * @author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * @date      9th June, 2011 (Thursday)
 */

#include "ex12_top_module.h"
#include "ex12_parameter_configurator.h"

/// Testbench which instantiates a top module and a parameter configurator
/// the top module will instantiate the rest of the platform
int sc_main(int sc_argc, char* sc_argv[]) {
  /// Instantiation of the owner and configurator sc_modules
  ex12_top_module top_mod("top_mod");
  ex12_parameter_configurator param_cfgr("param_cfgr");

  // Start and run the simulation till 50 nanoseconds
  sc_core::sc_start(30.0, sc_core::SC_NS);

  return EXIT_SUCCESS;
}
// sc_main
