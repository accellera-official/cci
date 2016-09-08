/*****************************************************************************
  Copyright 2016 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2009-2010 GreenSocs
  All rights reserved.
  Authors:
      Christian Schroeder <schroeder@eis.cs.tu-bs.de>
      Mark Burton <mark@greensocs.com>

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
  std::vector< cci::shared_ptr<cci::callb_adapt> >::iterator iter;
  for (iter = mCallbacks.begin(); iter != mCallbacks.end(); iter++) {
    (*iter)->unregister_at_parameter();
  }
}


void ObserverModule::main_action() {
  cout << endl;
  DEMO_DUMP(name(), "Executing main_action");

  // ******** register for new parameter callbacks ***************
  DEMO_DUMP(name(), "Registering for new parameter callbacks");
  cci::shared_ptr<cci::callb_adapt> cb_new_pa;
  // TODO: fixme
  /*cb_new_pa = mApi->register_str_callback(cci::create_param, "*", this,
                                   cci::bind(&ObserverModule::config_new_param_callback, this, _1, _2));*/
  mCallbacks.push_back(cb_new_pa);// This will not be deleted after end of main_action()

  // ******** register for parameter change callbacks ***************
  DEMO_DUMP(name(), "Registering for Owner.int_param post_write callbacks");
  cci::cci_param_handle p = mApi->get_param_handle("Owner.int_param");
  assert(p.is_valid());
  cci::shared_ptr<cci::callb_adapt> cb1, cb3;
  // TODO: fixme
  /*cb1 = p->register_callback(cci::post_write, this,
                             cci::bind(&ObserverModule::config_callback, this, _1, _2));*/
 
  DEMO_DUMP(name(), "Registering for Owner.uint_param post_write callbacks");
  cci::cci_param_handle p2 = mApi->get_param_handle("Owner.uint_param");
  assert(p2.is_valid());
  // TODO: fixme
  /*cb3 = p2->register_callback(cci::post_write, this,
                              cci::bind(&ObserverModule::config_callback, this, _1, _2));*/
  
  mCallbacks.push_back(cb3);// This will not be deleted after end of main_action()
}

/// Callback function with default signature showing changes.
// TODO: fixme
/*cci::callback_return_type ObserverModule::config_callback(cci::cci_base_param& par, const cci::callback_type& cb_reason) {
  assert(cb_reason == cci::post_write);
  std::string str = par.get_cci_value().to_json();
  DEMO_DUMP(name(), "Callback for parameter '" << par.get_name() << "' changed to value '"<<str<<"'");
  return cci::return_nothing;
}

/// Callback function with default signature announcing new parameters.
cci::callback_return_type ObserverModule::config_new_param_callback(const std::string& parname, const cci::callback_type& cb_reason) {
  assert(cb_reason == cci::create_param);
  cci::cci_base_param *p_param = mApi->get_param(parname);
  std::string str_val = p_param->get_cci_value().to_json();
  DEMO_DUMP(name(), "New parameter callback '" << parname << "', value '" << str_val << "'");
  return cci::return_nothing;
}*/
