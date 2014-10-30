/*******************************************************************************
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
 * *******************************************************************************/

#define SC_INCLUDE_DYNAMIC_PROCESSES
/*!
 * \file      main.cpp
 * \brief     This file instantiates the TOP module which, in turn, instantiates
 *            the PARENT and the CONFIGURATOR modules.  
 * \author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *            Girish Verma, CircuitSutra Technologies   <girish@circuitsutra.com>
 * \date      3rd September, 2011 (Saturday)
 */

#include <systemc.h>
#include <cci>

#include "./ex14_parent.h"
#include "./ex14_configurator.h"

#include "./gs_cci_cnf_private_broker_accessor.h"

/// This module creates the parent and configurator modules as well as the
/// private broker responsible for the parent module and all parameters of the
/// parent and the child modules.
SC_MODULE(ex14_top) {
 public:
  SC_CTOR(ex14_top)
      /// Register the cci-parameters of the PARENT & CHILD modules to the
      /// PRIVATE BROKER & GLOBAL BROKER
      /// 1. Instantiate a broker immediately above the 'desired' module
      /// cci::cnf::gs_cci_private_broker_accessor(sc_core::sc_module& owner,
      /// std::vector<std::string> public_parameters)
      /// 2. Feed this broker's instance to the 'desired' module
      /// parent_inst(new parent("parent_inst", privBroker))
      : privBroker(
            new cci::cnf::gs_cci_private_broker_accessor(
                *this,
                boost::assign::list_of("parent_inst.parent_int_buffer")(
                    "parent_inst.child_inst.pub_int_param"))),
        // Broker instantiated is passed as an argument to the parent module
        parent_inst(new ex14_parent("parent_inst", privBroker)),
        param_cfgr("param_cfgr") {}

  ~ex14_top() {
    delete parent_inst;
    delete privBroker;
  }

 protected:
  // Broker that hides the parameters not passed to it as argument
  cci::cnf::cci_cnf_broker_if* privBroker;
  // Parent module pointer
  ex14_parent* parent_inst;
  // Configurator module instance
  ex14_configurator param_cfgr;
};
/// ex14_top

/// Main testbench function that instantiates a top module
int sc_main(int sc_argc, char* sc_argv[]) {
  /// Instantiate TOP module
  ex14_top Top("Top");

  // Start and run the simulation
  sc_core::sc_start(26.0, sc_core::SC_NS);

  return EXIT_SUCCESS;
}
/// sc_main
