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


#include "ParameterOwnerModule.h"
#include <systemc.h>

void ParameterOwnerModule::main_action() {

  wait(1, SC_NS);

  cout << endl;
  DEMO_DUMP(name(), "Executing main_action");

  // get the config API which is responsible for this module
  cci::cci_broker_if* mApi = &cci::cci_broker_manager::get_current_broker(cci::cci_originator(*this));
  assert(mApi != NULL && "get_cnf_broker_instance returned is NULL");
  
  // create a local parameter
  DEMO_DUMP(name(), "- Creating uchar_param parameter");
  cci::cci_param<unsigned char> uchar_param("uchar_param", 0);

  // update parameter values, activating registered callbacks
  cout << endl;
  DEMO_DUMP(name(), "- Changing uint_param value to 1");
  uint_param = 1;

  cout << endl;
  DEMO_DUMP(name(), "- Changing uint_param2 value to 2");
  uint_param2 = 2;

  // show a parameter list
  std::vector<std::string> vec = mApi->get_param_list();
  std::vector<std::string>::iterator iter;
  std::stringstream ss_show;
  ss_show << "- List of all parameters:" << endl;
  for (iter = vec.begin() ; iter < vec.end(); iter++) {
    ss_show << "\t" << *iter << endl;
  }
  cout << endl;
  DEMO_DUMP(name(), ss_show.str());
}
