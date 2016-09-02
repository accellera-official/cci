/*****************************************************************************
  Copyright 2006-2014 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2010-2015 CircuitSutra Technologies Pvt. Ltd.
  All rights reserved.

  Copyright 2016 Ericsson
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
 *  @file     observer.h
 *  @brief    This header declares and defines the observer class
 *  @author   P V S Phaneendra, CircuitSutra Technologies <pvs@circuitsutra.com>
 *  @date     11th October, 2011 (Tuesday)
 */

#ifndef EXAMPLES_EX20_TRACING_PARAMETER_ACTIVITY_EX20_OBSERVER_H_
#define EXAMPLES_EX20_TRACING_PARAMETER_ACTIVITY_EX20_OBSERVER_H_

#include <cci_configuration>
#include <cassert>
#include <vector>

#include "xreport.hpp"

/**
 *  @class  ex20_observer
 *  @brief  The observer class illustrates the recommended and possible ways of
 *          tracing parameter activities of a cci-parameter by registering various
 *          callbacks
 */
class ex20_observer {
 public:
  /**
   *  @fn     ex20_observer()
   *  @brief  The class constructor.
   *  @return void
   */
  ex20_observer():
      observer_base(cci::cci_originator("ex20_observer")) {
    // Create an instance of the cci_originator
    cci::cci_originator observer_originator("observer");

    // Get handle of the broker responsible for the class/module
    Observer_BrokerIF =
        &cci::cci_broker_manager::get_current_broker(observer_originator);

    // Report if handle returned is NULL
    assert(Observer_BrokerIF != NULL && "Configuration Broker handle is NULL");

    // Registering callback for the creation of the pointer to the
    // cci_parameter of the owner module
    observer_cb.push_back(Observer_BrokerIF->register_str_callback(cci::create_param, "*",
                                                                   this,
                                                                   cci::bind(&ex20_observer::config_new_param_callback,
                                                                             this, _1, _2)));
  }

  /**
   *  @fn     ~ex20_observer()
   *  @brief  The class destructor
   *  @return void
   */
  ~ex20_observer() {
    //@TODO
  }

  /**
   *  @fn     cci::callback_return_type config_new_param_callback(const std::string& _base_param_str, const cci::callback_type& cb_reason)
   *  @brief  Callback implementation for scanning/detecting creation of new cci parameters.
   *  @param  _base_param_str Newly created cci parameter reference
   *  @param  cb_reason The reason for the callback being called
   *  @return The exit status of the callback function.
   */
  cci::callback_return_type
      config_new_param_callback(const std::string & _base_param_str,
                                const cci::callback_type & cb_reason) {
    switch (cb_reason) {
      case cci::create_param: {
        std::cout << "\n\t[OBSERVER - create_param_cb] : Retrieving reference"
                     " of newly created cci_parameter" << std::endl;

        // Get reference of newly created cci-parameters
        observer_base = Observer_BrokerIF->get_param_handle(_base_param_str);

        // Assert if reference of the cci-parameter returned is NULL
        assert(observer_base.is_valid()
               && "Reference for the requested cci-parameter is NULL");

        std::cout << "\n\t[OBSERVER - create_param_cb] : Parameter Name : "
                  << observer_base.get_name() << "\thas been created."
                  << std::endl;
        std::cout << "\n\t[OBSERVER - create_param_cb] : Parameter Value : "
                  << observer_base.get_cci_value().to_json()
                  << std::endl;

        /// Registering other (read/write/destroy) callbacks on the newly created cci-parameters
        std::cout << "\n\t[OBSERVER - create_param_cb] : Registering other"
                     " callbacks on the newly created cci-parameters"
                  << std::endl;

        // Recommended way of registering a callback (directly using base
        // parameter references)
        // TODO: fixme
        /*observer_cb.push_back(observer_base_ptr->register_callback(cci::pre_read,
                                                                   this,
                                                                   cci::bind(&ex20_observer::read_callback,
                                                                             this, _1, _2)));
        // Possible but not the recommended way of registering a callback
        observer_cb.push_back(Observer_BrokerIF->register_callback(cci::pre_write,
                                                                   _base_param_str.c_str(),
                                                                   this,
                                                                   cci::bind(&ex20_observer::write_callback,
                                                                             this, _1, _2)));
        // Possible (again) but not the recommended style of registering of
        // a callback
        observer_cb.push_back(Observer_BrokerIF->register_callback(cci::post_write,
                                                                   _base_param_str.c_str(),
                                                                   this,
                                                                   cci::bind(&ex20_observer::write_callback,
                                                                             this, _1, _2)));*/

        break;
      }

      default:
        std::cout << "\t\n[OBSERVER - create_param_cb] :"
                     " Invalid callback reason" << std::endl;
    }

    return cci::return_nothing;
  }

