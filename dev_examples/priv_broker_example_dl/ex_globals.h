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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

#ifndef __callb_globals_h__
#define __callb_globals_h__

// Some debug output for GrenConfig wrapped to CCI
//#define GS_PARAM_CALLBACK_VERBOSE

#define DEMO_VERBOSE

#ifdef DEMO_VERBOSE
# define DEMO_DUMP(name, msg) { printf("@%s /%d (%s): ", sc_core::sc_time_stamp().to_string().c_str(), (unsigned)sc_core::sc_delta_count(), name); std::cout << msg; printf("\n"); } 
#else
# define DEMO_DUMP(name, msg)
#endif

#endif
