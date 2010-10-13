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

#define GS_PARAM_CALLBACK_VERBOSE

// GreenConfig
#include "greencontrol/config.h" 

#include "OwningModule.h"
#include "TestModule.h"

// THIS IS ALL TODO - CHECK CCI examples for existing checks
/*
 GreenConfig callback tests
 
 - register
 - pre_read cb
 - post_read cb
 - reject_write
 - pre_write cb
 - post_write cb
 - destroy_param cb
 - legacy post_write_and_destroy cb
 - test all registered callbacks
 
 - unregister all registered callbacks
 - test all UNregistered callbacks
 
 
 - all tests for different gs_param types (as representant of types):
 - integer 
 - sc_event
 - sc_time
 - sc_bigint<64>
 - sc_attribute
 - gs_param_array 
 - with member callbacks switched on
 - with member callbacks switched off
*/

/// Testbench for the GreenConfig regresson test
int sc_main(int argc, char *argv[]) {
  
  /// GreenControl Core instance
  gs::ctr::GC_Core       core;
  gs::cnf::ConfigDatabase cnfdatabase("ConfigDatabase");
  gs::cnf::ConfigPlugin configPlugin(&cnfdatabase);

  OwningModule om("OM");
  TestModule tm("TM");
  
  sc_core::sc_start();
  
  std::cout << "END SIMULATION" << std::endl;
  std::cout << "Parameter list:" << std::endl;
  gs::ctr::show_vector<std::string>(gs::cnf::GCnf_Api::getApiInstance(NULL)->getParamList());
    
  return EXIT_SUCCESS; 
  
}
