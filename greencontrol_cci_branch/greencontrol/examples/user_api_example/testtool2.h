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

#ifndef __TESTTOOL2_H__
#define __TESTTOOL2_H__

#include <systemc>

#include <sstream>

#include "greencontrol/core/gc_globals.h"        // global settings
#include "greencontrol/gcnf/plugin/config_callbadapt.h"

#include "tool_api/tool_api.h"


/// Test tool which uses the user defined API adaptor Tool_Api.
/**
 * Test tool which uses the user defined Tool_Api to set, get and list parameters.
 */
class TestTool2
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(TestTool2);
	
  /// Constructor
  TestTool2(sc_core::sc_module_name name)
    : sc_core::sc_module(name)
  { 
    SC_THREAD(main_action);
  }
  
  /// SC_THREAD for main action.
  void main_action();

private:

  /// Tool API which is used by this test tool
  my_tool_api::Tool_Api toolApi;

};


#endif
