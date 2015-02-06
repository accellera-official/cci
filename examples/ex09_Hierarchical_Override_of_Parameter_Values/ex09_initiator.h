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
 * \file     initiator.h
 * \brief    initiator module implementation.
 *           This file declares and implements the functionality of the initiator.
 *           Few of the parameters of the initiator sc_module are configured by the
 *           router sc_module
 * \author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *           Parvinder Pal Singh, CircuitSutra Technologies   <parvinder@circuitsutra.com>
 * \date     29th April, 2011 (Friday)
 */

#ifndef EXAMPLES_EX09_HIERARCHICAL_OVERRIDE_OF_PARAMETER_VALUES_EX09_INITIATOR_H_
#define EXAMPLES_EX09_HIERARCHICAL_OVERRIDE_OF_PARAMETER_VALUES_EX09_INITIATOR_H_

#include <cci>
#include <tlm>
#include <string>
#include "tlm_utils/simple_initiator_socket.h"
#include "xreport.hpp"

// Implemenation of the initiator module with tlm2 socket for communication
SC_MODULE(ex09_initiator) {
 public:
  int data;

  //!< Instance of TLM2 simple initiator socket
  tlm_utils::simple_initiator_socket<ex09_initiator, 32> initiator_socket;

  SC_CTOR(ex09_initiator)
      : data(0),
        initiator_socket("initiator_socket"),
        initiator_ID("initiator_ID", "initiator_default") {
    XREPORT("[" << initiator_ID.json_serialize()
            << " C_TOR] ------- [INITIATOR CONSTRUCTOR BEGINS HERE] --------");

    /// initiator's SC_THREAD declaration
    SC_THREAD(run_initiator);
  }

  void run_initiator(void) {
    tlm::tlm_generic_payload *trans = new tlm::tlm_generic_payload;

    int i = 0;

    while (1) {
      tlm::tlm_command cmd = static_cast<tlm::tlm_command>(rand() % 2);

      if (cmd == tlm::TLM_WRITE_COMMAND)
        data = 0xFF000000 | i;

      trans->set_command(cmd);
      trans->set_address(i);
      trans->set_data_ptr(reinterpret_cast<unsigned char*>(&data));
      trans->set_data_length(4);
      trans->set_streaming_width(4);
      trans->set_byte_enable_ptr(0);
      trans->set_dmi_allowed(false);
      trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
      sc_core::sc_time delay = sc_core::sc_time(0, sc_core::SC_NS);

      if (cmd == tlm::TLM_WRITE_COMMAND) {
        XREPORT("[Initiators Message]=>At address " << std::hex << i
                << " sending transaction with command = Write" << ", data="
                << std::hex << data << " at time " << sc_core::sc_time_stamp());
      } else {
        XREPORT("[Initiators Message]=>At address " << std::hex << i
                << " sending transaction with command= Read " << " at time "
                << sc_core::sc_time_stamp());
      }

      initiator_socket->b_transport(*trans, delay);

      if (trans->is_response_error())
        XREPORT_ERROR("TLM_2" << trans->get_response_string().c_str());

      if (delay.to_double() != 0)
        wait(delay);

      if (cmd == tlm::TLM_WRITE_COMMAND) {
        XREPORT("[Initiators Message]=>At address " << std::hex << i
                << " received response of Write transaction " << " at time "
                << sc_core::sc_time_stamp());
      } else {
        XREPORT("[Initiators Message]=>At address " << std::hex << i
                << " received response of Read transaction " << " data "
                << data << " at time " << sc_core::sc_time_stamp());
      }

      XREPORT("--------------------------------------------------------");

      wait(5.0, sc_core::SC_NS);

      i = i + 4;
    }
  }

 private:
  // Elab Time Param for assigning initiator ID (initialized by top_module)
  cci::cnf::cci_param<std::string, cci::cnf::elaboration_time_param> initiator_ID;
};
// ex09_initiator

#endif  // EXAMPLES_EX09_HIERARCHICAL_OVERRIDE_OF_PARAMETER_VALUES_EX09_INITIATOR_H_
