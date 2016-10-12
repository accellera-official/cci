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
 *  @brief     This is the main testbench file.  This file shows the up-front
 *             planning required to be done in order to access an owner class
 *             private memmbers accessible within the configurator class by
 *             making the configurator a friend class of the owner
 *  @author    Girish Verma, CircuitSutra Technologies     <girish@circuitsutra.com>
 *             P V S Phaneendra, CircuitSutra Technologies <pvs@circuitsutra.com>
 *  @date      14th December, 2011 (Wednesday).
 *             (Modified On) 16th May, 2011 (Monday)
 */

#include <systemc.h>

#include "ex19_parameter_owner.h"
#include "ex19_parameter_configurator.h"

/**
 *  @fn     int sc_main(int sc_argc, char* sc_argv[])
 *  @brief  This function instantiates owner and configurator class objects and shows
 *          details of the up-front planning required to make the owner class private
 *          members accesible within the configurator
 *  @param  sc_argc The number of input arguments
 *  @param  sc_argv The list of input arguments
 *  @return An integer representing the exit status of the function.
 */
int sc_main(int sc_argc, char* sc_argv[]) {
  // Instantiation of the owner and configurator sc_modules
  ex19_parameter_owner param_owner("param_owner");
  ex19_parameter_configurator param_cfgr("param_setter");

  // The instance of the owner class being passed as an argument to an API of
  // the configurator class will serve in getting references to the owner class
  // protected/private members within the configurator later
  param_cfgr.set_cfgr_parameter(&param_owner);

  // Start and run the simulation till 50 nanoseconds
  sc_core::sc_start(50.0, sc_core::SC_NS);

  return EXIT_SUCCESS;
}
// sc_main
