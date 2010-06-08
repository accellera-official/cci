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
  mApi = cci::cnf::get_cnf_broker_instance(this);
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
  DEMO_DUMP(name(), "register pre write callback for int_param to lock it");

  // get access to a foreign parameter using the module's config API
  cci::cnf::cci_base_param* p = mApi->get_param("Owner.int_param");
  assert(p != NULL);

  // ******** register for parameter change callback ***************
  cci::shared_ptr<cci::cnf::callb_adapt_b> cb1, cb3;
  cb1 = p->register_callback(cci::cnf::post_write, this, 
                             cci::bind(&ObserverModule::config_callback, this, _1, _2));
  cci::cnf::cci_base_param* p2 = mApi->get_param("Owner.uint_param");
  assert(p2 != NULL);
  cb3 = p2->register_callback(cci::cnf::post_write, this, 
                              cci::bind(&ObserverModule::config_callback, this, _1, _2));
  mCallbacks.push_back(cb3);// This will not be deleted after end of main_action()

  // ******* Testing parameter lock with callback returns *************
  // Register Callback for parameter int_param in module other_ip (IP1)
  cci::shared_ptr<cci::cnf::callb_adapt_b> cb2;
  cb2 = p->register_callback(cci::cnf::pre_write, this, 
                             cci::bind(&ObserverModule::config_callback_reject_changes, this, _1, _2));
  std::string str = p->json_serialize();
  cout << "int_param has value = " << str << endl;
  cout << "int_param set value = 99" << endl;
  p->json_deserialize("99");
  str = p->json_serialize();
  cout << "int_param has value = " << str << endl;
  //p->unregister_all_callbacks(this); // this would unregister all calbacks to this module
  p->unregister_param_callback(cb2); // unregister a callback
  
  // ********* show param list **************
  cout << "param list:" << endl;
  std::vector<std::string> vec = mApi->get_param_list();
  for (std::vector<std::string>::iterator iter = vec.begin(); iter != vec.end(); iter++)
    cout << *iter << " ";
  cout << endl;
  
  cout << "----------------------------" << endl;
  std::cout << std::endl;

}


/// Callback function with default signature showing changes.
cci::cnf::callback_return_type ObserverModule::config_callback(cci::cnf::cci_base_param& par, const cci::cnf::callback_type& cb_reason) {
  assert(cb_reason == cci::cnf::post_write);
  std::string str = par.json_serialize();
  DEMO_DUMP(name(), "Callback for parameter '" << par.get_name() << "' changed to value '"<<str<<"'");
  return cci::cnf::return_nothing;
}

/// Callback function with default signature rejecting all changes.
cci::cnf::callback_return_type ObserverModule::config_callback_reject_changes(cci::cnf::cci_base_param& par, const cci::cnf::callback_type& cb_reason) {
  assert(cb_reason == cci::cnf::pre_write);
  DEMO_DUMP(name(), "Callback method called (which rejects changes):");
  cout << "  Parameter '" << par.get_name() << "' type " << cb_reason << endl;
  cout << "  REJECT VALUE CHANGE!!" << endl;
  cout << endl;
  return cci::cnf::return_value_change_rejected;
}
