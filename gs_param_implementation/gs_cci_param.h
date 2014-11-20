// LICENSETEXT
//
//   Copyright (C) 2009-2011 : GreenSocs Ltd
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


#ifndef __CCI_PARAM_H__
#define __CCI_PARAM_H__


#ifndef __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
  #error "failed include check"
#else
  #undef __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
#endif

#include "cci_cnf/cci_value.h"
#include "cci_cnf/cci_value_traits.h"

#include <string>
#include <iostream>
#include <sstream>

__OPEN_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__

  // Forward declaration
  template<typename T, cci::cnf::param_mutable_type TM>
  class gs_cci_param;

  /// Prototype parameter implementation
  template<typename T, cci::cnf::param_mutable_type TM = cci::cnf::mutable_param>
  class gs_cci_param 
  : public gs_cci_param_t <T, TM>
  {
  public:

    /// Typedef for the value.
    typedef T value_type;
    /// Typedef for the param itself.
    typedef gs_cci_param<value_type, TM> my_type;
    /// Typedef for base type
    typedef gs_cci_param_t<value_type, TM> base_type;

    using base_type::return_string;
    //using base_type::my_value;
    using base_type::operator=;
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Construction / Destruction   ///////////////////// //

    gs_cci_param(cci::cnf::cci_param<value_type, TM>& owner_par, const std::string &nam, const char* val    , const bool is_top_level_name, cci::cnf::cci_cnf_broker_if* broker_accessor) : base_type(owner_par, nam, std::string(val), is_top_level_name, /*register_at_db=*/true                 , broker_accessor)   { /* TODO: Remove this check to allow empty default values!! This check is to ensure the framework internally uses the correct constructor*/ if (std::string(val).length()==0) assert(false && "This shall not happen, use other constructor!"); /*base_type::init(); Done with InitParam function*/ }
    gs_cci_param(cci::cnf::cci_param<value_type, TM>& owner_par, const std::string &nam, const value_type& val, const bool is_top_level_name, cci::cnf::cci_cnf_broker_if* broker_accessor) : base_type(owner_par, nam, val,              is_top_level_name, /*register_at_db=*/true, /*(dummy)*/true, broker_accessor)   { /*base_type::init(); Done with InitParam function*/ }
    gs_cci_param(cci::cnf::cci_param<value_type, TM>& owner_par, const std::string &nam,                      const bool is_top_level_name, cci::cnf::cci_cnf_broker_if* broker_accessor) : base_type(owner_par, nam,                   is_top_level_name, /*register_at_db=*/true                 , broker_accessor)   { /*base_type::init(); Done with InitParam function*/ }
  
    ~gs_cci_param() {
    }
    
    /*my_type& operator +=  (value_type);
    my_type& operator -=  (value_type);
    my_type& operator /=  (value_type);
    my_type& operator *=  (value_type);
    value_type operator +  (value_type);
    value_type operator -  (value_type);
    value_type operator /  (value_type);
    value_type operator *  (value_type);
    my_type& operator %=  (value_type);
    my_type& operator ^=  (value_type);
    my_type& operator &=  (value_type);
    my_type& operator |=  (value_type);
    my_type& operator <<= (value_type);
    my_type& operator >>= (value_type);
    
    my_type& operator -- ();    // prefix
    value_type operator -- (int); // postfix
    my_type& operator ++ ();    // prefix
    value_type operator ++ (int); // postfix */ 

    // //////////////// JSON (DE)SERIALIZE /////////////////////////// //


    std::string json_serialize() const {
      ///@todo drop completely from CCI param interface?
      return json_serialize( this->get() );
    }

    void json_deserialize(const std::string& str) {
      ///@todo drop completely from CCI param interface?
      value_type t;
      json_deserialize(t, str);
      this->set(t);
    }

    std::string json_serialize(const value_type& val) const {
      return cci::cnf::cci_value::to_json( cci::cnf::cci_value(val) );
    }

    void json_deserialize(value_type& target_val, const std::string& str) {
      cci::cnf::cci_value v = cci::cnf::cci_value::from_json( str );
      target_val = v.get<value_type>();
    }

    // //////////////// CCI VALUE HANDLING /////////////////////////// //

    const value_type& get_default_value() {
      if (!base_type::m_gs_param.has_default_value())
        cci::cnf::cci_report_handler::get_param_failed("Param has no default value.");
      return base_type::m_gs_param.get_default_value();
    }

    // //////////////// CCI VALUE HANDLING /////////////////////////// //
    
    void set_value(const cci::cnf::cci_value& val) {
      value_type v = val.get<value_type>();
      this->set( v );
    }

    cci::cnf::cci_value get_value() {
      return cci::cnf::cci_value( this->get() );
    }
    
  };
      
  

  /// Prototype parameter implementation, specialization for string
  template<cci::cnf::param_mutable_type TM>
  class gs_cci_param<std::string, TM>
  : public gs_cci_param_t<std::string, TM>
  {
  protected:
    /// Typedef for the value.
    typedef std::string value_type;
    /// Typedef for the param itself.
    typedef gs_cci_param<value_type, TM> my_type;
    /// Typedef for base type
    typedef gs_cci_param_t<value_type, TM> base_type;
    
  public:
    
    using base_type::return_string;
    //using base_type::my_value;
    using base_type::operator=;
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Construction / Destruction   ///////////////////// //
    
    gs_cci_param(cci::cnf::cci_param<value_type, TM>& owner_par, const std::string &nam, const char* val    , const bool is_top_level_name, cci::cnf::cci_cnf_broker_if* broker_accessor) : base_type(owner_par, nam, std::string(val), is_top_level_name, true, (std::string(val).length()>0) ? true : false, broker_accessor) { /*base_type::init(); Done with InitParam function*/ }
    gs_cci_param(cci::cnf::cci_param<value_type, TM>& owner_par, const std::string &nam, const value_type& val, const bool is_top_level_name, cci::cnf::cci_cnf_broker_if* broker_accessor) : base_type(owner_par, nam, val, is_top_level_name, true, broker_accessor)   { /*base_type::init(); Done with InitParam function*/ }
    gs_cci_param(cci::cnf::cci_param<value_type, TM>& owner_par, const std::string &nam,                      const bool is_top_level_name, cci::cnf::cci_cnf_broker_if* broker_accessor) : base_type(owner_par, nam, std::string(""), is_top_level_name, true, false, broker_accessor) { /*base_type::init(); Done with InitParam function*/ }
    
    ~gs_cci_param() {
    }
    
    /*my_type& operator +=  (value_type);
     my_type& operator -=  (value_type);
     my_type& operator /=  (value_type);
     my_type& operator *=  (value_type);
     value_type operator +  (value_type);
     value_type operator -  (value_type);
     value_type operator /  (value_type);
     value_type operator *  (value_type);
     my_type& operator %=  (value_type);
     my_type& operator ^=  (value_type);
     my_type& operator &=  (value_type);
     my_type& operator |=  (value_type);
     my_type& operator <<= (value_type);
     my_type& operator >>= (value_type);
     
     my_type& operator -- ();    // prefix
     value_type operator -- (int); // postfix
     my_type& operator ++ ();    // prefix
     value_type operator ++ (int); // postfix */ 
    
    std::string json_serialize() const {
      ///@todo drop completely from CCI param interface?
      return json_serialize( this->get() );
    }

    void json_deserialize(const std::string& str) {
      value_type t;
      json_deserialize(t, str);
      this->set(t);
    }

    std::string json_serialize(const value_type& val) const {
      return cci::cnf::cci_value::to_json( cci::cnf::cci_value(val) );
    }

    void json_deserialize(value_type& target_val, const std::string& str) {
      cci::cnf::cci_value v = cci::cnf::cci_value::from_json( str );
      target_val = v.template get<value_type>();
    }

    const value_type& get_default_value() {
      if (!base_type::m_gs_param.has_default_value())
        cci::cnf::cci_report_handler::get_param_failed("Param has no default value.");
      return this->m_gs_param.get_default_value();
    }
    
    // TODO: check for update_latest_write_originator() for each successfull set (when adding new sets)
    void set_value(const cci::cnf::cci_value& val) {
      value_type v = val.template get<value_type>();
      this->set( v );
    }
    
    cci::cnf::cci_value get_value() {
      return cci::cnf::cci_value( this->get() );
    }
    
    const cci::cnf::basic_param_type get_basic_type() const { return cci::cnf::param_type_string; }

  };
  

__CLOSE_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__


#endif
