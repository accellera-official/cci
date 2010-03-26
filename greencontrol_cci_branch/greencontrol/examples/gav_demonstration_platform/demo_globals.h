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

#ifndef __demo_globals_h__
#define __demo_globals_h__

// Must be included BEFORE SystemC because SystemC 2.1
// has a buggy boost implementation included!!!!
#include <boost/tokenizer.hpp> // for parameter array!

#ifndef SC_INCLUDE_DYNAMIC_PROCESSES
#  define SC_INCLUDE_DYNAMIC_PROCESSES
#endif
#include <systemc>

#ifndef USE_STATIC_CASTS
#  define USE_STATIC_CASTS
#endif

#define GAV_ENABLED // Enable GreenAV for GreenBus SimpleBus implementation for transactions per second view

// ////////////// User defines: ///////////////////////////////// //

#define DEMO_VERBOSE
#define LRU_VERBOSE
//#define GAV_VERBOSE

#define NO_DEPRECATED_WARNINGS

// ///////////////////////////////////////////////////////////// //

#ifdef DEMO_VERBOSE
# define DEMO_TRACE(name, msg, ...) { printf("@%s /%d (%s): ", sc_core::sc_time_stamp().to_string().c_str(), (unsigned)sc_core::sc_delta_count(), name); printf(msg, ##__VA_ARGS__); printf("\n"); } 
#else
# define DEMO_TRACE(name, msg, ...)
#endif

#include "greencontrol/core/transport/gs_datatypes.h"
using tlm::gs_int8;
using tlm::gs_int16;
using tlm::gs_int32;
using tlm::gs_int64;
using tlm::gs_uint8;
using tlm::gs_uint16;
using tlm::gs_uint32;
using tlm::gs_uint64;


#endif
