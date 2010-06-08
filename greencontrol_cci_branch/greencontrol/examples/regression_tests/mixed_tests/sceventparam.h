//   GreenControl framework
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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// ////////////////////////////////////////////////////////////////////// //
// ///////// Regression tests for gs_param<sc_attribute<T> > //////////// //
// ////////////////////////////////////////////////////////////////////// //

#ifndef __SCEVENTPARAM_H__
#define __SCEVENTPARAM_H__

#include <systemc>

#include "greencontrol/config.h"


/// Test IP testing sc_attribute parameters
class Param_test_sc_event
: public sc_core::sc_module
{
  
public:
  
  GC_HAS_CALLBACKS();
  
  SC_HAS_PROCESS(Param_test_sc_event);
  
  /// Constructor
  Param_test_sc_event(sc_core::sc_module_name name)
  : sc_core::sc_module(name)
  , event_param("event_param")
  { 
    SC_THREAD(main_action);
  }
  
  /// Destructor
  ~Param_test_sc_event() {
    GC_UNREGISTER_CALLBACKS();
  }
  
  /// Main action to make tests with parameters.
  void main_action();
  
  /// Example parameter.
  gs::gs_param<sc_core::sc_event >           event_param;
  
};


#endif


