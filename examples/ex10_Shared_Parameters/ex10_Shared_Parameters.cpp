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

/*!
 * \file      main.cpp
 * \brief     This file shows the up-front planning needed to be done in order to
 *            access an owner class private memmbers accessible within the 
 *            configurator class by making the configurator a friend class of the owner
 * \author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com> 
 * \date      16th May, 2011 (Monday)
 */

#include "ex10_parameter_owner.h"
#include "ex10_parameter_configurator.h"

/// Testbench instantiates owner and configurator modules and shows up-front
/// planning required to make the owner's private memvers accessible within
/// the configurator
int sc_main(int sc_argc, char* sc_argv[]) {
  /// Instantiation of the owner and configurator sc_modules
  ex10_parameter_owner param_owner("param_owner");
  ex10_parameter_configurator param_cfgr("param_setter");

  /// The instance of the owner class being passed as an argument to an API
  /// of the configurator class will serve in getting references to the owner
  /// class protected/private members within the configurator later
  param_cfgr.set_cfgr_parameter(&param_owner);

  // Start and run the simulation till 50 nanoseconds
  sc_core::sc_start(50.0, sc_core::SC_NS);

  return EXIT_SUCCESS;
}
// sc_main
