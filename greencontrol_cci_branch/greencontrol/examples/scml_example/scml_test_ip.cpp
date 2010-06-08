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

#include "scml_test_ip.h"

void Scml_Test_IP::main_action() {
  
  std::cout << std::endl;
  DumpScmlTest("Get not existing parameter my.name");
  api->getParam("my.name");

  //DumpScmlTest("Set not existing parameter my.notExistingParam");
  //api->setParam("my.notExistingParam", "HelloWorld");


#ifndef USE_SCML_DATABASE_ADAPTER
  
  // The following setting take only effect if the GreenControl
  // scml_property class is used with a database which supports setting.
  
  std::cout << std::endl;

  DumpScmlTest("Set scml_property parameter 'ScmlOriginalPropertyIP.m_string_prop' = 'value written to registry'");
  api->setParam("ScmlOriginalPropertyIP.m_string_prop", "value written to registry");
  
  DumpScmlTest("Set scml_property parameter 'ScmlOriginalPropertyIP.m_int_prop' = 1");
  api->setParam("ScmlOriginalPropertyIP.m_int_prop", "1");

  DumpScmlTest("Set scml_property parameter 'ScmlOriginalPropertyIP.m_bool_prop' = 1");
  api->setParam("ScmlOriginalPropertyIP.m_bool_prop", "1");

  DumpScmlTest("Set scml_property parameter 'ScmlOriginalPropertyIP.m_double_prop' = 1");
  api->setParam("ScmlOriginalPropertyIP.m_double_prop", "1");

#endif

}
