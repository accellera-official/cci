#ifndef INC_XREPORT_HPP
#define INC_XREPORT_HPP

#include <sstream>
#include "systemc"

/**
 * @def     Example Reporting Macro (Base)
 * @param   func_name: Function Name
 * @param   msg_stream: Any argument that can be presented to a string stream
 * @note    This macro provides a simple hack to support streaming
 *          operators to format message and to auto-prefix timestamp to messages
 */
#define XREPORT_BASE(type, msg_stream) \
  XREPORT_PLAIN_BASE(type, "@"<< sc_core::sc_time_stamp()<< ", "<< msg_stream)

#define XREPORT_PLAIN_BASE(type, msg_stream) do { \
  std::ostringstream ss; \
  ss<< msg_stream; \
  SC_REPORT_##type( name(), ss.str().c_str()); \
} while(0)

#define XREPORT_INFO(msg_stream)    XREPORT_BASE(INFO, msg_stream)
#define XREPORT_WARNING(msg_stream) XREPORT_BASE(WARNING, msg_stream)
#define XREPORT_ERROR(msg_stream)   XREPORT_BASE(ERROR, msg_stream)
#define XREPORT_FATAL(msg_stream)   XREPORT_BASE(FATAL, msg_stream)
#define XREPORT(msg_stream)         XREPORT_INFO(msg_stream)
#define XREPORT_PLAIN(msg_stream)   XREPORT_PLAIN_BASE(INFO, msg_stream)

#endif // INC_XREPORT_HPP

