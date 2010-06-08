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


#ifndef __REPMSG_DATATYPES_H__
#define __REPMSG_DATATYPES_H__


BEGIN_GS_REPORTMSG_NAMESPACE


/// to be streamed into the debug stream to end the message
enum endmessage_type {
  no_endmessage = 0,
  endmessage = 1
};


/// to streame the filename into the debug stream to end the message
struct file_and_line_type {
  std::string filenam;
  unsigned int line;
  file_and_line_type(const char* n, unsigned int l) : filenam(n), line(l) { }
};


/// message level for system and debug message
enum debug_msg_level {
  no_dbg_msg_level = 99,
  dbg_msg_L0 = 0,
  dbg_msg_L1 = 1,
  dbg_msg_L2 = 2,
  dbg_msg_L3 = 3,
  dbg_msg_L4 = 4,
  dbg_msg_L5 = 5,
  dbg_msg_L6 = 6,
  dbg_msg_L7 = 7,
  dbg_msg_L8 = 8,
  dbg_msg_L9 = 9
};


/// message level for system and debug message
enum system_msg_level {
  no_sys_msg_level = 89,
  sys_msg_INFO = 0,
  sys_msg_WARNING = 1,
  sys_msg_ERROR = 2,
  sys_msg_FATAL = 3
};


/// internal message level for system and debug message
enum combined_message_level {
  int_no_message_level = 99, // = no_dbg_msg_level = no_sys_msg_level+10
  int_dbg_msg_L0 = 0,  // = dbg_msg_Lx
  int_dbg_msg_L1 = 1,
  int_dbg_msg_L2 = 2,
  int_dbg_msg_L3 = 3,
  int_dbg_msg_L4 = 4,
  int_dbg_msg_L5 = 5,
  int_dbg_msg_L6 = 6,
  int_dbg_msg_L7 = 7,
  int_dbg_msg_L8 = 8,
  int_dbg_msg_L9 = 9,
  int_sys_msg_INFO = 10,    // = sys_msg_INFO    + 10
  int_sys_msg_WARNING = 11, // = sys_msg_WARNING + 10
  int_sys_msg_ERROR = 12,   // = sys_msg_ERROR   + 10
  int_sys_msg_FATAL = 13    // = sys_msg_FATAL   + 10
};


/// Returns the string representation of the given message level
inline std::string message_level_to_string(int l) {
  std::string ret;
  switch (l) {
    case 99://no_message_level:
      ret = "no_message_level"; break;
    case 0://dbg_msg_L0:
      ret = "dbg_msg_L0"; break;
    case 1://dbg_msg_L1:
      ret = "dbg_msg_L1"; break;
    case 2://dbg_msg_L2:
      ret = "dbg_msg_L2"; break;
    case 3://dbg_msg_L3:
      ret = "dbg_msg_L3"; break;
    case 4://dbg_msg_L4:
      ret = "dbg_msg_L4"; break;
    case 5://dbg_msg_L5:
      ret = "dbg_msg_L5"; break;
    case 6://dbg_msg_L6:
      ret = "dbg_msg_L6"; break;
    case 7://dbg_msg_L7:
      ret = "dbg_msg_L7"; break;
    case 8://dbg_msg_L8:
      ret = "dbg_msg_L8"; break;
    case 9://dbg_msg_L9:
      ret = "dbg_msg_L9"; break;
    case 10://sys_msg_INFO:
      ret = "sys_msg_INFO"; break;
    case 11://sys_msg_WARNING:
      ret = "sys_msg_WARNING"; break;
    case 12://sys_msg_ERROR:
      ret = "sys_msg_ERROR"; break;
    case 13://sys_msg_FATAL:
      ret = "sys_msg_FATAL"; break;
    default:
      assert(false); exit(1);
  }
  return ret;
}

/// Returns the string representation of the given message level
inline std::string debug_msg_level_to_string(debug_msg_level l) {
  return message_level_to_string((int)l);
}

/// Returns the string representation of the given message level
inline std::string system_msg_level_to_string(system_msg_level l) {
  return message_level_to_string((int)l);
}

/// Struct to be used by user to call apply_configuration
struct msg_configuration {
  // filename or special output
  std::string msgconfig_name; 
  // time options
  bool msgconfig_starttime_en; // if start-/end-time enabled
  sc_core::sc_time msgconfig_starttime;
  sc_core::sc_time msgconfig_endtime;
  // debug level
  debug_msg_level msgconfig_dbglvl;  // max. debug level
  // system level
  bool msgconfig_info_en;
  bool msgconfig_warn_en;
  bool msgconfig_error_en;
  bool msgconfig_fatal_en;
  // output options
  bool msgconfig_printtime;
  bool msgconfig_printname;
  bool msgconfig_printfile;
  bool msgconfig_printlevel;
  // lists of modules/streams to be captured (all if empty)
  std::vector<std::string> msgconfig_module_id;
  
  msg_configuration() 
  : msgconfig_name("")
  , msgconfig_starttime_en(false)
  , msgconfig_starttime(sc_core::SC_ZERO_TIME)
  , msgconfig_endtime(sc_core::SC_ZERO_TIME)
  , msgconfig_dbglvl(no_dbg_msg_level)
  , msgconfig_info_en(false)
  , msgconfig_warn_en(false)
  , msgconfig_error_en(false)
  , msgconfig_fatal_en(false)
  , msgconfig_printtime(true)
  , msgconfig_printname(true)
  , msgconfig_printfile(false)
  , msgconfig_printlevel(false)
  {  }
};


END_GS_REPORTMSG_NAMESPACE


#endif 
