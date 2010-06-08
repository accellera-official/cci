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
