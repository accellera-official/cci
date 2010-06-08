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

#ifndef __TESTIP_H__
#define __TESTIP_H__

#include <systemc>

#include "greencontrol/gcnf/apis/scmlApi/scml_property.h"
#include "testip2.h"


/// Test IP which uses the SCML API.
/**
 * This is a SystemC module which demonstrates that the CoWare Scml-API
 * may be used with an underlying GreenConfig library.
 */
class TestIP
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(TestIP);
	
  /// Cosntructor
  TestIP(sc_core::sc_module_name name)
    : sc_core::sc_module(name),
      scmlIntParam("scmlIntParam", 1000),
      scmlStringParam("scmlStringParam"),
      sub_ip2("SUB_IP_2")
  { 
    SC_THREAD(main_action);
    //scml_property<int> *tmp_prop = new scml_property<int>(100);
  }
  
  /// Destructor
  ~TestIP() {
  }

  /// Main action to make tests with parameters.
  void main_action();

  /// Example scml int parameter.
  scml_property<int> scmlIntParam;
  /// Example scml string parameter.
  scml_property<std::string> scmlStringParam;

  /// Sub-module
  TestIP2 sub_ip2;

};


#endif

