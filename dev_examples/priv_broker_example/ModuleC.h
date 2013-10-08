// LICENSETEXT
//
//   Copyright (C) 2011 : GreenSocs Ltd
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


#ifndef __MODULEC_H__
#define __MODULEC_H__

#include <systemc>
#include "ex_globals.h"
#include "cci"

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "gs_cci_cnf_private_broker_accessor.h"


/// Module which owns some very private cci parameters.
class ModuleC
: public sc_core::sc_module
{
protected:
  /// for secure access by parameters
  cci::cnf::cci_cnf_broker_if* m_broker;
  
public:
  
  SC_HAS_PROCESS(ModuleC);
	
  /// Constructor
  ModuleC(sc_core::sc_module_name name, cci::cnf::cci_broker_manager priv_broker);
  
  /// Destructor
  ~ModuleC();
  
  /// Main action to make tests with parameters.
  void main_action();
  
  /// Example parameter.
  cci::cnf::cci_param<std::string>    priv_param;
  
};


#endif
