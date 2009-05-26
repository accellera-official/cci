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
  DEMO_DUMP(name(), "register new parameter callback");
  boost::shared_ptr<cci::callb_adapt_b> cb1 = mApi->register_callback(cci::create_param, "*", this, &ObserverModule::config_new_param_callback);

  DEMO_DUMP(name(), "register callback for int_param");
  // Register Callback for parameter int_param in module other_ip (IP1)
  cci::cci_param_base* p = mApi->get_param("Owner.int_param");
  if (p != NULL) {
    boost::shared_ptr<cci::callb_adapt_b> cbAdapt_int_param;
    cbAdapt_int_param = p->register_callback(cci::post_write, this, &ObserverModule::config_callback);
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


void ObserverModule::config_new_param_callback(cci::cci_param_base& par) {
  DEMO_DUMP(name(), "New parameter callback method called:");
  cout << "  New parameter '" << par.get_name() << "'"<< endl;
}


// Callback function with default signature.
void ObserverModule::config_callback(cci::cci_param_base& par) {
  DEMO_DUMP(name(), "Callback method called:");
  cout << "  Parameter '" << par.get_name() << "'"<< endl;
  std::string str; par.get_string(str);
  cout << "    changed to value '" << str << "'." << endl;
  cout << endl;
}
