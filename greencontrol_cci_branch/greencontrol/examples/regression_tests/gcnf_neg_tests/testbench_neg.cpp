//   GreenConfig framework
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
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

// GreenControl
#include "greencontrol/config.h"    // Core


//#define TEST_CORE_SINGLETON_ERROR
#define TEST_CNF_PLUGIN_SINGLETON_ERROR

/// negative regression test to test error message if automatic plugin creation conflicts with manual instantiation
int sc_main(int argc, char *argv[]) {

  std::cout << "***** THIS TEST IS PASSED IF THERE ARE ERRORS!!! ***** (error depending on #defines)" << std::endl;
  
  //sc_core::sc_report_handler::set_actions(sc_core::SC_ERROR, sc_core::SC_ABORT);
  //sc_core::sc_report_handler::set_actions(sc_core::SC_WARNING, sc_core::SC_ABORT);
  //sc_core::sc_report_handler::set_actions(sc_core::SC_FATAL, sc_core::SC_STOP);

  // This will automatically create a Core and a Config Plugin
  gs::gs_param<int> my_param("my_param");
  
  
  // ////////////////////////////////////////////////////
  // GreenControl Core
#ifdef TEST_CORE_SINGLETON_ERROR
  gs::ctr::GC_Core       core;
#endif
  //  or
  gs::ctr::GC_Core::get_instance(); // optional, done automatically by gc_ports
  //  or
  //nothing

  // ////////////////////////////////////////////////////
  // GreenConfig Service Plugin
  //gs::cnf::ConfigDatabase cnfdatabase("ConfigDatabase");
  //gs::cnf::ConfigPlugin configPlugin(&cnfdatabase);
  //  or
#ifdef TEST_CNF_PLUGIN_SINGLETON_ERROR
  gs::cnf::ConfigPlugin configPlugin;
#endif
  //  or
  gs::cnf::ConfigPlugin::get_instance();
  //  or
  //nothing
  

  sc_core::sc_start();

  return EXIT_SUCCESS; 
  
}