  /**
   *  @fn     cci::callback_return_type read_callback(cci::cci_base_param& _base_param, const cci::callback_type& cb_reason)
   *  @brief  Callback implementation for read activities on the cci parameters
   *  @param  _base_param Reference to the cci parameter associated with the callback
   *  @param  cb_reason The reason for the callback being called.
   *  @return The exit status of the callback function.
   */
  // TODO: fixme
  /*cci::callback_return_type
      read_callback(cci::cci_base_param & _base_param,
                    const cci::callback_type & cb_reason) {
    switch (cb_reason) {
      case cci::pre_read: {
        std::cout << "\t[OBSERVER - pre_read_cb] : Parammeter Name : "
                  << _base_param.get_name() << "\tvalue will be read."
                  << std::endl;
        break;
      }

      default:
        std::cout << "\n\t[OBSERVER - read_param_cb] : Invalid callback reason"
                  << std::endl;
    }
    return cci::return_nothing;
  }*/

  /**
   *  @fn     cci::callback_return_type write_callback(cci::cci_base_param& _base_param, const cci::callback_type& cb_reason)
   *  @brief  Callback implementation for write activities on the cci parameters
   *  @param  _base_param Reference to the cci parameter associated with the callback
   *  @param  cb_reason The reason for the callback being called.
   *  @return The exit status of the callback function.
   */
  // TODO: fixme
  /*cci::callback_return_type
      write_callback(cci::cci_base_param & _base_param,
                     const cci::callback_type & cb_reason) {
    switch (cb_reason) {
      case cci::pre_write: {
        std::cout << "\n\t[OBSERVER - pre_write_cb] : Retrieving details of"
                     " new cci-parameter" << std::endl;
        std::cout << "\t[OBSERVER - pre_write_cb] : Parameter Name : "
                  << _base_param.get_name() << "\tParameter Value : "
                  << _base_param.get_cci_value().to_json() << std::endl;
        break;
      }

      case cci::post_write: {
        std::cout << "\n\t[OBSERVER - post_write_cb] : Retrieving details of"
                     " new cci-parameter" << std::endl;
        std::cout << "\t[OBSERVER - post_write_cb] : Parameter Name : "
                  << _base_param.get_name() << "\tParameter Value : "
                  << _base_param.get_cci_value().to_json() << std::endl;
        break;
      }

      default:
        std::cout << "\n\t[OBSERVER - write_param_cb] : Invalid callback reason"
                  << std::endl;
    }
    return cci::return_nothing;
  }*/

 private:
  cci::cci_broker_if* Observer_BrokerIF; ///< Declaring a CCI configuration broker interface instance

  cci::cci_param_handle observer_base;  ///< Handle of Owner's CCI Parameter (integer type)

  std::vector<cci::shared_ptr<cci::callb_adapt> > observer_cb; ///< Registering all the various callbacks of the owner module cci_parameters
};
// ex20_observer

#endif  // EXAMPLES_EX20_TRACING_PARAMETER_ACTIVITY_EX20_OBSERVER_H_
