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



#ifndef __gav_globals_h__
#define __gav_globals_h__

#include "greencontrol/core/gc_globals.h"


namespace gs {
namespace av {
      
using namespace ctr;

#ifndef SC_INCLUDE_DYNAMIC_PROCESSES
#  define SC_INCLUDE_DYNAMIC_PROCESSES
#endif
  
  // ////////////////// BEGIN User defines /////////////////////////////// //

  
  // ////////////////// END User defines ///////////////////////////////// //

// Output of the parameter objects
#ifdef GAV_VERBOSE
# include <stdio.h>
//# define GAV_TRACE(name, msg, ...) { printf("@%s /%d (%s): ", sc_core::sc_time_stamp().to_string().c_str(), (unsigned)sc_core::sc_delta_count(), name); printf(msg, ##__VA_ARGS__); printf("\n"); } 
# define GAV_DUMP_N(name, msg)     { std::cout<<"@"<<sc_core::sc_time_stamp().to_string()<<" /"<<(unsigned)sc_core::sc_delta_count()<<" ("<<name<<"): " <<msg<<std::endl; } 
# define GAV_PRINT_SPACE           { std::cout << "." << std::endl; }
# define IF_GAV_VERBOSE(arg) arg
#else
//# define GAV_TRACE(name, msg, ...)
# define GAV_DUMP_N(name, msg)
# define GAV_PRINT_SPACE
# define IF_GAV_VERBOSE(arg)
#endif
  
} // end namespace av
} // end namespace gs

#endif
