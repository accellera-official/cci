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
 *  @file     observer.h
 *  @brief    This header declares and defines the observer class
 *  @author   P V S Phaneendra, CircuitSutra Technologies <pvs@circuitsutra.com>
 *  @date     11th October, 2011 (Tuesday)
 */

#ifndef EXAMPLES_EX20_TRACING_PARAMETER_ACTIVITY_EX20_OBSERVER_H_
#define EXAMPLES_EX20_TRACING_PARAMETER_ACTIVITY_EX20_OBSERVER_H_

#include <cci_configuration>
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
      m_broker(cci::cci_get_global_broker(
              cci::cci_originator("observer"))),
      new_param_handle(cci::cci_originator("ex20_observer")) // initialize as invalid
  {
    // Registering callback for the creation of the pointer to the
    // cci_parameter of the owner module
    m_broker.register_create_callback(
            sc_bind(&ex20_observer::config_new_param_callback,
                    this,
                    sc_unnamed::_1));
  }

  /**
   *  @fn     ~ex20_observer()
   *  @brief  The class destructor
   *  @return void
   */
  ~ex20_observer() {
  }

  /**
   *  @fn     cci::callback_return_type config_new_param_callback(const std::string& _base_param_str, const cci::callback_type& cb_reason)
   *  @brief  Callback implementation for scanning/detecting creation of new cci parameters.
   *  @param  _base_param_str Newly created cci parameter reference
   *  @param  cb_reason The reason for the callback being called
   *  @return The exit status of the callback function.
   */
  void config_new_param_callback(const cci::cci_param_untyped_handle& param_handle) {

    std::cout << "\n\t[OBSERVER - create_param_cb] : Retrieving handle"
                 " for newly created cci_parameter" << std::endl;

    // Get reference of newly created cci-parameters
    new_param_handle = m_broker.get_param_handle(param_handle.name());

    // Assert if reference of the cci-parameter returned is NULL
    sc_assert(new_param_handle.is_valid()
           && "Reference for the requested cci-parameter is NULL");

    std::cout << "\n\t[OBSERVER - create_param_cb] : Parameter Name : "
              << new_param_handle.name() << "\thas been created."
              << std::endl;
    std::cout << "\n\t[OBSERVER - create_param_cb] : Parameter Value : "
              << new_param_handle.get_cci_value().to_json()
              << std::endl;

    /// Register pre-read, pre/post-write callbacks on the newly created cci-parameters
    std::cout << "\n\t[OBSERVER - create_param_cb] : Registering other"
                 " callbacks on the newly created cci-parameters"
              << std::endl;

    observer_cb.push_back(new_param_handle.register_pre_read_callback(
            &ex20_observer::pre_read_callback,this));

    observer_cb.push_back(new_param_handle.register_pre_write_callback(
            &ex20_observer::pre_write_callback,this));

    observer_cb.push_back(new_param_handle.register_post_write_callback(
            &ex20_observer::post_write_callback,this));

  }

  /**
   *  @fn     void typed_pre_read_callback(const cci::cci_param_read_event<int> & ev)
   *  @brief  Pre Callback function for reads on parameters
   *  @return void
   */
  void pre_read_callback(const cci::cci_param_read_event<> & ev)
  {
    std::cout << "\t[OBSERVER - pre_read_cb] : Parammeter Name : "
              << ev.param_handle.name() << "\tvalue will be read."
              << std::endl;
  }

  /**
   *  @fn     bool typed_pre_write_callback(const cci::cci_param_write_event<int> & ev)
   *  @brief  Pre Callback function for writes on parameters
   *  @return True to indicate the write is valid
   */
  bool pre_write_callback(const cci::cci_param_write_event<> & ev)
  {
    std::cout << "\n\t[OBSERVER - pre_write_cb] : Retrieving details of"
               " new cci-parameter" << std::endl;
    std::cout << "\t[OBSERVER - pre_write_cb] : Parameter Name : "
              << ev.param_handle.name() << "\tParameter Value : "
              << ev.new_value << std::endl;
    return true;
  }

  /**
   *  @fn     void typed_post_write_callback(const cci::cci_param_write_event<int> & ev)
   *  @brief  Post Callback function for writes on parameters
   *  @return void
   */
  void post_write_callback(const cci::cci_param_write_event<> & ev)
  {
    std::cout << "\n\t[OBSERVER - post_write_cb] : Retrieving details of"
               " new cci-parameter" << std::endl;
    std::cout << "\t[OBSERVER - post_write_cb] : Parameter Name : "
              << ev.param_handle.name() << "\tParameter Value : "
              << ev.new_value << std::endl;
  }

 private:
  cci::cci_broker_handle m_broker; ///< Declaring a CCI configuration broker handle

  cci::cci_param_handle new_param_handle;  ///< Handle to newly created parameter

  std::vector<cci::cci_callback_untyped_handle> observer_cb; ///< Registering all the various callbacks of the owner module cci_parameters
};
// ex20_observer

#endif  // EXAMPLES_EX20_TRACING_PARAMETER_ACTIVITY_EX20_OBSERVER_H_
