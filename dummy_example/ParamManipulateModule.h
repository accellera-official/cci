//   OSCI CCI WG
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// 
// ENDLICENSETEXT


#ifndef __ParamManipulateModule_H__
#define __ParamManipulateModule_H__


#include <systemc>
#include "ex_globals.h"
#include "cci.h"
#include "../dummy_implementation/gs_cci.h"


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
  /// Pointer the the module's configuration API
  cci::cci_cnf_api* mApi;

};


#endif
