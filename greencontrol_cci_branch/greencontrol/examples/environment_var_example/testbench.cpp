//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Marcus Bartholomeu <bartho@greensocs.com>
//     GreenSocs
//     http://www.greensocs.com
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// GreenConfig
#include "greencontrol/config.h" 
// GreenConfig special API
#include "greencontrol/config_api_config_file_parser.h"  // API Tool to read configuration file(s)

// This example
#include "test_ip.h"


/// Testbench for the config reader with environment variable example
/**
 * The testbench instantiates the GreenCotrol Core and the needed user modules.
 */
int sc_main(int argc, char *argv[]) {
  
  // Configuration with GreenConfig config files
  gs::cnf::ConfigFile_Tool configTool("ConfigFileTool");
  configTool.config("config.cfg");   // reads in a config file

  Test_IP tip("TestIP");

  sc_core::sc_start();
  
  return EXIT_SUCCESS; 
  
}
