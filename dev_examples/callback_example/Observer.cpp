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
 * @file   Observer.cpp
 * @author Christian Schroeder, GreenSocs
 * @author Mark Burton, GreenSocs
 */

#include "Observer.h"
#include <systemc.h>

Observer::Observer(const char* name)
: mBroker(cci::cci_broker_manager::get_broker(cci::cci_originator("OBSERVER")))
, mName(name)
{ 
  DEMO_DUMP(mName.c_str(), "register for new parameter callbacks");
  mCallbacks.push_back( mBroker.register_create_callback(
          sc_bind(&Observer::config_new_param_callback,this,sc_unnamed::_1)));
}

Observer::~Observer() {
  // unregister all callbacks (this is optional, callbacks get unregistered if all references are deleted)
//  std::vector< cci::shared_ptr<cci::callb_adapt> >::iterator iter;
//  for (iter = mCallbacks.begin(); iter != mCallbacks.end(); iter++) {
//    (*iter)->unregister_at_parameter();
//  }
}

/// Callback function with string signature showing changes for implicit and explicit parameters.
void Observer::config_str_post_write_callback(const cci::cci_param_write_event<> & ev) {
    DEMO_DUMP(name(), "**** String callback for parameter '"
              << ev.param_handle.get_name() << "' changed to value="<< ev.new_value);
}

/// Callback function with string signature announcing new parameters.
void Observer::config_new_param_callback(const cci::cci_param_untyped_handle& param_handle) {
  DEMO_DUMP(name(), "***** New parameter callback: '" << param_handle.get_name() << "'");
  cci::cci_param_handle p = mBroker.get_param_handle(param_handle.get_name());
  assert(p.is_valid() && "This new param should already be available!");
  std::string str = p.get_cci_value().to_json();
  DEMO_DUMP(name(), "   value="<<str);
  // Now register for some other callbacks
  // It is recommended to register with the parameter object directly:

  mCallbacks.push_back(p.register_pre_write_callback(
          &Observer::config_pre_write_callback, this));

  mCallbacks.push_back(p.register_post_write_callback(
          &Observer::config_post_write_callback, this));

  mCallbacks.push_back(p.register_post_write_callback(
          &Observer::config_str_post_write_callback, this));
}

/// Callback function with default signature showing changes.
void Observer::config_post_write_callback(const cci::cci_param_write_event<> & ev) {
    DEMO_DUMP(name(), "**** Object callback for parameter '"
              << ev.param_handle.get_name() << "' changed to value="<<ev.new_value);
}

bool Observer::config_pre_write_callback(const cci::cci_param_write_event<> & ev) {
    DEMO_DUMP(name(), "**** Object callback for parameter '"
              << ev.param_handle.get_name() << "' will change value");
    return true;
}

const char* Observer::name() const {
  return mName.c_str();
}
