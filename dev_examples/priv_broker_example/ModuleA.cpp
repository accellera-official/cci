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


// within this contructor the private broker stack is valid containing the priv_broker broker as top
ModuleA::ModuleA(sc_core::sc_module_name name, cci::cnf::cci_broker_manager priv_broker)
: sc_core::sc_module(name)
//, cci::cnf::cci_broker_manager(new cci::cnf::gs_cci_private_broker_accessor(*this, boost::assign::list_of("int_param"), cci::cnf::cci_originator(*this)))
, m_broker(priv_broker)
, int_param ("int_param", 50, *m_broker )
, uint_param("uint_param", 12000, *m_broker )
, uint_param2("uint_param2", 12, *m_broker )
, str_param ("str_param", "This is a test string.", *m_broker)
, bool_param("bool_param", *m_broker) // no default value
, m_modB("ModuleB")
{ 
  SC_THREAD(main_action);
}

ModuleA::~ModuleA() {
}

void ModuleA::main_action() {

  // Note: Do NOT use cci_broker_manager::get_current_broker here, it won't return the private broker!
  
  
  wait(10, SC_SEC);
  
  // show a parameter list
  cout << endl << "**** Parameter list (visible in "<<name()<<"): " << endl;
  std::vector<std::string> vec = m_broker->get_param_list();
  std::vector<std::string>::iterator iter;
  std::stringstream ss_show;
  for (iter = vec.begin() ; iter < vec.end(); iter++) {
    ss_show << "   " << *iter << std::endl;
  }
  std::cout << ss_show.str() << std::endl<<std::endl;

  std::cout << "----------------------------" << std::endl;

}

