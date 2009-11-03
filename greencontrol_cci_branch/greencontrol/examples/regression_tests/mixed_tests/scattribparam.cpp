//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2008 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
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


#include "scattribparam.h"

#define IPsc_attribTest(a) std::cout << "IP sc_attrib test: " << a <<std::endl
#define SPACE std::cout << std::endl

void Param_test_sc_attribute::main_action() {
  wait(5, sc_core::SC_SEC);
  
  IPsc_attribTest("Default values: ");
  IPsc_attribTest(" attrib_paramInt " << attrib_paramInt.value); // -100
  IPsc_attribTest(" attrib_paramUInt " << attrib_paramUInt.value); // 100
  IPsc_attribTest(" attrib_paramDouble " << attrib_paramDouble.value); // 10.1
  IPsc_attribTest("attrib_paramString " << attrib_paramString.value); // "Init test string"
  SPACE;

  sc_core::sc_attribute<unsigned int> uintattr = attrib_paramUInt;
  IPsc_attribTest("uintattr = " << uintattr.value);
  SPACE;

  IPsc_attribTest("Set attrib_paramInt.value = 5");
  attrib_paramInt.value = 5;
  IPsc_attribTest("    attrib_paramInt.value = " << attrib_paramInt.value);
  SPACE;

  double d = attrib_paramDouble.value;
  IPsc_attribTest("attrib_paramDouble.value = " << d);
  SPACE;

  sc_core::sc_attribute<std::string> stringattr = attrib_paramString;
  IPsc_attribTest("stringattr = " << stringattr.value);
  SPACE;
  
  IPsc_attribTest("Set attrib_paramString.value = 'Hello'");
  attrib_paramString.value = "Hello";
  IPsc_attribTest("    attrib_paramString.value = " << attrib_paramString.value);
  SPACE;
  
  std::string s = attrib_paramString.value;
  IPsc_attribTest("attrib_paramString.value = " << s);

  SPACE;
  SPACE;
};
