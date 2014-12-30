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
 * @file     observer.h
 * @brief    This header declares and defines the observer class 
 * @author   P V S Phaneendra, CircuitSutra Technologies <pvs@circuitsutra.com>
 * @date     11th October, 2011 (Tuesday)
 */

#ifndef EXAMPLES_EX20_TRACING_PARAMETER_ACTIVITY_EX20_OBSERVER_H_
#define EXAMPLES_EX20_TRACING_PARAMETER_ACTIVITY_EX20_OBSERVER_H_

#include <cci>
#include <cassert>
#include <vector>

#include "xreport.hpp"

/// The observer class illustrates the recommended and possible ways of
/// tracing parameter activities of a cci-parameter by registering various
/// callbacks
class ex20_observer {
 public:
  ex20_observer() {
    /// Create an instance of the cci_originator
    cci::cnf::cci_originator observer_originator("observer");

    // Get handle of the broker responsible for the class/module
    Observer_BrokerIF =
        &cci::cnf::cci_broker_manager::get_current_broker(observer_originator);

    /// Report if handle returned is NULL
    assert(Observer_BrokerIF != NULL && "Configuration Broker handle is NULL");

    /// Registering callback for the creation of the pointer to the
    /// cci_parameter of the owner module
    observer_cb.push_back(Observer_BrokerIF->register_str_callback(cci::cnf::create_param, "*",
                                                                   this,
                                                                   cci::bind(&ex20_observer::config_new_param_callback,
                                                                             this, _1, _2)));
  }

  ~ex20_observer() {
    //@TODO
  }

/// Callback implementation for scanning/detecting creation of new cci-parameters
  cci::cnf::callback_return_type
      config_new_param_callback(const std::string & _base_param_str,
                                const cci::cnf::callback_type & cb_reason) {
    switch (cb_reason) {
      case cci::cnf::create_param: {
        std::cout << "\n\t[OBSERVER - create_param_cb] : Retrieving reference"
                     " of newly created cci_parameter" << std::endl;

        // Get reference of newly created cci-parameters
        observer_base_ptr = Observer_BrokerIF->get_param(_base_param_str);

        // Assert if reference of the cci-parameter returned is NULL
        assert(observer_base_ptr != NULL
               && "Reference for the requested cci-parameter is NULL");

        std::cout << "\n\t[OBSERVER - create_param_cb] : Parameter Name : "
                  << observer_base_ptr->get_name() << "\thas been created."
                  << std::endl;
        std::cout << "\n\t[OBSERVER - create_param_cb] : Parameter Value : "
                  << observer_base_ptr->json_serialize() << std::endl;

        /// Registering other (read/write/destroy) callbacks on the newly created cci-parameters
        std::cout << "\n\t[OBSERVER - create_param_cb] : Registering other"
                     " callbacks on the newly created cci-parameters"
                  << std::endl;

        /// Recommended way of registering a callback (directly using base
        /// parameter references)
        observer_cb.push_back(observer_base_ptr->register_callback(cci::cnf::pre_read,
                                                                   this,
                                                                   cci::bind(&ex20_observer::read_callback,
                                                                             this, _1, _2)));
        /// Possible but not the recommended way of registering a callback
        observer_cb.push_back(Observer_BrokerIF->register_callback(cci::cnf::pre_write,
                                                                   _base_param_str.c_str(),
                                                                   this,
                                                                   cci::bind(&ex20_observer::write_callback,
                                                                             this, _1, _2)));
        /// Possible (again) but not the recommended style of registering of
        /// a callback
        observer_cb.push_back(Observer_BrokerIF->register_callback(cci::cnf::post_write,
                                                                   _base_param_str.c_str(),
                                                                   this,
                                                                   cci::bind(&ex20_observer::write_callback,
                                                                             this, _1, _2)));

        break;
      }

      default:
        std::cout << "\t\n[OBSERVER - create_param_cb] :"
                     " Invalid callback reason" << std::endl;
    }

    return cci::cnf::return_nothing;
  }

/// Callback implementation for read activities on the cci-parameter(s)
  cci::cnf::callback_return_type
      read_callback(cci::cnf::cci_base_param & _base_param,
                    const cci::cnf::callback_type & cb_reason) {
    switch (cb_reason) {
      case cci::cnf::pre_read: {
        std::cout << "\t[OBSERVER - pre_read_cb] : Parammeter Name : "
                  << _base_param.get_name() << "\tvalue will be read."
                  << std::endl;
        break;
      }

      default:
        std::cout << "\n\t[OBSERVER - read_param_cb] : Invalid callback reason"
                  << std::endl;
    }
    return cci::cnf::return_nothing;
  }

/// Callback implementation for write activities on the cci-parameter(s)
  cci::cnf::callback_return_type
      write_callback(cci::cnf::cci_base_param & _base_param,
                     const cci::cnf::callback_type & cb_reason) {
    switch (cb_reason) {
      case cci::cnf::pre_write: {
        std::cout << "\n\t[OBSERVER - pre_write_cb] : Retrieving details of"
                     " new cci-parameter" << std::endl;
        std::cout << "\t[OBSERVER - pre_write_cb] : Parameter Name : "
                  << _base_param.get_name() << "\tParameter Value : "
                  << _base_param.json_serialize() << std::endl;
        break;
      }

      case cci::cnf::post_write: {
        std::cout << "\n\t[OBSERVER - post_write_cb] : Retrieving details of"
                     " new cci-parameter" << std::endl;
        std::cout << "\t[OBSERVER - post_write_cb] : Parameter Name : "
                  << _base_param.get_name() << "\tParameter Value : "
                  << _base_param.json_serialize() << std::endl;
        break;
      }

      default:
        std::cout << "\n\t[OBSERVER - write_param_cb] : Invalid callback reason"
                  << std::endl;
    }
    return cci::cnf::return_nothing;
  }

 private:
  /// Declaring a CCI configuration broker interface instance
  cci::cnf::cci_cnf_broker_if* Observer_BrokerIF;

  /// Reference of Owner's CCI Parameter (integer type)
  cci::cnf::cci_base_param* observer_base_ptr;

  /// Registering all the various callbacks of the owner module cci_parameters
  std::vector<cci::shared_ptr<cci::cnf::callb_adapt> > observer_cb;
};
/// ex20_observer

#endif  // EXAMPLES_EX20_TRACING_PARAMETER_ACTIVITY_EX20_OBSERVER_H_
