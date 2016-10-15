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
 * @file   ObserverModule.cpp
 * @author Christian Schroeder, GreenSocs
 * @author Mark Burton, GreenSocs
 */

#include "ObserverModule.h"
#include <systemc.h>

ObserverModule::ObserverModule(sc_core::sc_module_name name)
: sc_core::sc_module(name)
{ 
  // get the config API which is responsible for this module
  mApi = &cci::cci_broker_manager::get_current_broker(cci::cci_originator(*this));
  SC_THREAD(main_action);
}


ObserverModule::~ObserverModule() {
  // unregister all callbacks (this is optional, callbacks get unregistered if all references are deleted)
//  std::vector< cci::shared_ptr<cci::callb_adapt> >::iterator iter;
//  for (iter = mCallbacks.begin(); iter != mCallbacks.end(); iter++) {
//    (*iter)->unregister_at_parameter();
//  }
}


void ObserverModule::main_action() {
  cout << endl;
  DEMO_DUMP(name(), "Executing main_action");

  // ******** register for new parameter callbacks ***************
  DEMO_DUMP(name(), "Registering for new parameter callbacks");
  cci::cci_param_create_callback_handle cb_new_pa;

  cb_new_pa = mApi->register_create_callback(
          sc_bind(&ObserverModule::config_new_param_callback,this,sc_unnamed::_1),
          cci::cci_originator(*this));
  mCallbacks.push_back(cb_new_pa);// This will not be deleted after end of main_action()

  // ******** register for parameter change callbacks ***************
  DEMO_DUMP(name(), "Registering for Owner.int_param post_write callbacks");
  cci::cci_param_handle p = mApi->get_param_handle("Owner.int_param");
  assert(p.is_valid());
  cci::cci_callback_untyped_handle cb1, cb3;

  cb1 = p.register_post_write_callback(&ObserverModule::config_callback,this);
 
  DEMO_DUMP(name(), "Registering for Owner.uint_param post_write callbacks");
  cci::cci_param_handle p2 = mApi->get_param_handle("Owner.uint_param");
  assert(p2.is_valid());

  cb3 = p2.register_post_write_callback(&ObserverModule::config_callback,this);
  
  mCallbacks.push_back(cb3);// This will not be deleted after end of main_action()
}

/// Callback function with default signature showing changes.
void ObserverModule::config_callback(const cci::cci_param_write_event<> & ev) {
  DEMO_DUMP(name(), "Callback for parameter '" << ev.param_handle.get_name()
            << "' changed to value '"<<ev.new_value<<"'");
}

/// Callback function with default signature announcing new parameters.
void ObserverModule::config_new_param_callback(cci::cci_param_untyped_handle& param_handle) {
  DEMO_DUMP(name(), "New parameter callback '" << param_handle.get_name()
            << "', value '" << param_handle.get_cci_value() << "'");
}
