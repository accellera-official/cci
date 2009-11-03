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

// doxygen comments

#ifndef __EArrayOwnerModule_H__
#define __EArrayOwnerModule_H__

#include <systemc>

#include "globals.h"

#include "greencontrol/config.h"


/// Module which owns extended parameter arrays.
class EArrayOwnerModule
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(EArrayOwnerModule);
	
  /// Constructor
  EArrayOwnerModule(sc_core::sc_module_name name)
    : sc_core::sc_module(name),
      myTopArr("myTopArr")
  { 
    my2ndArray0 = new gs::gs_param_array("my2ndArray0", &myTopArr);
    my2ndArray0_myIntPar0   = new gs::gs_param<int>        ("myIntPar0",   "10",     my2ndArray0);
    my2ndArray0_myIntPar1   = new gs::gs_param<int>        ("myIntPar1",   "12",     my2ndArray0);
    my2ndArray0_myStringPar = new gs::gs_param<std::string>("myStringPar", "Def0",   my2ndArray0);
    my2ndArray0_myUIntArrPar= new gs::gs_param<unsigned int*>("myUIntArrPar", my2ndArray0);
    my2ndArray0_myUIntArrPar2= new gs::gs_param<unsigned int*>("myUIntArrPar2", my2ndArray0);
    
    my2ndArray1 = new gs::gs_param_array("my2ndArray1", &myTopArr);
    my2ndArray1_myIntPar0   = new gs::gs_param<int>        ("myIntPar0",   "100",    my2ndArray1);
    my2ndArray1_myIntPar1   = new gs::gs_param<int>        ("myIntPar1",   "120",    my2ndArray1);
    my2ndArray1_myStringPar = new gs::gs_param<std::string>("myStringPar", "Def1",   my2ndArray1);
    
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests with parameters.
  void main_action();

  /// Example array parameters.
  gs::gs_param_array  myTopArr;
  gs::gs_param_array        *my2ndArray0;
  gs::gs_param<int>         *my2ndArray0_myIntPar0;
  gs::gs_param<int>         *my2ndArray0_myIntPar1;
  gs::gs_param<std::string> *my2ndArray0_myStringPar;
  gs::gs_param<unsigned int*>*my2ndArray0_myUIntArrPar;  // with default size in config file
  gs::gs_param<unsigned int*>*my2ndArray0_myUIntArrPar2; // without default size in config file
  
  gs::gs_param_array *my2ndArray1;
  gs::gs_param<int>         *my2ndArray1_myIntPar0;
  gs::gs_param<int>         *my2ndArray1_myIntPar1;
  gs::gs_param<std::string> *my2ndArray1_myStringPar;
  
};


#endif

