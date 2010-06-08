//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2008 : GreenSocs Ltd
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

#ifndef __SCATTRIBPARAM_H__
#define __SCATTRIBPARAM_H__

#include <systemc>

#include "greencontrol/config.h"


/// Test IP testing sc_attribute parameters
class Param_test_sc_attribute
: public sc_core::sc_module
{
  
public:
  
  GC_HAS_CALLBACKS();
  
  SC_HAS_PROCESS(Param_test_sc_attribute);
  
  /// Constructor
  Param_test_sc_attribute(sc_core::sc_module_name name)
  : sc_core::sc_module(name)
  , attrib_paramInt("attrib_paramInt", "attrib_paramInt", -100)
  , attrib_paramUInt("attrib_paramUInt", "attrib_paramUInt", 100)
  , attrib_paramDouble("attrib_paramDouble", "attrib_paramDouble", 10.1)
  , attrib_paramString("attrib_paramString", "attrib_paramString", "Init test string")
  { 
    SC_THREAD(main_action);
  }
  
  /// Destructor
  ~Param_test_sc_attribute() {
    GC_UNREGISTER_CALLBACKS();
  }
  
  /// Main action to make tests with parameters.
  void main_action();
  
  /// Example parameter.
  gs::gs_param<sc_core::sc_attribute<int> >           attrib_paramInt;
  gs::gs_param<sc_core::sc_attribute<unsigned int> >  attrib_paramUInt;
  gs::gs_param<sc_core::sc_attribute<double> >        attrib_paramDouble;
  gs::gs_param<sc_core::sc_attribute<std::string> >   attrib_paramString;
  
};


#endif


