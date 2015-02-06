/*********************************************************************************
 *   The following code is derived, directly or indirectly, from the SystemC
 *   source code Copyright (c) 1996-2010 by all Contributors.
 *   All Rights reserved.
 *
 *   The contents of this file are subject to the restrictions and limitations
 *   set forth in the SystemC Open Source License Version 2.2.0 (the "License");
 *   One may not use this file except in compliance with such restrictions and
 *   limitations.  One may obtain instructions on how to receive a copy of the
 *   License at http://www.systemc.org/.  Software distributed by Contributors
 *   under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 *   ANY KIND, either express or implied. See the License for the specific
 *   language governing rights and limitations under the License.
 *******************************************************************************/

/**
 * @file    main.cpp
 * @brief   This function instantiates a parameter OWNER, CONFIGURATOR and an
 *          OBSERVER class
 * @author  P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date    12th September, 2011 (Monday)
 */

#include <cci>
#include <systemc.h>
#include <cassert>
#include <string>

#include "ex16_parameter_owner.h"
#include "ex16_parameter_configurer.h"
#include "ex16_observer.h"

/// Main testbanch function, instantiates an observers, owner, and configurer
int sc_main(int sc_argc, char* sc_argv[]) {
  // Creating an originator to access the global broker
  const std::string myOrgStr = "sc_main_originator";
  cci::cnf::cci_originator myOriginator(myOrgStr);

  // Get handle of the broker using the originator
  cci::cnf::cci_cnf_broker_if* globalBroker =
      &cci::cnf::cci_broker_manager::get_current_broker(myOriginator);

  // Assert if broker handle returned is NULL
  assert(globalBroker != NULL
         && "Reference to global broker returned (in sc_main) is NULL.");

  // Set initial value to the 'int_param' of 'parameter_owner' class before
  // their constructor begins
  SC_REPORT_INFO("sc_main", "[MAIN] : Setting initial value"
                 " 's_address:256,d_address:512,index:0' to UDT");

  /// Demonstrating use of 'json_deserialize_initial_value' API to assign
  /// initial value before the construction of the model hierarchy begins.
  std::string init_str("{\"s_address\":256,\"d_address\":512,\"index\":0}");
  globalBroker->json_deserialize_initial_value("param_owner.User_data_type_param", init_str);

  /// Instantiation of sc_modules
  ex16_parameter_owner param_owner("param_owner");
  ex16_parameter_configurer param_cfgr("param_cfgr");

  /// Instantiate the observer class
  ex16_observer observer_obj;

  sc_start(12.0, SC_NS);

  return EXIT_SUCCESS;
}
// sc_main
