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
  mApi = cci::cnf::get_cnf_api_instance(this);
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

  cout << "----------------------------" << endl<< endl;

  // show a parameter list
  cout << endl << "**** Parameter list (in "<<name()<<"): " << endl;
  std::vector<std::string> vec = mApi->get_param_list();
  std::vector<std::string>::iterator iter;
  std::stringstream ss_show;
  for (iter = vec.begin() ; iter < vec.end(); iter++) {
    ss_show << "   " << *iter << std::endl;
  }
  std::cout << ss_show.str() << std::endl<<std::endl;
  
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
