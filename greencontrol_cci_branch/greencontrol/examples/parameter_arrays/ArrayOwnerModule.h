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

#ifndef __ArrayOwnerModule_H__
#define __ArrayOwnerModule_H__

#include <systemc>

#include "globals.h"

#include "greencontrol/config.h"


using std::string;


/// Module which owns some parameter arrays.
class ArrayOwnerModule
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(ArrayOwnerModule);
	
  /// Constructor
  ArrayOwnerModule(sc_core::sc_module_name name)
    : sc_core::sc_module(name),
      normal_uint_param("normal_uint_param"),
      int_arr("int_arr"),
      string_arr("string_arr"),
      uint_arr("uint_arr")
      //,float_arr("float_arr")
  { 
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests with parameters.
  void main_action();

  /// Example Simple Parameter Arrays.
  gs::gs_param<unsigned int>  normal_uint_param;
  gs::gs_param<int*>     int_arr;
  gs::gs_param<string*>  string_arr;
  gs::gs_param<unsigned int*>    uint_arr;
  //gs::gs_param<float*>   float_arr;
  
};


#endif

