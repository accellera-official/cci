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

#ifndef __REPORTMSG_GLOBALS_H__
#define __REPORTMSG_GLOBALS_H__

// should be defined by top-level
//#define GS_MSG_DEBUG_EN 0 // disables report
//#define GS_MSG_DEBUG_EN 1 // enables  report

// namespace

#define BEGIN_GS_REPORTMSG_NAMESPACE \
  namespace gs { \
    namespace report {

#define END_GS_REPORTMSG_NAMESPACE \
    } \
  }

#define GS_REPORTMSG_NAMESPACE \
  gs::report


/// End message macro: streams the filename, line number and endline to the debug stream
#define GS_END_MSG \
GS_REPORTMSG_NAMESPACE::file_and_line_type(__FILE__, __LINE__) << GS_REPORTMSG_NAMESPACE::endmessage


#ifndef GS_MSG_DEBUG_EN
# warning You should define GS_MSG_DEBUG_EN
# define GS_MSG_DEBUG_EN 0
#endif

// TODO: this is not beautiful because of compiler warnings!
/// Debug enable macro
#define GS_MSG_DEBUG(ds) GS_MSG_DEBUG_EN && ds


#endif
