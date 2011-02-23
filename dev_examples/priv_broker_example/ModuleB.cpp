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


#include "ModuleB.h"
#include <systemc.h>

ModuleB::ModuleB(sc_core::sc_module_name name)
: sc_core::sc_module(name)
, int_param ("int_param", 50, false, cci::cnf::get_cnf_broker_instance(cci::cnf::cci_originator(*this)) )
, uint_param("uint_param", 12000, false, cci::cnf::get_cnf_broker_instance(cci::cnf::cci_originator(*this)))
, uint_param2("uint_param2", 12, false, cci::cnf::get_cnf_broker_instance(cci::cnf::cci_originator(*this)))
, str_param ("str_param", "This is a test string.", false, cci::cnf::get_cnf_broker_instance(cci::cnf::cci_originator(*this)))
, bool_param("bool_param", false, cci::cnf::get_cnf_broker_instance(cci::cnf::cci_originator(*this))) // no default value
{ 
  SC_THREAD(main_action);
}

void ModuleB::main_action() {

  // get the config broker which is responsible for this module
  cci::cnf::cci_cnf_broker_if* mBroker = cci::cnf::get_cnf_broker_instance(cci::cnf::cci_originator(*this));
  assert(mBroker != NULL && "get_cnf_broker_instance returned is NULL");
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
