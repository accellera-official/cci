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

  std::cout << "----------------------------" << std::endl;

  // get the config API which is responsible for this module
  cci::cnf::cci_cnf_broker_if* mApi = cci::cnf::get_cnf_broker_instance(this);
  assert(mApi != NULL && "get_cnf_broker_instance returned is NULL");
  
  // create and access a local parameter
  cout << name() << ": create parameter " << endl;
  cci::cnf::cci_param<unsigned char> uchar_param("uchar_param");
  cout << "  created " << uchar_param.get_name() << endl;
  cout << endl;

  // show a parameter list
  cout << endl << "**** Parameter list: " << endl;
  std::vector<std::string> vec = mApi->get_param_list();
  std::vector<std::string>::iterator iter;
  std::stringstream ss_show;
  for (iter = vec.begin() ; iter < vec.end(); iter++) {
    if (iter != vec.begin()) ss_show << ", ";
    ss_show << *iter;
  }
  std::cout << "   " << ss_show.str() << std::endl<<std::endl;

}
