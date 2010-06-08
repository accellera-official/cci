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

#include "EArrayOwnerModule.h"

using namespace std;
using namespace sc_core;

void EArrayOwnerModule::main_action() {

  cout << endl << "EArrayOwnerModule action" << endl;

  cout << "Array '"<< myTopArr.getName() << "' = " << myTopArr.getString() << endl;
  
  // Wait for the EArrayUserModule to register the callbacks
  wait(2, SC_NS);
  
  *my2ndArray0_myIntPar0 = 444;
  *my2ndArray0_myIntPar1 = 555;
  *my2ndArray1_myStringPar = "HELLO NEW WORLD!";
  
  // Wait to preserve the array instance!
  wait(20, SC_NS);
  // Now the parameter arrray will be deleted!
}
