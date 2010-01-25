//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2010 : GreenSocs Ltd
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

#ifndef __TEST_LOCK_IP_H__
#define __TEST_LOCK_IP_H__

#define SC_INCLUDE_FX
#include <systemc>

//#define GS_PARAM_VERBOSE
#include "greencontrol/config.h"

using std::string;


/// Test IP which uses the gs_param API and tests the lock.
class Test_Lock_IP
: public sc_core::sc_module
{
  
public:
  
  GC_HAS_CALLBACKS();

  SC_HAS_PROCESS(Test_Lock_IP);
	
  /// Constructor
  Test_Lock_IP(sc_core::sc_module_name name)
  : sc_core::sc_module(name)
  , int_param ("int_param" )
  , str_param ("str_param", "50")
  , str_param2("str_param2")
  { 
    SC_THREAD(main_action);
  }
  
  /// Destructor
  ~Test_Lock_IP() {
    GC_UNREGISTER_CALLBACKS();
  }

  /// Main action to make tests with parameters.
  void main_action();

  /// Example parameter.
  gs::gs_param<int>             int_param;
  gs::gs_param<std::string>     str_param;
  gs::gs_param<std::string>     str_param2;

};


#endif

