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

#include "testtool.h"
#include <typeinfo>
#include <stdexcept>
#include <cxxabi.h> 

using namespace std;
using namespace sc_core;

void TestTool::main_action() {
  wait(SC_ZERO_TIME);
  
  GC_PRINT_SPACE;
  GC_DUMP_N(name(), "This is the main action in TestTool");

  GC_PRINT_SPACE; GC_PRINT_SPACE;

  test_action_callback();

  // get parameter list
  toolApi.getParamList();

  // set parameter
  toolApi.getPar("IP1.scmlStringParam")->setString("TestValue1" );
  GC_PRINT_SPACE; GC_PRINT_SPACE;

  // get parameter
  toolApi.getPar("IP1.scmlIntParam");
  GC_PRINT_SPACE; GC_PRINT_SPACE;

  // get update event
  m_observer_event = &toolApi.getPar("IP3.int_param")->getUpdateEvent();
  m_observer_update_event.notify();
  GC_PRINT_SPACE; GC_PRINT_SPACE;

  wait(SC_ZERO_TIME);
  
  // set parameter
  toolApi.getPar("IP3.int_param")->setString("6666");
  GC_PRINT_SPACE; GC_PRINT_SPACE;
  wait(SC_ZERO_TIME);
  
  // set parameter
  toolApi.getPar("IP3.int_param")->setString("2002");
  GC_PRINT_SPACE; GC_PRINT_SPACE;
  wait(SC_ZERO_TIME);

  // get parameter
  toolApi.getPar("IP2.scmlStringParam");
  GC_PRINT_SPACE; GC_PRINT_SPACE;

  // get not existing parameter
  toolApi.getPar("IP1.param3NotExisting");
  cout << "OK if getting failed!!" << endl<<endl;
  GC_PRINT_SPACE; GC_PRINT_SPACE;

  // set not yet existing parameter (as implicit)
  toolApi.setInitValue("IP1.param4NotExisting", "implicitValue");
  GC_PRINT_SPACE; GC_PRINT_SPACE;

  // get implicit parameter
  toolApi.getPar("IP1.param4NotExisting");
  GC_PRINT_SPACE; GC_PRINT_SPACE;

  // get parameter list
  toolApi.getParamList();
  GC_PRINT_SPACE; GC_PRINT_SPACE;

  // make parameter explicit (beware: this Module must not use ADD for this parameter!!)
  toolApi.addParam("IP1.param4NotExisting", "explicitValue");
  GC_PRINT_SPACE; GC_PRINT_SPACE;
  cout << "Add warning(s) OK!" << endl<< endl;
  
  // change value of now emplicit parameter
  toolApi.getPar("IP1.param4NotExisting")->setString("newExplicitValue");
  GC_PRINT_SPACE; GC_PRINT_SPACE;

  // get parameter list
  toolApi.getParamList();
  GC_PRINT_SPACE; GC_PRINT_SPACE;

  // set parameter
  toolApi.getPar("IP2.scmlStringParam")->setString("newTestForCallback");
  GC_PRINT_SPACE; GC_PRINT_SPACE;

  // get parameter list
  toolApi.getParamList("IP1");
  GC_PRINT_SPACE; GC_PRINT_SPACE;

  // get parameter list
  toolApi.getParamList("IP1.*");
  GC_PRINT_SPACE; GC_PRINT_SPACE;

  cout << "END of automatic test" << endl;

}


void TestTool::test_action_callback() {

  GC_REGISTER_PARAM_CALLBACK(toolApi.getPar("IP2.scmlStringParam"), TestTool, config_callback);
  GC_PRINT_SPACE; GC_PRINT_SPACE;

  GC_REGISTER_PARAM_CALLBACK(toolApi.getPar("IP2.scmlStringParam"), TestTool, config_callback2);
  GC_PRINT_SPACE; GC_PRINT_SPACE;

  GC_REGISTER_PARAM_CALLBACK(toolApi.getPar("IP1.scmlStringParam"), TestTool, config_callback);
  GC_PRINT_SPACE; GC_PRINT_SPACE;

}
void TestTool::config_callback(gs::gs_param_base& par) {
  if (!par.is_destructing()) {
    GC_DUMP_N(name(), "config_callback: Callback for changed parameter "<<par.getName().c_str()<<". New value: "<<par.getString().c_str());
  }
  else {
    printf("TestIP3::config_callback: Parameter %s is destructing.\n", par.getName().c_str());
  }
}
void TestTool::config_callback2(gs::gs_param_base& par) {
  if (!par.is_destructing()) {
    GC_DUMP_N(name(), "config_callback2: Callback for changed parameter "<<par.getName().c_str()<<". New value: "<<par.getString().c_str());
  }
  else {
    printf("TestIP3::config_callback: Parameter %s is destructing.\n", par.getName().c_str());
  }
}


// SC_THREAD
void TestTool::event_observer() {
  cout << "event_observer started" << endl;
  while(true) {
    wait(*m_observer_event | m_observer_update_event);

    GC_DUMP_N(name(), "observer_scmethod: observed parameter has changed OR new event was set!");
    GC_DUMP_N(name(), "***********************************************************************");

  }
}
