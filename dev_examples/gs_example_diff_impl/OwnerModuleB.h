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


#ifndef __OWNERMODULEB_H__
#define __OWNERMODULEB_H__


#include <systemc>
#include "ex_globals.h"
#include "cci"
#include "param_impl/test_datatype.h"

/// Module which owns some std cci parameters (provided by lib A) and one additional one of lib implementation B.
class OwnerModuleB
: public sc_core::sc_module
{
  
public:
  
  SC_HAS_PROCESS(OwnerModuleB);
	
  /// Constructor
  OwnerModuleB(sc_core::sc_module_name name)
  : sc_core::sc_module(name)
  , int_param ("int_param", 150 )
  , bool_param("bool_param")
  , test_datatype_param("test_datatype_param")
  { 
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests with parameters.
  void main_action();
  
  /// Example parameter.
  cci::cnf::cci_param<int>             int_param;
  /// Example parameter.
  cci::cnf::cci_param<bool>            bool_param;
  /// Example parameter. which exists in the parameter implementation B
  cci::cnf::cci_param<test_datatype> test_datatype_param;
  
};


#endif
