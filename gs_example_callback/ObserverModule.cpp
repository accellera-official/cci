/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2009 by all Contributors.
  All Rights reserved.

  Developed by GreenSocs : http://www.greensocs.com/
   Christian Schroeder, schroeder@eis.cs.tu-bs.de
   Mark Burton, mark@greensocs.com

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/


#include "ObserverModule.h"
#include <systemc.h>


ObserverModule::ObserverModule(sc_core::sc_module_name name)
: sc_core::sc_module(name)
{ 
  mApi = cci::get_cnf_api_instance(this);
  SC_THREAD(main_action);
}


ObserverModule::~ObserverModule() {
  // unregister all callbacks
  std::vector< boost::shared_ptr<cci::callb_adapt_b> >::iterator iter;
  for (iter = mCallbacks.begin(); iter != mCallbacks.end(); iter++) {
    (*iter)->unregister_at_parameter();
  }
}


void ObserverModule::main_action() {
  DEMO_DUMP(name(), "register pre write callback for int_param to lock it");

  cci::cci_base_param* p = mApi->get_param("Owner.int_param");
  assert(p != NULL);

  // ******** register for parameter change callback ***************
  boost::shared_ptr<cci::callb_adapt_b> cb1, cb3;
  cb1 = p->register_callback(cci::post_write, this, 
                             boost::bind(&ObserverModule::config_callback, this, _1, _2));
  cci::cci_base_param* p2 = mApi->get_param("Owner.uint_param");
  assert(p2 != NULL);
  cb3 = p2->register_callback(cci::post_write, this, 
                              boost::bind(&ObserverModule::config_callback, this, _1, _2));
  mCallbacks.push_back(cb3);// This will not be deleted after end of main_action()

  // ******* Testing parameter lock with callback returns *************
  // Register Callback for parameter int_param in module other_ip (IP1)
  boost::shared_ptr<cci::callb_adapt_b> cb2;
  cb2 = p->register_callback(cci::pre_write, this, 
                             boost::bind(&ObserverModule::config_callback_reject_changes, this, _1, _2));
  std::string str = p->json_serialize();
  cout << "int_param has value = " << str << endl;
  cout << "int_param set value = 99" << endl;
  p->json_deserialize("99");
  str = p->json_serialize();
  cout << "int_param has value = " << str << endl;
  //p->unregister_all_callbacks(this);
  p->unregister_param_callback(cb2);
  
  // ********* show param list **************
  cout << "param list:" << endl;
  std::vector<std::string> vec = mApi->get_param_list();
  std::vector<std::string>::iterator iter;
  for (iter = vec.begin(); iter != vec.end(); iter++)
    cout << *iter << " ";
  cout << endl;
  
  cout << "----------------------------" << endl;
  
}


/// Callback function with default signature showing changes.
cci::callback_return_type ObserverModule::config_callback(cci::cci_base_param& par, const cci::callback_type& cb_reason) {
  assert(cb_reason == cci::post_write);
  std::string str = par.json_serialize();
  DEMO_DUMP(name(), "Callback for parameter '" << par.get_name() << "' changed to value '"<<str<<"'");
  return cci::return_nothing;
}

/// Callback function with default signature rejecting all changes.
cci::callback_return_type ObserverModule::config_callback_reject_changes(cci::cci_base_param& par, const cci::callback_type& cb_reason) {
  assert(cb_reason == cci::pre_write);
  DEMO_DUMP(name(), "Callback method called (which rejects changes):");
  cout << "  Parameter '" << par.get_name() << "' type " << cb_reason << endl;
  cout << "  REJECT VALUE CHANGE!!" << endl;
  cout << endl;
  return cci::return_value_change_rejected;
}
