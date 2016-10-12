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
#include <cassert>
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
  SC_CTOR(ex07_observer) {
    // Instantiate a cci_originator in order to get hold
    // of the configuration broker interface
    cci::cci_originator observerOriginator("observerOriginator");

    // Get the broker responsible for this module using
    // 'get_current_broker' API
    observerBrokerIF = &cci::cci_broker_manager::get_current_broker(
        observerOriginator);

    // Assert if broker handle returned is NULL
    assert(observerBrokerIF != NULL
           && "Observer Broker Handle Returned is NULL");

    // Check for the broker type (default or private) using
    // 'is_private_broker()' API
    if (observerBrokerIF->is_private_broker()) {
      /// Access broker's name using 'name()'
      XREPORT("[OBSERVER C_TOR] : Broker Type : " << observerBrokerIF->name());
    } else {
      XREPORT("[OBSERVER C_TOR] : Broker Type : " << observerBrokerIF->name()
              << "- is not a private broker.");
    }

    // Gets the reference to the 'int' type cci-parameter of OWNER module
   cci::cci_param_handle obsv_int_base =
           observerBrokerIF->get_param_handle("param_owner.mutable_int_param");

    assert(obsv_int_base.is_valid()
           && "Returned Handle of 'integer type' cci-parameter is NULL");

    // Observer registering PRE_READ, PRE_WRITE & POST_WRITE callbacks
    // on the integer parameter to monitor all actions on it
    // TODO: fixme
    /*int_pre_read_cb = obsv_int_base_ptr.register_callback(
        cci::pre_read, this,
        cci::bind(&ex07_observer::read_callback, this, _1, _2));
    int_pre_write_cb = obsv_int_base_ptr.register_callback(
        cci::pre_write, this,
        cci::bind(&ex07_observer::write_callbacks, this, _1, _2));
    int_post_write_cb = obsv_int_base_ptr.register_callback(
        cci::post_write, this,
        cci::bind(&ex07_observer::write_callbacks, this, _1, _2));*/
  }

  /**
   *  @fn      cci::callback_return_type read_callback(cci::cci_base_param &, const cci::callback_type &)
   *  @brief   'PRE_READ' Callbacks Implementations
   *  @param   cci::cci_base_param&   Reference of cci_base_param on which the (pre-read) callback is registered
   *  @param   cci::callback_type&  Callback type
   *  @return  cci::callback_return_type  Callback return type
   */
  // TODO: fixme
  /*cci::callback_return_type read_callback(
      cci::cci_base_param & _selected_base_param,
      const cci::callback_type & cb_reason) {
    switch (cb_reason) {
      case cci::pre_read: {
        const cci::cci_originator* myOriginator =
            cci::cci_originator::get_global_originator();

        XREPORT("[OBSERVER pre_read_cb] :  Parameter Name : "
                << _selected_base_param.get_name() << "\tOriginator info : "
                << myOriginator->name());

        break;
      }

      default:
        XREPORT("[OBSERVER pre_read_cb] - Unknown Callback Type");
    }

    return cci::return_nothing;
  }*/

  /**
   *  @fn      cci::callback_return_type write_callback(cci::cci_base_param &, const cci::callback_type &)
   *  @brief   'PRE_WRITE' & 'POST_WRITE' Callbacks Implementations
   *  @param   cci::cci_base_param&   Reference of cci_base_param on which the (pre/post-write) callback is registered
   *  @param   cci::callback_type&  Callback type (pre-read or post-read)
   *  @return  cci::callback_return_type  Callback return type
   */
  // TODO: fixme
  /*cci::callback_return_type write_callbacks(
      cci::cci_base_param & _selected_base_param,
      const cci::callback_type & cb_reason) {
    const cci::cci_originator* myOriginator =
        cci::cci_originator::get_global_originator();

    switch (cb_reason) {
      case cci::pre_write: {
        XREPORT("[OBSERVER pre_write_cb] :  Parameter Name : "
                << _selected_base_param.get_name() << "\tOriginator info : "
                << myOriginator->name());
        break;
      }

      case cci::post_write: {
        XREPORT("[OBSERVER post_write_cb] :  Parameter Name : "
                << _selected_base_param.get_name() << "\tOriginator info : "
                << myOriginator->name());
        break;
      }

      default:
        XREPORT("[OBSERVER write_cb] - Unknown Callback Type");
    }

    return cci::return_nothing;
  }*/

 private:

  cci::cci_broker_if* observerBrokerIF;  //!< Configuration broker instance

  cci::shared_ptr<cci::callb_adapt> int_pre_read_cb; //!< 'pre_read' callback adaptor obj for int type cci-param
  cci::shared_ptr<cci::callb_adapt> int_pre_write_cb;  //!< 'pre_write' callback adaptor obj for int type cci-param
  cci::shared_ptr<cci::callb_adapt> int_post_write_cb; //!< 'post_write' callback adaptor obj for int type cci-param
};
// ex07_observer

#endif  // EXAMPLES_EX07_PARAMETER_INFORMATION_EX07_OBSERVER_H_
