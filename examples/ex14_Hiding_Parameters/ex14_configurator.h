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
 * \file     configurator.h
 * \brief    This header declares and defines the 'configurator' module functionality
 * \author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * \date     5th July, 2011 (Tuesday)
 */

#ifndef EXAMPLES_EX14_HIDING_PARAMETERS_EX14_CONFIGURATOR_H_
#define EXAMPLES_EX14_HIDING_PARAMETERS_EX14_CONFIGURATOR_H_

#include <cci>
#include <cassert>
#include <vector>
#include <string>

#include "xreport.hpp"

/// This module defines the configurator module which will make changes to the
/// 'priv_int_param' of the child by making changes to the 'parent_inst.
/// parent_buffer' cci_parameter
SC_MODULE(ex14_configurator) {
 public:
  SC_CTOR(ex14_configurator) {
  /// Gets the reference of a DEFAULT GLOBAL BROKER
    myCfgrBrokerIF = &cci::cnf::cci_broker_manager::get_current_broker(
        cci::cnf::cci_originator(*this));

    // Asserts if the returned broker handle is NULL
    assert(myCfgrBrokerIF != NULL
           && "Returned default broker handle for 'configurator' is NULL");

    if (myCfgrBrokerIF != NULL) {
      if (myCfgrBrokerIF->param_exists("Top.parent_inst.parent_int_buffer")) {
        /// Get handle of the parent_module's cci-parameter
        parent_base_param_ptr = myCfgrBrokerIF->get_param(
            "Top.parent_inst.parent_int_buffer");

        // Assert if the handle returned is NULL
        assert(parent_base_param_ptr != NULL
               && "Returned handle of parent_module's cci-parameter is NULL");

        XREPORT("[CFGR] : Parameter Name : "
                << parent_base_param_ptr->get_name() << "\tParameter Value : "
                << parent_base_param_ptr->json_serialize());
      } else {
        XREPORT("[CFGR] : Parameter by name"
                " 'Top.parent_module.parent_int_buffer' doesn't exist");
      }
    }

    /// Declare SC_THREAD
    SC_THREAD(run_cfgr);
  }

  void run_cfgr(void) {
    while (1) {
      std::vector<std::string> cfgr_param_list =
          myCfgrBrokerIF->get_param_list();

      wait(15.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp()
              << "\tVisible parameters to the 'configurator' module");

      for (unsigned int i = 0; i < cfgr_param_list.size(); i++) {
        XREPORT("[CFGR] : Parameter Name : " << cfgr_param_list[i]);
      }

      wait(5.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());

      XREPORT("[CFGR] : Change the value of the 'parent_int_buffer' to '1000'");

      parent_base_param_ptr->json_deserialize("1000");

      XREPORT("[CFGR] : Parameter Name : "
              << parent_base_param_ptr->get_name() << "\tParameter Value : "
              << parent_base_param_ptr->json_serialize());

      wait(5.0, sc_core::SC_NS);
    }
  }

 private:
  // Configuration Broker for TOP_MODULE
  cci::cnf::cci_cnf_broker_if* myCfgrBrokerIF;

  // Few directly accessible cci-parameters
  cci::cnf::cci_base_param* parent_base_param_ptr;
};
/// ex14_configurator

#endif  // EXAMPLES_EX14_HIDING_PARAMETERS_EX14_CONFIGURATOR_H_
