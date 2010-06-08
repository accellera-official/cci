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

/*
   Simple Parameter Arrays tests:
 */

//#include "ShowSCObjects.h"

#include "globals.h"

// GreenConfig
#include "greencontrol/config.h" 
// GreenConfig special API
#include "greencontrol/config_api_config_file_parser.h"  // API Tool to read configuration file(s)

#include "ArrayOwnerModule.h"
#include "ArrayUserModule.h"

/// Testbench for the example GreenConfig
int sc_main(int argc, char *argv[]) {
  
  /// GreenControl Core instance
  gs::ctr::GC_Core       core;

  // GreenConfig Plugin
  gs::cnf::ConfigDatabase cnfdatabase("ConfigDatabase");
  gs::cnf::ConfigPlugin configPlugin(&cnfdatabase);

  // Configuration with GreenConfig config files
  gs::cnf::ConfigFile_Tool configTool("ConfigFileTool");
  configTool.parseCommandLine(argc, argv); // parses the command line for --configfile
  configTool.config("array_test1.cfg");   // reads in a config file

  ArrayOwnerModule  owner      ("Owner");
  ArrayUserModule   user       ("User");

  //ShowSCObjects::showSCObjects();

  sc_core::sc_report_handler::set_actions(sc_core::SC_ERROR, sc_core::SC_ABORT);
  std::cout << "------ Simulation start ---------" << std::endl;
  sc_core::sc_start();
  std::cout << "------ Simulation stopped ---------" << std::endl;
  
  return EXIT_SUCCESS; 
  
}
