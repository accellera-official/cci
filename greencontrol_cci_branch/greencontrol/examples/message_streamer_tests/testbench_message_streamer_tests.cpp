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
 * This example tests the message streamer 
 */

// Enable ReportMessage debug outputs
#define MSG_VERBOSE
//#define GCNF_VERBOSE
//#define GS_PARAM_ARRAY_VERBOSE


// GreenConfig
#include "greencontrol/config.h" 
// GreenConfig special APIs
#include "greencontrol/config_api_config_file_parser.h"

// GreenAV
#include "greencontrol/gav/plugin/gav_plugin.h"
#include "greencontrol/gav/plugin/Stdout_OutputPlugin.h"


// message streamer 
#define GS_MSG_DEBUG_EN 1
#include "greencontrol/reportmsg/gs_debug_stream.h"
#include "greencontrol/reportmsg/gs_system_stream.h"

// Example

//#include "ShowSCObjects.h"



class ModuleA
: sc_core::sc_module
{
protected:
  gs::report::gs_debug_stream dbgL2;
  gs::report::gs_debug_stream dbgL4;
  gs::report::gs_debug_stream dbgL9;
  gs::report::gs_system_stream sysINFO;
  gs::report::gs_system_stream sysWARN;
  gs::report::gs_system_stream sysERROR;
  gs::report::gs_system_stream sysFATAL;

public:

  SC_HAS_PROCESS(ModuleA);
  ModuleA(sc_core::sc_module_name name) 
  : sc_core::sc_module(name)
  , dbgL2("dbgL2", gs::report::dbg_msg_L2)
  , dbgL4("dbgL4", gs::report::dbg_msg_L4)
  , dbgL9("dbgL9", gs::report::dbg_msg_L9)
  , sysINFO("sysINFO", gs::report::sys_msg_INFO)
  , sysWARN("sysWARN", gs::report::sys_msg_WARNING)
  , sysERROR("sysERROR", gs::report::sys_msg_ERROR)
  , sysFATAL("sysFATAL", gs::report::sys_msg_FATAL)
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
    sysWARN << "This is an sc_warning!" << GS_END_MSG;
    sysERROR << "This is an sc_error!" << GS_END_MSG;

    wait(1, sc_core::SC_NS);

    dbgL9 << "Hallo L9 after 21ns" << GS_END_MSG;

    wait(9, sc_core::SC_NS);
    
    dbgL4 << "Hallo L4 after 30ns" << GS_END_MSG;
    
    dbgL9 << (std::hex) << 20 << std::endl << GS_END_MSG;

    //sysFATAL << "This is an sc_fatal!" << GS_END_MSG;
  }
  
};


