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
 *  @file   simple_ip.h
 *  @brief  A testbench that demonstrates Requesting and responding to value change notifications 
 *          for an integer parameter
 *  @author Sheshadri Chakravarthy, Texas Instruments
 */

#ifndef EXAMPLES_EX08_PARAMETER_VALUE_CHANGE_CALLBACKS_EX08_SIMPLE_IP_H_
#define EXAMPLES_EX08_PARAMETER_VALUE_CHANGE_CALLBACKS_EX08_SIMPLE_IP_H_

#include "cci_configuration"
#include "xreport.hpp"

/**
 *  @class  ex08_simple_ip
 *  @brief  The simple ip owns 2 CCI params with associated callbacks
 */
SC_MODULE(ex08_simple_ip) {
 public:
  /**
   *  @fn     ex08_simple_ip
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex08_simple_ip)
  // Initialize params
      : param_1("param_1", 1),
        param_2("param_2", 2) {
    SC_THREAD(execute);

    // Register callback on param_1
    param_1_pre_read_cb   = param_1.register_pre_read_callback(
        &ex08_simple_ip::typed_pre_read_callback,this);
    param_1_pre_write_cb  = param_1.register_pre_write_callback(
        &ex08_simple_ip::typed_pre_write_callback,this);
    param_1_post_write_cb = param_1.register_post_write_callback(
        &ex08_simple_ip::typed_post_write_callback,this);

    // Register callback on param_2
    param_2_pre_read_cb   = param_2.register_pre_read_callback(
        &ex08_simple_ip::typed_pre_read_callback,this);
    param_2_pre_write_cb  = param_2.register_pre_write_callback(
        &ex08_simple_ip::typed_pre_write_callback_rejected,this);
    param_2_post_write_cb = param_2.register_post_write_callback(
        &ex08_simple_ip::typed_post_write_callback,this);

    XREPORT("@Ctor: Value of " << param_1.get_name() << " is " << param_1
            << " (Default value Specified: 1)");
    XREPORT("@Ctor: Value of " << param_2.get_name() << " is " << param_2
            << " (Default value Specified: 2)");
  }

  /**
   *  @fn     ~ex08_simple_ip()
   *  @brief  The class destructor
   *  @return void
   */
  ~ex08_simple_ip() {
    XREPORT_PLAIN("@Dtor: Value of " << param_1.get_name() << " is "
                  << param_1);
    XREPORT_PLAIN("@Dtor: Value of " << param_2.get_name() << " is "
                  << param_2);
  }

  /**
   *  @fn     void typed_pre_read_callback(const cci::cci_param_read_event<int> & ev)
   *  @brief  Pre Callback function for reads on parameters
   *  @return void
   */
  void typed_pre_read_callback(const cci::cci_param_read_event<int> & ev)
  {
    XREPORT("typed_pre_read_callback:"
            << ev.param_handle.get_name() << " ("
            << ev.value
            << ") by " << ev.originator.name());
  }

  /**
   *  @fn     bool typed_pre_write_callback(const cci::cci_param_write_event<int> & ev)
   *  @brief  Pre Callback function for writes on parameters
   *  @return True to indicate the write is valid
   */
  bool typed_pre_write_callback(const cci::cci_param_write_event<int> & ev)
  {
    XREPORT("typed_pre_write_callback:"
            << ev.param_handle.get_name() << " ("
            << ev.old_value << " -> " << ev.new_value
            << ") by " << ev.originator.name());
    return true;
  }

  /**
   *  @fn     void typed_post_write_callback(const cci::cci_param_write_event<int> & ev)
   *  @brief  Post Callback function for writes on parameters
   *  @return void
   */
  void typed_post_write_callback(const cci::cci_param_write_event<int> & ev)
  {
    XREPORT("typed_post_write_callback:"
            << ev.param_handle.get_name() << " ("
            << ev.old_value << " -> " << ev.new_value
            << ") by " << ev.originator.name());
  }

  /**
   *  @fn     bool typed_pre_write_callback_rejected(const cci::cci_param_write_event<int> & ev)
   *  @brief  Pre Callback function for writes on parameters
   *  @return False to indicate the write is not valid
   */
  bool typed_pre_write_callback_rejected(const cci::cci_param_write_event<int> & ev)
  {
    XREPORT("reject_pre_write_callback: Rejecting Value change on "
            << ev.param_handle.get_name() << " ("
            << ev.old_value << " -> " << ev.new_value
            << ") by " << ev.originator.name());
    return false;
  }

  /**
   *  @fn     void execute()
   *  @brief  Execute function (does nothing)
   *  @return void
   */
  void execute() {
    wait(sc_core::SC_ZERO_TIME);
  }

 private:
  cci::cci_param<int> param_1; ///< CCI int parameter
  cci::cci_param<int> param_2; ///< CCI int parameter

  // Callback pointers
  cci::cci_callback_untyped_handle param_1_pre_read_cb;   ///< Parameter 1 pre read callback handle
  cci::cci_callback_untyped_handle param_1_pre_write_cb;  ///< Parameter 1 pre write callback handle
  cci::cci_callback_untyped_handle param_1_post_write_cb; ///< Parameter 1 post write callback handle

  cci::cci_callback_untyped_handle param_2_pre_read_cb;   ///< Parameter 2 pre read callback handle
  cci::cci_callback_untyped_handle param_2_pre_write_cb;  ///< Parameter 2 pre write callback handle
  cci::cci_callback_untyped_handle param_2_post_write_cb; ///< Parameter 2 post write callback handle
};
// ex08_simple_ip

#endif  // EXAMPLES_EX08_PARAMETER_VALUE_CHANGE_CALLBACKS_EX08_SIMPLE_IP_H_
