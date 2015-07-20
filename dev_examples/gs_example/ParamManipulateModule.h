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


#ifndef __PARAMMANIPULATEMODULE_H__
#define __PARAMMANIPULATEMODULE_H__


#include <systemc>
#include "ex_globals.h"
#include "cci_configuration"


/// Module which changes parameter values of the Module ParameterOwnerModule
class ParamManipulateModule
: public sc_core::sc_module
{
public:

  SC_HAS_PROCESS(ParamManipulateModule);
  ParamManipulateModule(sc_core::sc_module_name name);
  
  /// Main action to make tests with parameters.
  void main_action();

protected:
  /// Pointer the the module's configuration broker
  cci::cci_broker_if* mBroker;

};


#endif
