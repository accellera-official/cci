/*****************************************************************************
  Copyright 2006-2014 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2010-2015 Texas Instruments Inc.
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
 *  @file   simple_ip.h
 *  @brief  A testbench that demonstrates Requesting and responding to value change notifications 
 *          for an integer parameter
 *  @author Sheshadri Chakravarthy, Texas Instruments
 */

#ifndef EXAMPLES_EX08_PARAMETER_VALUE_CHANGE_CALLBACKS_EX08_SIMPLE_IP_H_
#define EXAMPLES_EX08_PARAMETER_VALUE_CHANGE_CALLBACKS_EX08_SIMPLE_IP_H_

#include <cassert>
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

    // Register call back on param_1
    param_1_pre_read_cb = param_1.register_callback(
        cci::pre_read, this,
        cci::bind(&ex08_simple_ip::read_config_callback, this, _1, _2));
    param_1_pre_write_cb = param_1.register_callback(
        cci::pre_write, this,
        cci::bind(&ex08_simple_ip::write_config_callback, this, _1, _2));
    param_1_post_write_cb = param_1.register_callback(
        cci::post_write, this,
        cci::bind(&ex08_simple_ip::write_config_callback, this, _1, _2));

    param_2_pre_read_cb = param_2.register_callback(
        cci::pre_read, this,
        cci::bind(&ex08_simple_ip::read_config_callback, this, _1, _2));
    param_2_pre_write_cb = param_2.register_callback(
        cci::pre_write, this,
        cci::bind(&ex08_simple_ip::write_config_callback, this, _1, _2));
    param_2_reject_write_cb = param_2.register_callback(
        cci::reject_write, this,
        cci::bind(&ex08_simple_ip::write_config_callback, this, _1, _2));
    param_2_post_write_cb = param_2.register_callback(
        cci::post_write, this,
        cci::bind(&ex08_simple_ip::write_config_callback, this, _1, _2));

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
   *  @fn     cci::callback_return_type write_config_callback(cci::cci_base_param& selected_param, const cci::callback_type& cb_reason)
   *  @brief  Callback function of writes on parameters
   *  @param  selected_param  Paramter callback was trigger on
   *  @param  cb_reason The reason for the callback
   *  @return A callback_return_type
   */
  cci::callback_return_type write_config_callback(
      cci::cci_base_param& selected_param,
      const cci::callback_type& cb_reason) {
    // Take action based on cb_reason
    switch (cb_reason) {
      case cci::pre_write:
        XREPORT("write_config_callback: pre_write: "
                << selected_param.get_name() << " ("
                << selected_param.get_cci_value().to_json() << ")");
        break;
      case cci::reject_write:
        if (selected_param.get_name() == "sim_ip.param_2") {
          XREPORT("write_config_callback: reject_write: Rejecting Value"
                  " change on " << selected_param.get_name() << " ("
                  << selected_param.get_cci_value().to_json() << ")");
          return cci::return_value_change_rejected;
        } else {
          XREPORT("write_config_callback: reject_write: "
                  << selected_param.get_name() << " ("
                  << selected_param.get_cci_value().to_json() << ")");
        }
        break;
      case cci::post_write:
        XREPORT("write_config_callback: post_write: "
                << selected_param.get_name());
        break;
      default:
        XREPORT_WARNING("write_config_callback: Unexpected callback" " type ("
                        << cb_reason << ")");
    }
    return cci::return_nothing;
  }

  /**
     *  @fn     cci::callback_return_type read_config_callback(cci::cci_base_param& selected_param, const cci::callback_type& cb_reason)
     *  @brief  Callback function for reads on parameters
     *  @param  selected_param  Paramter callback was trigger on
     *  @param  cb_reason The reason for the callback
     *  @return A callback_return_type
     */
  cci::callback_return_type read_config_callback(
      cci::cci_base_param& selected_param,
      const cci::callback_type& cb_reason) {
    XREPORT_PLAIN("read_config_callback: pre_read: "
                  << selected_param.get_name() << " ("
                  << selected_param.get_cci_value().to_json() << ")");
    return cci::return_nothing;
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
  cci::shared_ptr<cci::callb_adapt> param_1_pre_read_cb; ///< Parameter 1 pre read callback pointer
  cci::shared_ptr<cci::callb_adapt> param_1_pre_write_cb;  ///< Parameter 1 pre write callback pointer
  cci::shared_ptr<cci::callb_adapt> param_1_post_write_cb; ///< Parameter 1 post write callback pointer

  cci::shared_ptr<cci::callb_adapt> param_2_pre_read_cb; ///< Parameter 2 pre read callback pointer
  cci::shared_ptr<cci::callb_adapt> param_2_pre_write_cb;  ///< Parameter 2 pre write callback pointer
  cci::shared_ptr<cci::callb_adapt> param_2_reject_write_cb; ///< Parameter 2 reject callback pointer
  cci::shared_ptr<cci::callb_adapt> param_2_post_write_cb; ///< Parameter 2 post write callback pointer
};
// ex08_simple_ip

#endif  // EXAMPLES_EX08_PARAMETER_VALUE_CHANGE_CALLBACKS_EX08_SIMPLE_IP_H_
