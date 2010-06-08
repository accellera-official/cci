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

