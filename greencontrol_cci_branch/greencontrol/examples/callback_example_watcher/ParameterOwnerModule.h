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

#ifndef __ParameterOwnerModule_H__
#define __ParameterOwnerModule_H__

#include <systemc>

#include "callb_globals.h"

#include "greencontrol/config.h"


using std::string;
using std::vector;

/// Module which owns some gs_params.
class ParameterOwnerModule
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(ParameterOwnerModule);
	
  /// Constructor
  ParameterOwnerModule(sc_core::sc_module_name name)
    : sc_core::sc_module(name),
      int_param ("int_param", 50 ),
      str_param ("str_param", "This is a test string."),
      uint_param("uint_param", 12000)
  { 
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests with parameters.
  void main_action();

  /// Example parameter.
  gs::gs_param<int>             int_param;
  gs::gs_param<std::string>     str_param;
  gs::gs_param<unsigned int>    uint_param;

};


#endif

