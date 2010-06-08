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

#include "ParamListIP.h"

using namespace std;
using namespace sc_core;

void ParamListIP::main_action() {

  wait(1, SC_MS);
  
  cout << "."<<endl;
  cout << "."<<endl;
  DEMO_TRACE(name(), "This is the main action in ParamListIP");

  cout << "Show getParamList(IP1):"<<endl;
  gs::ctr::show_vector<std::string>(mApi->getParamList("IP1.*"));

  cout << "Show getParams(IP1):"<<endl;
  std::vector<gs::gs_param_base*> pv = mApi->getParams("IP1");
  show_params_vec(pv);
  

  cout << "Show getParamList(IP1.*):"<<endl;
  gs::ctr::show_vector<std::string>(mApi->getParamList("IP1.*"));

  cout << "Show getParams(IP1.*):"<<endl;
  show_params_vec(mApi->getParams("IP1.*"));

  cout << "."<<endl;
  cout << "."<<endl;
  cout << "."<<endl;
  
  
}


void ParamListIP::show_params_vec(std::vector<gs::gs_param_base*> params_vec) {
  std::vector<gs::gs_param_base*>::iterator iter;
  for (iter = params_vec.begin(); iter != params_vec.end(); iter++) {
    cout << " "<< (*iter)->getName() << endl;
  }
}

