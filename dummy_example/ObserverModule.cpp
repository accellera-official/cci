
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
  DEMO_DUMP(name(), "register new parameter callback");
  boost::shared_ptr<cci::callb_adapt_b> cb1 = 
  mApi->register_callback(cci::create_param, "*", this, 
                          boost::bind(&ObserverModule::config_new_param_callback, this, _1, _2));

  DEMO_DUMP(name(), "register callback for int_param");
  // Register Callback for parameter int_param in module other_ip (IP1)
  cci::cci_base_param* p = mApi->get_param("Owner.int_param");
  if (p != NULL) {
    boost::shared_ptr<cci::callb_adapt_b> cbAdapt_int_param;
    cbAdapt_int_param = p->register_callback(cci::post_write, this, 
                                             boost::bind(&ObserverModule::config_callback, this, _1, _2));
    mCallbacks.push_back(cbAdapt_int_param);
  }
  
  // check if callback is there
  if (mApi->has_callbacks("Owner.int_param"))
    cout << "callback successfully registered" << endl;
  else
    SC_REPORT_WARNING(name(), "ERROR: callback NOT registered!");
    cout << endl;
  
  // first check of value:
  cout << "json string value of Owner.int_param=" << mApi->get_json_string("Owner.int_param") << endl << endl;
  
  // show param list
  cout << "param list:" << endl;
  std::vector<std::string> vec = mApi->get_param_list();
  std::vector<std::string>::iterator iter;
  for (iter = vec.begin(); iter != vec.end(); iter++)
    cout << *iter << " ";
  cout << endl;
  
  cout << "----------------------------" << endl;
}


void ObserverModule::config_new_param_callback(cci::cci_base_param& par, const cci::callback_type& cb_reason) {
  DEMO_DUMP(name(), "New parameter callback method called:");
  cout << "  New parameter '" << par.get_name() << "'"<< endl;
}


// Callback function with default signature.
void ObserverModule::config_callback(cci::cci_base_param& par, const cci::callback_type& cb_reason) {
  DEMO_DUMP(name(), "Callback method called:");
  cout << "  Parameter '" << par.get_name() << "'"<< endl;
  std::string str; par.get_string(str);
  cout << "    changed to value '" << str << "'." << endl;
  cout << "    type " << cb_reason << endl;
  cout << endl;
}
