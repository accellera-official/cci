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
 * @file     param_value_sync.h
 * @brief    This file declares and defines the 'param_value_sync' class that
 *           registers callbacks on the cci-parameters of the owner modules.  This
 *           registration of callbacks is in order to establish synchronization between
 *           the respective cci-parameters
 * @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * @date     24th August, 2011 (Wednesday)
 */

#ifndef EXAMPLES_EX11_SEARCHING_AND_DIRECTLY_ASSOCIATING_PARAMETERS_EX11_PARAM_VALUE_SYNC_H_
#define EXAMPLES_EX11_SEARCHING_AND_DIRECTLY_ASSOCIATING_PARAMETERS_EX11_PARAM_VALUE_SYNC_H_

#include <cci>
#include <cassert>
#include <vector>
#include <sstream>
#include "xreport.hpp"

/// This module registers post write callbacks on the owner's parameters in
/// order to update an owner cci_parameter directly when another is modified
SC_MODULE(ex11_param_value_sync) {
 public:
  SC_HAS_PROCESS(ex11_param_value_sync);

  /// Overloaded constructor
  //// No default constructor
  ex11_param_value_sync(sc_core::sc_module_name _name,
                        std::vector<cci::cnf::cci_base_param *> BaseParamList)
      /// Define an originator in order to get hold of the default broker
      : ValueSyncOriginator("ValueSyncOriginator") {
    /// Get handle of the broker responsible for the class/module
    ValueSyncBrokerIF =
        &cci::cnf::cci_broker_manager::get_current_broker(ValueSyncOriginator);

    /// Copy the list of selected base parameters to a local std::vector
    returnBaseParamList = BaseParamList;

    for (unsigned int i = 1; i < returnBaseParamList.size(); i++) {
      synchValues(returnBaseParamList[0], returnBaseParamList[i]);
    }
  }

  /// PRE_WRITE and POST_WRITE callback implementation
  cci::cnf::callback_return_type
      write_callback(const cci::cnf::cci_base_param& _base_param_1,
                     const cci::cnf::callback_type& cb_reason,
                     cci::cnf::cci_base_param * _base_param_2) {
    // Post-Write callbacks
    XREPORT("[PARAM_VALUE_SYNC - post_write callback] : Parameter Name : "
            << _base_param_1.get_name() << "\tValue : "
            << _base_param_1.json_serialize());

    _base_param_2->json_deserialize(_base_param_1.json_serialize());

    return cci::cnf::return_nothing;
  }

  /// Function for synchronizing the values of cci_parameter of OWNER modules
  /// via the PARAM_VALUE_SYNC
  void synchValues(cci::cnf::cci_base_param * _base_param_1,
                   cci::cnf::cci_base_param * _base_param_2) {
    /// In order to synchronize even the default values of the owner modules,
    /// use cci_base_param of one parameter as reference, write the same value
    /// to the other pararmeter's cci_base_param using JSON
    _base_param_2->json_deserialize(_base_param_1->json_serialize());

    post_write_cb_vec.push_back(
        _base_param_1->register_callback(cci::cnf::post_write,
                                         this,
                                         cci::bind(&ex11_param_value_sync::write_callback,
                                                   this, _1, _2,
                                                   _base_param_2)));

    post_write_cb_vec.push_back(
        _base_param_2->register_callback(cci::cnf::post_write,
                                         this,
                                         cci::bind(&ex11_param_value_sync::write_callback,
                                                   this, _1, _2,
                                                   _base_param_1)));
  }

 private:
  // Declaring a CCI originator
  cci::cnf::cci_originator ValueSyncOriginator;

  // Declaring a CCI configuration broker interface instance
  cci::cnf::cci_cnf_broker_if* ValueSyncBrokerIF;

  // Callback Adaptor Objects
  std::vector<cci::shared_ptr<cci::cnf::callb_adapt> > post_write_cb_vec;

  // std::vector storing the searched owner parameters references to
  // CCI base parameter pointers
  std::vector<cci::cnf::cci_base_param*> returnBaseParamList;
};
// ex11_parameter_value_sync

#endif  // EXAMPLES_EX11_SEARCHING_AND_DIRECTLY_ASSOCIATING_PARAMETERS_EX11_PARAM_VALUE_SYNC_H_

