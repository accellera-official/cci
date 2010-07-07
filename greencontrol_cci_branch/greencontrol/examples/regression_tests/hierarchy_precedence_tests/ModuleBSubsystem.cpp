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

#include "ModuleBSubsystem.h" 

ModuleBSubsystem::ModuleBSubsystem(sc_core::sc_module_name name) 
: sc_core::sc_module(name) 
, m_submodule(NULL)
{
  m_cnf_api = gs::cnf::GCnf_Api::getApiInstance(this);
  SC_THREAD(main_action);

  std::string val = "Init value set by "; val += sc_core::sc_module::name();
  m_cnf_api->setInitValue ("ModuleATop1.ModuleBSubsystem1.ModuleC1.ModuleD1.my_param2", val);
  m_cnf_api->lockInitValue("ModuleATop1.ModuleBSubsystem1.ModuleC1.ModuleD1.my_param2");
  m_submodule = new ModuleC("ModuleC1");
}

ModuleBSubsystem::~ModuleBSubsystem() {
  /// GreenConfig callbacks unregistering
  GC_UNREGISTER_CALLBACKS();
}

void ModuleBSubsystem::main_action() {
}
