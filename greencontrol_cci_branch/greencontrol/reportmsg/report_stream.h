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

#ifndef __REPORT_STREAM_H__
#define __REPORT_STREAM_H__

#include "MessageStreamer.h"
#include <iostream>


BEGIN_GS_REPORTMSG_NAMESPACE


/// Report stream object
/**
 * Debug and system stream object derive from this class.
 */
class report_stream 
: public sc_core::sc_object
, public std::ostringstream
, public report_stream_if
{
protected:
  
  MessageStreamer* m_message_streamer;
  
  /// The name (id) of this debug stream
  std::string m_id;
  /// The sc module name of the parent
  std::string m_parent_name;
  
  /// The debug verbosity of this debug stream
  combined_message_level m_msg_level;
  
  /// if this stream is enabled
  bool enabled;
  
  /// the file where the message was initiated
  std::string m_message_filename;
  /// the line where the message was initiated
  unsigned int m_message_line;
  
public:
  
  /// Constructor
  report_stream(const char* id, debug_msg_level msg_level)
  : sc_core::sc_object(id)
  , std::ostringstream(std::ostringstream::out)
  , m_id(id)
  , enabled(false)
  { 
    m_msg_level = (combined_message_level) msg_level;
    init(id);
  }
  
  /// Constructor
  report_stream(const char* id, system_msg_level msg_level)
  : sc_core::sc_object(id)
  , std::ostringstream(std::ostringstream::out)
  , m_id(id)
  , enabled(false)
  { 
    m_msg_level = (combined_message_level)( (int)(msg_level + 10) );
    init(id);
  }
  
  ~report_stream() { }
  
  // ///////////////  Operators  ///////////////////// // 
  
  friend std::ostream& operator << (std::ostream &out, const file_and_line_type fl);
  friend std::ostream& operator << (std::ostream &out, const endmessage_type endm);
  
  // see below outside class
  //report_stream& operator << (const endmessage_type endm) { ...
  //  return *this; }
  //report_stream& operator << (const file_and_line_type fl) { ...
  //  return *this; }
  
  // //////////////  other functions  ///////////////// //
  
  /// En-/disables this stream (called by MessageStreamer)
  void enable(bool en = true) { enabled = en; }
  
  /// Returns the name if this stream
  const char* name() const {
    return sc_object::name();
  }
  
  /// Returns the combined (debug and system) message level of this stream.
  combined_message_level get_message_level() const {
    return m_msg_level;
  }

  /// Returns the debug message level of this stream.
  debug_msg_level get_debug_message_level() const {
    return (debug_msg_level)m_msg_level;
  }

  /// Returns the system message level of this stream.
  system_msg_level get_system_message_level() const {
    return (system_msg_level)(m_msg_level-10);
  }
  
  const char* get_parent_name() const {
    return m_parent_name.c_str();
  }

  const char* get_id() const {
    return m_id.c_str();
  }
  
protected:
  
  /// Called by the constructors
  inline void init(const char* id) {
    // test if the name of this object has not been changed (error if streams with same name!)
    if (std::string(basename()).compare(id) != 0) {
      SC_REPORT_ERROR(name(), "You created report streams with same name!");
    }
    sc_core::sc_object* obj = get_parent_object();
    if (obj) m_parent_name = obj->name();
    else m_parent_name = "";
    
    m_message_streamer = MessageStreamer::get_global_message_streamer();
    m_message_streamer->register_stream_object(*this);
  }
  
  
  /// Ends this message, called by operator <<
  void end_message() {
    if (enabled) {
      assert(!fail());
      REPMSG_DUMP("end message");
      // send stream
      m_message_streamer->out(m_parent_name.c_str(), m_id.c_str(), m_msg_level, 
                              str(), m_message_filename, m_message_line, m_parent_name);
    }
    // reset this stream
    clear();
    str("");
  }
  
};

/// Stream operator for end message
inline std::ostream& operator << (std::ostream &out, const GS_REPORTMSG_NAMESPACE::endmessage_type endm) {
  assert(endm == endmessage);
  GS_REPORTMSG_NAMESPACE::report_stream* dbgstr = dynamic_cast<GS_REPORTMSG_NAMESPACE::report_stream*> (&out);
  assert(dbgstr != NULL);
  dbgstr->end_message();
  //((report_stream&)out).end_message(); // works, too
  return out;
}

/// Stream operator for filename and line
inline std::ostream& operator << (std::ostream &out, const GS_REPORTMSG_NAMESPACE::file_and_line_type fl) {
  GS_REPORTMSG_NAMESPACE::report_stream* dbgstr = dynamic_cast<GS_REPORTMSG_NAMESPACE::report_stream*> (&out);
  assert(dbgstr != NULL);
  dbgstr->m_message_filename = fl.filenam;
  dbgstr->m_message_line = fl.line;
  //((report_stream&)out). ...; // works, too
  return out;
}


END_GS_REPORTMSG_NAMESPACE

#endif
