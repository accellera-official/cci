// LICENSETEXT
//
//   Copyright (C) 2011 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
//
// ENDLICENSETEXT

#ifndef CCI_CNF_CCI_REPORT_HANDLER_H_INCLUDED_
#define CCI_CNF_CCI_REPORT_HANDLER_H_INCLUDED_

#include "cci_core/systemc.h"
#include <string>

namespace cci {
namespace cnf {

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
    // set the actions for this CCI message to SC_THROW
    // the report should be cached so that it persists until it is caught
    set_actions(cci_msg_type.c_str(), severity, sc_core::SC_THROW + sc_core::SC_CACHE_REPORT);
    sc_report_handler::report(severity,cci_msg_type.c_str(),msg,file,line);
  }

  //functions that throw a report for each cci_param_failure type 
  static void set_param_failed(const char* msg="") {
    report(sc_core::SC_ERROR,"SET_PARAM_FAILED",msg,__FILE__,__LINE__);
  }

  static void get_param_failed(const char* msg="") {
    report(sc_core::SC_ERROR,"GET_PARAM_FAILED",msg,__FILE__,__LINE__);
  }

  static void add_param_failed(const char* msg="") {
    report(sc_core::SC_ERROR,"ADD_PARAM_FAILED",msg,__FILE__,__LINE__);
  }

  static void remove_param_failed(const char* msg="") {
    report(sc_core::SC_ERROR,"REMOVE_PARAM_FAILED",msg,__FILE__,__LINE__);
  }

  static void cci_value_failure(const char* msg="") {
    report(sc_core::SC_ERROR,"CCI_VALUE_FAILURE",msg,__FILE__,__LINE__);
  }

  // function to return cci_param_failure that matches thrown (or cached) report
  static cci_param_failure get_param_failure(const sc_core::sc_report& rpt) {
    std::string failure_type_string = rpt.get_msg_type();
    if ( failure_type_string.compare(0,9,__CCI_SC_REPORT_MSG_TYPE_PREFIX__) == 0) {
      if (failure_type_string == "/ASI/CCI/SET_PARAM_FAILED") return CCI_SET_PARAM_FAILURE;
      else if (failure_type_string == "/ASI/CCI/GET_PARAM_FAILED") return CCI_GET_PARAM_FAILURE;
      else if (failure_type_string == "/ASI/CCI/ADD_PARAM_FAILED") return CCI_ADD_PARAM_FAILURE;
      else if (failure_type_string == "/ASI/CCI/REMOVE_PARAM_FAILED") return CCI_REMOVE_PARAM_FAILURE;
      else if (failure_type_string == "/ASI/CCI/CCI_VALUE_FAILURE") return CCI_VALUE_FAILURE;
      else return CCI_UNDEFINED_FAILURE;
    }
    else //not a CCI failure report
      return CCI_NOT_FAILURE;
  }

};

#define CCI_REPORT_INFO(_id, _message) \
cci_report_handler::report(SC_INFO,_id,_message,__FILE__,__LINE__);

#define CCI_REPORT_WARNING(_id, _message) \
cci_report_handler::report(SC_WARNING,_id,_message,__FILE__,__LINE__);

#define CCI_REPORT_ERROR(_id, _message) \
cci_report_handler::report(SC_ERROR,_id,_message,__FILE__,__LINE__);

#define CCI_REPORT_FATAL(_id, _message) \
cci_report_handler::report(SC_FATAL,_id,_message,__FILE__,__LINE__);


} //namespace cnf
} //namespace cci

#endif // CCI_CNF_CCI_REPORT_HANDLER_H_INCLUDED_
