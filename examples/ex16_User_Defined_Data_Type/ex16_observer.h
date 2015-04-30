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
 *  @file    observer.h
 *  @brief   This file defines an observer class demonstrates
 *  @author  P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *           Girish Verma, CircuitSutra Technologies   <girish@circuitsutra.com>
 *  @date    12th September, 2011 (Monday)
 */

#ifndef EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_OBSERVER_H_
#define EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_OBSERVER_H_

#include <cci>
#include <cassert>
#include <vector>

/**
 *  @class  ex16_observer
 *  @brief  This observer class registers callbacks on the cci_parameter values
 *          of interest in order to find originator for the appropriate actions
 *          on the respective cci_parameters
 */
class ex16_observer {
 public:
  /**
   *  @fn     ex16_observer
   *  @brief  The class constructor
   *  @return void
   */
  ex16_observer() {
    // Instantiate a cci_originator in order to get hold of the
    // configuration broker interface
    cci::cnf::cci_originator observerOriginator("observerOriginator");

    // Get the broker responsible for this module using
    // 'get_current_broker' API
    observerBrokerIF =
        &cci::cnf::cci_broker_manager::get_current_broker(observerOriginator);

    // Assert if broker handle returned is NULL
    assert(observerBrokerIF != NULL
           && "Observer Broker Handle Returned is NULL");

    // Check for the broker type (default or private) using
    // 'is_private_broker()' API
    if (observerBrokerIF->is_private_broker()) {
      // Access broker's name using 'name()'
      std::cout << "\n\t[OBSERVER C_TOR] : Broker Type : "
                << observerBrokerIF->name() << endl;
    } else {
      std::cout << "\n\t[OBSERVER C_TOR] : Broker Type : "
                << observerBrokerIF->name() << " - is not a private broker."
                << endl;
    }

    // Gets the reference to the 'udt' type cci-parameter of OWNER module
    obsv_udt_base_ptr =
        observerBrokerIF->get_param("param_owner.User_data_type_param");

    assert(obsv_udt_base_ptr != NULL
           && "Returned Handle of 'integer type' cci-parameter is NULL");

    // Observer registering 'PRE_READ', 'PRE_WRITE' & 'POST_WRITE' callbacks
    // on the UDT parameter to monitor all actions on it
    udt_pre_read_cb = obsv_udt_base_ptr->register_callback(cci::cnf::pre_read,
                                                           this,
                                                           cci::bind(&ex16_observer::read_callback,
                                                                     this,
                                                                     _1, _2));
    udt_pre_write_cb = obsv_udt_base_ptr->register_callback(cci::cnf::pre_write,
                                                            this,
                                                            cci::bind(&ex16_observer::write_callbacks,
                                                                      this,
                                                                      _1, _2));
    udt_post_write_cb = obsv_udt_base_ptr->register_callback(cci::cnf::post_write,
                                                             this,
                                                             cci::bind(&ex16_observer::write_callbacks,
                                                                       this,
                                                                       _1, _2));
  }

  /**
   *  @fn     cci::cnf::callback_return_type read_callback(const cci::cnf::cci_base_param& _selected_base_param, const cci::cnf::callback_type& cb_reason)
   *  @brief  Implementation of the pre-read callback
   *  @param  _selected_base_param  The parameter for the callback
   *  @param  cb_reason The reason for the callback
   *  @return The exit status of the function
   */
  cci::cnf::callback_return_type read_callback(const cci::cnf::cci_base_param& _selected_base_param,
                                               const cci::cnf::callback_type& cb_reason) {
    switch (cb_reason) {
      case cci::cnf::pre_read: {
        const cci::cnf::cci_originator* myOriginator =
            cci::cnf::cci_originator::get_global_originator();

        std::cout << "\n\t[OBSERVER pre_read_cb] :  Parameter Name : "
                  << _selected_base_param.get_name() << "\tOriginator info : "
                  << myOriginator->name() << std::endl;

        break;
      }

      default:
        std::cout << "\n\t[OBSERVER pre_read_cb] - Unknown Callback Type"
                  << std::endl;
    }

    return cci::cnf::return_nothing;
  }

  /**
   *  @fn     cci::cnf::callback_return_type write_callback(const cci::cnf::cci_base_param& _seleceted_param, const cci::cnf::callback_return_type& cb_reason)
   *  @brief  The implementation of the pre-write and post-write callbacks
   *  @param  _seleceted_base_param The cci parameter for the callback
   *  @param  cb_reason The reason for the callback being called
   *  @return The exit status for the callback function
   */
  cci::cnf::callback_return_type write_callbacks(const cci::cnf::cci_base_param& _selected_base_param,
                                                 const cci::cnf::callback_type& cb_reason) {
    const cci::cnf::cci_originator* myOriginator =
        cci::cnf::cci_originator::get_global_originator();

    switch (cb_reason) {
      case cci::cnf::pre_write: {
        std::cout << "\n\t[OBSERVER pre_write_cb] :  Parameter Name : "
                  << _selected_base_param.get_name() << "\tOriginator info : "
                  << myOriginator->name() << std::endl;
        break;
      }

      case cci::cnf::post_write: {
        std::cout << "\n\t[OBSERVER post_write_cb] :  Parameter Name : "
                  << _selected_base_param.get_name() << "\tOriginator info : "
                  << myOriginator->name() << std::endl;
        break;
      }

      default:
        std::cout << "\n\t[OBSERVER write_cb] - Unknown Callback Type"
                  << std::endl;
    }

    return cci::cnf::return_nothing;
  }

 private:
  cci::cnf::cci_cnf_broker_if* observerBrokerIF;  ///< CCI configuration broker instance

  cci::cnf::cci_base_param* obsv_udt_base_ptr;  ///< Declare cci_base_param for int type cci-parameter

  // Callback Adaptor Objects for 'int' type parameter
  cci::shared_ptr<cci::cnf::callb_adapt> udt_pre_read_cb; ///< Callback adapter object for pre-read
  cci::shared_ptr<cci::cnf::callb_adapt> udt_pre_write_cb;  ///< Callback adapter object for pre-write
  cci::shared_ptr<cci::cnf::callb_adapt> udt_post_write_cb; ///< Callback adapter object for post-write
};
// ex16_observer.h

#endif  // EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_OBSERVER_H_
