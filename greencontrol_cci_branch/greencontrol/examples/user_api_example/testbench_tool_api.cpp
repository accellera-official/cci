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

// SystemC library
#include <systemc>


// GreenConfig
#include "greencontrol/config.h" 

#include "testip4.h"
#include "testtool2.h"
#include "tool_api/tool_api.h" // include user defined API

//
// OUTDATED example
//

/// Testbench for the example GreenConfig
/**
 * The testbench instantiates the GreenCotrol Core and the needed user modules.
 */
int sc_main(int argc, char *argv[]) {
  
  /// GreenControl Core instance
  gs::ctr::GC_Core       core;

  // GreenConfig Plugin
  gs::cnf::ConfigDatabase cnfdatabase("ConfigDatabase");
  gs::cnf::ConfigPlugin configPlugin(&cnfdatabase);

  TestIP4      ip("IP4");
  TestTool2    tool("TestTool2");

  sc_core::sc_start();
  
  return EXIT_SUCCESS; 
  
}
