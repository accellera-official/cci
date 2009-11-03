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

#include "testtool2.h"

#include "../demonstration_platform/demo_globals.h" // for DEMO_TRACE(...)
#include "greencontrol/core/helpfunctions.h"

void TestTool2::main_action() {

  DEMO_TRACE(name(), "This is the main action in TestTool2.");


  // get parameter list
  std::vector<std::string> list = toolApi.getParamList();
  gs::ctr::show_vector(list);

  // get parameter
  DEMO_TRACE(name(), "IP4.int_param = %s", toolApi.getParam("IP4.int_param" ) );
  
  // set parameter
  toolApi.setParam("IP4.int_param", "1200" );
  DEMO_TRACE(name(), "set IP4.int_param = 1200 => %s", toolApi.getParam("IP4.int_param" ) );

  // get parameter
  DEMO_TRACE(name(), "IP4.int_param = %s", toolApi.getParam("IP4.str_param" ) );
  
  // set parameter
  toolApi.setParam("IP4.str_param", "Another test string." );
  DEMO_TRACE(name(), "set IP4.str_param = 'Another test string.' => %s", toolApi.getParam("IP4.str_param" ) );

  // get parameter
  DEMO_TRACE(name(), "IP4.int_param = %s", toolApi.getParam("IP4.uint_param" ) );
  
  // set parameter
  toolApi.setParam("IP4.uint_param", "1280003699" );
  DEMO_TRACE(name(), "set IP4.str_param = 1280003699 => %s", toolApi.getParam("IP4.uint_param" ) );

  // get parameter list
  list = toolApi.getParamList();
  gs::ctr::show_vector(list);

}