/// Testbench
int sc_main(int argc, char *argv[]) {
  
  sc_core::sc_report_handler::set_actions(sc_core::SC_INFO, sc_core::SC_DISPLAY);
  sc_core::sc_report_handler::set_actions(sc_core::SC_WARNING, sc_core::SC_DISPLAY);
  sc_core::sc_report_handler::set_actions(sc_core::SC_ERROR, sc_core::SC_DISPLAY);
  //sc_core::sc_report_handler::set_actions(sc_core::SC_WARNING, sc_core::SC_ABORT);

  // GreenControl Core instance
  gs::ctr::GC_Core       core;

  // GreenConfig Plugin
  gs::cnf::ConfigDatabase cnfdatabase("ConfigDatabase");
  gs::cnf::ConfigPlugin configPlugin(&cnfdatabase);

  // GreenAV Plugin
  gs::av::GAV_Plugin analysisPlugin(gs::av::STDOUT_OUT);
  
  gs::report::MessageStreamer::get_global_message_streamer();

  {
    gs::report::msg_configuration cnf;
    cnf.msgconfig_name = "report_message_file1.txt"; 
    cnf.msgconfig_starttime_en = true;
    cnf.msgconfig_starttime = sc_core::sc_time(13, sc_core::SC_NS);
    cnf.msgconfig_endtime = sc_core::sc_time(23, sc_core::SC_NS);
    cnf.msgconfig_dbglvl = gs::report::dbg_msg_L9;
    cnf.msgconfig_printtime = true;
    cnf.msgconfig_printname = true;
    cnf.msgconfig_printfile = true;
    cnf.msgconfig_module_id.push_back("ModuleA"); // ModuleA
    cnf.msgconfig_module_id.push_back(""); // top-level
    gs::report::MessageStreamer::apply_configuration(cnf);

    // reuse cnf object
    cnf.msgconfig_name = "stdout"; 
    cnf.msgconfig_starttime_en = false;
    cnf.msgconfig_dbglvl = gs::report::dbg_msg_L6;
    cnf.msgconfig_module_id.clear();
    gs::report::MessageStreamer::apply_configuration(cnf);
  }
  
  {
    gs::report::msg_configuration cnf;
    cnf.msgconfig_name = "report_message_file_only_ModuleA2_and_top.txt"; 
    cnf.msgconfig_dbglvl = gs::report::dbg_msg_L9;
    cnf.msgconfig_printtime = true;
    cnf.msgconfig_printname = true;
    cnf.msgconfig_printfile = true;
    cnf.msgconfig_module_id.push_back("ModuleA2");
    cnf.msgconfig_module_id.push_back("");
    gs::report::MessageStreamer::apply_configuration(cnf);
  }
  
  {
    // system message output with new cnf object
    gs::report::msg_configuration cnf;
    cnf.msgconfig_name = "SC_REPORT_INFO"; 
    cnf.msgconfig_info_en = true;
    gs::report::MessageStreamer::apply_configuration(cnf);
  }

  {
    // system message output with new cnf object
    gs::report::msg_configuration cnf;
    cnf.msgconfig_name = "SC_REPORT_WARNING"; 
    cnf.msgconfig_warn_en = true;
    gs::report::MessageStreamer::apply_configuration(cnf);
  }

  {
    // system message output with new cnf object
    gs::report::msg_configuration cnf;
    cnf.msgconfig_name = "SC_REPORT_ERROR"; 
    cnf.msgconfig_error_en = true;
    gs::report::MessageStreamer::apply_configuration(cnf);
  }

  {
    // system message output with new cnf object
    gs::report::msg_configuration cnf;
    cnf.msgconfig_name = "SC_REPORT_FATAL"; 
    cnf.msgconfig_fatal_en = true;
    gs::report::MessageStreamer::apply_configuration(cnf);
  }
  
  {
    gs::report::msg_configuration cnf;
    cnf.msgconfig_name = "sc_report_message_file.txt"; 
    cnf.msgconfig_info_en = true;
    cnf.msgconfig_warn_en = true;
    cnf.msgconfig_error_en = true;
    cnf.msgconfig_fatal_en = true;
    cnf.msgconfig_printfile = true;
    cnf.msgconfig_printlevel = true;
    gs::report::MessageStreamer::apply_configuration(cnf);
  }
  
  // read in config file and apply
  gs::cnf::ConfigFile_Tool configTool("ConfigFileTool");
  gs::cnf::ConfigSet cnf_set = configTool.create_config_set_from_file("message_file_config.cfg");
  gs::report::MessageStreamer::apply_config_set(cnf_set);
  
  // Stream 1
  gs::report::gs_debug_stream b("my_dbg_stream1", gs::report::dbg_msg_L2);
  b << "STREAM CONTENT" << std::endl
    << " another line with stream content"
    << GS_END_MSG;
  
  // Stream 2
  gs::report::gs_debug_stream dbst("my_dbg_stream2", gs::report::dbg_msg_L5);
  dbst << "Hallo"
    << GS_END_MSG
    << "This is a new message"
    << GS_END_MSG;
  
  ModuleA moda("ModuleA");
  ModuleA moda2("ModuleA2");
  
  std::cout << "-------------------- START SIMULATION -----------------------" << std::endl;
  sc_core::sc_start();
  std::cout << "-------------------- SIMULATION ENDED -----------------------" << std::endl;
    
  std::cout << "Parameter list:" << std::endl;
  // Show full database
  std::vector<std::string> vec = gs::cnf::GCnf_Api::getApiInstance(NULL)->getParamList();
  std::cout << "  vector: size="<< vec.size()<< std::endl;
  std::vector<std::string>::iterator iter;
  for (unsigned int i = 0; i<vec.size(); i++)
    std::cout << "   " <<vec[i] << " = " << gs::cnf::GCnf_Api::getApiInstance(NULL)->getValue(vec[i])<< std::endl;
  std::cout << std::endl;
  
  //ShowSCObjects::showSCObjects();

  return EXIT_SUCCESS; 
  
}
