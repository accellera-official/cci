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


#ifndef __MODULEB_H__
#define __MODULEB_H__


#include <systemc>
#include "ex_globals.h"
#include "cci.h"


/// Module which owns some cci parameters.
class ModuleB
: public sc_core::sc_module
{
  
public:
  
  SC_HAS_PROCESS(ModuleB);
	
  /// Constructor
  ModuleB(sc_core::sc_module_name name)
  : sc_core::sc_module(name)
  , int_paramb ("int_paramb", 50, false, cci::cnf::get_cnf_broker_instance(this) )
  , uint_paramb("uint_paramb", 12000, false, cci::cnf::get_cnf_broker_instance(this))
  , uint_param2b("uint_param2b", 12, false, cci::cnf::get_cnf_broker_instance(this))
  , str_paramb ("str_paramb", "This is a test string.", false, cci::cnf::get_cnf_broker_instance(this))
  , bool_paramb("bool_paramb", false, cci::cnf::get_cnf_broker_instance(this)) // no default value
  { 
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests with parameters.
  void main_action();
  
  /// Example parameter.
  cci::cnf::cci_param<int>             int_paramb;
  /// Example parameter.
  cci::cnf::cci_param<unsigned int>    uint_paramb;
  /// Example parameter.
  cci::cnf::cci_param<unsigned int>    uint_param2b;
  /// Example parameter.
  cci::cnf::cci_param<std::string>     str_paramb;
  /// Example parameter.
  cci::cnf::cci_param<bool>            bool_paramb;
  
};


#endif
