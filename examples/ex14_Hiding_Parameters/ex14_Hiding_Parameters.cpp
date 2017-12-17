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
#include <cci_utils/broker.h>

/**
 *  @class  ex14_private
 *  @brief  This module creates a module hierarchy with a private broker
 */
SC_MODULE(ex14_private) {
public:
    /**
     *  @fn     ex14_private
     *  @brief  The class constructor
     *  @return void
     */
    SC_CTOR(ex14_private)
        : pbroker ("My Private Broker")
    {
      pbroker.expose.insert("Top.private.parent_inst.parent_int_buffer");
      pbroker.expose.insert("Top.private.parent_inst.child_inst.pub_int_param");
      cci::cci_register_broker(pbroker);

      m_parent_inst = new ex14_parent("parent_inst");
    }

    ~ex14_private() {
      delete m_parent_inst;
    }

protected:
    cci_utils::broker pbroker;
    ex14_parent* m_parent_inst; ///< Parent module pointer
};
/// ex14_top

/**
 *  @class  ex14_top
 *  @brief  This module creates the parent and configurator modules
 */
SC_MODULE(ex14_top) {
 public:
  /**
   *  @fn     ex14_top
   *  @brief The class constructor
   *  @return void
   */
  SC_CTOR(ex14_top):
            m_private("private"),
            m_param_cfgr("param_cfgr")
  {}

 protected:
  ex14_private m_private; ///< Private subtop module
  ex14_configurator m_param_cfgr; ///< Configurator module instance pointer
};
/// ex14_top

/**
 *  @fn     int sc_main(int sc_argc, char* sc_argv[])
 *  @brief  Main testbench function that instantiates a top module
 */
int sc_main(int sc_argc, char* sc_argv[]) {
  cci::cci_register_broker(new cci_utils::broker("My Global Broker"));
  // Instantiate TOP module
  ex14_top Top("Top");

  // Start and run the simulation
  sc_core::sc_start(26.0, sc_core::SC_NS);

  return EXIT_SUCCESS;
}
/// sc_main
