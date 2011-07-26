// LICENSETEXT
//
//   Copyright (C) 2009-2011 : GreenSocs Ltd
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


#include "ModuleC.h"
#include <systemc.h>

void ModuleC::main_action() {

  // get the config broker which is responsible for this module
  // Note: Do NOT use cci_broker_manager::get_current_broker here, it won't return the private broker!
  cci::cnf::cci_cnf_broker_if* mBroker = &get_broker(); // use my base class broker manager which cares for the correct broker!

  wait(10, SC_SEC);
  
  // show a parameter list
  cout << endl << "**** Parameter list (in "<<name()<<"): " << endl;
  std::vector<std::string> vec = mBroker->get_param_list();
  std::vector<std::string>::iterator iter;
  std::stringstream ss_show;
  for (iter = vec.begin() ; iter < vec.end(); iter++) {
    ss_show << "   " << *iter << std::endl;
  }
  std::cout << ss_show.str() << std::endl<<std::endl;
  
  std::cout << "----------------------------" << std::endl;

}