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
 *  @file     router.h
 *  @brief    Definition of the router module.
 *            This file declares and implements the functionality of the router.
 *            Few of the parameters of the target and initiator sc_module(s) are
 *            configured by the router sc_module
 *  @authors  P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date     29th April, 2011 (Friday)
 */

#ifndef EXAMPLES_EX09_HIERARCHICAL_OVERRIDE_OF_PARAMETER_VALUES_EX09_ROUTER_H_
#define EXAMPLES_EX09_HIERARCHICAL_OVERRIDE_OF_PARAMETER_VALUES_EX09_ROUTER_H_

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

#include <cci_configuration>
#include <tlm>
#include <cassert>
#include <vector>
#include <sstream>
#include <iomanip>

#include "tlm_utils/multi_passthrough_target_socket.h"
#include "tlm_utils/multi_passthrough_initiator_socket.h"
#include "xreport.hpp"

/**
 *  @class  ex09_router
 *  @brief  Thes module implements a router functionality
 */
SC_MODULE(ex09_router) {
 public:
  // Declare tlm multi-passthrough sockets for target and initiator modules
  tlm_utils::multi_passthrough_target_socket<ex09_router, 32> Router_target;
  tlm_utils::multi_passthrough_initiator_socket<ex09_router, 32> Router_initiator;

  /**
   *  @fn     ex09_router
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex09_router)
      : Router_target("Router_target"),
        Router_initiator("Router_initiator"),
        r_initiators("r_initiators", 0),
        r_targets("r_targets", 0),
        addr_limit("addr_max", 64),
        addrSize(0) {
    XREPORT("[ROUTER C_TOR] ----- [ROUTER CONSTRUCTOR BEGINS HERE] ------");

    // Get handle of the broker
    myBrokerForRouter = &cci::cci_broker_manager::get_current_broker(
        cci::cci_originator(*this));

    // Assert if NULL
    assert(myBrokerForRouter != NULL
           && "Router broker handle returned is NULL");

    // Register b_transport
    Router_target.register_b_transport(this, &ex09_router::b_transport);
  }

  /**
   *  @fn     void before_end_of_elaboration(void)
   *  @brief  The router table information is filled in during this function
   *  @return void
   */
  void before_end_of_elaboration(void) {
    XREPORT("[ROUTER in beoe] : Number of initiator(s) : "
            << r_initiators.json_serialize());
    XREPORT("[ROUTER in beoe] : Number of target(s) : " << r_targets.get());
    XREPORT("[ROUTER in beoe] : Maximum Addressable Limit of the router : "
            << addr_limit.get());

    char targetName[10];      ///< Holds router table's fields' names
    addrSize = (unsigned int) (addr_limit.get() / r_targets);

    // Printing the Router Table contents
    XREPORT("============= ROUTER TABLE INFORMATION ==============");
    XREPORT("-----------------------------------------------------");
    XREPORT("| Target ID  | Start Addr |  End Addr  | Base Addr  |");
    XREPORT("-----------------------------------------------------");

    // Sets the contents of the routing table with (default) values
    // calculated within 'beoe' phase
    for (int i = 0; i < r_targets; i++) {
      snprintf(targetName, sizeof(targetName), "r_index_%d", i);
      r_target_index.push_back(
          new cci::cci_param<unsigned int, cci::elaboration_time_param>(
              targetName, i));

      snprintf(targetName, sizeof(targetName), "r_sa_%d", i);
      r_addr_start.push_back(
          new cci::cci_param<unsigned int, cci::elaboration_time_param>(
              targetName, (i * addrSize)));

      snprintf(targetName, sizeof(targetName), "r_ea_%d", i);
      r_addr_end.push_back(
          new cci::cci_param<unsigned int, cci::elaboration_time_param>(
              targetName, ((i + 1) * addrSize - 1)));
    }

    for (int i = 0; i < r_targets; i++) {
      snprintf(stringName, sizeof(stringName),
               "top_module_inst.target_%d.s_base_addr", i);

      if (myBrokerForRouter->param_exists(stringName)) {
        base_ptr = myBrokerForRouter->get_param(stringName);
        assert(base_ptr != NULL
               && "target Base Address Handle returned is NULL");
      }
	  std::stringstream row_ss;
	  row_ss << "| " << std::setw(10) << r_target_index[i]->get()
		     << " | " << std::setw(10) << std::hex << std::showbase << r_addr_start[i]->get()
		     << " | " << std::setw(10) << r_addr_end[i]->get() 
		     << " | " << std::setw(10) << base_ptr->json_serialize() << " |";
	XREPORT(row_ss.str().c_str());
    XREPORT("-----------------------------------------------------");
    }
  }

  // Blocking transport implementation of the router
  void b_transport(int i, tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
    wait(delay);

    delay = sc_core::SC_ZERO_TIME;

    sc_dt::uint64 addr = trans.get_address();

    if (addr >= static_cast<sc_dt::uint64>(addr_limit.get())) {
      trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
      return;
    }

    for (unsigned int i = 0; i < r_target_index.size(); i++) {
      if ((addr >= (r_addr_start[i]->get()))
          && (addr <= (r_addr_end[i]->get()))) {
        XREPORT("[Router in 'b_transport' layer]");
        XREPORT("Address       = " << std::hex << addr);
        XREPORT("Index         = " << (r_target_index[i])->get());
        XREPORT("Start addres  = " << std::hex << (r_addr_start[i]->get()));
        XREPORT("End   Address = " << std::hex << (r_addr_end[i]->get()));
        Router_initiator[(r_target_index[i])->get()]->b_transport(trans, delay);
        break;
      }
    }
  }

 private:
  /// Demonstrates Model-to-Model Configuration (UC12)
  /// Elaboration Time Parameters for setting up the model hierarcy;
  cci::cci_param<int, cci::elaboration_time_param> r_initiators;  ///< initiator ID assigned by the top_module upon instantiation
  cci::cci_param<int, cci::elaboration_time_param> r_targets; ///< target ID assigned by the top_module upon instantiation
  cci::cci_param<unsigned int, cci::mutable_param> addr_limit;  ///< Router Addressing Range
  cci::cci_broker_if* myBrokerForRouter; ///< CCI configuration broker

  /// Router Table contents holding targets related information
  std::vector<cci::cci_param<unsigned int,
                                  cci::elaboration_time_param> *> r_target_index;  ///< Router table target index
  std::vector<cci::cci_param<unsigned int,
                                  cci::elaboration_time_param> *> r_addr_start;  ///< Router table start address
  std::vector<cci::cci_param<unsigned int,
                                  cci::elaboration_time_param> *> r_addr_end;  ///< Router table end address

  cci::cci_base_param* base_ptr; ///< CCI base parameter for target base address

  int addrSize;
  char stringName[50];
};
// ex09_router

#endif  // EXAMPLES_EX09_HIERARCHICAL_OVERRIDE_OF_PARAMETER_VALUES_EX09_ROUTER_H_
