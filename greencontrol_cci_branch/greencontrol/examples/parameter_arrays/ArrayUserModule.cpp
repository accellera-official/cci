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

#include "ArrayUserModule.h"

using namespace std;
using namespace sc_core;

void ArrayUserModule::main_action() {

  wait(10, SC_NS);
  
  cout << endl << "ArrayUserModule action" << endl;
  
  gs::gs_param<int*> &arr = *static_cast<gs::gs_param<int*>* >(mApi->getPar("Owner.int_arr"));
  
  cout << name() << ": register callback for Owner.int_arr" << endl;
  // Register Callback for parameter int_param in module other_ip (IP1)
  GC_REGISTER_PARAM_CALLBACK(&arr, ArrayUserModule, config_callback);
  
  cout << name() << ": int_arr:" << arr.getString() << endl << endl;
  
  // Test setting members
  cout << name() << "********** Test setting members" << endl;
  cout << name() << ": set int_arr {0 1 2 3 4 5 6 7 8 9}" << endl;
  arr.setString("{0 1 2 3 4 5 6 7 8 9}");
  cout << name() << ": int_arr: " << arr.getString() << endl;
  REGR_TEST("PASS/FAIL: Passed if array is set to int_arr.0=0 int_arr.1=1 ... int_arr.9=9");
  cout << endl;

  // Test resize -> bigger
  cout << name() << "********** Test resize -> bigger" << endl;
  cout << name() << ": int_arr.resize(15)" << endl;
  arr.resize(15);
  cout << name() << ": int_arr: " << arr.getString() << endl;
  cout << name() << ": int_arr.size(): " << arr.size() << endl;
  REGR_TEST("PASS/FAIL: Passed if array is of size 15 and the former members are kept.");
  cout << endl;

  // Test resize -> smaller
  cout << name() << "********** Test resize -> smaller" << endl;
  cout << name() << ": int_arr.resize(8)" << endl;
  arr.resize(8);
  cout << name() << ": int_arr: " << arr.getString() << endl;
  cout << name() << ": int_arr.size(): " << arr.size() << endl;
  REGR_TEST("PASS/FAIL: Passed if array is of size 8 and the former members (0 to 7) are kept.");
  cout << endl;
  
  // Test setting members
  cout << name() << "********** Test setting members" << endl;
  cout << name() << ": set int_arr {0 1 2 3 4 5 6 7 8 9}" << endl;
  arr.setString("{0 1 2 3 4 5 6 7 8 9}");
  cout << name() << ": int_arr: " << arr.getString() << endl;
  REGR_TEST("PASS/FAIL: Passed if array is set to int_arr.0=0 int_arr.1=1 ... int_arr.9=9");
  cout << endl;
  
  // Test reduction of member number
  cout << name() << "********** Test reduction of member number" << endl;
  cout << name() << ": set int_arr {10 12 13 14}" << endl;
  arr.setString("{10 12 13 14}");
  cout << name() << ": int_arr: " << arr.getString() << endl;
  REGR_TEST("PASS/FAIL: Passed if array is set to int_arr.0=10 int_arr.1=12 int_arr.1=13 int_arr.1=14");
  cout << endl;
  
  // Test increasment of member number
  cout << name() << "********** Test increasment of member number" << endl;
  cout << name() << ": set int_arr {\"11\" \"22\" \"33\" \"44\" \"55\" \"66\" \"77\" \"88\" \"99\" \"1010\" \"1111\" \"1212\"}" << endl;
  arr.setString("{\"11\" \"22\" \"33\" \"44\" \"55\" \"66\" \"77\" \"88\" \"99\" \"1010\" \"1111\" \"1212\"}");
  cout << name() << ": int_arr: " << arr.getString() << endl;
  REGR_TEST("PASS/FAIL: Passed if array is set to int_arr.0=11 int_arr.x=xx ...");
  cout << endl;
  
  // Test different set string style
  cout << name() << "********** Test different set string style" << endl;
  cout << name() << ": set int_arr {\"152\" \"9154\"}" << endl;
  arr.setString("{\"152\" \"9154\"}");
  cout << name() << ": int_arr:" << arr.getString() << endl;
  REGR_TEST("PASS/FAIL: Passed if array is set to int_arr.0=152 int_arr.1=9154");
  cout << endl;

  // Test operator[]
  cout << name() << "********** Test operator[]" << endl;
  for (unsigned int i=0; i < arr.size(); i++) {
    cout << "member #" << i << " = " << arr[i].getString() << endl;
  }
  REGR_TEST("PASS/FAIL: Passed the members are shown correct");
  cout << endl;

  // Test method gs_param<T*>::at(int)
  cout << name() << "********** Test method gs_param<T*>::at(int)" << endl;
  for (unsigned int i=0; i < arr.size(); i++) {
    cout << "member #"<<i<<" = "<< arr.at(i).getString() << endl;
  }
  REGR_TEST("PASS/FAIL: Passed the members are shown correct");
  cout << endl;
  
  // Test constructor(name, vector)

  // Test operator=(vector)
  std::vector<int> intvec;
  for( int i = 0; i < 20; i++ ) {
    intvec.push_back( 110 + i );
  }
  cout << name() << "********** Test operator=(vector)" << endl;
  arr = intvec;
  cout << "Array = " << arr.getString() << endl;
  REGR_TEST("PASS/FAIL: Passed the members have been set to {110, 111, ..., 129}");
  cout << endl;

  // Test constructor with default vector values
  cout << name() << "********** Test constructor with default vector values" << endl;
  gs::gs_param<int*> my_new_arr("my_new_arr", intvec);
  cout << "Array '"<< my_new_arr.getName()<<"' = " << my_new_arr.getString() << endl;
  REGR_TEST("PASS/FAIL: Passed the members of my_new_arr have been set to {110, 111, ..., 129}");
  cout << endl;

  // Test setting members (get wit at() and operator[]) with = operator
  cout << name() << "********** Test setting members (get wit at() and operator[]) with = operator" << endl;
  gs::gs_param<int*> my_new_arr2("my_new_arr2", 5);
  cout << "Array (5 default members) '"<< my_new_arr2.getName()<<"' = " << my_new_arr2.getString() << endl;
  cout << "my_new_arr2.at(2) = 200" << endl;
  my_new_arr2.at(2) = 200;
  cout << "my_new_arr2[2] = 300" << endl;
  my_new_arr2[3] = 300;
  cout << "Array (5 default members) '"<< my_new_arr2.getName()<<"' = " << my_new_arr2.getString() << endl;
  REGR_TEST("PASS/FAIL: Passed the members of my_new_arr have been set to {0,0,200,300,0}");
  cout << endl;
  // Test at() automatic resize
  cout << "my_new_arr2.at(5) = 555" << endl;
  my_new_arr2.at(5) = 555;
  cout << "Array '"<< my_new_arr2.getName()<<"' = " << my_new_arr2.getString() << endl;
  cout << "my_new_arr2.at(9) = 555" << endl;
  my_new_arr2.at(9) = 999;
  cout << "Array '"<< my_new_arr2.getName()<<"' = " << my_new_arr2.getString() << endl;
  REGR_TEST("PASS/FAIL: Passed the members of my_new_arr have been set to {0,0,200,300,0,555,0,0,0,999}");
  cout << endl;
  
}

// Callback function with default signature.
void ArrayUserModule::config_callback(gs::gs_param_base& par) {
  cout << name() << ": Callback method called:" << endl;
  cout << "  Parameter '" << par.getName() << "'"<< endl
  << "    of type '" << par.getTypeString() << "'" << endl;
  
  if (par.is_destructing()) {
    cout << "    is just being destroyed!" << endl;
  } 
  
  else if (par.is_simple_array()) {
    cout << "    is a Simple Parameter Array with changed members (value '" << par.getString() << "')." << endl;
    gs::gs_param<int*>* arr_par = dynamic_cast<gs::gs_param<int*>* >(&par);
    if (arr_par != NULL) {
      cout << "    of type int*." << endl;
      cout << "    Register callbacks for all members." <<endl;
      // TODO: Use Iterator!
      for (unsigned int i=0; i < arr_par->size(); i++) {
        GC_REGISTER_PARAM_CALLBACK(&arr_par->at(i), ArrayUserModule, config_callback);
      }
    } else
      cout << "    of unknown type." << endl;
  }
  
  else {
    cout << "    changed to value '" << par.getString() << "'." << endl;
  }
  cout << endl;
}
