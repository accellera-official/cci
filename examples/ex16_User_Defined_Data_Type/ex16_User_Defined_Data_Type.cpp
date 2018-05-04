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
 *  @brief   This function instantiates a parameter OWNER, CONFIGURATOR and an
 *           OBSERVER class
 *  @author  P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 *  @date    12th September, 2011 (Monday)
 */

#include <cci_configuration>
#include <systemc.h>
#include <string>

#include "ex16_parameter_owner.h"
#include "ex16_parameter_configurer.h"
#include "ex16_observer.h"
#include <cci_utils/broker.h>

/**
 *  @fn     int sc_main(int sc_argc, char* sc_argv[])
 *  @brief  The main testbench function, instantiates an obserers, own, and configurer
 *  @param  sc_argc The number of input arguments
 *  @param  sc_argv The list of the input arhuments
 *  @return An interger representing the exit status of the function.
 */
int sc_main(int sc_argc, char* sc_argv[]) {
  cci::cci_register_broker(new cci_utils::broker("DEFAULT_BROKER"));
  // Creating an originator to access the global broker
  const std::string myOrgStr = "sc_main_originator";
  cci::cci_originator myOriginator(myOrgStr);

  // Get handle of the broker using the originator
  cci::cci_broker_handle globalBroker =
      cci::cci_get_global_broker(myOriginator);

  // Set preset value to the 'int_param' of 'parameter_owner' class before
  // their constructor begins
  SC_REPORT_INFO("sc_main", "[MAIN] : Setting preset value"
                 " 's_address:256,d_address:512,index:0' to UDT");

  // Demonstrating use of 'set_preset_cci_value' API to assign
  // preset value before the construction of the model hierarchy begins.
  std::string init_str("{\"s_address\":256,\"d_address\":512,\"index\":0}");
  globalBroker.set_preset_cci_value("param_owner.User_data_type_param", cci::cci_value::from_json(init_str));

  // Instantiation of sc_modules
  ex16_parameter_owner param_owner("param_owner");
  ex16_parameter_configurer param_cfgr("param_cfgr");

  // Instantiate the observer class
  ex16_observer observer_obj;

  SC_REPORT_INFO("sc_main", "Begin Simulation.");
  sc_core::sc_start(12.0, sc_core::SC_NS);
  SC_REPORT_INFO("sc_main", "End Simulation.");

  return EXIT_SUCCESS;
}
// sc_main
