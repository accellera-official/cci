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
 *  @file     initiator.h
 *  @brief    initiator module implementation.
 *            This file declares and implements the functionality of the initiator.
 *            Few of the parameters of the initiator sc_module are configured by the
 *            router sc_module
 *  @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *            Parvinder Pal Singh, CircuitSutra Technologies   <parvinder@circuitsutra.com>
 *  @date     29th April, 2011 (Friday)
 */

#ifndef EXAMPLES_EX09_HIERARCHICAL_OVERRIDE_OF_PARAMETER_VALUES_EX09_INITIATOR_H_
#define EXAMPLES_EX09_HIERARCHICAL_OVERRIDE_OF_PARAMETER_VALUES_EX09_INITIATOR_H_

#include <cci_configuration>
#include <tlm>
#include <string>
#include "tlm_utils/simple_initiator_socket.h"
#include "xreport.hpp"

/**
 *  @class  ex09_initiator
 *  @brief  The implementation of the initiator module with tlm2 socket for communication
 *  @return void
 */
SC_MODULE(ex09_initiator) {
 public:
  int data;

  tlm_utils::simple_initiator_socket<ex09_initiator, 32> initiator_socket;  ///< Instance of TLM2 simple initiator socket

  /**
   *  @fn     ex09_initiator
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex09_initiator)
      : data(0),
        initiator_socket("initiator_socket"),
        initiator_ID("initiator_ID", "initiator_default") {
    XREPORT("[" << initiator_ID.get()
            << " C_TOR] ------- [INITIATOR CONSTRUCTOR BEGINS HERE] --------");

    // initiator's SC_THREAD declaration
    SC_THREAD(run_initiator);
  }

  /**
   *  @fn     void run_initiator(void)
   *  @brief  Main function to send transactions
   *  @return void
   */
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
  cci::cci_param<std::string, cci::elaboration_time_param> initiator_ID;  ///< Elab Time Param for assigning initiator ID (initialized by top_module)
};
// ex09_initiator

#endif  // EXAMPLES_EX09_HIERARCHICAL_OVERRIDE_OF_PARAMETER_VALUES_EX09_INITIATOR_H_
