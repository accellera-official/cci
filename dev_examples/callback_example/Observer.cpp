/*****************************************************************************
  Copyright 2016 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2011 GreenSocs
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


#include "Observer.h"
#include <systemc.h>


Observer::Observer(const char* name)
: mBroker(&cci::cci_broker_manager::get_current_broker(cci::cci_originator("OBSERVER")))
, mName(name)
{ 
  DEMO_DUMP(mName.c_str(), "register for new parameter callbacks");
  mCallbacks.push_back( mBroker->register_str_callback(cci::create_param, "*", this, 
                                                       cci::bind(&Observer::config_new_param_callback, this, _1, _2)) );
}

Observer::~Observer() {
  // unregister all callbacks (this is optional, callbacks get unregistered if all references are deleted)
  std::vector< cci::shared_ptr<cci::callb_adapt> >::iterator iter;
  for (iter = mCallbacks.begin(); iter != mCallbacks.end(); iter++) {
    (*iter)->unregister_at_parameter();
  }
}

/// Callback function with string signature showing changes for implicit and explicit parameters.
cci::callback_return_type Observer::config_str_callback(const std::string& par_name, const cci::callback_type& cb_reason) {
  std::string str = mBroker->get_cci_value(par_name).to_json();
  switch (cb_reason) {
  case cci::pre_write:
    DEMO_DUMP(name(), "**** String callback for parameter '" << par_name << "' will change value");
    break;
  case cci::post_write:
    DEMO_DUMP(name(), "**** String callback for parameter '" << par_name << "' changed to value="<<str);
    break;
  default:
    assert(false && "not awaited cb reason");
  }
  return cci::return_nothing;
}

/// Callback function with string signature announcing new parameters.
cci::callback_return_type Observer::config_new_param_callback(const std::string& par_name, const cci::callback_type& cb_reason) {
  assert(cb_reason == cci::create_param);
  DEMO_DUMP(name(), "***** New parameter callback: '" << par_name << "'");
  cci::cci_param_handle p = mBroker->get_param_handle(par_name);
  assert(p.is_valid() && "This new param should already be available!");
  std::string str = p.get_cci_value().to_json();
  DEMO_DUMP(name(), "   value="<<str);
  // Now register for some other callbacks
  // It is recommended to register with the parameter object directly:
  // TODO: fixme
  /*mCallbacks.push_back(p->register_callback(cci::pre_write, this,
                                            cci::bind(&Observer::config_callback, this, _1, _2)));
  // It is possible (but not recommended) to let the broker forward the callback registration to the parameter
  mCallbacks.push_back( mBroker->register_callback(cci::post_write, par_name.c_str(), this, 
                                                   cci::bind(&Observer::config_callback, this, _1, _2)));
  // it is still possible to register a string callback function
  mCallbacks.push_back( mBroker->register_str_callback(cci::post_write, par_name.c_str(), this, 
                                                       cci::bind(&Observer::config_str_callback, this, _1, _2)));*/

  return cci::return_nothing;
}

/// Callback function with default signature showing changes.
// TODO: fixme
/*cci::callback_return_type Observer::config_callback(cci::cci_base_param& par, const cci::callback_type& cb_reason) {
  std::string str;
  switch (cb_reason) {
    case cci::pre_write:
      str = par.get_cci_value().to_json();
      DEMO_DUMP(name(), "**** Object callback for parameter '" << par.get_name() << "' will change value");
      break;
    case cci::post_write:
      str = par.get_cci_value().to_json();
      DEMO_DUMP(name(), "**** Object callback for parameter '" << par.get_name() << "' changed to value="<<str);
      break;
    default:
      assert(false && "not awaited cb reason");
  }
  return cci::return_nothing;
}*/

const char* Observer::name() const {
  return mName.c_str();
}
