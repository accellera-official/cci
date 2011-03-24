// LICENSETEXT
//
//   Copyright (C) 2009-2011 : GreenSocs Ltd
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
#include "cci.h"
#include "gs_cci_cnf_private_broker_accessor.h"


/// Module which owns some cci parameters.
class ModuleC
: public sc_core::sc_module
, public cci::cnf::cci_broker_manager
{
  
public:
  
  SC_HAS_PROCESS(ModuleC);
	
  /// Constructor
  ModuleC(sc_core::sc_module_name name)
  : sc_core::sc_module(name)
  , cci::cnf::cci_broker_manager(new cci::cnf::gs_cci_private_broker_accessor(*this, boost::assign::list_of("int_paramc")("uint_param2c")("bool_paramc"), cci::cnf::cci_originator(*this) ))
  , int_paramc ("int_paramc", 50, get_broker() )
  , uint_paramc("uint_paramc", 12000, get_broker())
  , uint_param2c("uint_param2c", 12, get_broker())
  , str_paramc ("str_paramc", "This is a test string.", get_broker())
  , bool_paramc("bool_paramc", cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this))) // no default value, demonstrates alternative if no broker manager was there
  { 
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests with parameters.
  void main_action();
  
  /// Example parameter.
  cci::cnf::cci_param<int>             int_paramc;
  /// Example parameter.
  cci::cnf::cci_param<unsigned int>    uint_paramc;
  /// Example parameter.
  cci::cnf::cci_param<unsigned int>    uint_param2c;
  /// Example parameter.
  cci::cnf::cci_param<std::string>     str_paramc;
  /// Example parameter.
  cci::cnf::cci_param<bool>            bool_paramc;
  
};


#endif
