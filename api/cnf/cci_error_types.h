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
#include <sstream>


#define CCI_SC_REPORT_MSG_TYPE_PREFIX "/OSCI/CCI/"

__CCI_OPEN_CONFIG_NAMESPACE__
  
  // ///////////////////// Error handling /////////////////////// //

  /// Error/warning/info type class for sc_reports, using id prefix CCI_SC_REPORT_MSG_TYPE_PREFIX
  /**
   * Usage:
   * <code>
   * CCI_THROW_ERROR(cci::cnf::cci_report::cci_value_failure().get_type(), "Any report text");
   * // Catch:
   * try {
   *   b = bool_param.get_default_value(); // e.g.
   * } catch(sc_core::sc_report e) {
   *   // If get_param_failed error, catch it
   *   if (strcmp(e.get_msg_type(), cci::cnf::cci_report::get_param_failed().get_msg_type()) == 0)
   *     cout <<endl<< name() << ": Caught " << e.what() << endl;
   *   // If other error, throw it again
   *   else throw e;
   * }
   * </code>
   * @todo Shall this report error/warning/info type class be made more elegant?
   */
  struct cci_report_base
  : public std::string {
    cci_report_base(const char* _type) 
    : std::string(_type) 
    {
      std::stringstream sid;
      sid << CCI_SC_REPORT_MSG_TYPE_PREFIX << _type;
      m_msg_type = sid.str();
    }
    const char* get_msg_type() const { return m_msg_type.c_str(); }
    const char* get_type() const { return this->c_str(); }
    //operator std::string&() const { return *this; }
    //std::string& operator&() {return *this; }
    std::string m_msg_type;
  };

  #define CCI_REPORT_FUNCTION_FOR(REPORT_NAME)   \
    static const cci_report_base& REPORT_NAME() { \
      static cci_report_base t = cci_report_base(#REPORT_NAME); \
      return t; \
    }
  class cci_report {
  protected:
    cci_report()  {}
  public:
    CCI_REPORT_FUNCTION_FOR(set_param_failed)
    CCI_REPORT_FUNCTION_FOR(get_param_failed)
    CCI_REPORT_FUNCTION_FOR(add_param_failed)
    CCI_REPORT_FUNCTION_FOR(remove_param_failed)
    CCI_REPORT_FUNCTION_FOR(cci_value_failure)
    CCI_REPORT_FUNCTION_FOR(broker_failure)
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
