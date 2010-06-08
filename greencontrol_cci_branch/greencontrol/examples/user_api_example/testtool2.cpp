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
