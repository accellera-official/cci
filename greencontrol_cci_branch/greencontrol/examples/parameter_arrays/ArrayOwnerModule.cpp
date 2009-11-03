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

#include "ArrayOwnerModule.h"

using namespace std;
using namespace sc_core;

void ArrayOwnerModule::main_action() {

  cout << endl << "ArrayOwnerModule action" << endl;
  
  cout << "normal_uint_param="<< normal_uint_param << endl;
  
  // Test inital value settings
  cout << name() << "********** Test inital value settings" << endl;
  cout << name() << ": string_arr: " << string_arr.getString() << endl;
  REGR_TEST("PASS/FAIL: Passed if array is set to the default values set in the config file");
  cout << name() << ": uint_arr: " << uint_arr.getString() << endl;
  REGR_TEST("PASS/FAIL: Passed if uint_arr array is set to the default values set in the config file up to #12");
  REGR_TEST("PASS/FAIL: Passed if uint_arr array has been initialized to the size 12 (implicitely set by member 'init_size' in the config file)");
  //cout << name() << ": float_arr: " << float_arr.getString() << endl;
  //REGR_TEST("PASS/FAIL: Passed if array is set to the default values set in the config file");
  cout << endl;
  
  
  /*cout << "ArrayOwnerModule: show array:" << endl;
  cout << int_arr.getString() << endl;
  
  int_arr.setString("{\"152\" \"9154\"}");

  cout << "ArrayOwnerModule: show array:" << endl;
  cout << int_arr.getString() << endl;

  int_arr.setString("{\"11\" \"22\" \"33\" \"44\" \"55\" \"66\" \"77\" \"88\" \"99\" \"1010\" \"1111\" \"1212\"}");
  
  cout << "ArrayOwnerModule: show array:" << endl;
  cout << int_arr.getString() << endl;

  int_arr.setString("{10 12 13 14}");
  
  cout << "ArrayOwnerModule: show array:" << endl;
  cout << int_arr.getString() << endl;*/

  // Wait to preserve the array instance!
  wait(20, SC_NS);
  // Now the parameter arrray will be deleted!
}
