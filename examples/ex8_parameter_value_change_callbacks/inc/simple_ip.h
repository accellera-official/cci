/*******************************************************************************
  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2010 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.
******************************************************************************/

/**
 *  @file   simple_ip.h
 *  @brief  A testbench that demonstrates Requesting and responding to value change notifications 
 *          for an integer parameter
 *  @author Sheshadri Chakravarthy, Texas Instruments
 */

#ifndef INC_SIMPLE_IP_H
#define INC_SIMPLE_IP_H

//------------------------------------------------------------------------------
#include <cassert>
#include "cci.h"
#include "xreport.hpp"

//------------------------------------------------------------------------------
/// The SimpleIP owns 2 CCI params with associated callbacks
SC_MODULE(simple_ip) {
 public:
  //SC_HAS_PROCESS(simple_ip);

  /// Ctor
 SC_CTOR(simple_ip)
   // Initialize params
   : param_1("param_1", 1)
    , param_2("param_2", 2)
    {
      SC_THREAD(execute);

      // Register call back on param_1
      param_1_pre_read_cb   = param_1.register_callback(cci::cnf::pre_read,     this, cci::bind(&simple_ip::read_config_callback, this, _1, _2));
      param_1_pre_write_cb  = param_1.register_callback(cci::cnf::pre_write,    this, cci::bind(&simple_ip::write_config_callback, this, _1, _2));
      param_1_post_write_cb = param_1.register_callback(cci::cnf::post_write,   this, cci::bind(&simple_ip::write_config_callback, this, _1, _2));
            
      param_2_pre_read_cb   = param_2.register_callback(cci::cnf::pre_read,     this, cci::bind(&simple_ip::read_config_callback, this, _1, _2));
      param_2_pre_write_cb  = param_2.register_callback(cci::cnf::pre_write,    this, cci::bind(&simple_ip::write_config_callback, this, _1, _2));
      param_2_reject_write_cb= param_2.register_callback(cci::cnf::reject_write,this, cci::bind(&simple_ip::write_config_callback, this, _1, _2));
      param_2_post_write_cb = param_2.register_callback(cci::cnf::post_write,   this, cci::bind(&simple_ip::write_config_callback, this, _1, _2));

      XREPORT("@Ctor: Value of "<< param_1.get_name()<< " is "<< param_1<<" (Default value Specified: 1)");
      XREPORT("@Ctor: Value of "<< param_2.get_name()<< " is "<< param_2<<" (Default value Specified: 2)");
    }

  /// Dtor
  ~simple_ip() {
    XREPORT_PLAIN("@Dtor: Value of "<< param_1.get_name()<< " is "<< param_1);
    XREPORT_PLAIN("@Dtor: Value of "<< param_2.get_name()<< " is "<< param_2);
  }

  /// Write callback
  cci::cnf::callback_return_type
    write_config_callback(cci::cnf::cci_base_param& selected_param, const cci::cnf::callback_type& cb_reason) {
    // Take action based on cb_reason
    switch(cb_reason)
      {
      case cci::cnf::pre_write:
        XREPORT("write_config_callback: pre_write: "<< selected_param.get_name()<< " ("<< selected_param.json_serialize()<< ")");
        break;
      case cci::cnf::reject_write:
        if(selected_param.get_name() == "sim_ip.param_2") {
          XREPORT("write_config_callback: reject_write: Rejecting Value change on " << selected_param.get_name()<< " ("<< selected_param.json_serialize()<< ")");
          return cci::cnf::return_value_change_rejected;
        }
    else {
          XREPORT("write_config_callback: reject_write: "<< selected_param.get_name()<< " ("<< selected_param.json_serialize()<< ")");
    }
        break;
      case cci::cnf::post_write:
        XREPORT("write_config_callback: post_write: "<< selected_param.get_name());
        break;
      default:
        XREPORT_WARNING("write_config_callback: Unexpected callback type ("<< cb_reason<<")");
      }
    return cci::cnf::return_nothing;
  }

  /// Read callback
  cci::cnf::callback_return_type
    read_config_callback(cci::cnf::cci_base_param& selected_param, const cci::cnf::callback_type& cb_reason) {
    // some action
    XREPORT_PLAIN("read_config_callback: pre_read: "<< selected_param.get_name()<< " ("<< selected_param.json_serialize()<< ")");
    return cci::cnf::return_nothing;
  }

  /// Do nothing
  void execute() {
    wait(sc_core::SC_ZERO_TIME);
  }

 private:
  /// CCI params to illustrate callback
  cci::cnf::cci_param<int> param_1;
  cci::cnf::cci_param<int> param_2;

  /// Callback pointers
  cci::shared_ptr<cci::cnf::callb_adapt_b> param_1_pre_read_cb;
  cci::shared_ptr<cci::cnf::callb_adapt_b> param_1_pre_write_cb;
  cci::shared_ptr<cci::cnf::callb_adapt_b> param_1_post_write_cb;

  cci::shared_ptr<cci::cnf::callb_adapt_b> param_2_pre_read_cb;
  cci::shared_ptr<cci::cnf::callb_adapt_b> param_2_pre_write_cb;
  cci::shared_ptr<cci::cnf::callb_adapt_b> param_2_reject_write_cb;
  cci::shared_ptr<cci::cnf::callb_adapt_b> param_2_post_write_cb;
}; // simple_ip

#endif // INC_SIMPLE_IP_H

