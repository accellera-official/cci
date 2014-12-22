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

/*!
 * \file     top_module.h
 * \brief    Implementation the TOP_MODULE.
 *           This header contains code related to the top module which decides
 *           the model hierarchy for example#9.  
 * \author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *           Girish Verma, CircuitSutra Technologies      <girish@circuitsutra.com>
 *           Parvinder Pal Singh, CircuitSutra Technologies   <parvinder@circuitsutra.com>
 * \date     29th April, 2011 (Friday)
 */

#ifndef EXAMPLES_EX09_HIERARCHICAL_OVERRIDE_OF_PARAMETER_VALUES_EX09_TOP_MODULE_H_
#define EXAMPLES_EX09_HIERARCHICAL_OVERRIDE_OF_PARAMETER_VALUES_EX09_TOP_MODULE_H_

#include <cci>
#include <tlm>
#include <cassert>
#include <vector>
#include <sstream>
#include "xreport.hpp"

#include "ex09_initiator.h"
#include "ex09_target.h"
#include "ex09_router.h"

/// This module instantiates a initiator, target, and router
/// and binds them correctly for communication
SC_MODULE(ex09_top_module) {
 public:
  SC_CTOR(ex09_top_module)
      : n_initiators("number_of_initiators", 0),
        n_targets("number_of_targets", 0) {
    std::stringstream ss;

    XREPORT("[TOP_MODULE C_TOR] -- [TOP MODULE CONSTRUCTOR BEGINS HERE]");

    /// Get handle of the default broker
    myDefaultBroker = &cci::cnf::cci_broker_manager::get_current_broker(
        cci::cnf::cci_originator(*this));

    /// Assert if broker handle returned is NULL
    assert(myDefaultBroker != NULL
           && "Default broker accessed by TOP_MODULE is NULL");

    XREPORT("[TOP_MODULE C_TOR] :  Number of initiators : "
            << n_initiators.json_serialize());
    XREPORT("[TOP_MODULE C_TOR] :  Number of targets : "
            << n_targets.json_serialize());

    /// Set and lock the number of initiators in Router Table
    /// to value passed from 'sc_main'
    myDefaultBroker->json_deserialize_initial_value(
        "top_module_inst.RouterInstance.r_initiators", n_initiators.json_serialize());
    myDefaultBroker->lock_initial_value(
        "top_module_inst.RouterInstance.r_initiators");

    /// Set and lock the number of targets in Router Table
    /// to value passed from 'sc_main'
    myDefaultBroker->json_deserialize_initial_value(
        "top_module_inst.RouterInstance.r_targets", n_targets.json_serialize());
    myDefaultBroker->lock_initial_value(
        "top_module_inst.RouterInstance.r_targets");

    /// Declaring and defining router module
    char routerName[15] = "RouterInstance";
    XREPORT("[TOP_MODULE C_TOR] : Creating Router : " << routerName);
    routerInstance = new ex09_router(routerName);

    /// Top_Module begins construction of the model hierarchy from here
    // ----------------------------------------------------------------

    if (myDefaultBroker->param_exists(
        "top_module_inst.RouterInstance.addr_limit")) {
      cci::cnf::cci_base_param* r_addr_limit_ptr = myDefaultBroker->get_param(
          "top_module_inst.RouterInstance.addr_limit");
      r_addr_max = atoi((r_addr_limit_ptr->json_serialize()).c_str());

      XREPORT("[TOP_MODULE C_TOR] : Router's maximum addressable limit : "
              << r_addr_max);
    }

    /// Creating instances of initiator(s)
    for (int i = 0; i < n_initiators; i++) {
      snprintf(initiatorName, sizeof(initiatorName), "initiator_%d", i);
      XREPORT("[TOP_MODULE C_TOR] : Creating initiator : " << initiatorName);

      snprintf(stringMisc, sizeof(stringMisc), "%s.%s.initiator_ID", name(),
               initiatorName);

      myDefaultBroker->json_deserialize_initial_value(stringMisc, initiatorName);
      initiatorList.push_back(new ex09_initiator(initiatorName));

      ///     Binding of initiator to Router
      XREPORT("[TOP MODULE C_TOR] : Binding Router_Initiator to "
              << initiatorName);
      initiatorList[i]->initiator_socket.bind(routerInstance->Router_target);
    }

    // Defining target size
    targetSize = 128;

    /// Creating instances of target(s)
    for (int i = 0; i < n_targets; i++) {
      snprintf(targetName, sizeof(targetName), "target_%d", i);
      XREPORT("[TOP_MODULE C_TOR] : Creating target : " << targetName);

      snprintf(stringMisc, sizeof(stringMisc), "%s.%s.target_ID", name(),
               targetName);
      myDefaultBroker->json_deserialize_initial_value(stringMisc, targetName);

      // Set initial value for maximum target size(memory)
      snprintf(stringMisc, sizeof(stringMisc), "%s.%s.s_size", name(),
               targetName);
      ss.clear();
      ss.str("");
      ss << targetSize;

      myDefaultBroker->json_deserialize_initial_value(stringMisc, ss.str());
      targetList.push_back(new ex09_target(targetName));

      /// Binding Router to target
      XREPORT("[TOP MODULE C_TOR] : Binding Router_Initiator to " << targetName);
      routerInstance->Router_initiator.bind(targetList[i]->target_socket);
    }

    /// Try re-setting locked values for Router Table contents
    for (int i = 0; i < n_targets; i++) {
      snprintf(targetName, sizeof(targetName), "%s.RouterInstance.r_index_%d",
               name(), i);
      ss.clear();
      ss.str("");
      ss << i;

      try {
        XREPORT("[TOP_MODULE C_TOR] : Re-setting fields of target_" << i);
        myDefaultBroker->json_deserialize_initial_value(targetName, ss.str());
      } catch (sc_core::sc_report const & exception) {
        XREPORT("[ROUTER : Caught] : " << exception.what());
      }

      snprintf(targetName, sizeof(targetName), "%s.RouterInstance.r_sa_%d",
               name(), i);
      ss.clear();
      ss.str("");
      ss << (i * targetSize);

      snprintf(targetBaseAddr, sizeof(targetBaseAddr), "%s.target_%d.s_base_addr",
               name(), i);
      myDefaultBroker->json_deserialize_initial_value(targetBaseAddr, ss.str());

      try {
        XREPORT("[TOP_MODULE C_TOR] : Re-setting start addr of target_" << i);
        myDefaultBroker->json_deserialize_initial_value(targetName, ss.str());
      } catch (sc_core::sc_report const & exception) {
        XREPORT("[ROUTER : Caught] : " << exception.what());
      }

      snprintf(targetName, sizeof(targetName), "%s.RouterInstance.r_ea_%d",
               name(), i);
      ss.clear();
      ss.str("");
      ss << ((i + 1) * targetSize - 1);

      try {
        XREPORT("[TOP_MODULE C_TOR] : Re-setting end addr of target_" << i);
        myDefaultBroker->json_deserialize_initial_value(targetName, ss.str());
      } catch (sc_core::sc_report const & exception) {
        XREPORT("[ROUTER : Caught] : " << exception.what());
      }
    }
  }

  ~ex09_top_module() {
    // @TODO De-allocate all initiator and target properly
    if (!initiatorList.empty())
      initiatorList.clear();

    if (!targetList.empty())
      targetList.clear();
  }

 private:
  // Immutable type cci-parameters
  //!< Number of initiators to be instantiated
  cci::cnf::cci_param<int, cci::cnf::immutable_param> n_initiators;
  //!< Number of targets to be instantiated
  cci::cnf::cci_param<int, cci::cnf::immutable_param> n_targets;

  //!< Configuration broker instance
  cci::cnf::cci_cnf_broker_if* myDefaultBroker;

  //!< Declaration of a router pointer
  ex09_router* routerInstance;

  // STD::VECTORs for creating instances of initiator and target
  //!< STD::VECTOR for initiators
  std::vector<ex09_initiator*> initiatorList;
  //!< STD::VECTOR for targets
  std::vector<ex09_target*> targetList;

  //!< initiator_ID
  char initiatorName[50];
  //!< target_ID
  char targetName[50];
  //!< String to be used for misc things
  char stringMisc[50];
  char targetBaseAddr[50];

  //!< Address Value
  int addrValue;
  //!< Maximum target Size (initial value)
  int targetSize;
  //!< Maximum Router Table's memory range
  int r_addr_max;
};
// ex09_top_module

#endif  // EXAMPLES_EX09_HIERARCHICAL_OVERRIDE_OF_PARAMETER_VALUES_EX09_TOP_MODULE_H_
