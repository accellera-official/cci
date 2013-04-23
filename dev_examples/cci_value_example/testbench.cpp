// LICENSETEXT
//
//   Copyright (C) 2011 : GreenSocs Ltd
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
#include "cci"

#include "ParameterOwnerModule.h"


//
//  This example tests and demonstrates the usage of cci_value set and get
//  including wrong usage and errors.
//
//  The user data type test_datatype_without_parameter_implementation is used to demonstrate
//  how cci_calue does NOT work if the conversion functions are not implemented.
//  The macro ENABLE_CCI_VALUE_SUPPORT_FOR_TEST_DATATYPE can be defined to enable this support
//  (@see file gs_cci_symbols_c.cpp)
//


/// Testbench for the CCI development example testing cci_value
int sc_main(int argc, char *argv[]) {
  //sc_core::sc_report_handler::set_actions(sc_core::SC_ERROR, sc_core::SC_ABORT);
  //sc_core::sc_report_handler::set_actions(sc_core::SC_WARNING, sc_core::SC_ABORT);

  // Note: this prevents from catching the error!!
  //sc_core::sc_report_handler::set_actions("/OSCI/CCI/set_param_failed",  sc_core::SC_DISPLAY);
  //sc_core::sc_report_handler::set_actions("/OSCI/CCI/cci_value_failure", sc_core::SC_DISPLAY);

  sc_set_time_resolution( 1, sc_core::SC_FS ); // for testing sc_time to string fs

  ParameterOwnerModule   owner      ("Owner");
  
  std::cout << std::endl << "------ sc_start() ----------------" << std::endl << std::endl;
  sc_core::sc_start(); 
  std::cout << std::endl << "------ sc_start() returned -------" << std::endl << std::endl;
  
  return EXIT_SUCCESS; 
  
}
