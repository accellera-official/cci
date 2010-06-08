//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//   Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

#include "ObserverModule.h"

using namespace std;
using namespace sc_core;

void ObserverModule::main_action() {
  DEMO_TRACE(name(), "register new parameter callback");
  m_Api->REGISTER_NEW_PARAM_CALLBACK(ObserverModule, config_new_param_callback);
  
  cout << name() << ": register callbacks for int_param (twice!) and str_param" << endl;

  // Register Callback for parameter int_param in module other_ip (IP1)
  boost::shared_ptr<gs::cnf::ParamCallbAdapt_b> cbAdapt_int_param;
  cbAdapt_int_param = GC_REGISTER_PARAM_CALLBACK(m_Api->getPar("Owner.int_param"), ObserverModule, config_callback);

  // second callback for int_param
  GC_REGISTER_PARAM_CALLBACK(m_Api->getPar("Owner.int_param"), ObserverModule, config_callback);

  // Register Callback for parameter str_param in module other_ip (IP1)
  GC_REGISTER_PARAM_CALLBACK(m_Api->getPar("Owner.str_param"), ObserverModule, config_callback);

  cout << name() << ": register deprecated callback for uint_param" << endl;
  // Register deprecated callback at Api:
  m_Api->REGISTER_CALLBACK(ObserverModule, config_callback_deprecated, "Owner.uint_param");
  
  wait(6, SC_US);
  wait(1, SC_NS);

  cout << "----------------------------" << endl;
  DEMO_TRACE(name(), "Unregister callback for int_param");
  GC_UNREGISTER_CALLBACK(cbAdapt_int_param);

  cout << "  Now there must only be ONE callback for each int_param change!" << endl;
  cout << "----------------------------" << endl<<endl;

  wait(6, SC_US);

  cout << "----------------------------" << endl;
  DEMO_TRACE(name(), "Unregister all callbacks to this module in the param int_param");
  m_Api->getPar("Owner.int_param")->unregisterParamCallbacks(this);
  cout << "  Now there must not be callbacks for parameter int_param any more!" << endl;
  cout << "----------------------------" << endl<<endl;
}


// ///////////////////// ///////////////// ///////////////////////////////////// //
// ///////////////////// reacting methods: ///////////////////////////////////// //
// ///////////////////// ///////////////// ///////////////////////////////////// //


void ObserverModule::event_reactor_new_param() {
  DEMO_TRACE(name(), "Got new parameter update event! This should happen each time a new parameter is added during runtime!\n");
}
void ObserverModule::event_reactor_param_change() {
  DEMO_TRACE(name(), "Got parameter update event! This should happen when Owner.int_param changed!\n");
}

void ObserverModule::config_new_param_callback(const std::string parname, const std::string val) {
  DEMO_TRACE(name(), "New parameter callback method called:");
  cout << "  New parameter '" << parname << "'"<< endl;
  gs::gs_param_base *par = m_Api->getPar(parname);
  if (par == NULL) {
    cout << "  implicit" << endl;
  } else {
    cout << "  exlicit of type '" << par->getTypeString() << "'" << endl;
  }
}

// Callback function with default signature.
void ObserverModule::config_callback(gs::gs_param_base& par) {
  DEMO_TRACE(name(), "Callback method called:");
  cout << "  Parameter '" << par.getName() << "'"<< endl
       << "    of type '" << par.getTypeString() << "'" << endl;
  if (par.is_destructing()) {
    cout << "    is just being destructed!" << endl;
  } else {
    cout << "    changed to value '" << par.getString() << "'." << endl;
  }
  cout << endl;
}

void ObserverModule::config_callback_deprecated(const std::string parname, const std::string val) {
  DEMO_TRACE(name(), "Deprecated callback method called:");
  cout << "  Parameter '" << parname << "'"<< endl
       << "    changed to value '" << val << "'." << endl;
  cout << endl;
}
