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
#include "cci_params.h"
#include "cci_api.h"


/// Module which owns some cci parameters.
class ModuleB
: public sc_core::sc_module
{
  
public:
  
  SC_HAS_PROCESS(ModuleB);
	
  /// Constructor
  ModuleB(sc_core::sc_module_name name)
  : sc_core::sc_module(name)
  , int_param ("int_param", 50, false, cci::cnf::get_cnf_broker_instance(this) )
  , uint_param("uint_param", 12000, false, cci::cnf::get_cnf_broker_instance(this))
  , uint_param2("uint_param2", 12, false, cci::cnf::get_cnf_broker_instance(this))
  , str_param ("str_param", "This is a test string.", false, cci::cnf::get_cnf_broker_instance(this))
  , bool_param("bool_param", "", false, cci::cnf::get_cnf_broker_instance(this)) // no default value
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
