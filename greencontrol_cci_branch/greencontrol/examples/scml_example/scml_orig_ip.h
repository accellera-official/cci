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
//         Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>

// doxygen comments

#ifndef __SCML_ORIG_IP_H__
#define __SCML_ORIG_IP_H__

#include <boost/tokenizer.hpp> // Because of buggy boost integrated in SystemC 2.1
#include <systemc>

// Here is included either the CoWare scml or the GreenSocs scml file
// The CoWare scml.h file is part of the free downloadable CoWare package scml 1.1.
//#include <scml.h> // CoWare scml
#include "greencontrol/gcnf/apis/scmlApi/scml.h" // GreenSocs scml


#include <string>
using std::string;

/// Test IP which uses original scml properties.
/**
 * This is a SystemC module which tests the scml framework.
 */
class Scml_Orig_IP
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(Scml_Orig_IP);
	
  /// Constructor
  Scml_Orig_IP(sc_core::sc_module_name name)
    : sc_core::sc_module(name),
      intProp("intProp"),
      boolProp("boolProp"),
      doubleProp("doubleProp"),
      stringProp("stringProp")
  { 
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests.
  void main_action();

  scml_property<int>    intProp;
  scml_property<bool>   boolProp;
  scml_property<double> doubleProp;
  scml_property<string> stringProp;

};


#endif

