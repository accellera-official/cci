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


#ifndef __MODULEA_H__
#define __MODULEA_H__


#include <systemc>
#include "ex_globals.h"
#include "cci_configuration"

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "gs_cci_cnf_private_broker_accessor.h"

#include "ModuleB.h"

/// Module which owns some cci parameters.
class ModuleA
: public sc_core::sc_module
{
protected:
  /// for secure access by parameters
  cci::cci_broker_if* m_broker;

public:
  
  SC_HAS_PROCESS(ModuleA);
	
  /// Constructor
  ModuleA(sc_core::sc_module_name name, cci::cci_broker_manager); 
  // Important: never use a private_broker_manager anywhere else than as constructor argument!!!
  
  /// Destructor
  ~ModuleA();
  
  /// Main action to make tests with parameters.
  void main_action();
  
  /// Example parameter.
  cci::cci_param<int>             int_param;
  /// Example parameter.
  cci::cci_param<unsigned int>    uint_param;
  /// Example parameter.
  cci::cci_param<unsigned int>    uint_param2;
  /// Example parameter.
  cci::cci_param<std::string>     str_param;
  /// Example parameter.
  cci::cci_param<bool>            bool_param;
  
protected:
  ModuleB m_modB;
};


#endif
