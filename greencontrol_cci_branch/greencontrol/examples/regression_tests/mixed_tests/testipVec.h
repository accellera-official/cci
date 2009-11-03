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

#ifndef __TESTIPVEC_H__
#define __TESTIPVEC_H__


#include "../demo_globals.h"
#include "greencontrol/config.h"


using std::string;


/// Test IP which tests vector parameters.
/**
 * This is a SystemC module which demonstrates the GreenControl parameter API.
 */
class TestIPVec
: public sc_core::sc_module
{
  
public:
  
  GC_HAS_CALLBACKS();

  SC_HAS_PROCESS(TestIPVec);
	
  /// Constructor
  TestIPVec(sc_core::sc_module_name name)
  : sc_core::sc_module(name)
  , vec1 ("vec1" )
  , vec2 ("vec2" )
  , vec3 ("vec3" )
  , vec4 ("vec4" )
  , vec5 ("vec5" )
  { 
    SC_THREAD(main_action);
  }
  
  /// Destructor
  ~TestIPVec() {
    GC_UNREGISTER_CALLBACKS();
  }

  /// Main action to make tests with parameters.
  void main_action();

  /// Example parameter.
  gs::gs_param< std::vector<std::string> >    vec1;
  gs::gs_param< std::vector<std::string> >    vec2;
  gs::gs_param< std::vector<std::string> >    vec3;
  gs::gs_param< std::vector<std::string> >    vec4;
  gs::gs_param< std::vector<std::string> >    vec5;

  // Callback function for changed parameters
  void config_callback(gs::gs_param_base& par);

  // Callback funtion to be called when new parameters are added or set implicitely
  void new_param_callback(const std::string parname, const std::string value);

};


#endif

