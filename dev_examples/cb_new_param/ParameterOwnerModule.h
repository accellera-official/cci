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


#ifndef __PARAMETEROWNERMODULE_H__
#define __PARAMETEROWNERMODULE_H__


#include <systemc>
#include "ex_globals.h"
#include "cci"


/// Module which owns some cci parameters.
class ParameterOwnerModule
: public sc_core::sc_module
{
  
public:
  
  SC_HAS_PROCESS(ParameterOwnerModule);
	
  /// Constructor
  ParameterOwnerModule(sc_core::sc_module_name name)
  : sc_core::sc_module(name)
  , int_param ("int_param", 50 )
  , uint_param("uint_param", 12000)
  , uint_param2("uint_param2", 12)
  , str_param ("str_param", "This is a test string.")
  , bool_param("bool_param") // no default value
  { 
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests with parameters.
  void main_action();
  
  /// Example parameter.
  cci::cnf::cci_param<int>             int_param;
  /// Example parameter.
  cci::cnf::cci_param<unsigned int>    uint_param;
  /// Example parameter.
  cci::cnf::cci_param<unsigned int>    uint_param2;
  /// Example parameter.
  cci::cnf::cci_param<std::string>     str_param;
  /// Example parameter.
  cci::cnf::cci_param<bool>            bool_param;
  
};


#endif
