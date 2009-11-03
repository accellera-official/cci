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
// doxygen comments

#ifndef __AttributeTestsIP_H__
#define __AttributeTestsIP_H__

#include <systemc>

//#define GS_PARAM_VERBOSE

#include "../demo_globals.h"
#include "greencontrol/config.h"
#include "greencontrol/core/helpfunctions.h"

/// Test IP which tests parameter attributes
/**
 */
class AttributeTestsIP
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(AttributeTestsIP);
	
  /// Constructor
  AttributeTestsIP(sc_core::sc_module_name name)
    : sc_core::sc_module(name)
  { 
    SC_THREAD(main_action);
    mApi = gs::cnf::GCnf_Api::getApiInstance(this);
  }
  
  /// Destructor
  ~AttributeTestsIP() {
  }

  /// Main action to make tests with parameters.
  void main_action();

private:
  
  gs::cnf::cnf_api* mApi;
  
};


#endif

