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


//
// Regression tests for GreenAV
//
//

#include "test_globals.h"

// GreenConfig
#include "greencontrol/config.h" 
// GreenAV
#include "greencontrol/gav/plugin/gav_plugin.h"
// include the output plugin(s) that are needed
#include "greencontrol/gav/plugin/Stdout_OutputPlugin.h"

#include "greencontrol/core/helpfunctions.h"

#include "OutputPluginEnabledParamsMod.h" // EnaMod

/// Testbench for the GreenAV regression tests
/**
 */
int sc_main(int argc, char *argv[]) {
  
  sc_core::sc_report_handler::set_actions(sc_core::SC_ERROR, sc_core::SC_ABORT);
  
  // GreenControl Core instance
  gs::ctr::GC_Core       core;
  
  // GreenConfig Plugin
  gs::cnf::ConfigDatabase cnfdatabase("ConfigDatabase");
  gs::cnf::ConfigPlugin configPlugin(&cnfdatabase);
  
  // GreenAV Plugin
  gs::av::GAV_Plugin analysisPlugin(gs::av::STDOUT_OUT);
  
  
  // Modules
  EnaMod enaMod("EnaMod");
  

  gs::gs_param<int> mytopparamexpl("top_param_expl", 22);
  gs::gs_param<int> mytopparamexplB("top_param_expl_B", 22);
  
  gs::cnf::cnf_api_if *mApi = gs::cnf::GCnf_Api::getApiInstance(NULL);
  // test output plugin owned by analysis plugin
  mApi->setInitValue("AnalysisPlugin.STDOUT_OUT_default.EnaMod.ExplicitParam", "1");
  mApi->setInitValue("AnalysisPlugin.STDOUT_OUT_default.EnaMod.ImplicitParam", "1");
  mApi->setInitValue("AnalysisPlugin.STDOUT_OUT_default.EnaMod.ImplicitParam2", "0");
  mApi->setInitValue("AnalysisPlugin.STDOUT_OUT_default.top_param_expl", "1");
  mApi->setInitValue("AnalysisPlugin.STDOUT_OUT_default.top_param_impl", "1");
  mApi->setInitValue("AnalysisPlugin.STDOUT_OUT_default.top_param_impl2", "0");

  // test output plugin owned by a module
  mApi->setInitValue("EnaMod.main_action.STDOUT_OUT_any.EnaMod.ExplicitParamB", "1");
  mApi->setInitValue("EnaMod.main_action.STDOUT_OUT_any.EnaMod.ImplicitParamB", "1");
  mApi->setInitValue("EnaMod.main_action.STDOUT_OUT_any.top_param_expl_B", "1");
  mApi->setInitValue("EnaMod.main_action.STDOUT_OUT_any.top_param_impl_B", "1");

  
  std::cout << std::endl << "-------- BEGIN SIMULATION ---------" << std::endl;
  sc_core::sc_start();
  std::cout << "--------- END SIMULATION ----------" << std::endl << std::endl;

  std::cout << std::endl << "Parameter list:" << std::endl;
  gs::ctr::show_vector<std::string>(gs::cnf::GCnf_Api::getApiInstance(NULL)->getParamList());
  std::cout << std::endl;
  
  return EXIT_SUCCESS; 
  
}
