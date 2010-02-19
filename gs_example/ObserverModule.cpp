// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//    Mark Burton, mark@greensocs.com
//
//
//   This program is free software.
// 
//   If you have no applicable agreement with GreenSocs Ltd, this software
//   is licensed to you, and you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   If you have a applicable agreement with GreenSocs Ltd, the terms of that
//   agreement prevail.
// 
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
//   02110-1301  USA 
// 
// ENDLICENSETEXT


#include "ObserverModule.h"
#include <systemc.h>


ObserverModule::ObserverModule(sc_core::sc_module_name name)
: sc_core::sc_module(name)
{ 
  // get the config API which is responsible for this module
  mApi = cci::get_cnf_api_instance(this);
  SC_THREAD(main_action);
}


ObserverModule::~ObserverModule() {
  // unregister all callbacks (this is optional, callbacks get unregistered if all references are deleted)
  std::vector< boost::shared_ptr<cci::callb_adapt_b> >::iterator iter;
  for (iter = mCallbacks.begin(); iter != mCallbacks.end(); iter++) {
    (*iter)->unregister_at_parameter();
  }
}


void ObserverModule::main_action() {
  DEMO_DUMP(name(), "register pre write callback for int_param to lock it");

  // get access to a foreign parameter using the module's config API
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
