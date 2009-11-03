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

#ifndef __TESTIP_H__
#define __TESTIP_H__

#include <systemc>

#include "greencontrol/gcnf/apis/scmlApi/scml_property.h"
#include "testip2.h"


/// Test IP which uses the SCML API.
/**
 * This is a SystemC module which demonstrates that the CoWare Scml-API
 * may be used with an underlying GreenConfig library.
 */
class TestIP
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(TestIP);
	
  /// Cosntructor
  TestIP(sc_core::sc_module_name name)
    : sc_core::sc_module(name),
      scmlIntParam("scmlIntParam", 1000),
      scmlStringParam("scmlStringParam"),
      sub_ip2("SUB_IP_2")
  { 
    SC_THREAD(main_action);
    //scml_property<int> *tmp_prop = new scml_property<int>(100);
  }
  
  /// Destructor
  ~TestIP() {
  }

  /// Main action to make tests with parameters.
  void main_action();

  /// Example scml int parameter.
  scml_property<int> scmlIntParam;
  /// Example scml string parameter.
  scml_property<std::string> scmlStringParam;

  /// Sub-module
  TestIP2 sub_ip2;

};


#endif

