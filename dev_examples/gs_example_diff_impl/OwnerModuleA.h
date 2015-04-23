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


#ifndef __OWNERMODULEA_H__
#define __OWNERMODULEA_H__


#include <systemc>
#include "ex_globals.h"
#include "cci"


/// Module which owns some std cci parameters.
class OwnerModuleA
: public sc_core::sc_module
{
  
public:
  
  SC_HAS_PROCESS(OwnerModuleA);
	
  /// Constructor
  OwnerModuleA(sc_core::sc_module_name name)
  : sc_core::sc_module(name)
  , int_param ("int_param", 50 )
  , bool_param("bool_param", false)
  , string_param("string_param", "This is from std library A")
  { 
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests with parameters.
  void main_action();
  
  /// Example parameter.
  cci::cnf::cci_param<int>             int_param;
  /// Example parameter.
  cci::cnf::cci_param<bool>            bool_param;
  /// Example parameter.
  cci::cnf::cci_param<std::string>     string_param;
  
};


#endif
