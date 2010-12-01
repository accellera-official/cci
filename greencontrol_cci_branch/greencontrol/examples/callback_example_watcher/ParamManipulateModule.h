//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//   Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// doxygen comments

#ifndef __ParamManipulateModule_H__
#define __ParamManipulateModule_H__

#include <systemc>

#include "callb_globals.h"

#include "greencontrol/config.h"


/// Module which changes parameter values of the Module ParameterOwnerModule
class ParamManipulateModule
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(ParamManipulateModule);
	
  /// Constructor
  ParamManipulateModule(sc_core::sc_module_name name)
    : sc_core::sc_module(name)
  { 
    mGcnfApi = gs::cnf::GCnf_Api::getApiInstance(this);
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests with parameters.
  void main_action();

protected:

  gs::cnf::cnf_api_if *mGcnfApi;

};


#endif
