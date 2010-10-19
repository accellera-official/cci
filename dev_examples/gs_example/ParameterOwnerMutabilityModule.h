// LICENSETEXT
//
//   Copyright (C) 2010 : GreenSocs Ltd
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


#ifndef __PARAMETEROWNERMUTABILITYMODULE_H__
#define __PARAMETEROWNERMUTABILITYMODULE_H__


#include <systemc>
#include "ex_globals.h"
#include "cci.h"


/// Module which owns some cci parameters of different mutybility.
class ParameterOwnerMutabilityModule
: public sc_core::sc_module
{
  
public:
  
  SC_HAS_PROCESS(ParameterOwnerMutabilityModule);
	
  /// Constructor
  ParameterOwnerMutabilityModule(sc_core::sc_module_name name)
  : sc_core::sc_module(name)
  , mutable_int_param   ("mutable_int_param", 50 )
  , immutable_int_param ("immutable_int_param", 51 )
  , elab_time_int_param ("elab_time_int_param", 52 )
  { 
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests with parameters.
  void main_action();
  
  /// Example mutable parameter.
  cci::cnf::cci_param<int, cci::cnf::mutable_parameter>    mutable_int_param;
  /// Example immutable parameter.
  cci::cnf::cci_param<int, cci::cnf::immutable_parameter>  immutable_int_param;
  /// Example elaboration_time_parameter parameter.
  cci::cnf::cci_param<int, cci::cnf::elaboration_time_parameter>  elab_time_int_param;
  
};


#endif
