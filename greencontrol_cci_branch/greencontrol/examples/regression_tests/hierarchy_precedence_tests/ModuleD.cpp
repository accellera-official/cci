//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2010 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

#include "ModuleD.h" 

ModuleD::ModuleD(sc_core::sc_module_name name) 
: sc_core::sc_module(name) 
, my_param1("my_param1", "Default value set by owner ModuleD")
, my_param2("my_param2", "Default value set by owner ModuleD")
, my_param3("my_param3", "Default value set by owner ModuleD")
{
  m_cnf_api = gs::cnf::GCnf_Api::getApiInstance(this);
  SC_THREAD(main_action);
}

ModuleD::~ModuleD() {
  /// GreenConfig callbacks unregistering
  GC_UNREGISTER_CALLBACKS();
}

void ModuleD::main_action() {
}
