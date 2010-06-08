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

// for CommandLine_Tool: timed or not timed models
//  value to be put into wait(...) statement, e.g.:
//    SC_ZERO_TIME or
//    sc_time(1, SC_NC)
#define TEST_TOOL_POLL_TIME sc_time(1, SC_MS)

#include "../demo_globals.h"

// GreenConfig
#include "greencontrol/config.h" 
// GreenConfig special APIs
#include "greencontrol/config_api_config_file_parser.h"  // API Tool to read configuration file(s)
#include "greencontrol/config_api_command_line_parser.h" // API Tool to process config command line parameters
#include "greencontrol/config_api_command_line_tool.h" // Tool which provides a command line

#include "testip_neg.h" // test TestIP_neg
#include "testtool_neg_wrong_api.h" // TestTool which uses the Wrong_Transactions_User_Api

/// Negative regression tests testbench
/**
 * Negative regression test.
 * See Wiki page http://www.greensocs.com/GreenConfig/FirstImplementation/RegressionTests
 */
int sc_main(int argc, char *argv[]) {
  

  // change order of these three instantiations to test instantiation order
  // <change order>
  // GreenControl Core
  gs::ctr::GC_Core       core;

  // GreenConfig Plugin
  gs::cnf::ConfigDatabase cnfdatabase("ConfigDatabase");
  gs::cnf::ConfigPlugin configPlugin(&cnfdatabase);

  // IP
  TestIP_neg   ip_neg("IP_neg");
  // </change order>

  // Test if registration of one service fails when already registered by a plugin
  //gs::cnf::ConfigPlugin configPlugin2("ConfigPlugin2", new gs::cnf::ConfigDatabase("ConfigDatabase"));
  
  //  TestTool_neg tool("TestTool_neg");

  gs::cnf::ConfigFile_Tool configTool("ConfigFileTool");
  //configTool.config("not_existing.cfg"); // parse not existing config file: error
  //configTool.config("configtest_wrong.cfg"); // config file with failure leading to error
  configTool.config("configtest_wrong2.cfg"); // config file with failure leading to warning
  configTool.parseCommandLine(argc, argv);
  
  gs::cnf::CommandLineConfigParser configParser;
  configParser.parse(argc, argv);

  //gs::cnf::CommandLine_Tool commandLineTool("CommandLineTool");

  TestTool_Wrong_Api wrongTestTool("WrongTestTool");

  sc_core::sc_start();
  
  return EXIT_SUCCESS; 
  
}
