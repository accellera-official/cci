// LICENSETEXT
//
//   Copyright (C) 2009-2010 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


#include "OwnerModuleA.h"
#include <systemc.h>

void OwnerModuleA::main_action() {

  std::cout << "----------------------------" << std::endl;

  // get the broker which is responsible for this module
  cci::cnf::cci_cnf_broker* mBroker = cci::cnf::get_cnf_broker_instance(this);
  assert(mBroker != NULL && "get_cnf_broker_instance returned is NULL");

  
  std::cout << "----------------------------" << std::endl;

}
