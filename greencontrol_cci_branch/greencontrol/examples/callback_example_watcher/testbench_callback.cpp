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

/*
   Callback and notify tests:
    - Module ParameterOwnerModule owns some parameters
    - Module ObserverModule has some callback functions that are registered to the parameters
    - Module ParamManipulateModule changes the parameter values that should be observed by the MyIP2
 */

#include "callb_globals.h"

// GreenConfig
#include "greencontrol/config.h" 
// GreenConfig special plugin
#include "greencontrol/gcnf_watcher/plugin/cnfplugin_watcher.h"

#include "ParameterOwnerModule.h"
#include "ObserverModule.h"
#include "ParamManipulateModule.h"

/// Testbench for the example GreenConfig
int sc_main(int argc, char *argv[]) {
  //sc_report_handler::set_actions(SC_ERROR, SC_ABORT);
  //sc_report_handler::set_actions(SC_WARNING, SC_ABORT);

  /// GreenControl Core instance
  gs::ctr::GC_Core       core;

  // GreenConfig Plugin
  // 
  gs::cnf::ConfigDatabase cnfdatabase("ConfigDatabase");
  gs::cnf::ConfigPlugin configPlugin(&cnfdatabase);

  gs::cnf::ConfigPluginWatcher configPluginWatcher("ConfigWatcherPlugin");

  ParameterOwnerModule  owner      ("Owner");
  ObserverModule        observer   ("Observer");
  ParamManipulateModule manipulator("Manipulator");

  sc_core::sc_start();
  
  return EXIT_SUCCESS; 
  
}
