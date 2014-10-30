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

/**
 * @file     parameter_configurator.h
 * @brief    This header declares and defines configurator class.  The configurator
 *           class tries to find the list of unconsumed parameters with the model
 * @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * @date     21st July, 2011 (Thursday)
 */

#ifndef EXAMPLES_EX18_SUPERSET_PARAMETER_VALUE_SPECIFICATION_EX18_PARAMETER_CONFIGURATOR_H_
#define EXAMPLES_EX18_SUPERSET_PARAMETER_VALUE_SPECIFICATION_EX18_PARAMETER_CONFIGURATOR_H_

#include <cci>
#include <cassert>
#include <vector>
#include <string>

#include "xreport.hpp"

/// The configurator class illustrates way of retrieving cci-parameters list
/// within a cci-model by using the 'get_param_list()' API
SC_MODULE(ex18_parameter_configurator) {
 public:
  SC_CTOR(ex18_parameter_configurator) {
    // Get handle of the broker responsible for the class/module
    myCfgrBrokerIF =&cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));

    // Report if handle returned is NULL
    assert(myCfgrBrokerIF != NULL && "Configuration Broker handle is NULL");

    /// Retrieve the list of all cci-parameters within a model.
    complete_parameter_list = myCfgrBrokerIF->get_param_list();
  }

/// This callback function demonstrates ways for a tool developer to retrieve
/// the list of unconsumed parameters in a model.
  void end_of_elaboration(void) {
    for (unsigned int i = 0; i < complete_parameter_list.size(); i++) {
      if (!myCfgrBrokerIF->is_used(complete_parameter_list[i])) {
        unconsumed_parameter_list.push_back(complete_parameter_list[i]);

        XREPORT("[CFGR within EOE] : 'used status' of cci-parameter : "
                << complete_parameter_list[i] << "\tis : "
                << myCfgrBrokerIF->is_used(complete_parameter_list[i]));
      } else {
        XREPORT("[CFGR within EOE] : 'used status' of cci-parameter : "
                << complete_parameter_list[i] << "\tis : "
                << myCfgrBrokerIF->is_used(complete_parameter_list[i]));
      }
    }

    XREPORT("@ " << sc_time_stamp());
    XREPORT("[CFGR] : List of all unconsumed parameters in the model");

    for (unsigned int i = 0; i < unconsumed_parameter_list.size(); i++) {
      XREPORT("[CFGR] : Unconsumed Parameter Name : "
              << unconsumed_parameter_list[i]);
    }
  }

 private:
  /// Declaring a CCI configuration broker interface instance
  cci::cnf::cci_cnf_broker_if* myCfgrBrokerIF;

  /// CCI base parameter
  cci::cnf::cci_base_param* cfgr_param_ptr;

  /// std::vector to store the list of the unconsumed parameters
  std::vector<std::string> complete_parameter_list;
  std::vector<std::string> unconsumed_parameter_list;
};
/// ex18_parameter_configurator

#endif  // EXAMPLES_EX18_SUPERSET_PARAMETER_VALUE_SPECIFICATION_EX18_PARAMETER_CONFIGURATOR_H_
