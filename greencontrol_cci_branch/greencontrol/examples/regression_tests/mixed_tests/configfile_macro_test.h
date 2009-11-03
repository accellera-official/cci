//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
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

#include <systemc>
#include "greencontrol/config.h"

/// Test module owning some parameters being set with config file macros
SC_MODULE(ConfigFileMacroMod)
{
  gs::gs_param<int> param1;
  gs::gs_param<int> param2;
  gs::gs_param<int> param3;
  gs::gs_param<int> param4;
  
  SC_HAS_PROCESS(ConfigFileMacroMod);
  ConfigFileMacroMod(sc_core::sc_module_name name)
  : sc_core::sc_module(name)
  , param1("param1")
  , param2("param2")
  , param3("param3")
  , param4("param4")
  {
    std::cout << std::endl << "ConfigFileMacroMod has been configured with these params:" << std::endl;
    std::cout << "   param1 = " << param1 << std::endl;    
    std::cout << "   param2 = " << param2 << std::endl;    
    std::cout << "   param3 = " << param3 << std::endl;    
    std::cout << "   param4 = " << param4 << std::endl;    
  }
  
};
