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
 *  @file     param_value_sync.h
 *  @brief    This file declares and defines the 'param_value_sync' class that
 *            registers callbacks on the cci-parameters of the owner modules.  This
 *            registration of callbacks is in order to establish synchronization between
 *            the respective cci-parameters
 *  @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date     24th August, 2011 (Wednesday)
 */

#ifndef EXAMPLES_EX11_SEARCHING_AND_DIRECTLY_ASSOCIATING_PARAMETERS_EX11_PARAM_VALUE_SYNC_H_
#define EXAMPLES_EX11_SEARCHING_AND_DIRECTLY_ASSOCIATING_PARAMETERS_EX11_PARAM_VALUE_SYNC_H_

#include <cci_configuration>
#include <cassert>
#include <vector>
#include <sstream>
#include "xreport.hpp"

/**
 *  @class  ex11_param_value_sync
 *  @brief  This module registers post write callbacks on the owner's parameters
 *          in order to update an owner cci_parameter directly when another is modified
 */
SC_MODULE(ex11_param_value_sync) {
 public:
  SC_HAS_PROCESS(ex11_param_value_sync);

  /**
   *  @fn     ex11_param_value_sync(sc_core::sc_module_name _name, std::vector<cci::cci_param_handle> BaseParamList)
   *  @brief  The class constructor
   *  @param  _name The name of the class
   *  @param  BaseParamList The list of CCI parameter handles
   *  @return void
   */
  ex11_param_value_sync(sc_core::sc_module_name _name,
                        std::vector<cci::cci_param_handle> BaseParamList)
      // Define an originator in order to get hold of the default broker
      : ValueSyncOriginator("ValueSyncOriginator") {
    // Get handle of the broker responsible for the class/module
    ValueSyncBrokerIF =
        &cci::cci_broker_manager::get_current_broker(ValueSyncOriginator);

    // Copy the list of selected base parameters to a local std::vector
    returnBaseParamList = BaseParamList;

    for (unsigned int i = 1; i < returnBaseParamList.size(); i++) {
      synchValues(returnBaseParamList[0], returnBaseParamList[i]);
    }
  }

  /**
   *  @fn     cci::cmf::callback_return_type write_callback(const cci::cci_base_param& _base_param_1, const cci::callback_type& cb_reason, cci::cci_base_param* _base_param_2)
   *  @brief  Pre-write and post-write callback implementation
   *  @param  _base_param_1 Parameter to sync from
   *  @param  cb_reason Reason the callback was triggered
   *  @param  _base_param_2 The parameter to sync to
   *  @return The result of the callback execution
   */
  // TODO: fixme
  /*cci::callback_return_type
      write_callback(const cci::cci_base_param& _base_param_1,
                     const cci::callback_type& cb_reason,
                     cci::cci_base_param * _base_param_2) {
    // Post-Write callbacks
    XREPORT("[PARAM_VALUE_SYNC - post_write callback] : Parameter Name : "
            << _base_param_1.get_name() << "\tValue : "
            << _base_param_1.get_cci_value().to_json());

    _base_param_2->set_cci_value(cci::cci_value::from_json(
            _base_param_1.get_value().to_json()));

    return cci::return_nothing;
  }*/

  /**
   *  @fn     void synchValues(cci::cci_param_handle _param_handle_1, cci::cci_param_handle _param_handle_2)
   *  @brief  Function for synchronizing the values of cci_parameter of OWNER
   *          modules via the PARAM_VALUE_SYNC
   *  @param  _param_handle_1 The first parameter to be synced
   *  @param  _param_handle_2 The second parameter to be synced
   *  @return void
   */
  void synchValues(cci::cci_param_handle _param_handle_1,
                   cci::cci_param_handle _param_handle_2) {
    // In order to synchronize even the default values of the owner modules,
    // use cci_base_param of one parameter as reference, write the same value
    // to the other pararmeter's cci_base_param using JSON
    _param_handle_2.set_cci_value(_param_handle_1.get_cci_value());

    // TODO: fixme
    /*post_write_cb_vec.push_back(
        _param_handle_1->register_callback(cci::post_write,
                                         this,
                                         cci::bind(&ex11_param_value_sync::write_callback,
                                                   this, _1, _2,
                                                   _param_handle_2)));

    post_write_cb_vec.push_back(
        _param_handle_2->register_callback(cci::post_write,
                                         this,
                                         cci::bind(&ex11_param_value_sync::write_callback,
                                                   this, _1, _2,
                                                   _param_handle_1)));*/
  }

 private:
  cci::cci_originator ValueSyncOriginator; ///< Declaring a CCI originator
  cci::cci_broker_if* ValueSyncBrokerIF; ///< Declaring a CCI configuration broker interface instance
  std::vector<cci::shared_ptr<cci::callb_adapt> > post_write_cb_vec; ///< Callback Adaptor Objects
  std::vector<cci::cci_param_handle> returnBaseParamList; ///< std::vector storing the searched owner parameters references to CCI parameter handles
};
// ex11_parameter_value_sync

#endif  // EXAMPLES_EX11_SEARCHING_AND_DIRECTLY_ASSOCIATING_PARAMETERS_EX11_PARAM_VALUE_SYNC_H_

