// LICENSETEXT
//
//   Copyright (C) 2010 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//    Mark Burton, mark@greensocs.com
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT



#ifndef __CCI_ERROR_TYPES_H__
#define __CCI_ERROR_TYPES_H__

#include <string>
#include <iostream>


#define CCI_SC_REPORT_MSG_TYPE_PREFIX "/OSCI/CCI/"

__CCI_OPEN_CONFIG_NAMESPACE__
  
  // ///////////////////// Error handling /////////////////////// //

  /// Error/warning/info types class for sc_reports, using id prefix <code>/OSCI/CCI/</code>
  /**
   * Usage:
   * <code>
   * CCI_THROW_ERROR(cci::cnf::cci_report_types::type().cci_value_failure, "Any report text");
   * </code>
   * @todo This report error/warning/info types class could be made more elegant!
   */
  class cci_report_types {

  protected:
    cci_report_types()
    : set_param_failed("set_param_failed")
    , get_param_failed("get_param_failed")
    , add_param_failed("add_param_failed")
    , remove_param_failed("remove_param_failed")
    , cci_value_failure("cci_value_failure")
    {}
    
  public:
    /// Static function to be called to get accesses to the cci_report_types instances
    static const cci_report_types& type() {
      static cci_report_types t = cci_report_types();
      return t;
    }
    
    const std::string set_param_failed;         // set parameter failed
    // set_param_bad_type;     // provided value is of the wrong type
    // set_param_bad_value;    // provided value is not useful (out of range or illegal for other reason)
    // set_param_not_settable; // this parameter is not settable TODO: supported?
    // set_param_init_value_failed; // the setting of the initial value in the database failed
    const std::string get_param_failed;         // get parameter failed
    // get_param_not_gettable; // param not gettable TODO: supported?
    const std::string add_param_failed;         // parameter not added
    // add_param_success_but_renamed; // parameter added successfully but it has been renamed automatically
    const std::string remove_param_failed;                 // Removing parameter from database failed
    // remove_param_failed_not_available; // The given pointer was not in the registry (e.g. it was NULL or simply not there)
    const std::string cci_value_failure;
  };


#define CCI_THROW_ERROR(_id, _message) { \
  std::stringstream sid, smsg; sid << CCI_SC_REPORT_MSG_TYPE_PREFIX << _id; smsg << _message; \
  SC_REPORT_ERROR(sid.str().c_str(), smsg.str().c_str()); \
}
  
  /*inline void throw_error(const std::string& id, const std::string& message) {
    std::stringstream sid, smsg;
    sid << CCI_SC_REPORT_MSG_TYPE_PREFIX << id;
    smsg //<< "ERROR: "
    << message;
    SC_REPORT_ERROR(sid.str().c_str(), smsg.str().c_str());
  }*/

#define CCI_THROW_WARNING(_id, _message) { \
  std::stringstream sid, smsg; sid << CCI_SC_REPORT_MSG_TYPE_PREFIX << _id; smsg << _message; \
  SC_REPORT_WARNING(sid.str().c_str(), smsg.str().c_str()); \
}
  /*inline void throw_warning(const std::string& id, const std::string& message) {
    std::stringstream sid, smsg;
    sid << CCI_SC_REPORT_MSG_TYPE_PREFIX << id;
    smsg //<< "WARNING: "
    << message;
    SC_REPORT_WARNING(sid.str().c_str(), smsg.str().c_str());
  }*/

#define CCI_THROW_INFO(_id, _message) { \
  std::stringstream sid, smsg; sid << CCI_SC_REPORT_MSG_TYPE_PREFIX << _id; smsg << _message; \
  SC_REPORT_INFO(sid.str().c_str(), smsg.str().c_str()); \
}
  /*inline void throw_info(const std::string& id, const std::string& message) {
    std::stringstream sid, smsg;
    sid << CCI_SC_REPORT_MSG_TYPE_PREFIX << id;
    smsg //<< "INFO: " 
    << message;
    SC_REPORT_INFO(sid.str().c_str(), smsg.str().c_str());
  }*/
  
__CCI_CLOSE_CONFIG_NAMESPACE__


#endif
