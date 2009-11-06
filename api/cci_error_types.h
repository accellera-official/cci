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


namespace cci {
  
  // ///////////////////// Error handling /////////////////////// //
  
  enum cci_exception_type {
    cci_set_param_exception,
    cci_get_param_exception,
    cci_add_param_exception,
    cci_remove_param_exception,
    other_cci_exception
  };
  
  /// Error code enumeration for setting methods
  enum set_param_error_type {
    set_param_success,      // successfully set value
    set_param_bad_type,     // provided value is of the wrong type
    set_param_bad_value,    // provided value is not useful (out of range or illegal for other reason)
    set_param_not_settable, // this parameter is not settable TODO: supported?
    set_param_init_value_failed, // the setting of the initial value in the database failed
    set_param_failed        // other error like memory allocations inside the called object failed or something else just went bad
  };
  /// stream operator to return string for error type
  inline std::ostream& operator<<(std::ostream& s, const set_param_error_type& p){
    switch ((unsigned int)p){
      case set_param_success:      s<<"set_param_success"; break;
      case set_param_bad_type:     s<<"set_param_bad_type"; break;
      case set_param_bad_value:    s<<"set_param_bad_value"; break;
      case set_param_not_settable: s<<"set_param_not_settable"; break;
      case set_param_failed:       s<<"set_param_failed"; break;
      default: assert(false && "This should never happen");
    }
    return s;
  }
  
  /// Error code enumeration for getting methods
  enum get_param_error_type {
    get_param_success,       // sucessfully got value
    get_param_not_gettable,  // param not gettable TODO: supported?
    get_param_failed         // other error, e.g. failed to allocate return object
  };
  /// stream operator to return string for error type
  inline std::ostream& operator<<(std::ostream& s, const get_param_error_type& p){
    switch ((unsigned int)p){
      case get_param_success:      s<<"get_param_success"; break;
      case get_param_not_gettable: s<<"get_param_not_gettable"; break;
      case get_param_failed:       s<<"get_param_failed"; break;
      default: assert(false && "This should never happen");
    }
    return s;
  }
  
  /// Error code enumeration for parameter adds
  enum add_param_error_type {
    add_param_success,             // parameter added successfully
    add_param_success_but_renamed, // parameter added successfully but it has been renamed automatically
    add_param_failed               // parameter not added
  };
  /// stream operator to return string for error type
  inline std::ostream& operator<<(std::ostream& s, const add_param_error_type& p){
    switch ((unsigned int)p){
      case add_param_success:             s<<"add_param_success"; break;
      case add_param_success_but_renamed: s<<"add_param_success_but_renamed"; break;
      case add_param_failed:              s<<"add_param_failed"; break;
      default: assert(false && "This should never happen");
    }
    return s;
  }
  
  /// Error code enumeration for parameter removes
  enum remove_param_error_type {
    remove_param_successful,           // success
    remove_param_failed_not_available, // The given pointer was not in the registry (e.g. it was NULL or simply not there)
    remove_param_failed                // other error
  };
  /// stream operator to return string for error type
  inline std::ostream& operator<<(std::ostream& s, const remove_param_error_type& p){
    switch ((unsigned int)p){
      case remove_param_successful:           s<<"remove_param_successful"; break;
      case remove_param_failed_not_available: s<<"remove_param_failed_not_available"; break;
      case remove_param_failed:               s<<"remove_param_failed"; break;
      default: assert(false && "This should never happen");
    }
    return s;
  }
  
  class cci_exception 
  : public std::exception {
  public:
    cci_exception () throw() { }
    //cci_exception (const cci_exception&) throw();
    //cci_exception& operator= (const cci_exception&) throw();
    virtual ~cci_exception() throw() { }
    virtual const char* what() const throw() { return "other_cci_exception";}    
    virtual cci_exception_type general_type() const throw() { return other_cci_exception; }
  };
  
  class cci_exception_set_param 
  : public cci_exception {
  public:
    cci_exception_set_param (set_param_error_type _type, const char* _message) 
    : m_message(_message)
    , m_type(_type) {    }
    virtual ~cci_exception_set_param() throw() { }
    cci_exception_type general_type() const throw() { return cci_set_param_exception; }
    set_param_error_type type() const throw() { return m_type; }
    const char* what() const throw() { std::ostringstream s; s << m_type << ": " << m_message; return s.str().c_str(); }    
  protected:
    std::string m_message;
    set_param_error_type m_type;
  };
  
  class cci_exception_get_param 
  : public cci_exception {
  public:
    cci_exception_get_param (get_param_error_type _type, const char* _message) 
    : m_message(_message)
    , m_type(_type) {    }
    virtual ~cci_exception_get_param() throw() { }
    cci_exception_type general_type() const throw() { return cci_get_param_exception; }
    get_param_error_type type() const throw() { return m_type; }
    const char* what() const throw() { std::ostringstream s; s << m_type << ": " << m_message; return s.str().c_str(); }    
  protected:
    std::string m_message;
    get_param_error_type m_type;
  };
  
  class cci_exception_add_param 
  : public cci_exception {
  public:
    cci_exception_add_param (add_param_error_type _type, const char* _message) 
    : m_message(_message)
    , m_type(_type) {    }
    virtual ~cci_exception_add_param() throw() { }
    cci_exception_type general_type() const throw() { return cci_add_param_exception; }
    add_param_error_type type() const throw() { return m_type; }
    const char* what() const throw() { std::ostringstream s; s << m_type << ": " << m_message; return s.str().c_str(); }    
  protected:
    std::string m_message;
    add_param_error_type m_type;
  };
  
  class cci_exception_remove_param 
  : public cci_exception {
  public:
    cci_exception_remove_param (remove_param_error_type _type, const char* _message) 
    : m_message(_message)
    , m_type(_type) {    }
    virtual ~cci_exception_remove_param() throw() { }
    cci_exception_type general_type() const throw() { return cci_remove_param_exception; }
    remove_param_error_type type() const throw() { return m_type; }
    const char* what() const throw() { std::ostringstream s; s << m_type << ": " << m_message; return s.str().c_str(); }    
  protected:
    std::string m_message;
    remove_param_error_type m_type;
  };
  
} // end namespace cci


#endif
