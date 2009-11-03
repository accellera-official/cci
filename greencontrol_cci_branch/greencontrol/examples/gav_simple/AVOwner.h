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

#ifndef __AVOwner_h__
#define __AVOwner_h__

#include "simple_analysis_globals.h"
#include "greencontrol/config.h"
#include "greencontrol/gav/apis/gav_api/GAV_Api.h"


/// Module which owns some gs_params.
class AVOwner
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(AVOwner);
	
  /// Constructor
  AVOwner(sc_core::sc_module_name name)
    : sc_core::sc_module(name),
      int_param ("int_param", 50 ),
      str_param ("str_param", "This is a test string."),
      uint_param("uint_param", 12000),
      int_param1("int_param1", 1),
      int_param2("int_param2", 2),
      int_param3("int_param3", 3),
      int_param4("sub.int_param4", 4),
      int_param5("sub.int_param5", 5)
  { 
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests with parameters.
  void main_action();

  /// Example parameter.
  gs::gs_param<int>             int_param;
  gs::gs_param<std::string>     str_param;
  gs::gs_param<unsigned int>    uint_param;
  gs::gs_param<int>             int_param1;
  gs::gs_param<int>             int_param2;
  gs::gs_param<int>             int_param3;
  gs::gs_param<int>             int_param4;
  gs::gs_param<int>             int_param5;

  gs::av::GAV_Api m_GAV_Api;
};


#endif

