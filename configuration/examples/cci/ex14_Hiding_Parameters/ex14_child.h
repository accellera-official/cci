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

/**
 *  @file      child.h
 *  @brief     This file declares and implements the 'child' module. The
 *             'child' module is instantiated by the 'parent' module and the
 *             parent module hides the private cci-parameter of the 'child'
 *             details from the external world.
 *  @author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date      3rd September, 2011 (Saturday)
 */

#ifndef EXAMPLES_EX14_HIDING_PARAMETERS_EX14_CHILD_H_
#define EXAMPLES_EX14_HIDING_PARAMETERS_EX14_CHILD_H_

#include <cci_configuration>
#include <vector>
#include <string>

#include "xreport.hpp"

/**
 *  @class  ex14_child
 *  @brief  The child module registers public and private cci_parameters to
 *          broker responsible for the module
 */
SC_MODULE(ex14_child) {
 public:
  /**
   *  @fn     ex14_child
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex14_child)
      : m_broker(cci::cci_get_broker()),
        priv_int_param("priv_int_param",
                       100),
        pub_int_param("pub_int_param",
                      150)
  {
    XREPORT("[CHILD C_TOR] : Is Private Broker? : " << std::boolalpha
            << !m_broker.is_global_broker());

    XREPORT("[CHILD C_TOR] : Parameter Name   : "
            << priv_int_param.name() << "\tParameter Value : "
            << priv_int_param.get_value());

    XREPORT("[CHILD C_TOR] : Parameter Name   : "
            << pub_int_param.name() << "\tParameter Value : "
            << pub_int_param.get_value());

    // Declare a SC_THREAD
    SC_THREAD(run_child);
  }

  /**
   *  @fn     void run_child(void)
   *  @brief  Main executiong function to update/read parameters
   *  @return void
   */
  void run_child(void) {
    // List of cci_parameters directly visible to the outside world
    std::vector<cci::cci_param_untyped_handle> child_param_list =
        m_broker.get_param_handles();

    while (1) {
      XREPORT("@ " << sc_core::sc_time_stamp()
              << "\tVisible parameters to 'child' module");

      for (unsigned int i = 0; i < child_param_list.size(); i++) {
        XREPORT("[CHILD] : Parameter Name : "
                        << child_param_list[i].name());
      }

      wait(25.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());

      XREPORT("[CHILD] : Parameter Name : " << priv_int_param.name()
              << "\tParameter Value : " << priv_int_param.get_value());

      wait(10.0, sc_core::SC_NS);
    }
  }

 private:
  cci::cci_broker_handle m_broker;  ///< Declare a configuration broker handle

  // Declare instances of mutable CCI parameters of type 'int'
  cci::cci_param<int> priv_int_param;  ///< private int CCI parameter
  cci::cci_param<int> pub_int_param; ///< public int CCI parameter
};
/// ex14_child

#endif  // EXAMPLES_EX14_HIDING_PARAMETERS_EX14_CHILD_H_
