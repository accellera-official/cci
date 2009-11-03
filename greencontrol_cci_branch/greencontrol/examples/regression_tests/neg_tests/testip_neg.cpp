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

#include "testip_neg.h"

using namespace std;
using namespace sc_core;

#define IPnegtest(a) { cout << "IP_neg test: " << a <<endl; }
#define SPACE {cout << endl; }

// ///////////////////////// //
// NEGATIVE REGRESSION TESTS //
// ///////////////////////// //

void TestIP_neg::main_action() {

  DEMO_PRINT_SPACE;
  DEMO_TRACE(name(), "This is the main action in TestIP_neg for negative regression tests");

  // get update event with not hierarchical parameter name: results in an exception
  //const sc_event *ev;
  //ev = &gs::cnf::GCnf_Api::getApiInstance(this)->getUpdateEvent("notHierarchicalName");

  // /// gs_param tests
  
  // Set parameter values with strings 
  cout << "int_param = " << int_param << endl;
  cout << "set int_param = new param named 'Hello'" << endl;
  // results in compile error
  // int_param = "Hello";   // add comment here to avoid error
  cout << "int_param = " << int_param << endl;
  
  // Add two parameters with same name
  cout << "Add two parameters with same name:" << endl;
  gs::gs_param<int> param1("param1");
  // gs::gs_param<int> param2("param1");  // add comment here to avoid error
  cout << "done" << endl;
  
  str_param = "Hello string";
  //int_param = str_param;  // add comment here to avoid error
  cout << "int_param = " << int_param << endl;
  
}
