/******************************************************************************** 
 * The following code is derived, directly or indirectly, from the SystemC
 * source code Copyright (c) 1996-2010 by all Contributors.
 * All Rights reserved.
 *
 * The contents of this file are subject to the restrictions and limitations
 * set forth in the SystemC Open Source License Version 2.2.0 (the "License");
 * One may not use this file except in compliance with such restrictions and
 * limitations.  One may obtain instructions on how to receive a copy of the
 * License at http://www.systemc.org/.  Software distributed by Contributors
 * under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 * ANY KIND, either express or implied. See the License for the specific
 * language governing rights and limitations under the License.
 ********************************************************************************/

#define SC_INCLUDE_DYNAMIC_PROCESSES

/*!
 * \file     main.cpp 
 * \brief    Testbench file
 *           This file declares and implements the functionality of the target.
 *           Few of the parameters of the target sc_module are configured by the 
 *           router sc_module.
 * \author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * \date     29th April, 2011 (Friday)
 */

#include <cci>
#include <cassert>
#include <string>
#include "ex09_top_module.h"

/// Testbench for the hierarchical override of parameter values
int sc_main(int sc_argc, char* sc_argv[]) {
  /// Instantiating originator to get access to the global broker
  std::string myString = "sc_main_originator";
  cci::cnf::cci_originator myOriginator(myString);

  /// Get handle to the default broker
  cci::cnf::cci_cnf_broker_if* myGlobalBroker =
      &cci::cnf::cci_broker_manager::get_current_broker(myOriginator);

  // Assert if broker handle returned is NULL
  assert(myGlobalBroker != NULL
         && "Handle of the returned global broker is NULL");

  SC_REPORT_INFO("sc_main",
                 "[MAIN] : Setting initial value of the number of initiators"
                 " to 2");

  /// Set initial value to the number of initiator(s) (within top_module)
  std::string initiatorHierarchicalName = "top_module_inst.number_of_initiators";
  myGlobalBroker->json_deserialize_initial_value(initiatorHierarchicalName, "2");

  SC_REPORT_INFO("sc_main", "[MAIN] : Setting initial value of the number"
                 " of initiators to 1");

  // The program considers only the last set initial value
  myGlobalBroker->json_deserialize_initial_value(initiatorHierarchicalName, "1");

  SC_REPORT_INFO("sc_main",
                 "[MAIN] : Setting initial value of the number of targets to 4");

  /// Set initial value to the number of target(s) (within top_module)
  std::string targetHierarchicalName = "top_module_inst.number_of_targets";
  myGlobalBroker->json_deserialize_initial_value(targetHierarchicalName, "4");

  /// Set the maximum addressing limit for the router
  myGlobalBroker->json_deserialize_initial_value(
      "top_module_inst.RouterInstance.addr_max", "1024");

  /// Set and lock the Router Table initials values for target_1
  //  These values have again been tried to set within the Top_MODULE
  //  @see top_module.h
  SC_REPORT_INFO("sc_main",
                 "[MAIN] : Set and lock Router Table target_1 contents");
  myGlobalBroker->json_deserialize_initial_value(
      "top_module_inst.RouterInstance.r_index_1", "1");
  myGlobalBroker->lock_initial_value(
      "top_module_inst.RouterInstance.r_index_1");

  SC_REPORT_INFO("sc_main",
                 "[MAIN] : Set and lock Router Table Start Address for target_1"
                 " to 128");
  myGlobalBroker->json_deserialize_initial_value(
      "top_module_inst.RouterInstance.r_sa_1", "128");
  myGlobalBroker->lock_initial_value("top_module_inst.RouterInstance.r_sa_1");

  SC_REPORT_INFO("sc_main",
                 "[MAIN] : Set and lock Router Table End Address for target_1"
                 " to 255");
  myGlobalBroker->json_deserialize_initial_value(
      "top_module_inst.RouterInstance.r_ea_1", "255");
  myGlobalBroker->lock_initial_value("top_module_inst.RouterInstance.r_ea_1");

  SC_REPORT_INFO("sc_main",
                 "[MAIN] : Instantiate top module after setting initial"
                 " values to top_module, router and target parameters");

  /// Instantiate TOP_MODULE responsible for creating the model hierarchy
  ex09_top_module top_mod("top_module_inst");

  /// Start the simulation
  sc_core::sc_start(1140, sc_core::SC_NS);

  return EXIT_SUCCESS;
}  // End of 'sc_main'
