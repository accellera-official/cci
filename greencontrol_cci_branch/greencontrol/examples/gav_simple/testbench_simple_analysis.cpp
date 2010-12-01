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

// SystemC included in globals!

/*
 * This example is a simple GreenAV analysis example
 */

#include "simple_analysis_globals.h"

// GreenConfig
#include "greencontrol/config.h" 

// GreenAV
#include "greencontrol/gav/plugin/Stdout_OutputPlugin.h"

// Example
#include "AVOwner.h"
#include "AVAnalyserTool.h"
#include "AVscvAnalyserTool.h"

//#include "ShowSCObjects.h"

// without ModelSim
#ifndef MTI_SYSTEMC

/// Testbench for the GreenControl analysis (GreenAV) example
int sc_main(int argc, char *argv[]) {
  
  // GreenControl Core instance
  gs::ctr::GC_Core       core;

  // GreenConfig Plugin
  gs::cnf::ConfigDatabase cnfdatabase("ConfigDatabase");
  gs::cnf::ConfigPlugin configPlugin(&cnfdatabase);

  // GreenAV Plugin
  gs::av::GAV_Plugin analysisPlugin(gs::av::STDOUT_OUT);
  // negative test:
  //gs::av::GAV_Plugin analysisPlugin("AnalysisPlugin"); // will fail adding params to DEFAULT_OUT type
  
  gs::cnf::cnf_api_if *mApi = gs::cnf::GCnf_Api::getApiInstance(NULL);
  
  AVOwner            owner ("Owner");
  AVAnalyserTool     tool  ("AVTool");
  AVscvAnalyserTool  scvtool  ("AVscvTool");
  
  sc_core::sc_start();
  
  std::cout << "-------------------- SIMULATION ENDED -----------------------" << std::endl;
  
  std::cout << std::endl;
  gs::ctr::show_vector<std::string>(mApi->getParamList());
  std::cout << std::endl;
  
  //ShowSCObjects::showSCObjects();

  return EXIT_SUCCESS; 
  
}

//If using the ModelSim simulator, sccom compiles this 
#else

SC_MODULE(topSimpleAnalysis) 
{ 
  // GreenControl Core instance
  gs::ctr::GC_Core core;
  // GreenConfig Database
  gs::cnf::ConfigDatabase cnfdatabase;
  // GreenConfig Plugin
  gs::cnf::ConfigPlugin configPlugin;
  // GreenAV Plugin
  gs::av::GAV_Plugin analysisPlugin;
  // User modules
  AVOwner            owner;
  AVAnalyserTool     tool;
  AVscvAnalyserTool  scvtool;

  SC_CTOR(topSimpleAnalysis) 
  : core,
    cnfdatabase("ConfigDatabase"),
    configPlugin(&cnfdatabase), // This is not recommended coding style!! May lead to SystemC module stack error!
    analysisPlugin("AnalysisPlugin"),
    owner ("Owner"),
    tool  ("AVTool"),
    scvtool  ("AVscvTool")
  { 
    SC_THREAD(sc_main_body); 
  } 
  
  void sc_main_body();
};
void topSimpleAnalysis::sc_main_body() {
}

SC_MODULE_EXPORT(topSimpleAnalysis); 

#endif // end ModelSim
