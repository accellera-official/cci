// LICENSETEXT
//
//   Copyright (C) 2009-2010 : GreenSocs Ltd
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


#include "ObserverModule.h"
#include <systemc.h>


ObserverModule::ObserverModule(sc_core::sc_module_name name)
: sc_core::sc_module(name)
{ 
  // get the config API which is responsible for this module
  mApi = cci::cnf::get_cnf_broker_instance(cci::cnf::cci_originator(*this));
  SC_THREAD(main_action);
}


ObserverModule::~ObserverModule() {
  // unregister all callbacks (this is optional, callbacks get unregistered if all references are deleted)
  std::vector< cci::shared_ptr<cci::cnf::callb_adapt_b> >::iterator iter;
  for (iter = mCallbacks.begin(); iter != mCallbacks.end(); iter++) {
    (*iter)->unregister_at_parameter();
  }
}


void ObserverModule::main_action() {
  cout << "----------------------------" << endl;

  // ******** register for new parameter callbacks ***************
  DEMO_DUMP(name(), "register for new parameter callbacks");
  cci::shared_ptr<cci::cnf::callb_adapt_b> cb_new_pa;
  cb_new_pa = mApi->register_callback(cci::cnf::create_param, "*", this, 
                                   cci::bind(&ObserverModule::config_new_param_callback, this, _1, _2));
  mCallbacks.push_back(cb_new_pa);// This will not be deleted after end of main_action()

  // ******** register for parameter change callbacks ***************
  DEMO_DUMP(name(), "register for post_write callbacks");
  cci::cnf::cci_base_param* p = mApi->get_param("Owner.int_param");
  assert(p != NULL);
  cci::shared_ptr<cci::cnf::callb_adapt_b> cb1, cb3;
  cb1 = p->register_callback(cci::cnf::post_write, this, 
                             cci::bind(&ObserverModule::config_callback, this, _1, _2));
  cci::cnf::cci_base_param* p2 = mApi->get_param("Owner.uint_param");
  assert(p2 != NULL);
  cb3 = p2->register_callback(cci::cnf::post_write, this, 
                              cci::bind(&ObserverModule::config_callback, this, _1, _2));
  mCallbacks.push_back(cb3);// This will not be deleted after end of main_action()

  std::cout << std::endl;
}

/// Callback function with default signature showing changes.
cci::cnf::callback_return_type ObserverModule::config_callback(cci::cnf::cci_base_param& par, const cci::cnf::callback_type& cb_reason) {
  assert(cb_reason == cci::cnf::post_write);
  std::string str = par.json_serialize();
  DEMO_DUMP(name(), "Callback for parameter '" << par.get_name() << "' changed to value '"<<str<<"'");
  return cci::cnf::return_nothing;
}

/// Callback function with default signature announcing new parameters.
cci::cnf::callback_return_type ObserverModule::config_new_param_callback(cci::cnf::cci_base_param& par, const cci::cnf::callback_type& cb_reason) {
  assert(cb_reason == cci::cnf::create_param);
  std::string str = par.json_serialize();
  DEMO_DUMP(name(), "New parameter callback '" << par.get_name() << "', value '"<<str<<"'");
  return cci::cnf::return_nothing;
}
