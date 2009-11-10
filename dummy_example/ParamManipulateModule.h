// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//    Mark Burton, mark@greensocs.com
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



#ifndef __PARAMMANIPULATEMODULE_H__
#define __PARAMMANIPULATEMODULE_H__


#include <systemc>
#include "ex_globals.h"
#include "cci.h"
#include "../dummy_implementation/dummy_cci.h"


/// Module which changes parameter values of the Module ParameterOwnerModule
class ParamManipulateModule
: public sc_core::sc_module
{
public:

  SC_HAS_PROCESS(ParamManipulateModule);
  ParamManipulateModule(sc_core::sc_module_name name);
  
  /// Main action to make tests with parameters.
  void main_action();

protected:
  /// Pointer the the module's configuration API
  cci::cci_cnf_api* mApi;

};


#endif
