/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 ****************************************************************************/

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
  SC_REPORT_##type(name(), ss.str().c_str()); \
} while (0)

#define XREPORT_INFO(msg_stream)    XREPORT_BASE(INFO, msg_stream)
#define XREPORT_WARNING(msg_stream) XREPORT_BASE(WARNING, msg_stream)
#define XREPORT_ERROR(msg_stream)   XREPORT_BASE(ERROR, msg_stream)
#define XREPORT_FATAL(msg_stream)   XREPORT_BASE(FATAL, msg_stream)
#define XREPORT(msg_stream)         XREPORT_INFO(msg_stream)
#define XREPORT_PLAIN(msg_stream)   XREPORT_PLAIN_BASE(INFO, msg_stream)

#endif  // INC_XREPORT_HPP
