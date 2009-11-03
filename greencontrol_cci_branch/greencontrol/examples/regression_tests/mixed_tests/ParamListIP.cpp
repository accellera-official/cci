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

