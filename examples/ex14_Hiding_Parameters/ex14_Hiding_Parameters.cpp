/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 ****************************************************************************/

#ifndef SC_INCLUDE_DYNAMIC_PROCESSES
  #define SC_INCLUDE_DYNAMIC_PROCESSES
#endif

/**
 *  @file      main.cpp
 *  @brief     This file instantiates the TOP module which, in turn, instantiates
 *             the PARENT and the CONFIGURATOR modules.
 *  @author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *             Girish Verma, CircuitSutra Technologies   <girish@circuitsutra.com>
 *  @date      3rd September, 2011 (Saturday)
 */

#include <boost/assign/list_of.hpp>
#include <cci_configuration>

#include "ex14_parent.h"
#include "ex14_configurator.h"

#include "gs_cci_cnf_private_broker_handle.h"

/**
 *  @class  ex14_top
 *  @brief  This module creates the parent and configurator modules as well as
 *          the private broker responsible for the parent module and all
 *          parameters of the parent and the child modules.
 */
SC_MODULE(ex14_top) {
 public:
  /**
   *  @fn     ex14_top
   *  @brief The class constructor
   *  @return void
   */
  SC_CTOR(ex14_top)
      // Register the cci-parameters of the PARENT & CHILD modules to the
      // PRIVATE BROKER & GLOBAL BROKER
      // 1. Instantiate a broker immediately above the 'desired' module
      // cci::gs_cci_private_broker_handle(sc_core::sc_module& owner,
      // std::vector<std::string> public_parameters)
      // 2. Feed this broker's instance to the 'desired' module
      // parent_inst(new parent("parent_inst", privBroker))
      : privBroker(
            new cci::gs_cci_private_broker_handle(
                *this,
                boost::assign::list_of("parent_inst.parent_int_buffer")(
                    "parent_inst.child_inst.pub_int_param"))),
        // Broker instantiated is passed as an argument to the parent module
        parent_inst(new ex14_parent("parent_inst", privBroker)),
        param_cfgr("param_cfgr") {}

  /**
   *  @fn     ~ex14_top()
   *  @brief  The class destructor
   *  @return void
   */
  ~ex14_top() {
    delete parent_inst;
    delete privBroker;
  }

 protected:
  cci::cci_broker_if* privBroker;  ///< Broker that hides the parameters not passed to it as argument
  ex14_parent* parent_inst; ///< Parent module pointer
  ex14_configurator param_cfgr; ///< Configurator module instance
};
/// ex14_top

/**
 *  @fn     int sc_main(int sc_argc, char* sc_argv[])
 *  @brief  Main testbench function that instantiates a top module
 */
int sc_main(int sc_argc, char* sc_argv[]) {
  // Instantiate TOP module
  ex14_top Top("Top");

  // Start and run the simulation
  sc_core::sc_start(26.0, sc_core::SC_NS);

  return EXIT_SUCCESS;
}
/// sc_main
