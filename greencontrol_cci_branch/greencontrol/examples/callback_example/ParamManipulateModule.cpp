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

#include "ParamManipulateModule.h"

using namespace std;
using namespace sc_core;

void ParamManipulateModule::main_action() {

  DEMO_TRACE(name(), " waits for later");
  wait(6, SC_US);
  
  gs::gs_param<int> &p = *static_cast<gs::gs_param<int>* >(mGcnfApi->getPar("Owner.int_param"));
  gs::gs_param<std::string> &pu = *static_cast<gs::gs_param<std::string>* >(mGcnfApi->getPar("Owner.str_param"));
  
  DEMO_TRACE(name(), "Set parameter Owner.int_param to value=5000");
  p.setString("5000");
  cout << endl;
  wait(SC_ZERO_TIME);
  
  DEMO_TRACE(name(), "Set parameter Owner.int_param to value=5001");
  p.setValue(5001);
  cout << endl;
  wait(SC_ZERO_TIME);
  
  DEMO_TRACE(name(), "Set parameter Owner.int_param to value=5002");
  p = 5002;
  cout << endl;
  wait(SC_ZERO_TIME);
  
  // //// wait ////////////////////
  wait(1, SC_US);

  DEMO_TRACE(name(), "Set parameter Owner.int_param to value=6002");
  p = 6002;
  cout << endl;
  wait(SC_ZERO_TIME);

  DEMO_TRACE(name(), "Set parameter Owner.str_param to value=\"SHOW THIS\"");
  pu = "SHOW THIS";
  cout << endl;
  wait(SC_ZERO_TIME);
  
  // //// wait ////////////////////
  wait(6, SC_US);
  
  DEMO_TRACE(name(), "Set parameter Owner.int_param to value=7002");
  p = 7002;
  cout << endl;
  wait(SC_ZERO_TIME);

  DEMO_TRACE(name(), "Set parameter Owner.str_param to value=\"NOT OBSERVE THIS\"");
  pu = "NOT OBSERVE THIS";
  cout << endl;
  wait(SC_ZERO_TIME);
  
}

