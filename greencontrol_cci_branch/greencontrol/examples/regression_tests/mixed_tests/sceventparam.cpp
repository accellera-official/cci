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


#include "sceventparam.h"

#define IPsc_eventTest(a) std::cout << "IP sc_event test: " << a <<std::endl
#define SPACE std::cout << std::endl

void Param_test_sc_event::main_action() {
  wait(10, sc_core::SC_SEC);
  
  IPsc_eventTest("Default value: ");
  IPsc_eventTest(" event_param " << event_param.getString());
  SPACE;

  // TODO more tests
  
  SPACE;
  SPACE;
};
