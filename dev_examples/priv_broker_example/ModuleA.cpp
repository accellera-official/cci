// LICENSETEXT
//
//   Copyright (C) 2010 : GreenSocs Ltd
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


#include "ModuleA.h"
#include <systemc.h>


ModuleA::ModuleA(sc_core::sc_module_name name)
: sc_core::sc_module(name)
, cci::cnf::cci_broker_manager(new cci::cnf::gs_cci_private_broker(this, cci::cnf::gs_cci_private_broker::vector_factory("int_param", END_OF_PUBLIC_PARAM_LIST)))
, int_param ("int_param", 50, false, get_broker() )
, uint_param("uint_param", 12000, false, get_broker() )
, uint_param2("uint_param2", 12, false, get_broker() )
, str_param ("str_param", "This is a test string.", false, get_broker())
, bool_param("bool_param", false, get_broker()) // no default value
, m_modB("ModuleB")
{ 
  SC_THREAD(main_action);
}

ModuleA::~ModuleA() {
  // Don't delete while params existing!
  /*cci::cnf::cci_cnf_broker_if* pb = get_broker();
   register_private_broker(NULL);
   delete pb;*/
}

void ModuleA::main_action() {

  // get the config broker which is responsible for this module
  cci::cnf::cci_cnf_broker_if* mBroker = cci::cnf::get_cnf_broker_instance(this);
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
