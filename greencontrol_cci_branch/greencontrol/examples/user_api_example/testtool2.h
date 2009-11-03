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
