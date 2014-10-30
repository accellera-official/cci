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
 * \file     slave.h
 * \brief    Slave module implementation.
 *           This file declares and implements the functionality of the slave.
 *           Few of the parameters of the slave sc_module are configured by the 
 *           router sc_module
 * \author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *           Parvinder Pal Singh, CircuitSutra Technologies   <parvinder@circuitsutra.com>
 * \date     5th May, 2011 (Thursday)
 */

#ifndef EXAMPLES_EX09_HIERARCHICAL_OVERRIDE_OF_PARAMETER_VALUES_EX09_SLAVE_H_
#define EXAMPLES_EX09_HIERARCHICAL_OVERRIDE_OF_PARAMETER_VALUES_EX09_SLAVE_H_

#include <cci>
#include <tlm>
#include <string>
#include "tlm_utils/simple_target_socket.h"
#include "xreport.hpp"

/// This module implements the functionality of a slave IP
SC_MODULE(ex09_slave) {
 public:
  tlm_utils::simple_target_socket<ex09_slave, 32> Slave_socket;
  //!< latency
  sc_time read_latency, write_latency;

  SC_CTOR(ex09_slave)
      : Slave_socket("Slave_socket"),
        slave_ID("slave_ID", "slave_default"),
        s_base_addr("s_base_addr", 0),
        s_size("s_size", 256) {
    XREPORT("[" << slave_ID.json_serialize()
            << " C_TOR] ------- [SLAVE CONSTRUCTOR BEGINS HERE] --------");
    XREPORT("[" << slave_ID.json_serialize() << " C_TOR] : Base Address : "
            << s_base_addr.get());

    /// Register b_transport
    Slave_socket.register_b_transport(this, &ex09_slave::b_transport);

    write_latency = sc_time(3, SC_NS);
    read_latency = sc_time(5, SC_NS);

    mem = new int[s_size.get()];

    for (unsigned int i = 0; i < s_size.get(); i++)
      mem[i] = 0xAABBCCDD | i;

    /// Slave's SC_THREAD declaration
    SC_THREAD(run_slave);
  }

  void run_slave(void) {
  }

  /// Implementation of the blocking transport in the slave
  void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
    tlm::tlm_command cmd = trans.get_command();
    sc_dt::uint64 adr = trans.get_address() - s_base_addr.get();
    unsigned char* ptr = trans.get_data_ptr();
    unsigned int len = trans.get_data_length();
    unsigned char* byt = trans.get_byte_enable_ptr();
    unsigned int wid = trans.get_streaming_width();

    XREPORT("[SLAVE] : adr ---- " << hex << adr);
    XREPORT("[SLAVE] : base addr ---- " << hex << s_base_addr.get());

    // Check for storage address overflow
    if (adr > s_size.get()) {
      trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
      return;
    }

    // Target unable to support byte enable attribute
    if (byt) {
      trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
      return;
    }

    // Target unable to support streaming width attribute
    if (wid < len) {
      trans.set_response_status(tlm::TLM_BURST_ERROR_RESPONSE);
      return;
    }

    if (cmd == tlm::TLM_READ_COMMAND) {
      memcpy(ptr, &mem[adr], len);
      delay = delay + read_latency;
    } else if (cmd == tlm::TLM_WRITE_COMMAND) {
      memcpy(&mem[adr], ptr, len);
      delay = delay + write_latency;
    }

    trans.set_response_status(tlm::TLM_OK_RESPONSE);
  }

 private:
  // Elaboration Time Param for assigning slave ID (initialized by top_module)
  cci::cnf::cci_param<std::string, cci::cnf::elaboration_time_param> slave_ID;

  // Mutable time param for setting slave's base addr (initialized by router)
  cci::cnf::cci_param<int, cci::cnf::elaboration_time_param> s_base_addr;

  // Mutable time parameter for setting slave's size (initialized by router);
  cci::cnf::cci_param<unsigned int> s_size;

  int* mem;
};
// ex09_slave

#endif  // EXAMPLES_EX09_HIERARCHICAL_OVERRIDE_OF_PARAMETER_VALUES_EX09_SLAVE_H_
