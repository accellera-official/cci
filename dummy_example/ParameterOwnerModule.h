/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2009 by all Contributors.
  All Rights reserved.

  Developed by GreenSocs : http://www.greensocs.com/
   Christian Schroeder, schroeder@eis.cs.tu-bs.de
   Mark Burton, mark@greensocs.com

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/


#ifndef __PARAMETEROWNERMODULE_H__
#define __PARAMETEROWNERMODULE_H__


#include <systemc>
#include "ex_globals.h"
#include "cci.h"
#include "../dummy_implementation/gs_cci.h"


/// Module which owns some cci parameters.
class ParameterOwnerModule
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(ParameterOwnerModule);
	
  /// Constructor
  ParameterOwnerModule(sc_core::sc_module_name name)
    : sc_core::sc_module(name)
    , int_param ("int_param", 50 )
    , uint_param("uint_param", 12000)
  //, str_param ("str_param", "This is a test string.")
  { 
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests with parameters.
  void main_action();

  /// Example parameter.
  cci::gs_cci_param<int>             int_param;
  cci::gs_cci_param<unsigned int>    uint_param;
  //cci::gs_cci_param<std::string>     str_param;

};


#endif
