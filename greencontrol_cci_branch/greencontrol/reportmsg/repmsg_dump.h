// LICENSETEXT
// 
//   Copyright (C) 2008 : GreenSocs Ltd
//       http://www.greensocs.com/ , email: info@greensocs.com
// 
//   Developed by :
// 
//   Christian Schröder
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
// 
// 
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

#ifndef __REPMSG_DUMP_h__
#define __REPMSG_DUMP_h__

#ifdef MSG_VERBOSE
# define REPMSG_DUMP(msg)         std::cout<<"@"<<sc_core::sc_time_stamp()<<" /"<<(unsigned)sc_core::sc_delta_count()<<" ("<<name()<<"): "<<msg<<std::endl
# define REPMSG_DUMP_N(name, msg) std::cout<<"@"<<sc_core::sc_time_stamp()<<" /"<<(unsigned)sc_core::sc_delta_count()<<" ("<<name  <<"): "<<msg<<std::endl
#else
# define REPMSG_DUMP(msg)
# define REPMSG_DUMP_N(name, msg)
#endif

#endif
