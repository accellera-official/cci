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
#include "greencontrol/config_api_lua_file_parser.h" // API Tool to read lua configuration file(s)

// This example
#include "test_ip.h"


/// Testbench for the lua config reader example
/**
 * The testbench instantiates the GreenCotrol Core and the needed user modules.
 */
int sc_main(int argc, char *argv[]) {
  
  /// GreenControl Core instance
  gs::ctr::GC_Core       core;

  gs::cnf::ConfigDatabase cnfdatabase("ConfigDatabase");
  gs::cnf::ConfigPlugin configPlugin(&cnfdatabase);
 
  Test_IP tip("TestIP");

  gs::cnf::LuaFile_Tool luareader("luareader");
  luareader.config("config.lua");

  sc_core::sc_start();
  
  return EXIT_SUCCESS; 
  
}
