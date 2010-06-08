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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

#ifndef __GS_DEBUG_STREAM_H__
#define __GS_DEBUG_STREAM_H__

#include "report_stream.h"

BEGIN_GS_REPORTMSG_NAMESPACE


/// Debug stream object to report debug messages
/**
 * This object may be instantiated by a user to report messages
 * of a chosen verbosity level.
 */
class gs_debug_stream 
: public report_stream
{
public:
  /// Constructor
  gs_debug_stream(const char* id, debug_msg_level msg_level)
  : report_stream(id, msg_level)
  {
  }
  
};


END_GS_REPORTMSG_NAMESPACE

#endif
