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
#include <assert.h>
#include <vector>
#include <sstream>
#include "xreport.hpp"

#include "ex09_Hierarchical_Override_of_Parameter_Values/ex09_master.h"
#include "ex09_Hierarchical_Override_of_Parameter_Values/ex09_slave.h"
#include "ex09_Hierarchical_Override_of_Parameter_Values/ex09_router.h"

/// This module instantiates a master, slave, and router
/// and binds them correctly for communication
SC_MODULE(ex09_top_module) {
 public:
  SC_CTOR(ex09_top_module)
      : n_masters("number_of_masters", 0),
        n_slaves("number_of_slaves", 0) {
    std::stringstream ss;

    XREPORT("[TOP_MODULE C_TOR] -- [TOP MODULE CONSTRUCTOR BEGINS HERE]");

    /// Get handle of the default broker
    myDefaultBroker = &cci::cnf::cci_broker_manager::get_current_broker(
        cci::cnf::cci_originator(*this));

    /// Assert if broker handle returned is NULL
    assert(myDefaultBroker != NULL
           && "Default broker accessed by TOP_MODULE is NULL");

    XREPORT("[TOP_MODULE C_TOR] :  Number of masters : "
            << n_masters.json_serialize());
    XREPORT("[TOP_MODULE C_TOR] :  Number of slaves : "
            << n_slaves.json_serialize());

    /// Set and lock the number of masters in Router Table
    /// to value passed from 'sc_main'
    myDefaultBroker->json_deserialize_initial_value(
        "top_module_inst.RouterInstance.r_masters", n_masters.json_serialize());
    myDefaultBroker->lock_initial_value(
        "top_module_inst.RouterInstance.r_masters");

    /// Set and lock the number of slaves in Router Table
    /// to value passed from 'sc_main'
    myDefaultBroker->json_deserialize_initial_value(
        "top_module_inst.RouterInstance.r_slaves", n_slaves.json_serialize());
    myDefaultBroker->lock_initial_value(
        "top_module_inst.RouterInstance.r_slaves");

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

    /// Creating instances of master(s)
    for (int i = 0; i < n_masters; i++) {
      snprintf(masterName, sizeof(masterName), "Master_%d", i);
      XREPORT("[TOP_MODULE C_TOR] : Creating Master : " << masterName);

      snprintf(stringMisc, sizeof(stringMisc), "%s.%s.master_ID", name(),
               masterName);

      myDefaultBroker->json_deserialize_initial_value(stringMisc, masterName);
      masterList.push_back(new ex09_master(masterName));

      ///     Binding of Master to Router
      XREPORT("[TOP MODULE C_TOR] : Binding Router_Initiator to "
              << masterName);
      masterList[i]->Master_socket.bind(routerInstance->Router_target);
    }

    // Defining slave size
    slaveSize = 128;

    /// Creating instances of slave(s)
    for (int i = 0; i < n_slaves; i++) {
      snprintf(slaveName, sizeof(slaveName), "Slave_%d", i);
      XREPORT("[TOP_MODULE C_TOR] : Creating Slave : " << slaveName);

      snprintf(stringMisc, sizeof(stringMisc), "%s.%s.slave_ID", name(),
               slaveName);
      myDefaultBroker->json_deserialize_initial_value(stringMisc, slaveName);

      // Set initial value for maximum slave size(memory)
      snprintf(stringMisc, sizeof(stringMisc), "%s.%s.s_size", name(),
               slaveName);
      ss.clear();
      ss.str("");
      ss << slaveSize;

      myDefaultBroker->json_deserialize_initial_value(stringMisc, ss.str());
      slaveList.push_back(new ex09_slave(slaveName));

      /// Binding Router to Slave
      XREPORT("[TOP MODULE C_TOR] : Binding Router_Initiator to " << slaveName);
      routerInstance->Router_initiator.bind(slaveList[i]->Slave_socket);
    }

    /// Try re-setting locked values for Router Table contents
    for (int i = 0; i < n_slaves; i++) {
      snprintf(slaveName, sizeof(slaveName), "%s.RouterInstance.r_index_%d",
               name(), i);
      ss.clear();
      ss.str("");
      ss << i;

      try {
        XREPORT("[TOP_MODULE C_TOR] : Re-setting fields of Slave_" << i);
        myDefaultBroker->json_deserialize_initial_value(slaveName, ss.str());
      } catch (sc_core::sc_report exception) {
        XREPORT("[ROUTER : Caught] : " << exception.what());
      }

      snprintf(slaveName, sizeof(slaveName), "%s.RouterInstance.r_sa_%d",
               name(), i);
      ss.clear();
      ss.str("");
      ss << (i * slaveSize);

      snprintf(slaveBaseAddr, sizeof(slaveBaseAddr), "%s.Slave_%d.s_base_addr",
               name(), i);
      myDefaultBroker->json_deserialize_initial_value(slaveBaseAddr, ss.str());

      try {
        XREPORT("[TOP_MODULE C_TOR] : Re-setting start addr of Slave_" << i);
        myDefaultBroker->json_deserialize_initial_value(slaveName, ss.str());
      } catch (sc_core::sc_report exception) {
        XREPORT("[ROUTER : Caught] : " << exception.what());
      }

      snprintf(slaveName, sizeof(slaveName), "%s.RouterInstance.r_ea_%d",
               name(), i);
      ss.clear();
      ss.str("");
      ss << ((i + 1) * slaveSize - 1);

      try {
        XREPORT("[TOP_MODULE C_TOR] : Re-setting end addr of Slave_" << i);
        myDefaultBroker->json_deserialize_initial_value(slaveName, ss.str());
      } catch (sc_core::sc_report exception) {
        XREPORT("[ROUTER : Caught] : " << exception.what());
      }
    }
  }

  ~ex09_top_module() {
    // @TODO De-allocate all master and slave properly
    if (!masterList.empty())
      masterList.clear();

    if (!slaveList.empty())
      slaveList.clear();
  }

 private:
  // Immutable type cci-parameters
  //!< Number of Masters to be instantiated
  cci::cnf::cci_param<int, cci::cnf::immutable_param> n_masters;
  //!< Number of Slaves to be instantiated
  cci::cnf::cci_param<int, cci::cnf::immutable_param> n_slaves;

  //!< Configuration broker instance
  cci::cnf::cci_cnf_broker_if* myDefaultBroker;

  //!< Declaration of a router pointer
  ex09_router* routerInstance;

  // STD::VECTORs for creating instances of master and slave
  //!< STD::VECTOR for masters
  std::vector<ex09_master*> masterList;
  //!< STD::VECTOR for slaves
  std::vector<ex09_slave*> slaveList;

  //!< Master_ID
  char masterName[50];
  //!< Slave_ID
  char slaveName[50];
  //!< String to be used for misc things
  char stringMisc[50];
  char slaveBaseAddr[50];

  //!< Address Value
  int addrValue;
  //!< Maximum Slave Size (initial value)
  int slaveSize;
  //!< Maximum Router Table's memory range
  int r_addr_max;
};
// ex09_top_module

#endif  // EXAMPLES_EX09_HIERARCHICAL_OVERRIDE_OF_PARAMETER_VALUES_EX09_TOP_MODULE_H_
