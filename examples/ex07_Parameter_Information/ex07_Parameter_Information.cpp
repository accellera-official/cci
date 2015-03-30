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
 * \file    main.cpp
 * \brief   The main function starts here.
 *          The sc_main function instantiates 'PARAMETER_OWNER', 'PARAMETER_CONFIGURATOR'
 *          modules and 'OBSERVER' class
 * \author  P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * \date    12th September, 2011
 */
#include <systemc>
#include <cci>
#include <cassert>
#include <string>

#include "ex07_parameter_owner.h"
#include "ex07_parameter_configurer.h"
#include "ex07_observer.h"

/// Testbench for parameter information example
int sc_main(int sc_argc, char* sc_argv[]) {
  /// Creating an originator to access the global broker
  const std::string myOrgStr = "sc_main_originator";
  cci::cnf::cci_originator myOriginator(myOrgStr);

  /// Get handle of the broker using the originator
  cci::cnf::cci_cnf_broker_if* globalBroker =
      &cci::cnf::cci_broker_manager::get_current_broker(myOriginator);

  // Assert if broker handle returned is NULL
  assert(globalBroker != NULL
         && "Reference to global broker returned (in sc_main) is NULL.");

  // Set initial value to the 'int_param' of 'parameter_owner' class before
  // their constructor begins
  SC_REPORT_INFO("sc_main",
                 "[MAIN] : Setting 'param_owner.mutable_string_param'"
                 " value to 'Initialized within sc_main()'");

  /// Demonstrating use of 'json_deserialize_initial_value'
  /// API to assign initial value before the
  /// construction of the model hierarchy begins.
  globalBroker->json_deserialize_initial_value(
      "param_owner.mutable_string_param", "Initialized within sc_main");

  SC_REPORT_INFO("sc_main",
                 "[MAIN] : Demonstrating 'comparison' between the values"
                 " of a data type for different mutability types");

  /// Instantiate cci-parameters of all the three mutability types for a
  /// particular (say String) data-type

  cci::cnf::cci_param<std::string, cci::cnf::mutable_param>
      mutab_str_param("string_mutab_param", "String_Value_A");
  cci::cnf::cci_param<std::string, cci::cnf::immutable_param>
      immutab_str_param("string_immutab_param", "String_Value_A");
  cci::cnf::cci_param<std::string, cci::cnf::elaboration_time_param>
      elab_str_param("string_elab_param", "String_Value_B");

  if (mutab_str_param.get() == immutab_str_param.get()) {
    SC_REPORT_INFO("sc_main", "[MAIN] : 'mutable' & 'immutable' type String"
                   " parameters - VALUES MATCH");
  } else {
    SC_REPORT_INFO("sc_main", "[MAIN] : 'mutable' & 'immutable' type String"
                   " parameters - VALUES DO NOT MATCH");
  }

  if (mutab_str_param.get() == elab_str_param.get()) {
    SC_REPORT_INFO("sc_main", "[MAIN] : 'mutable' & 'elaboration_time' type"
                   " String - VALUES MATCH");
  } else {
    SC_REPORT_INFO("sc_main", "[MAIN] : 'mutable' & 'elaboration_time' type"
                   " String - VALUES DO NOT MATCH");
  }

  /// Infrastructure created within the example for example illustration
  SC_REPORT_INFO("sc_main", "[MAIN] : parameter_container module declares two"
                 " cci type parameters.");
  SC_REPORT_INFO("sc_main", "One is of 'Integer type' and the other is of"
                 " 'String type'");
  SC_REPORT_INFO("sc_main", "'Integer type' has 'pre_read', 'pre/post_write'"
                 " callback registered within the OBSERVER");

  /// Instantiation of sc_modules
  ex07_parameter_owner param_owner("param_owner");
  ex07_parameter_configurer param_cfgr("param_cfgr");

  /// Instantiating observer class
  ex07_observer observer_class("observer");

  /// Simulation time
  sc_core::sc_start(30.0, sc_core::SC_NS);

  return EXIT_SUCCESS;
}  // End of sc_main
