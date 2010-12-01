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

/*
   GreenConfig Private Parameters tests
 */

#include "globals.h"

//#include "ShowSCObjects.h"

// for CommandLine_Tool: timed model
#define TEST_TOOL_POLL_TIME sc_time(1, SC_MS)
// enables the command line
//#define ACTIVATE_COMMAND_THREAD

// GreenConfig
#include "greencontrol/config.h" 
// GreenConfig special APIs
#include "greencontrol/config_api_config_file_parser.h"  // API Tool to read configuration file(s)
#include "greencontrol/config_api_command_line_tool.h" // Command Line Tool

#include "ModuleTop.h"
#include "ModuleA.h"
#include "ModulePub.h"

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

  // Command line tool which provides a command line to access parameters
  gs::cnf::CommandLine_Tool commandLineTool("CommandLineTool");

  ModuleTop mod("ModuleTop");
  ModuleA mod2("ModuleTopA");
  ModulePub mod3("ModulePub");

  gs::cnf::cnf_api_if *mApi = gs::cnf::GCnf_Api::getApiInstance(NULL);
  
  //ShowSCObjects::showSCObjects();

  std::cout << "---- Start simulation ----" << std::endl;
  sc_core::sc_start();
  std::cout << "---- End simulation ----" << std::endl;

  std::cout << std::endl << "from toplevel: getParams(ModuleTop.ModuleA.*)" << std::endl;
  std::vector<gs::gs_param_base*> params_vec = mApi->getParams("ModuleTop.ModuleA.*");
  for (std::vector<gs::gs_param_base*>::iterator iter = params_vec.begin(); iter != params_vec.end(); iter++)
    std::cout << " "<< (*iter)->getName() << std::endl;
  std::cout << "from toplevel: getParams(ModuleTop.ModuleA)" << std::endl;
  params_vec = mApi->getParams("ModuleTop.ModuleA");
  for (std::vector<gs::gs_param_base*>::iterator iter = params_vec.begin(); iter != params_vec.end(); iter++)
    std::cout << " "<< (*iter)->getName() << std::endl;
  
  std::vector<std::string> lst = mApi->getParamList();
  std::cout << std::endl << " *** Param List of <top> ***:" <<std::endl; 
  gs::ctr::show_vector(lst);
  std::cout << std::endl;
  
  return EXIT_SUCCESS; 
  
}
