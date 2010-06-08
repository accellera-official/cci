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

#ifndef __TESTIP2_H__
#define __TESTIP2_H__

#include <systemc>

#include "../demo_globals.h"
#include "greencontrol/gcnf/apis/scmlApi/scml_property.h"

// This is a SystemC module which demonstrates that the CoWare scml-API
// may be used with an underlying GreenConfig library

#ifdef ENABLE_SCML_NAMESPACE
using gs::cnf::scml_property;
#endif


/// Test IP which uses the SCML API.
/**
 * This is a SystemC module which demonstrates that the CoWare Scml-API
 * may be used with an underlying GreenConfig library.
 */
class TestIP2
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(TestIP2);
	
  /// Constructor
  TestIP2(sc_core::sc_module_name name)
    : sc_core::sc_module(name),
      scmlIntParam("scmlIntParam", 1000),
      scmlUIntParam("scmlUIntParam"),
      scmlBoolParam("scmlBoolParam"),
      scmlDoubleParam("scmlDoubleParam"),
      scmlStringParam("scmlStringParam", "strValue2")

  { 
    SC_THREAD(main_action);
    //scml_property<int> *tmp_prop = new scml_property<int>(100);
  }
  
  /// Destructor
  ~TestIP2() {
  }

  /// Main action to make tests with parameters.
  void main_action();


  /// Example scml int parameter.
  scml_property<int>          scmlIntParam;
  /// Example scml unsigned int parameter.
  scml_property<unsigned int> scmlUIntParam;
  /// Example scml bool parameter.
  scml_property<bool>         scmlBoolParam;
  /// Example scml double parameter.
  scml_property<double>       scmlDoubleParam;
  /// Example scml string parameter.
  scml_property<std::string>  scmlStringParam;

  
};


#endif

