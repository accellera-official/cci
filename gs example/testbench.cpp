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


#include <systemc>
//#include "greencontrol/all.h"
#include "ParameterOwnerModule.h"
//#include "ObserverModule.h"
//#include "ParamManipulateModule.h"
#include <cci.h>
#include "greencontrol/all.h"

/// Testbench for the example GreenConfig
int sc_main(int argc, char *argv[]) {
  
  GS_INIT_STANDARD_GREENCONTROL;
  
  ParameterOwnerModule  owner      ("Owner");
  //ObserverModule        observer   ("Observer");
  //ParamManipulateModule manipulator("Manipulator");

  sc_core::sc_start();
  
  return EXIT_SUCCESS; 
  
}
