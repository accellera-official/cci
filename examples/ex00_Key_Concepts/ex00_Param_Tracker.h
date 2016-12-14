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
 *  @file   ex00_Param_Tracker.h
 *  @brief  This file keeps the track of parameter updates.
 *  @author Sonal Poddar, Intel
 */

#ifndef EXAMPLES_EX00_KEY_CONCEPTS_EX00_PARAM_TRACKER_H_
#define EXAMPLES_EX00_KEY_CONCEPTS_EX00_PARAM_TRACKER_H_

#include <cci_configuration>
#include <vector>
#include "xreport.hpp"

/**
 *  @class  ex00_Param_Tracker
 *  @brief  This module register callbacks on CCI parameters, keeps a track of parameter
 *          name, originator info and the value of parameter.
 */
SC_MODULE(ex00_Param_Tracker) {
 public:
  /**
   *  @fn     ex00_Param_Tracker
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex00_Param_Tracker):
            TrackerBrokerIF(cci::cci_broker_manager::get_broker())
  {
    // Instantiate a cci_originator in order to get hold
    // of the configuration broker interface
    cci::cci_originator TrackerOriginator("TrackerOriginator");

    // Check for the broker type (default or private) using
    // 'is_private_broker()' API
    if (TrackerBrokerIF.is_private_broker()) {
      /// Access broker's name using 'name()'
      XREPORT("[TRACKER C_TOR] : Broker Type : " << TrackerBrokerIF.name());
    } else {
      XREPORT("[TRACKER C_TOR] : Broker Type : " << TrackerBrokerIF.name()
              << "- is not a private broker.");
    }

    // Gets the reference to the 'int' type cci-parameter of OWNER module
    cci::cci_param_handle track_int_base =
           TrackerBrokerIF.get_param_handle("top.subsystem1.param_owner.int_param");

    sc_assert(track_int_base.is_valid()
           && "Returned Handle of 'integer type' cci-parameter is NULL");

    // Tracker registering POST_WRITE callbacks
    // on the integer parameter to monitor all actions on it
   
    int_post_write_cb = track_int_base.register_post_write_callback(
           &ex00_Param_Tracker::post_write_callback, this, cci::cci_untyped_tag());
  }

  /**
   *  @fn      cci::callback_return_type write_callback(cci::cci_base_param &, const cci::callback_type &)
   *  @brief   'POST_WRITE' Callbacks Implementations
   *  @param   cci::cci_base_param&   Reference of cci_base_param on which the (pre/post-write) callback is registered
   */
  void post_write_callback(const cci::cci_param_write_event<> & ev) {
    XREPORT("[TRACKER post_write_cb] :  Parameter Name : "
            << ev.param_handle.get_name() << "\tOriginator info : "
            << ev.originator.name() <<"\tValue:" << ev.new_value);
  }

 private:
  cci::cci_broker_if& TrackerBrokerIF;  //!< Configuration broker instance
  cci::cci_callback_untyped_handle int_post_write_cb; //!< 'post_write' callback handle for int type cci-param
};
// ex00_Param_Tracker

#endif  // EXAMPLES_EX00_KEY_CONCEPTS_EX00_PARAM_TRACKER_H_