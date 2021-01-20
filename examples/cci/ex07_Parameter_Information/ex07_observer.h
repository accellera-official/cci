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
 *  @file    observer.h
 *  @brief   This file defines an observer class
 *  @author  P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date    12th September, 2011 (Monday)
 */
#ifndef EXAMPLES_EX07_PARAMETER_INFORMATION_EX07_OBSERVER_H_
#define EXAMPLES_EX07_PARAMETER_INFORMATION_EX07_OBSERVER_H_

#include <cci_configuration>
#include <vector>
#include "xreport.hpp"

/**
 *  @class  ex07_observer
 *  @brief  This module register callbacks on CCI parameters
 */
SC_MODULE(ex07_observer) {
 public:
  /**
   *  @fn     ex07_observer
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex07_observer):
            m_broker(cci::cci_get_broker())
  {
    // Instantiate a cci_originator in order to get hold
    // of the configuration broker interface
    cci::cci_originator observerOriginator();

    // Check for the broker type (default or private) using
    // 'is_global_broker()' API
    if (!m_broker.is_global_broker()) {
      /// Access broker's name using 'name()'
      XREPORT("[OBSERVER C_TOR] : Broker Type : " << m_broker.name());
    } else {
      XREPORT("[OBSERVER C_TOR] : Broker Type : " << m_broker.name()
              << "- is not a private broker.");
    }

    // Gets the reference to the 'int' type cci-parameter of OWNER module
   cci::cci_param_handle obsv_int_base_handle =
           m_broker.get_param_handle("param_owner.mutable_int_param");

    sc_assert(obsv_int_base_handle.is_valid()
           && "Returned Handle of 'integer type' cci-parameter is NULL");

    // Observer registering PRE_READ, PRE_WRITE & POST_WRITE callbacks
    // on the integer parameter to monitor all actions on it
    int_pre_read_cb = obsv_int_base_handle.register_pre_read_callback(
            &ex07_observer::pre_read_callback, this, cci::cci_untyped_tag());
    int_pre_write_cb = obsv_int_base_handle.register_pre_write_callback(
            &ex07_observer::pre_write_callback, this, cci::cci_untyped_tag());
    int_post_write_cb = obsv_int_base_handle.register_post_write_callback(
            &ex07_observer::post_write_callback, this, cci::cci_untyped_tag());
  }

  /**
   *  @fn      cci::callback_return_type read_callback(cci::cci_base_param &, const cci::callback_type &)
   *  @brief   'PRE_READ' Callbacks Implementations
   *  @param   cci::cci_base_param&   Reference of cci_base_param on which the (pre-read) callback is registered
   */
  void pre_read_callback(const cci::cci_param_read_event<> & ev) {
    XREPORT("[OBSERVER pre_read_cb] :  Parameter Name : "
            << ev.param_handle.name() << "\tOriginator info : "
            << ev.originator.name());
  }

  /**
   *  @fn      cci::callback_return_type write_callback(cci::cci_base_param &, const cci::callback_type &)
   *  @brief   'PRE_WRITE' Callbacks Implementations
   *  @param   cci::cci_base_param&   Reference of cci_base_param on which the (pre/post-write) callback is registered
   *  @return  true
   */
  bool pre_write_callback(const cci::cci_param_write_event<> & ev) {
    XREPORT("[OBSERVER pre_write_cb] :  Parameter Name : "
            << ev.param_handle.name() << "\tOriginator info : "
            << ev.originator.name());
    return true;
  }

  /**
   *  @fn      cci::callback_return_type write_callback(cci::cci_base_param &, const cci::callback_type &)
   *  @brief   'POST_WRITE' Callbacks Implementations
   *  @param   cci::cci_base_param&   Reference of cci_base_param on which the (pre/post-write) callback is registered
   */
  void post_write_callback(const cci::cci_param_write_event<> & ev) {
    XREPORT("[OBSERVER post_write_cb] :  Parameter Name : "
            << ev.param_handle.name() << "\tOriginator info : "
            << ev.originator.name());
  }

 private:
  cci::cci_broker_handle m_broker;  //!< Configuration broker handle

  cci::cci_callback_untyped_handle int_pre_read_cb; //!< 'pre_read' callback handle for int type cci-param
  cci::cci_callback_untyped_handle int_pre_write_cb;  //!< 'pre_write' callback handle for int type cci-param
  cci::cci_callback_untyped_handle int_post_write_cb; //!< 'post_write' callback handle for int type cci-param
};
// ex07_observer

#endif  // EXAMPLES_EX07_PARAMETER_INFORMATION_EX07_OBSERVER_H_
