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
  // Register Callback for parameter int_param in module other_ip (IP1)
  cci::cci_base_param* p = mApi->get_param("Owner.int_param");
  assert(p != NULL);
  p->register_callback(cci::pre_write, this, 
                       boost::bind(&ObserverModule::config_callback, this, _1, _2));
  
  // show param list
  cout << "param list:" << endl;
  std::vector<std::string> vec = mApi->get_param_list();
  std::vector<std::string>::iterator iter;
  for (iter = vec.begin(); iter != vec.end(); iter++)
    cout << *iter << " ";
  cout << endl;
  
  cout << "----------------------------" << endl;
}


// Callback function with default signature.
cci::callback_return_type ObserverModule::config_callback(cci::cci_base_param& par, const cci::callback_type& cb_reason) {
  DEMO_DUMP(name(), "Callback method called:");
  assert(cb_reason == cci::pre_write);
  cout << "  Parameter '" << par.get_name() << "'"<< endl;
  cout << "    type " << cb_reason << endl;
  cout << "  REJECT VALUE CHANGE!!" << endl;
  cout << endl;
  return cci::return_value_change_rejected;
}
