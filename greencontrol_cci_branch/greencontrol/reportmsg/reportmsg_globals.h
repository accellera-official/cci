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
