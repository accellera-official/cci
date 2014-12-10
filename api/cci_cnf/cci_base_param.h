// LICENSETEXT
//
//   Copyright (C) 2010-2011 : GreenSocs Ltd
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


#ifndef CCI_CNF_BASE_PARAM_H_INCLUDED_
#define CCI_CNF_BASE_PARAM_H_INCLUDED_

#include "cci_base_param_impl_if.h"

CCI_OPEN_CONFIG_NAMESPACE_

  // CCI Configuration parameter base class
  /**
   * Type independent base class all cci parameters derive from.
   * This class gives some easier access to type independent functions.
   */
  class cci_base_param 
  : virtual public cci_base_param_impl_if // TODO: Just to make sure they are compatible // needed to be removed due to originator difference
  {
  public:
    //explicit cci_base_param() { }
    
    virtual ~cci_base_param() { }
    
    virtual void json_deserialize(const std::string& json_string) = 0; // TODO: make them not virtual??
    virtual std::string json_serialize() const = 0;
    virtual const basic_param_type get_basic_type() const = 0;
    virtual void set_value(const cci_value& val) = 0;
    virtual cci_value get_value() = 0;
    virtual void set_documentation(const std::string& doc) = 0;
    virtual std::string get_documentation() const = 0;
    virtual bool is_default_value() = 0;
    virtual bool is_invalid_value() = 0;
    virtual void set_invalid_value() = 0;
    virtual bool is_initial_value() const = 0;
    virtual const std::string& get_name() const = 0;
    virtual cci::shared_ptr<callb_adapt> register_callback(const callback_type type, void* observer, param_callb_func_ptr function) = 0;
    virtual cci::shared_ptr<callb_adapt> register_callback(const callback_type type, cci::shared_ptr<callb_adapt> callb) = 0;
    virtual void unregister_all_callbacks(void* observer) = 0;
    virtual bool unregister_callback(cci::shared_ptr<callb_adapt> callb) = 0;
    virtual bool unregister_callback(callb_adapt* callb) = 0;
    virtual bool has_callbacks() = 0;
    virtual bool lock(void* pwd = NULL) = 0;
    virtual bool unlock(void* pwd = NULL) = 0;
    virtual bool is_locked() const = 0;
    virtual bool is_accessor() const = 0;
    virtual cci_base_param* create_accessor(const cci_originator& originator) = 0;
    virtual const cci_originator* get_latest_write_originator() const = 0;
  };

CCI_CLOSE_CONFIG_NAMESPACE_
  
#endif
