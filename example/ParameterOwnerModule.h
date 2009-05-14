//   OSCI CCI WG
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// 
// ENDLICENSETEXT

#ifndef __ParameterOwnerModule_H__
#define __ParameterOwnerModule_H__

#include <systemc>
#include "ex_globals.h"
#include "cci.h"
#include "../gs_implementation/gs_cci.h"


/// Module which owns some gs_params.
class ParameterOwnerModule
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(ParameterOwnerModule);
	
  /// Constructor
  ParameterOwnerModule(sc_core::sc_module_name name)
    : sc_core::sc_module(name)
      ,int_param ("int_param", 50 )
      // TODO ,str_param ("str_param", "This is a test string.")
      //,uint_param("uint_param", 12000)
  { 
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests with parameters.
  void main_action();

  /// Example parameter.
  cci::gs_cci_param<int>             int_param;
  // TODO cci::cci_param<std::string>     str_param;
  //cci::cci_param<unsigned int>    uint_param;

};


#endif

