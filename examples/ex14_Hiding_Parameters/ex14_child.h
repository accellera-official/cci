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

/*!
 * \file      child.h
 * \brief     This file declares and implements the 'child' module. The
 *            'child' module is instantiated by the 'parent' module and the
 *            parent module hides the private cci-parameter of the 'child'
 *            details from the external world.
 * \author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * \date      3rd September, 2011 (Saturday)
 */

#ifndef EXAMPLES_EX14_HIDING_PARAMETERS_EX14_CHILD_H_
#define EXAMPLES_EX14_HIDING_PARAMETERS_EX14_CHILD_H_

#include <cci>
#include <cassert>
#include <vector>
#include <string>

#include "xreport.hpp"

/// The child module registers public and private cci_parameters to broker
/// responsible for the module
SC_MODULE(ex14_child) {
 public:
  SC_CTOR(ex14_child)
      : priv_int_param("priv_int_param",
                       100,
                       cci::cnf::cci_broker_manager::get_current_broker(
                           cci::cnf::cci_originator(*this))),
        pub_int_param("pub_int_param",
                      150,
                      cci::cnf::cci_broker_manager::get_current_broker(
                          cci::cnf::cci_originator(*this))) {
    /// Get the reference to the broker responsible for this module
    /// child_BrokerIF = &cci::cnf::cci_broker_manager::get_current_broker(
    /// cci::cnf::cci_originator(*this));
    child_BrokerIF =
        &cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));

    assert(child_BrokerIF != NULL
           && "Returned broker handle of 'child' module is NULL");

    XREPORT("[CHILD C_TOR] : Is Private Broker? : " << std::boolalpha
            << child_BrokerIF->is_private_broker());

    XREPORT("[CHILD C_TOR] : Parameter Name   : "
            << priv_int_param.get_name() << "\tParameter Value : "
            << priv_int_param.get());

    XREPORT("[CHILD C_TOR] : Parameter Name   : "
            << pub_int_param.get_name() << "\tParameter Value : "
            << pub_int_param.get());

    /// Declare a SC_THREAD
    SC_THREAD(run_child);
  }

  void run_child(void) {
    // List of cci_parameters directly visible to the outside world
    std::vector<std::string> child_param_list =
        child_BrokerIF->get_param_list();

    while (1) {
      XREPORT("@ " << sc_time_stamp()
              << "\tVisible parameters to 'child' module");

      for (unsigned int i = 0; i < child_param_list.size(); i++) {
        XREPORT("[CHILD] : Parameter Name : " << child_param_list[i]);
      }

      wait(25.0, SC_NS);

      XREPORT("@ " << sc_time_stamp());

      XREPORT("[CHILD] : Parameter Name : " << priv_int_param.get_name()
              << "\tParameter Value : " << priv_int_param.get());

      wait(10.0, SC_NS);
    }
  }

 private:
  // Declare a configuration broker
  cci::cnf::cci_cnf_broker_if* child_BrokerIF;

  // Declare instances of mutable CCI parameters of type 'int'
  cci::cnf::cci_param<int> priv_int_param;
  cci::cnf::cci_param<int> pub_int_param;
};
/// ex14_child

#endif  // EXAMPLES_EX14_HIDING_PARAMETERS_EX14_CHILD_H_
