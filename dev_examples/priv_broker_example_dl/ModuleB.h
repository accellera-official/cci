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
  , int_paramb ("int_paramb", 50, cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this)) )
  , uint_paramb("uint_paramb", 12000, cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this)))
  , uint_param2b("uint_param2b", 12, cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this)))
  , str_paramb ("str_paramb", "This is a test string.", cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this)))
  , bool_paramb("bool_paramb", cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this))) // no default value
  { 
    SC_THREAD(main_action);

    // This needs to be done during construction (NOT within the sc_thread)!
    m_broker_accessor = &cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));
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
  
protected:
  
  /// Points to the broker being responsible for this module; This is needed to be set during construction to get the correct private broker from stack
  /// Alternatively this module could derive from the cci_broker_manager even without having an own private broker - that would allow to use its get_broker() function
  cci::cnf::cci_cnf_broker_if* m_broker_accessor;

};


#endif
