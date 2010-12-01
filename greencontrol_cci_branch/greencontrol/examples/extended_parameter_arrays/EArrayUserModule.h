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

#ifndef __EArrayUserModule_H__
#define __EArrayUserModule_H__

#include <systemc>

#include "globals.h"

#include "greencontrol/core/gc_core.h" 
#include "greencontrol/config.h"


/// Module which owns some parameter arrays.
class EArrayUserModule
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(EArrayUserModule);
	
  GC_HAS_CALLBACKS();
  
  /// Constructor
  EArrayUserModule(sc_core::sc_module_name name)
    : sc_core::sc_module(name)
  { 
    m_Api = gs::cnf::GCnf_Api::getApiInstance(this);
    SC_THREAD(main_action);
  }
  
  ~EArrayUserModule() {
    GC_UNREGISTER_CALLBACKS();
  }
  
  /// Main action to make tests with parameters.
  void main_action();

  // Callback function with default signature.
  void config_callback(gs::gs_param_base& par);

protected:
  gs::cnf::cnf_api_if *m_Api;
  
};


#endif

