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
 * file configurations defined directly in sc_main (not in files)
 */

// Enable ReportMessage debug outputs
#define MSG_VERBOSE

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
  
  {
    gs::report::msg_configuration cnf;
    cnf.msgconfig_name = "report_message_file1.txt"; 
    cnf.msgconfig_starttime_en = true;
    cnf.msgconfig_starttime = sc_core::sc_time(13, sc_core::SC_NS);
    cnf.msgconfig_endtime = sc_core::sc_time(23, sc_core::SC_NS);
    cnf.msgconfig_dbglvl = gs::report::dbg_msg_L9;
    cnf.msgconfig_printfile = true;
    cnf.msgconfig_module_id.push_back("ModuleA"); // ModuleA
    cnf.msgconfig_module_id.push_back(""); // top-level
    gs::report::MessageStreamer::apply_configuration(cnf);
  }
  
  {
    gs::report::msg_configuration cnf;
    cnf.msgconfig_name = "stdout"; 
    cnf.msgconfig_dbglvl = gs::report::dbg_msg_L6;
    gs::report::MessageStreamer::apply_configuration(cnf);
  }
  
  {
    // system message output with new cnf object
    gs::report::msg_configuration cnf;
    cnf.msgconfig_name = "SC_REPORT_INFO"; 
    cnf.msgconfig_info_en = true;
    gs::report::MessageStreamer::apply_configuration(cnf);
  }
  
  
  ModuleA moda("ModuleA");
  ModuleA modz("ModuleZ");
  
  sc_core::sc_start();
    
  return EXIT_SUCCESS; 
  
}
