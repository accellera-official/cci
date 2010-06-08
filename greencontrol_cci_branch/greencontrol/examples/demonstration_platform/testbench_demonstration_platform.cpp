//   GreenConfig framework
//
// LICENSETEXT
//
//   Copyright (C) 2007-2009 : GreenSocs Ltd
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

// remember NOT to link scv before systemc! (seg faults)

// enable/disable command line tool
//#define ACTIVATE_COMMAND_THREAD
// for CommandLine_Tool: timed or not timed models
//  value to be put into wait(...) statement, e.g.:
//    SC_ZERO_TIME or
//    sc_time(1, SC_NC)
#define TEST_TOOL_POLL_TIME sc_time(1, SC_MS)

#include "demo_globals.h"

// GreenBus
#include "greenbus/transport/GP/GP.h" // Info: this needs not to be included before GreenControl/GreenConfig!

// GreenConfig
#include "greencontrol/config.h"
// Special GreenConfig APIs:
#include "greencontrol/config_api_config_file_parser.h"  // API Tool to read configuration file(s)
#include "greencontrol/config_api_command_line_parser.h" // API Tool to process config command line parameters
#include "greencontrol/config_api_command_line_tool.h" // Command Line Tool

// SimpleBus
#include "greenbus/transport/genericRouter.h"
#include "greenbus/protocol/SimpleBus/simpleBusProtocol.h"
#include "greenbus/scheduler/fixedPriorityScheduler.h"

// Demonstration Platform
#include "Mem.h"
#include "TG.h"
#include "Cache.h"
#include "PCIeDevice.h"

/// Testbench for the Demonstration Platform for GreenConfig
/**
 * If there is still a deprecated warning this is the addParam of the scml
 * API which is not yet ported to use directly gs_param objects!
 *
 * The testbench instantiates the GreenCotrol Core and the needed user modules.
 * The user modules communicate over GreenBus.
 *
 * Usage e.g.:
 * \code
 * ./testbench_demonstration_platform --configfile demoplatform.cfg
 * \endcode
 */
int sc_main(int argc, char *argv[]) {

  //sc_report_handler::set_actions(SC_ERROR, SC_ABORT);
  //sc_report_handler::set_actions(SC_WARNING, SC_ABORT);

  // ////////////////////////////////////////////////////
  // GreenControl Core
  //gs::ctr::GC_Core       core;
  //  or
  //gs::ctr::GC_Core::get_instance(); // optional, done automatically by gc_ports
  //  or
  //nothing

  // ////////////////////////////////////////////////////
  // GreenConfig Service Plugin
  //gs::cnf::ConfigDatabase cnfdatabase("ConfigDatabase");
  //gs::cnf::ConfigPlugin configPlugin(&cnfdatabase);
  //  or
  //gs::cnf::ConfigPlugin configPlugin("ConfigPlugin");
  //  or
  //gs::cnf::ConfigPlugin::get_instance();
  //  or
  //nothing
  
  // Configuration with GreenConfig config files
  gs::cnf::ConfigFile_Tool configTool("ConfigFileTool");
  configTool.parseCommandLine(argc, argv); // parses the command line for --configfile
  configTool.config("demoplatform.cfg");   // reads in a config file
  
  // Configuration with GreenConfig command line options
  gs::cnf::CommandLineConfigParser configParser;
  configParser.parse(argc, argv); // parses the command line

  // Command line tool which provides a command line to access parameters
  gs::cnf::CommandLine_Tool commandLineTool("CommandLineTool");

  // ////////////////////////////////////////////////////
  // Slaves

  Mem           mem("Mem");
  Cache         cache("Cache1");
  PCIeDevice    pcie("PCIeDevice2");

  // ////////////////////////////////////////////////////
  // Masters

  TG            tg("TG1");  // generate _after_ slaves because slave addresses are read from slave instances

  // ////////////////////////////////////////////////////
  // Connect TG and Cache directly
  
  tg.master_port(cache.slave_port);

  // ////////////////////////////////////////////////////
  // GreenBus SimpleBus (Cache_Devices)

  SimpleBusProtocol p("Protocol_SimpleBus", sc_time(10, SC_NS));
  fixedPriorityScheduler s("Scheduler_SimpleBus");
  GenericRouter r("Router_SimpleBus");
  r.protocol_port(p);
  p.router_port(r);
  p.scheduler_port(s);
  
  /* connect Cache and devices */
  // Connect Masters
  cache.master_port(r.target_port);
  // Connect Slaves
  r.init_port(mem.slave_port);
  r.init_port(pcie.slave_port);

  /*
   * connect TG directly to devices (without Cache) (Then leave out the Cache and SimpleBus 1.)
   // Connect Masters
   tg.master_port(r.target_port);
   // Connect Slaves
   r.init_port(mem.slave_port);
   r.init_port(pcie.slave_port);
  */

  // ////////////////////////////////////////////////////

  sc_start();
  
  cout << "//////////////////////// SIMULATION ENDED ////////////////////////////" << endl;
  cout << "//////////////////////// SIMULATION ENDED ////////////////////////////" << endl;
  cout << "//////////////////////// SIMULATION ENDED ////////////////////////////" << endl;

  return EXIT_SUCCESS; 
  
}
