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

#ifndef __GS_SYSTEM_STREAM_H__
#define __GS_SYSTEM_STREAM_H__

#include "report_stream.h"

BEGIN_GS_REPORTMSG_NAMESPACE


/// System stream object to report system messages
/**
 * This object may be instantiated by a user to report messages
 * of a chosen severity level.
 */
class gs_system_stream 
: public report_stream
{
public:
  /// Constructor
  gs_system_stream(const char* id, system_msg_level msg_level)
  : report_stream(id, msg_level)
  {
  }
  
};


END_GS_REPORTMSG_NAMESPACE

#endif
