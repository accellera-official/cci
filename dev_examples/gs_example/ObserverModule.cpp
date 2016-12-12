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
: sc_core::sc_module(name),
  mBroker(cci::cci_broker_manager::get_broker())
{
  SC_THREAD(main_action);
}


ObserverModule::~ObserverModule() {
  // unregister all callbacks (this is optional, callbacks get unregistered if all references are deleted)
  std::vector<cci::cci_callback_untyped_handle>::iterator iter;
  // TODO: FIXME
  /*for (iter = mCallbacks.begin(); iter != mCallbacks.end(); iter++) {
    mBroker.
  }*/
}


void ObserverModule::main_action() {
  cout << "----------------------------" << endl;

  cci::cci_originator orig = mBroker.get_latest_write_originator("Owner.int_param");
  assert (!orig.is_unknown());
  if (mBroker.get_param_handle("Owner.int_param").is_valid()) {
    DEMO_DUMP(name(), "Write originator for EXPLICIT param Owner.int_param (from broker): " << orig.name());
  } else {
    DEMO_DUMP(name(), "Write originator for IMPLICIT param Owner.int_param (from broker): " << orig.name());
  }
  
  DEMO_DUMP(name(), "register for new parameter callbacks");
  cci::cci_callback_untyped_handle cb_new_pa;
  cb_new_pa = mBroker.register_create_callback(sc_bind(
          &ObserverModule::config_new_param_callback, this, sc_unnamed::_1));
  mCallbacks.push_back(cb_new_pa);// This will not be deleted after end of main_action()
  
  DEMO_DUMP(name(), "register pre write callback for int_param to check value settings and reject them");

  // get access to a foreign parameter using the module's config API
  cci::cci_param_handle p = mBroker.get_param_handle("Owner.int_param");
  assert(p.is_valid());

  // ******** register for parameter change callback ***************
  cci::cci_callback_untyped_handle cb1a, cb1b, cb3a, cb3b;
  cb1a = p.register_pre_write_callback(&ObserverModule::config_pre_write_callback, this);
  cb1b = p.register_post_write_callback(&ObserverModule::config_post_write_callback, this);
  mCallbacks.push_back(cb1a);// This will not be deleted after end of main_action()
  mCallbacks.push_back(cb1b);// This will not be deleted after end of main_action()*/
  cci::cci_param_handle p2 = mBroker.get_param_handle("Owner.uint_param");
  assert(p2.is_valid());
  p2.register_pre_write_callback(&ObserverModule::config_pre_write_callback, this);
  p2.register_post_write_callback(&ObserverModule::config_post_write_callback, this);
  mCallbacks.push_back(cb3a);// This will not be deleted after end of main_action()
  mCallbacks.push_back(cb3b);// This will not be deleted after end of main_action()

  // ******* Testing parameter lock with callback returns *************
  // Register Callback for parameter int_param in module other_ip (IP1)
  cci::cci_callback_untyped_handle cb2;
  cb2 = p.register_pre_write_callback(&ObserverModule::config_callback_reject_changes, this);
  std::string str = p.get_cci_value().to_json();
  cout << "int_param has value = " << str << endl;
  cout << "int_param set value = 99 (shall fail)" << endl;
  try {
    p.set_cci_value(cci::cci_value::from_json("99")); // this shall fail because rejected!
  } catch(sc_core::sc_report e) {
    // If set_param_failed error, catch it
    switch ( cci::cci_report_handler::get_param_failure(e) ) {
      case cci::CCI_SET_PARAM_FAILURE: 
        std::cout << std::endl << name() << ": Caught " << e.what() << std::endl;
        break;
      default:
        // If other error, throw it again
        throw e;
    }
  }
  str = p.get_cci_value().to_json();
  cout << "int_param has value = " << str << endl;
  //p.unregister_all_callbacks(this); // this would unregister all calbacks to this module
  p.unregister_pre_write_callback(cb2); // unregister a callback
  
  // ********* show param list **************
  cout << "param list:" << endl;
  std::vector<std::string> vec = mBroker.get_param_list();
  for (std::vector<std::string>::iterator iter = vec.begin(); iter != vec.end(); iter++)
    cout << *iter << " ";
  cout << endl;
  
  std::cout << std::endl;
  
  wait(11, SC_NS);
  // **** check for latest originator using the alternative API on parameter objects 
  // (instead of global originator function within callbacks)
  
  DEMO_DUMP(name(), "latest write originator of parameter '"<< p.get_name() << "': " << p.get_latest_write_originator().name());
  p.set_cci_value(cci::cci_value::from_json("666666"));
  DEMO_DUMP(name(), "latest write originator of parameter '"<< p.get_name() << "': " << p.get_latest_write_originator().name());
  
  orig = mBroker.get_latest_write_originator(p.get_name());
  assert (!orig.is_unknown());
  if (mBroker.get_param_handle(p.get_name()).is_valid()) {
    DEMO_DUMP(this->name(), "Write originator for EXPLICIT param "<<p.get_name()<<" (from broker): " << orig.name());
  } else {
    DEMO_DUMP(this->name(), "Write originator for IMPLICIT param "<<p.get_name()<<" (from broker): " << orig.name());
  }
  
}


/// Callback function with default signature showing changes.
bool ObserverModule::config_pre_write_callback(const cci::cci_param_write_event<> & ev) {
  std::string str;
  str = ev.param_handle.get_cci_value().to_json();
  DEMO_DUMP(name(), "Callback for parameter '" << ev.param_handle.get_name() << "' will change value, originator: "<< ev.originator.name());
  return true;
}

void ObserverModule::config_post_write_callback(const cci::cci_param_write_event<> & ev) {
  std::string str;
  str = ev.param_handle.get_cci_value().to_json();
  DEMO_DUMP(name(), "Callback for parameter '" << ev.param_handle.get_name() << "' changed to value '"<<str<<"', originator: "<< ev.originator.name());
}

/// Callback function with default signature announcing new parameters.
void ObserverModule::config_new_param_callback(
        const cci::cci_param_untyped_handle& param_handle) {
  assert(param_handle.is_valid() && "This new param should already be valid!");
  std::string str = param_handle.get_cci_value().to_json();
  DEMO_DUMP(name(), "New parameter callback '" << param_handle.get_name() << "', value '"<<str<<"', originator: "<< param_handle.get_originator().name());
}

/// Callback function with default signature rejecting all changes.
bool ObserverModule::config_callback_reject_changes(const cci::cci_param_write_event<> & ev) {
  DEMO_DUMP(name(), "Callback method called (which rejects changes):");
  cout << "  Parameter '" << ev.param_handle.get_name() << "', originator: "<< ev.originator.name() << endl;
  cout << "  REJECT VALUE CHANGE!!" << endl;
  cout << endl;
  return false;
}
