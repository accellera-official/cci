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
