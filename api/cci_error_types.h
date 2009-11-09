/*****************************************************************************
 
 The following code is derived, directly or indirectly, from the SystemC
 source code Copyright (c) 1996-2009 by all Contributors.
 All Rights reserved.
 
 Developed by GreenSocs : http://www.greensocs.com/
 Christian Schroeder, schroeder@eis.cs.tu-bs.de
 Mark Burton, mark@greensocs.com
 
 The contents of this file are subject to the restrictions and limitations
 set forth in the SystemC Open Source License Version 3.0 (the "License");
 You may not use this file except in compliance with such restrictions and
 limitations. You may obtain instructions on how to receive a copy of the
 License at http://www.systemc.org/. Software distributed by Contributors
 under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 ANY KIND, either express or implied. See the License for the specific
 language governing rights and limitations under the License.
 
 *****************************************************************************/


#ifndef __CCI_ERROR_TYPES_H__
#define __CCI_ERROR_TYPES_H__

#include <string>
#include <iostream>


#define CCI_SC_REPORT_MSG_TYPE_PREFIX "/OSCI/CCI/"

namespace cci {
  
  // ///////////////////// Error handling /////////////////////// //

  /// @todo: This report error types is just a urgently hacked class, needs to be made elegant!
  class cci_report_types {

  public:
    cci_report_types()
    : set_param_success("set_param_success") 
    , set_param_bad_type("set_param_bad_type")
    , set_param_bad_value("set_param_bad_value")
    , set_param_not_settable("set_param_not_settable")
    , set_param_init_value_failed("set_param_init_value_failed")
    , set_param_failed("set_param_failed")
    , get_param_success("get_param_success")
    , get_param_not_gettable("get_param_not_gettable")
    , get_param_failed("get_param_failed")
    , add_param_success("add_param_success")
    , add_param_success_but_renamed("add_param_success_but_renamed")
    , add_param_failed("add_param_failed")
    , remove_param_successful("remove_param_successful")
    , remove_param_failed_not_available("remove_param_failed_not_available")
    , remove_param_failed("remove_param_failed")
    {}
    
  public:
    static const cci_report_types& type() {
      static cci_report_types t = cci_report_types();
      return t;
    }
    
    const std::string set_param_success;      // successfully set value
    const std::string set_param_bad_type;     // provided value is of the wrong type
    const std::string set_param_bad_value;    // provided value is not useful (out of range or illegal for other reason)
    const std::string set_param_not_settable; // this parameter is not settable TODO: supported?
    const std::string set_param_init_value_failed; // the setting of the initial value in the database failed
    const std::string set_param_failed;        // other error like memory allocations inside the called object failed or something else just went bad
    const std::string get_param_success;       // sucessfully got value
    const std::string get_param_not_gettable;  // param not gettable TODO: supported?
    const std::string get_param_failed;        // other error, e.g. failed to allocate return object
    const std::string add_param_success;             // parameter added successfully
    const std::string add_param_success_but_renamed; // parameter added successfully but it has been renamed automatically
    const std::string add_param_failed;              // parameter not added
    const std::string remove_param_successful;           // success
    const std::string remove_param_failed_not_available; // The given pointer was not in the registry (e.g. it was NULL or simply not there)
    const std::string remove_param_failed;                // other error
  };

  
  inline void throw_error(const std::string& id, const std::string& message) {
    std::stringstream sid, smsg;
    sid << CCI_SC_REPORT_MSG_TYPE_PREFIX << id;
    smsg << "ERROR: " << message;
    SC_REPORT_ERROR(sid.str().c_str(), smsg.str().c_str());
  }
  
} // end namespace cci


#endif
