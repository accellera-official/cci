//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2008 : GreenSocs Ltd
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


/*
 * This example uses the message streamer with 
 * file configurations defined in files
 */

// Enable ReportMessage debug outputs
#define MSG_VERBOSE
//#define GS_PARAM_ARRAY_VERBOSE
//#define GS_PARAM_VERBOSE
//#define GCNF_VERBOSE

// Standard GreenControl with config and analysis
#include "greencontrol/all.h"

// Extra output plugin (example)
#include "greencontrol/gav/plugin/Stdout_OutputPlugin.h" // optional, already included by all.h

// message streamer 
#define GS_MSG_DEBUG_EN 1
#include "greencontrol/reportmsg/gs_debug_stream.h"
#include "greencontrol/reportmsg/gs_system_stream.h"


class ModuleA
: sc_core::sc_module
{
protected:
  gs::report::gs_debug_stream dbgL2;
  gs::report::gs_debug_stream dbgL4;
  gs::report::gs_debug_stream dbgL9;
  gs::report::gs_system_stream sysINFO;

public:

  SC_HAS_PROCESS(ModuleA);
  ModuleA(sc_core::sc_module_name name) 
  : sc_core::sc_module(name)
  , dbgL2("dbgL2", gs::report::dbg_msg_L2)
  , dbgL4("dbgL4", gs::report::dbg_msg_L4)
  , dbgL9("dbgL9", gs::report::dbg_msg_L9)
  , sysINFO("sysINFO", gs::report::sys_msg_INFO)
  {

    SC_THREAD(main_action);
    
  }
  
  void main_action() {
    wait(10, sc_core::SC_NS);

    dbgL2 << "Hallo L2 after 10ns" << GS_END_MSG;
    
    dbgL9 << "Hallo L9 after 10ns" << std::endl << "new line" << GS_END_MSG;
    
    wait(10, sc_core::SC_NS);

    dbgL4 << "Hallo L4 after 20ns" << std::endl << "new line" << GS_END_MSG;
    
    sysINFO << "This is an sc_info!" << GS_END_MSG;

    wait(1, sc_core::SC_NS);

    dbgL9 << "Hallo L9 after 21ns" << GS_END_MSG;

    wait(9, sc_core::SC_NS);
    
    dbgL4 << "Hallo L4 after 30ns" << GS_END_MSG;
  }
  
};


/// Testbench
int sc_main(int argc, char *argv[]) {
  
  GS_INIT_STANDARD_GREENCONTROL;
  
  // You must first read in fixed configuration
  // read in config file and apply to database (old style)
  std::cout << std::endl << "** read in config file:" << std::endl;
  gs::cnf::ConfigFile_Tool configTool("ConfigFileTool");
  configTool.config("message_file_config.cfg");

  std::cout << std::endl << "** create global message streamer:" << std::endl;
  gs::report::MessageStreamer::get_global_message_streamer();

  std::cout << std::endl << "** read in config files and apply to MessageStreamer:" << std::endl;
  gs::cnf::ConfigSet cnf_set;
  // After fixed configuration add the variable one
  // read in config file and apply
  cnf_set = configTool.create_config_set_from_file("message_file_config_set1.cfg");
  gs::report::MessageStreamer::apply_config_set(cnf_set);
  // read in config file and apply
  cnf_set = configTool.create_config_set_from_file("message_file_config_set2.cfg");
  gs::report::MessageStreamer::apply_config_set(cnf_set);

  std::cout << std::endl << "** create module with streams:" << std::endl;
  ModuleA moda("ModuleA");
  
  std::cout << std::endl << "** start simulation **" << std::endl;
  sc_core::sc_start();
  std::cout << std::endl << "** stopped simulation **" << std::endl;
    
  std::cout << "** Parameter list:" << std::endl;
  gs::ctr::show_vector<std::string>(gs::cnf::GCnf_Api::getApiInstance(NULL)->getParamList());

  return EXIT_SUCCESS; 
  
}
