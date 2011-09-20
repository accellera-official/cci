// LICENSETEXT
//
//   Copyright (C) 2011 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//    Mark Burton, mark@greensocs.com
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


#include "Observer.h"
#include <systemc.h>


Observer::Observer(const char* name)
: mBroker(&cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator("OBSERVER")))
, mName(name)
{ 
  DEMO_DUMP(mName.c_str(), "register for new parameter callbacks");
  mCallbacks.push_back( mBroker->register_str_callback(cci::cnf::create_param, "*", this, 
                                                       cci::bind(&Observer::config_new_param_callback, this, _1, _2)) );
}

Observer::~Observer() {
  // unregister all callbacks (this is optional, callbacks get unregistered if all references are deleted)
  std::vector< cci::shared_ptr<cci::cnf::callb_adapt> >::iterator iter;
  for (iter = mCallbacks.begin(); iter != mCallbacks.end(); iter++) {
    (*iter)->unregister_at_parameter();
  }
}

/// Callback function with string signature showing changes for implicit and explicit parameters.
cci::cnf::callback_return_type Observer::config_str_callback(const std::string& par_name, const cci::cnf::callback_type& cb_reason) {
  std::string str = mBroker->get_json_string(par_name);
  switch (cb_reason) {
  case cci::cnf::pre_write:
    DEMO_DUMP(name(), "**** String callback for parameter '" << par_name << "' will change value");
    break;
  case cci::cnf::post_write:
    DEMO_DUMP(name(), "**** String callback for parameter '" << par_name << "' changed to value '"<<str<<"'");
    break;
  default:
    assert(false && "not awaited cb reason");
  }
  return cci::cnf::return_nothing;
}

/// Callback function with string signature announcing new parameters.
cci::cnf::callback_return_type Observer::config_new_param_callback(const std::string& par_name, const cci::cnf::callback_type& cb_reason) {
  assert(cb_reason == cci::cnf::create_param);
  DEMO_DUMP(name(), "***** New parameter callback: '" << par_name << "'");
  cci::cnf::cci_base_param* p = mBroker->get_param(par_name);
  assert(p && "This new param should already be available!");
  std::string str = p->json_serialize();
  DEMO_DUMP(name(), "   value '"<<str<<"'");
  // Now register for some other callbacks
  // It is recommended to register with the parameter object directly:
  mCallbacks.push_back(p->register_callback(cci::cnf::pre_write, this, 
                                            cci::bind(&Observer::config_callback, this, _1, _2)));
  // It is possible (but not recommended) to let the broker forward the callback registration to the parameter
  mCallbacks.push_back( mBroker->register_callback(cci::cnf::post_write, par_name.c_str(), this, 
                                                   cci::bind(&Observer::config_callback, this, _1, _2)));
  // it is still possible to register a string callback function
  mCallbacks.push_back( mBroker->register_str_callback(cci::cnf::post_write, par_name.c_str(), this, 
                                                       cci::bind(&Observer::config_str_callback, this, _1, _2)));

  return cci::cnf::return_nothing;
}

/// Callback function with default signature showing changes.
cci::cnf::callback_return_type Observer::config_callback(cci::cnf::cci_base_param& par, const cci::cnf::callback_type& cb_reason) {
  std::string str;
  switch (cb_reason) {
    case cci::cnf::pre_write:
      str = par.json_serialize();
      DEMO_DUMP(name(), "**** Object callback for parameter '" << par.get_name() << "' will change value");
      break;
    case cci::cnf::post_write:
      str = par.json_serialize();
      DEMO_DUMP(name(), "**** Object callback for parameter '" << par.get_name() << "' changed to value '"<<str<<"'");
      break;
    default:
      assert(false && "not awaited cb reason");
  }
  return cci::cnf::return_nothing;
}

const char* Observer::name() const {
  return mName.c_str();
}
