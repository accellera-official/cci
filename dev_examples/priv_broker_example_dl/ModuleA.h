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
#include "cci.h"

#include "gs_cci_cnf_private_broker.h"

#include "ModuleB.h"

/// Module which owns some cci parameters.
class ModuleA
: public sc_core::sc_module
, public cci::cnf::cci_broker_manager
{
  
public:
  
  SC_HAS_PROCESS(ModuleA);
	
  /// Constructor
  ModuleA(sc_core::sc_module_name name)
  : sc_core::sc_module(name)
  , cci::cnf::cci_broker_manager(new cci::cnf::gs_cci_private_broker(this, cci::cnf::gs_cci_private_broker::vector_factory("int_param", END_OF_PUBLIC_PARAM_LIST)))
  //, cci::cnf::cci_broker_manager(new cci::cnf::gs_cci_private_broker(this, std::vector<std::string>(1,"int_param") ))
  //, cci::cnf::cci_broker_manager(new cci::cnf::gs_cci_private_broker(this, {std::string("int_param"), std::string("uint_param")} ))
  , int_param ("int_param", 10, false, get_broker() )
  , uint_param("uint_param", 12345, false, get_broker() )
  , uint_param2("uint_param2", 89, false, get_broker() )
  , str_param ("str_param", "This is a test string in module A.", false, get_broker())
  , bool_param("bool_param", false, get_broker()) // no default value
  , m_modB("m_modB")
  { 
    SC_THREAD(main_action);
  }
  
  ~ModuleA() {
    // Don't delete while params existing!
    /*cci::cnf::cci_cnf_broker_if* pb = get_broker();
    register_private_broker(NULL);
    delete pb;*/
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
  
protected:
  ModuleB m_modB;
};


#endif
