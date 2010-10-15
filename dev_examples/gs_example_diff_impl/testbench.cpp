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


#include <systemc>
#include <cci.h>
#include "cci_api.h"
#include "cci_params.h"

#include "OwnerModuleA.h"
//#include "OwnerModuleB.h"

/// Example testbench for the CCI example demonstrating that it is possible 
/// to use two different parameter implementations within one simulation
/**
 * OwnerModuleA is compiled an linked against the proof-of-concept parameter implementation,
 * OwnerModuleB is compiled an linked against a modified version of the proof-of-concept parameter implementation.
 */
int sc_main(int argc, char *argv[]) {
  //sc_core::sc_report_handler::set_actions(sc_core::SC_ERROR, sc_core::SC_ABORT);
  //sc_core::sc_report_handler::set_actions(sc_core::SC_WARNING, sc_core::SC_ABORT);

  // Note: this prevents from catching the error!!
  //sc_core::sc_report_handler::set_actions("/OSCI/CCI/set_param_failed",  sc_core::SC_DISPLAY);
  sc_core::sc_report_handler::set_actions("/OSCI/CCI/cci_value_failure", sc_core::SC_DISPLAY);


  OwnerModuleA     owner      ("OwnerA");
  //OwnerModuleB     owner      ("OwnerB");

  std::cout << std::endl << "------ sc_start() ----------------" << std::endl << std::endl;
  sc_core::sc_start(); 
  std::cout << std::endl << "------ sc_start() returned -------" << std::endl << std::endl;
  
  return EXIT_SUCCESS; 
  
}
