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
