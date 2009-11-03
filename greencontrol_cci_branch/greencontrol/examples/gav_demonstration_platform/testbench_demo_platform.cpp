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
//   This program is free software.
// 
//   If you have no applicable agreement with GreenSocs Ltd, this software
//   is licensed to you, and you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   If you have a applicable agreement with GreenSocs Ltd, the terms of that
//   agreement prevail.
// 
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
//   02110-1301  USA 
// 
// ENDLICENSETEXT

// SystemC included in globals!

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
// GreenConfig special APIs
#include "greencontrol/config_api_command_line_tool.h" // Command Line Tool
#include "greencontrol/config_api_config_file_parser.h"  // API Tool to read configuration file(s)
#include "greencontrol/config_api_command_line_parser.h" // API Tool to process config command line parameters
// GreenAV
#include "greencontrol/gav/plugin/gav_plugin.h"
// include the output plugin(s) that are needed
#include "greencontrol/gav/plugin/Stdout_OutputPlugin.h"
#include "greencontrol/analysis_file_outputplugin.h"
#include "greencontrol/analysis_csv_outputplugin.h"

// SimpleBus
#include "greenbus/transport/genericRouter.h"
#include "greenbus/protocol/SimpleBus/simpleBusProtocol.h"
#include "greenbus/scheduler/fixedPriorityScheduler.h"

// Demonstration Platform
#include "Mem.h"
#include "TG.h"
#include "Cache.h"
#include "PCIeDevice.h"

//#include "ShowSCObjects.h"

/// Testbench for the Demonstration Platform for GreenAV
/**
 * The testbench instantiates the GreenControl Core and the needed user modules.
 * The user modules communicate over GreenBus
 *
 * This demo platform does _not_ use PCIe because this is untimed and 
 * for the analysis test timing is needed.
 *
 * See the GreenAV Tutorial for a documentation on this example
 * (greencontrol/docs/GreenAV/tutorial)!
 *
 * Usage e.g.:
 * \code
 * ./testbench_demonstration_platform --configfile demoplatform.cfg
 * \endcode
 */
