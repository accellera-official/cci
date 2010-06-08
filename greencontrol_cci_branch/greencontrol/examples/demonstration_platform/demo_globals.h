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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

#ifndef __demo_globals_h__
#define __demo_globals_h__

#include <boost/config.hpp> // needed for SystemC 2.1 issue with ulong_long
#include <systemc>

#define NO_DEPRECATED_WARNINGS

//#define GCNF_VERBOSE // TODO

#define DEMO_VERBOSE

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
