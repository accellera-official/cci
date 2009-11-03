//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Marcus Bartholomeu <bartho@greensocs.com>
//     GreenSocs
//     http://www.greensocs.com
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

#ifndef __TEST_IP_H__
#define __TEST_IP_H__

#include "greencontrol/config.h"


/// Test IP which uses the lua config reader
class Test_IP
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(Test_IP);
	
  /// Constructor
  Test_IP(sc_core::sc_module_name name)
    : sc_core::sc_module(name), param1("param1", 1000), param2("param2", "hello")
  { 
    api = gs::cnf::GCnf_Api::getApiInstance(this);
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests.
  void main_action();

private:
  gs::cnf::cnf_api *api;

  gs::gs_param<unsigned int> param1;
  gs::gs_param<std::string> param2;
  
};


#endif