int sc_main(int argc, char *argv[]) {

  //sc_report_handler::set_actions(SC_ERROR, SC_ABORT);  // make a breakpoint in SystemC file sc_stop_here.cpp
  //sc_report_handler::set_actions(SC_WARNING, SC_ABORT);  // make a breakpoint in SystemC file sc_stop_here.cpp
  sc_report_handler::set_actions(SC_INFO, SC_DO_NOTHING);

  
  // ////////////////////////////////////////////////////
  // GreenControl Core
  gs::ctr::GC_Core       core("ControlCore");


  // ////////////////////////////////////////////////////
  // GreenConfig
  
  // ConfigPlugin
  gs::cnf::ConfigDatabase cnfdatabase("ConfigDatabase");
  gs::cnf::ConfigPlugin configPlugin("ConfigPlugin", &cnfdatabase);
  
  // Configuration with GreenConfig config files
  gs::cnf::ConfigFile_Tool configTool("ConfigFileTool");
  configTool.config("demoplatform.cfg");      // reads in a config file
  //configTool.config("demoplatform_long.cfg"); // reads in a config file
  configTool.parseCommandLine(argc, argv); // parses the command line for --configfile
  
  // Configuration with GreenConfig command line options
  gs::cnf::CommandLineConfigParser configParser("ConfigParser");
  configParser.parse(argc, argv); // parses the command line

  // Command line tool which provides a command line to access parameters
  gs::cnf::CommandLine_Tool commandLineTool("CommandLineTool");


  // ////////////////////////////////////////////////////
  // GreenAV

  // GreenAV Plugin
  gs::av::GAV_Plugin analysisPlugin("AnalysisPlugin");

  // sc_main Config API
  gs::cnf::cnf_api* mainApi = gs::cnf::GCnf_Api::getApiInstance(NULL);
  // sc_main Analysis API
  boost::shared_ptr<gs::av::GAV_Api>  mainGAVApi = gs::av::GAV_Api::getApiInstance(NULL);

  
  // ////////////////////////////////////////////////////
  // Slaves

  Mem           mem("Mem");
  PCIeDevice    pcie1("PCIeDevice1");
  PCIeDevice    pcie2("PCIeDevice2");

  
  // ////////////////////////////////////////////////////
  // Masters

  TG            tg1("TG1");  // instantiate _after_ slaves because slave addresses are read from slave instances
  Cache         cache1("Cache1");
  TG            tg2("TG2");  // instantiate _after_ slaves because slave addresses are read from slave instances
  Cache         cache2("Cache2");
  TG            tg3("TG3");  // instantiate _after_ slaves because slave addresses are read from slave instances
  Cache         cache3("Cache3");

  
  // ////////////////////////////////////////////////////
  // Connect TG and Cache directly
  
  tg1.master_port(cache1.slave_port);
  tg2.master_port(cache2.slave_port);
  tg3.master_port(cache3.slave_port);

  
  // ////////////////////////////////////////////////////
  // GreenBus SimpleBus (Cache_Devices and Mem)

  SimpleBusProtocol p1("Protocol_SimpleBus", sc_time(10, SC_NS));
  fixedPriorityScheduler s1("Scheduler_SimpleBus");
  GenericRouter r1("Router_SimpleBus");
  r1.protocol_port(p1);
  p1.router_port(r1);
  p1.scheduler_port(s1);
  
  /* connect Cache and devices */
  // Connect Masters
  cache1.master_port(r1.target_port);
  cache2.master_port(r1.target_port);
  cache3.master_port(r1.target_port);
  // Connect Slaves
  r1.init_port(mem.slave_port);
  
  
  // ////////////////////////////////////////////////////
  // GreenBus PCIeBridge_SimpleBus (PCIeDevices)
  
  SimpleBusProtocol p2("Protocol_PCIeBridge_SimpleBus", sc_time(5, SC_NS));
  fixedPriorityScheduler s2("Scheduler_PCIeBridge_SimpleBus");
  GenericRouter r2("PCIeBridge_SimpleBus");
  r2.protocol_port(p2);
  p2.router_port(r2);
  p2.scheduler_port(s2);

  // Connect PCIeBridge_SimpleBus to Router_SimpleBus
  r1.init_port(r2.target_port);
  // Connect PCIeDevices
  r2.init_port(pcie1.slave_port);
  r2.init_port(pcie2.slave_port);

  // ////////////////////////////////////////////////////
  // Analysis
  
  // ** Analysis file-output for dynamic transactions per second over all ports
  gs::gs_param_base *dy_overall_tr_p_sec = mainApi->getPar("Protocol_SimpleBus.dynamic_tr_p_sec_overall_result_0");
  mainGAVApi->add_to_default_output(gs::av::CSV_FILE_OUT, dy_overall_tr_p_sec);
  
  // ** Analysis file-output for recorded transactions at Mem slave port
  gs::av::OutputPlugin_if* record_opp 
    = mainGAVApi->create_OutputPlugin(gs::av::TXT_FILE_OUT, "recorded_tr.log.txt");
  // add recorded transaction to file output
  gs::gs_param_base *recorded_mem_transaction = mainApi->getPar("Mem.simplebus_slaveport.recorded_transaction");
  mainGAVApi->add_to_output(record_opp, recorded_mem_transaction);
  // only record transactions from the master Cache1
  gs::gs_param<long> *record_transaction_master_id_filter 
    = dynamic_cast<gs::gs_param<long>*>(mainApi->getPar("Mem.simplebus_slaveport.record_transaction_master_id_filter"));
  *record_transaction_master_id_filter = cache1.master_port.get_master_port_number();
  cout << "only record transactions from master Cache1 (mID=0x"<<hex << (long)cache1.master_port.get_master_port_number()<<dec<<") at the Mem port (see file recorded_tr.log.txt)"<<endl;
  // add recorded transaction to file output
  //mainApi->getPar("PCIeDevice2.simplebus_slaveport.record_transactions_enabled")->setString("true"); // enable recording
  //gs::gs_param_base *recorded_dev_transaction = mainApi->getPar("PCIeDevice2.simplebus_slaveport.recorded_transaction");
  //mainGAVApi->add_to_output(record_opp, recorded_dev_transaction);
  
  // ** Analysis file-output for transaction latency for all masters (Cache[1|2|3])
  gs::av::OutputPlugin_if* trlatency_opp 
    = mainGAVApi->create_OutputPlugin(gs::av::TXT_FILE_OUT, "tr_latency.log.txt");
  // TG1 / Cache1
  mainGAVApi->add_to_output(trlatency_opp, mainApi->getPar("TG1.iport.transaction_overall_latency_result_0"));
  mainGAVApi->add_to_output(trlatency_opp, mainApi->getPar("TG1.iport.transaction_transport_latency_result_0"));
  mainGAVApi->add_to_output(trlatency_opp, mainApi->getPar("Cache1.iport.transaction_transport_latency_result_0"));
  // TG2 / Cache2
  mainGAVApi->add_to_output(trlatency_opp, mainApi->getPar("TG2.iport.transaction_overall_latency_result_0"));
  mainGAVApi->add_to_output(trlatency_opp, mainApi->getPar("TG2.iport.transaction_transport_latency_result_0"));
  mainGAVApi->add_to_output(trlatency_opp, mainApi->getPar("Cache2.iport.transaction_transport_latency_result_0"));
  // TG3 / Cache3
  mainGAVApi->add_to_output(trlatency_opp, mainApi->getPar("TG3.iport.transaction_overall_latency_result_0"));
  mainGAVApi->add_to_output(trlatency_opp, mainApi->getPar("TG3.iport.transaction_transport_latency_result_0"));
  mainGAVApi->add_to_output(trlatency_opp, mainApi->getPar("Cache3.iport.transaction_transport_latency_result_0"));
  
  cout << "//////////////////////// SIMULATION START ////////////////////////////" << endl;
  
  sc_start(10001, SC_MS); // 10 seconds
  
  cout << "//////////////////////// SIMULATION END //////////////////////////////" << endl;
  cout << "Simulation runtime: " << sc_core::sc_time_stamp() << endl;;
  cout << endl << endl;
  
  //ShowSCObjects::showSCObjects();

  // ** List of all parameters
  cout << "--------------------------------------------------------" << endl;
  cout << "Parameter list: "<< endl << endl;
  std::vector<std::string> parlist = mainApi->getParamList();
  for (unsigned int i = 0; i < parlist.size(); i++)
    cout << "  " << parlist[i] << endl;
  cout << endl << endl;
  
  gs::gs_param_base *p;
  // ** Show cache hit rates
  cout << "--------------------------------------------------------" << endl;
  cout << "---------   ANALYSIS  RESULTS   ------------------------" << endl;
  cout << "--------------------------------------------------------" << endl << endl;
  cout << "Cache hit rates:" << endl;
  // Cache1
  cout << "   Cache1 (overall):      " << mainApi->getPar("Cache1.LRUCache.overall_hit_rate_result_2")->getString() << "%" << endl;
  cout << "          overall hits:   " << mainApi->getPar("Cache1.LRUCache.overall_hits")->getString() << endl;
  cout << "          overall misses: " << mainApi->getPar("Cache1.LRUCache.overall_misses")->getString() << endl;
  cout << "   Cache1 (address range "<<mainApi->getPar("Cache1.LRUCache.hitrate_lower_addr")->getString()<<
          "-"<<mainApi->getPar("Cache1.LRUCache.hitrate_upper_addr")->getString()<<"): " << 
          mainApi->getPar("Cache1.LRUCache.addr_range_hit_rate_result_2")->getString() << "%" << endl;
  cout << "          address range hits:   " << mainApi->getPar("Cache1.LRUCache.addr_range_hits")->getString() << endl;
  cout << "          address range misses: " << mainApi->getPar("Cache1.LRUCache.addr_range_misses")->getString() << endl;
  // Cache2
  cout << "   Cache2 (overall):      " << mainApi->getPar("Cache2.LRUCache.overall_hit_rate_result_2")->getString() << "%" << endl;
  cout << "          overall hits:   " << mainApi->getPar("Cache2.LRUCache.overall_hits")->getString() << endl;
  cout << "          overall misses: " << mainApi->getPar("Cache2.LRUCache.overall_misses")->getString() << endl;
  cout << "   Cache2 (address range "<<mainApi->getPar("Cache2.LRUCache.hitrate_lower_addr")->getString()<<
          "-"<<mainApi->getPar("Cache2.LRUCache.hitrate_upper_addr")->getString()<<"): " << 
          mainApi->getPar("Cache2.LRUCache.addr_range_hit_rate_result_2")->getString() << "%" << endl;
  cout << "          address range hits:   " << mainApi->getPar("Cache2.LRUCache.addr_range_hits")->getString() << endl;
  cout << "          address range misses: " << mainApi->getPar("Cache2.LRUCache.addr_range_misses")->getString() << endl;
  // Cache3
  cout << "   Cache3 (overall):      " << mainApi->getPar("Cache3.LRUCache.overall_hit_rate_result_2")->getString() << "%" << endl;
  cout << "          overall hits:   " << mainApi->getPar("Cache3.LRUCache.overall_hits")->getString() << endl;
  cout << "          overall misses: " << mainApi->getPar("Cache3.LRUCache.overall_misses")->getString() << endl;
  cout << "   Cache3 (address range "<<mainApi->getPar("Cache3.LRUCache.hitrate_lower_addr")->getString()<<
          "-"<<mainApi->getPar("Cache3.LRUCache.hitrate_upper_addr")->getString()<<"): " << 
          mainApi->getPar("Cache3.LRUCache.addr_range_hit_rate_result_2")->getString() << "%" << endl;
  cout << "          address range hits:   " << mainApi->getPar("Cache3.LRUCache.addr_range_hits")->getString() << endl;
  cout << "          address range misses: " << mainApi->getPar("Cache3.LRUCache.addr_range_misses")->getString() << endl;
  cout << endl;
  
  cout << "--------------------------------------------------------" << endl << endl;
  cout << "Transaction per second:" << endl;
  cout << " over all ports (actual dynamic transactions per second: see default output file)" << endl;
  cout << " over all ports Router_SimpleBus (extrapolated over whole time):      " << mainApi->getPar("Protocol_SimpleBus.tr_p_sec_overall_result_0")->getString() << endl;
  cout << " over all ports PCIeBridge_SimpleBus (extrapolated over whole time):  " << mainApi->getPar("Protocol_PCIeBridge_SimpleBus.tr_p_sec_overall_result_0")->getString() << endl;
  cout << endl;
  cout << " Router in ports:" << endl;
  // Cache1
  {
    #if SYSTEMC_API == 210
    gs::gs_param_base *trCntCache1 = mainApi->getPar("in_port_tr_counter_Cache1");
    gs::gs_param_base *trPSCache1 = mainApi->getPar("in_port_tr_p_sec_Cache1_result_0");
    #else
    gs::gs_param_base *trCntCache1 = mainApi->getPar("Router_SimpleBus.tport.payload_event_queue_0.in_port_tr_counter_Cache1");
    gs::gs_param_base *trPSCache1 = mainApi->getPar("Router_SimpleBus.tport.payload_event_queue_0.in_port_tr_p_sec_Cache1_result_0");
    #endif
    if (trPSCache1)  cout << "  Cache1:   extrapolated transactions per second:  " << trPSCache1->getString() << endl;
    if (trCntCache1) cout << "                       absolut transaction count:  " << trCntCache1->getString() << endl;
  }
  // Cache2
  {
    #if SYSTEMC_API == 210
    gs::gs_param_base *trCntCache2 = mainApi->getPar("in_port_tr_counter_Cache2");
    gs::gs_param_base *trPSCache2 = mainApi->getPar("in_port_tr_p_sec_Cache2_result_0");
    #else
    gs::gs_param_base *trCntCache2 = mainApi->getPar("Router_SimpleBus.tport.payload_event_queue_0.in_port_tr_counter_Cache2");
    gs::gs_param_base *trPSCache2 = mainApi->getPar("Router_SimpleBus.tport.payload_event_queue_0.in_port_tr_p_sec_Cache2_result_0");
    #endif
    if (trPSCache2)  cout << "  Cache2:   extrapolated transactions per second:  " << trPSCache2->getString() << endl;
    if (trCntCache2) cout << "                       absolut transaction count:  " << trCntCache2->getString() << endl;
  }
  // Cache3
  {
    #if SYSTEMC_API == 210
    gs::gs_param_base *trCntCache3 = mainApi->getPar("in_port_tr_counter_Cache3");
    gs::gs_param_base *trPSCache3 = mainApi->getPar("in_port_tr_p_sec_Cache3_result_0");
    #else
    gs::gs_param_base *trCntCache3 = mainApi->getPar("Router_SimpleBus.tport.payload_event_queue_0.in_port_tr_counter_Cache3");
    gs::gs_param_base *trPSCache3 = mainApi->getPar("Router_SimpleBus.tport.payload_event_queue_0.in_port_tr_p_sec_Cache3_result_0");
    #endif
    if (trPSCache3)  cout << "  Cache3:   extrapolated transactions per second:  " << trPSCache3->getString() << endl;
    if (trCntCache3) cout << "                       absolut transaction count:  " << trCntCache3->getString() << endl;
  }
  // Mem
  p = mainApi->getPar("Router_SimpleBus.out_port_tr_p_sec_Mem_result_0");
  if (p) cout << "  Mem:   extrapolated transactions per second:  " << p->getString() << endl;
  p = mainApi->getPar("Router_SimpleBus.out_port_tr_counter_Mem");
  if (p) cout << "                    absolut transaction count:  " << p->getString() << endl;
  // PCIeDevice1
  p = mainApi->getPar("PCIeBridge_SimpleBus.out_port_tr_p_sec_PCIeDevice1_result_0");
  if (p) cout << "  PCIeDevice1:   extrapolated transactions per second:  " << p->getString() << endl;
  p = mainApi->getPar("PCIeBridge_SimpleBus.out_port_tr_counter_PCIeDevice1");
  if (p) cout << "                            absolut transaction count:  " << p->getString() << endl;
  // PCIeDevice2
  p = mainApi->getPar("PCIeBridge_SimpleBus.out_port_tr_p_sec_PCIeDevice2_result_0");
  if (p) cout << "  PCIeDevice2:   extrapolated transactions per second:  " << p->getString() << endl;
  p = mainApi->getPar("PCIeBridge_SimpleBus.out_port_tr_counter_PCIeDevice2");
  if (p) cout << "                            absolut transaction count:  " << p->getString() << endl;
  cout << endl;
  cout << "--------------------------------------------------------" << endl << endl;
  
  cout << "Transaction recording at Mem slave port recording transactions from Cache1: " << endl;
  cout << "  see output file 'recorded_tr.log.txt'" << endl;
  cout << endl;
  cout << "--------------------------------------------------------" << endl << endl;
  
  return EXIT_SUCCESS; 
  
}
