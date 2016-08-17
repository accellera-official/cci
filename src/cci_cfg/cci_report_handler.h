/*****************************************************************************
  Copyright 2016 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2011 GreenSocs
  All rights reserved.
  Authors:
      Christian Schroeder <schroeder@eis.cs.tu-bs.de>

  Copyright 2016 Ericsson
  All rights reserved.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 *****************************************************************************/

#ifndef CCI_CCI_REPORT_HANDLER_H_INCLUDED_
#define CCI_CCI_REPORT_HANDLER_H_INCLUDED_

#include "cci_cfg/cci_config_macros.h"

#include "cci_core/systemc.h"
#include <string>

CCI_OPEN_NAMESPACE_

enum cci_param_failure {
  CCI_NOT_FAILURE = 0,
  CCI_SET_PARAM_FAILURE,
  CCI_GET_PARAM_FAILURE,
  CCI_ADD_PARAM_FAILURE,
  CCI_REMOVE_PARAM_FAILURE,
  CCI_VALUE_FAILURE,
  CCI_UNDEFINED_FAILURE
};


class cci_report_handler: public sc_core::sc_report_handler {

public:
  static void report(sc_core::sc_severity severity, const char* msg_type, const char* msg, const char* file, int line) {
    std::string cci_msg_type = __CCI_SC_REPORT_MSG_TYPE_PREFIX__;
    cci_msg_type.append(msg_type);
    if(severity >= sc_core::SC_ERROR) {
      // set the actions for this CCI message to SC_THROW
      // the report should be cached so that it persists until it is caught
      set_actions(cci_msg_type.c_str(), severity,
                  sc_core::SC_THROW + sc_core::SC_CACHE_REPORT);
    }
    sc_report_handler::report(severity,cci_msg_type.c_str(),msg,file,line);
  }

  //functions that throw a report for each cci_param_failure type 
  static void set_param_failed(const char* msg="", const char* file=NULL, int line = 0) {
    report(sc_core::SC_ERROR,"SET_PARAM_FAILED",msg,file,line);
  }

  static void get_param_failed(const char* msg="", const char* file=NULL, int line = 0) {
    report(sc_core::SC_ERROR,"GET_PARAM_FAILED",msg,file,line);
  }

  static void add_param_failed(const char* msg="", const char* file=NULL, int line = 0) {
    report(sc_core::SC_ERROR,"ADD_PARAM_FAILED",msg,file,line);
  }

  static void remove_param_failed(const char* msg="", const char* file=NULL, int line = 0) {
    report(sc_core::SC_ERROR,"REMOVE_PARAM_FAILED",msg,file,line);
  }

  static void cci_value_failure(const char* msg="", const char* file=NULL, int line = 0) {
    report(sc_core::SC_ERROR,"CCI_VALUE_FAILURE",msg,file,line);
  }

  // function to return cci_param_failure that matches thrown (or cached) report
  static cci_param_failure get_param_failure(const sc_core::sc_report& rpt) {
    const std::string cci_sc_report_type_prefix =
            __CCI_SC_REPORT_MSG_TYPE_PREFIX__;
    std::string failure_type_string = rpt.get_msg_type();

    if (failure_type_string.compare(0, cci_sc_report_type_prefix.length(),
                                    cci_sc_report_type_prefix) == 0) {
      std::string failure_type_string_wp =
              failure_type_string.substr(cci_sc_report_type_prefix.length(),
                                         failure_type_string.length());

      if (failure_type_string_wp == "SET_PARAM_FAILED")
        return CCI_SET_PARAM_FAILURE;
      else if (failure_type_string_wp == "GET_PARAM_FAILED")
        return CCI_GET_PARAM_FAILURE;
      else if (failure_type_string_wp == "ADD_PARAM_FAILED")
        return CCI_ADD_PARAM_FAILURE;
      else if (failure_type_string_wp == "REMOVE_PARAM_FAILED")
        return CCI_REMOVE_PARAM_FAILURE;
      else if (failure_type_string_wp == "CCI_VALUE_FAILURE")
        return CCI_VALUE_FAILURE;
      else return CCI_UNDEFINED_FAILURE;
    }
    else //not a CCI failure report
      return CCI_NOT_FAILURE;
  }

};

#define CCI_REPORT_INFO(_id, _message) \
cci_report_handler::report(sc_core::SC_INFO,_id,_message,__FILE__,__LINE__);

#define CCI_REPORT_WARNING(_id, _message) \
cci_report_handler::report(sc_core::SC_WARNING,_id,_message,__FILE__,__LINE__);

#define CCI_REPORT_ERROR(_id, _message) \
cci_report_handler::report(sc_core::SC_ERROR,_id,_message,__FILE__,__LINE__);

#define CCI_REPORT_FATAL(_id, _message) \
cci_report_handler::report(sc_core::SC_FATAL,_id,_message,__FILE__,__LINE__);


CCI_CLOSE_NAMESPACE_

#endif // CCI_CCI_REPORT_HANDLER_H_INCLUDED_
