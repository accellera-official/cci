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
      : m_broker(cci::cci_get_broker())
  {
    // Copy the list of selected base parameters to a local std::vector
    returnBaseParamList = BaseParamList;

    for (unsigned int i = 1; i < returnBaseParamList.size(); i++) {
      synchValues(returnBaseParamList[0], returnBaseParamList[i]);
    }
  }

  /**
   *  @fn     void typed_post_write_callback(const cci::cci_param_write_event<int> & ev)
   *  @brief  Post Callback function to sync ongoing written parameter value with synced_handle value
   *  @return void
   */
  void untyped_post_write_callback(const cci::cci_param_write_event<> & ev ,
                                   cci::cci_param_handle synced_handle)
  {
    XREPORT("[PARAM_VALUE_SYNC - post_write callback] : Parameter Name : "
            << ev.param_handle.get_name() << "\tValue : "
            << ev.new_value);

    synced_handle.set_cci_value(ev.new_value);
  }

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

    post_write_cb_vec.push_back(_param_handle_1.register_post_write_callback(
            sc_bind(&ex11_param_value_sync::untyped_post_write_callback,
                    this, sc_unnamed::_1, _param_handle_2)));

    post_write_cb_vec.push_back(_param_handle_2.register_post_write_callback(
            sc_bind(&ex11_param_value_sync::untyped_post_write_callback,
                    this, sc_unnamed::_1, _param_handle_1)));
  }

 private:
  cci::cci_broker_handle m_broker; ///< Declaring a CCI configuration broker handle
  std::vector<cci::cci_callback_untyped_handle> post_write_cb_vec; ///< Callback Adaptor Objects
  std::vector<cci::cci_param_handle> returnBaseParamList; ///< std::vector storing the searched owner parameters references to CCI parameter handles
};
// ex11_parameter_value_sync

#endif  // EXAMPLES_EX11_SEARCHING_AND_DIRECTLY_ASSOCIATING_PARAMETERS_EX11_PARAM_VALUE_SYNC_H_

