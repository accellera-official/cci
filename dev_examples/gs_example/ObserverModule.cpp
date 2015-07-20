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
  // get the config broker which is responsible for this module
  //mBroker = &cci::cci_broker_manager::get_current_broker(cci::cci_originator(*this));
  // new:
  mBroker = &cci::cci_broker_manager::get_current_broker(cci::cci_originator(*this));
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
  cout << "----------------------------" << endl;

  const cci::cci_originator* orig = mBroker->get_latest_write_originator("Owner.int_param");
  assert (orig && "Originator must not be NULL here!");
  if (mBroker->get_param("Owner.int_param")) {
    DEMO_DUMP(name(), "Write originator for EXPLICIT param Owner.int_param (from broker): " << orig->name());
  } else {
    DEMO_DUMP(name(), "Write originator for IMPLICIT param Owner.int_param (from broker): " << orig->name());
  }
  
  DEMO_DUMP(name(), "register for new parameter callbacks");
  cci::shared_ptr<cci::callb_adapt> cb_new_pa;
  cb_new_pa = mBroker->register_str_callback(cci::create_param, "*", this, 
                                             cci::bind(&ObserverModule::config_new_param_callback, this, _1, _2));
  mCallbacks.push_back(cb_new_pa);// This will not be deleted after end of main_action()
  
  DEMO_DUMP(name(), "register pre write callback for int_param to check value settings and reject them");

  // get access to a foreign parameter using the module's config API
  cci::cci_base_param* p = mBroker->get_param("Owner.int_param");
  assert(p != NULL);

  // ******** register for parameter change callback ***************
  cci::shared_ptr<cci::callb_adapt> cb1a, cb1b, cb3a, cb3b;
  cb1a = p->register_callback(cci::pre_write, this, 
                              cci::bind(&ObserverModule::config_callback, this, _1, _2));
  cb1b = p->register_callback(cci::post_write, this, 
                              cci::bind(&ObserverModule::config_callback, this, _1, _2));
  cci::cci_base_param* p2 = mBroker->get_param("Owner.uint_param");
  assert(p2 != NULL);
  cb3a = p2->register_callback(cci::pre_write, this, 
                               cci::bind(&ObserverModule::config_callback, this, _1, _2));
  cb3b = p2->register_callback(cci::post_write, this, 
                               cci::bind(&ObserverModule::config_callback, this, _1, _2));
  mCallbacks.push_back(cb3a);// This will not be deleted after end of main_action()
  mCallbacks.push_back(cb3b);// This will not be deleted after end of main_action()

  // ******* Testing parameter lock with callback returns *************
  // Register Callback for parameter int_param in module other_ip (IP1)
  cci::shared_ptr<cci::callb_adapt> cb2;
  cb2 = p->register_callback(cci::reject_write, this,
                             cci::bind(&ObserverModule::config_callback_reject_changes, this, _1, _2));
  std::string str = p->json_serialize();
  cout << "int_param has value = " << str << endl;
  cout << "int_param set value = 99 (shall fail)" << endl;
  try {
    p->json_deserialize("99"); // this shall fail because rejected!
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
  str = p->json_serialize();
  cout << "int_param has value = " << str << endl;
  //p->unregister_all_callbacks(this); // this would unregister all calbacks to this module
  p->unregister_callback(cb2); // unregister a callback
  
  // ********* show param list **************
  cout << "param list:" << endl;
  std::vector<std::string> vec = mBroker->get_param_list();
  for (std::vector<std::string>::iterator iter = vec.begin(); iter != vec.end(); iter++)
    cout << *iter << " ";
  cout << endl;
  
  std::cout << std::endl;
  
  wait(11, SC_NS);
  // **** check for latest originator using the alternative API on parameter objects 
  // (instead of global originator function within callbacks)
  
  DEMO_DUMP(name(), "latest write originator of parameter '"<< p->get_name() << "': " << p->get_latest_write_originator()->name());
  p->json_deserialize("666666");
  DEMO_DUMP(name(), "latest write originator of parameter '"<< p->get_name() << "': " << p->get_latest_write_originator()->name());
  
  orig = mBroker->get_latest_write_originator(p->get_name());
  assert (orig && "Originator must not be NULL here!");
  if (mBroker->get_param(p->get_name())) {
    DEMO_DUMP(this->name(), "Write originator for EXPLICIT param "<<p->get_name()<<" (from broker): " << orig->name());
  } else {
    DEMO_DUMP(this->name(), "Write originator for IMPLICIT param "<<p->get_name()<<" (from broker): " << orig->name());
  }
  
}


/// Callback function with default signature showing changes.
cci::callback_return_type ObserverModule::config_callback(cci::cci_base_param& par, const cci::callback_type& cb_reason) {
  std::string str;
  assert(cci::cci_originator::get_global_originator() && "Originator shall not be null!");
  switch (cb_reason) {
  case cci::pre_write:
    str = par.json_serialize();
    DEMO_DUMP(name(), "Callback for parameter '" << par.get_name() << "' will change value, originator: "<< cci::cci_originator::get_global_originator()->name());
    break;
  case cci::post_write:
    str = par.json_serialize();
    DEMO_DUMP(name(), "Callback for parameter '" << par.get_name() << "' changed to value '"<<str<<"', originator: "<< cci::cci_originator::get_global_originator()->name());
    break;
  default:
    assert(false && "not awaited cb reason");
  }
  return cci::return_nothing;
}

/// Callback function with default signature announcing new parameters.
cci::callback_return_type ObserverModule::config_new_param_callback(const std::string& par_name, const cci::callback_type& cb_reason) {
  assert(cb_reason == cci::create_param);
  cci::cci_base_param* p = mBroker->get_param(par_name);
  assert(p && "This new param should already be available!");
  std::string str = p->json_serialize();
  DEMO_DUMP(name(), "New parameter callback '" << par_name << "', value '"<<str<<"', originator: "<< cci::cci_originator::get_global_originator()->name());
  return cci::return_nothing;
}

/// Callback function with default signature rejecting all changes.
cci::callback_return_type ObserverModule::config_callback_reject_changes(cci::cci_base_param& par, const cci::callback_type& cb_reason) {
  assert(cb_reason == cci::reject_write);
  DEMO_DUMP(name(), "Callback method called (which rejects changes):");
  cout << "  Parameter '" << par.get_name() << "' type " << cb_reason << ", originator: "<< cci::cci_originator::get_global_originator()->name() << endl;
  cout << "  REJECT VALUE CHANGE!!" << endl;
  cout << endl;
  return cci::return_value_change_rejected;
}
