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

#include "ParameterOwnerModule.h"

using namespace std;
using namespace sc_core;

void ParameterOwnerModule::main_action() {

  wait(1, SC_NS);

  wait(1, SC_US);

  
  wait(1, SC_NS);
  cout << name() << ": set int_param = 100" << endl;
  int_param = 100;
  cout << endl;
  wait(SC_ZERO_TIME);

  cout << name() << ": set str_param = 'Hello World!'" << endl;
  str_param = "Hello World!";
  cout << endl;
  wait(SC_ZERO_TIME);
  
  
  wait(1, SC_NS);
  cout << name() << ": set int_param = 200" << endl;
  int_param = 200;
  cout << endl;
  wait(SC_ZERO_TIME);
  
  cout << name() << ": set str_param = 'Hello Germany!'" << endl;
  str_param = "Hello Germany!";
  cout << endl;
  wait(SC_ZERO_TIME);
  
  
  cout << name() << ": Create two new parameters which can be observed with new param callbacks/events and set them" << endl;

  cout << name() << ": unnamed param" << endl;
  gs::gs_param<unsigned char> uchar_param;
  cout << endl;
  wait(SC_ZERO_TIME);
  cout << name() << ": Set the new parameter uchar_param" << endl;
  uchar_param = 'u';
  cout << endl;

  cout << endl << "**** Parameter list: " << endl;
  vector<string> vec = gs::cnf::GCnf_Api::getApiInstance(this)->getParamList();
  // Show vector
  vector<string>::iterator iter;
  std::stringstream ss_show;
  iter = vec.begin();
  while( iter != vec.end() ) {
    if (iter != vec.begin()) ss_show << ", ";
    ss_show << *iter;
    iter++;
  }
  cout << "   " << ss_show.str() << endl<<endl<<endl;
  
  wait(SC_ZERO_TIME);
  gs::gs_param<unsigned long long> ull_param("ull_param", 1000ULL);
  cout << endl;
  wait(SC_ZERO_TIME);
  cout << name() << ": Set the new parameter ull_param" << endl;
  ull_param = 2000LL;
  cout << endl;
 
  wait(100, SC_US);
  cout << "----------------------------" << endl;

}
