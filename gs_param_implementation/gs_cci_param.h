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


#include <string>
#include <iostream>
#include <sstream>

#include "json_spirit/json_spirit.h"


__OPEN_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__

  // Forward declaration
  template<typename T, cci::cnf::param_mutable_type TM>
  class gs_cci_param;

  /// Helper class: Needs to be specialized for all data types where cci_values shall work
  /**
   * This default implementation causes errors.
   */
  template<typename T, cci::cnf::param_mutable_type TM>
  struct cci_value_helper {
    typedef T my_type;
    /// Causes error by default; called by gs_cci_param::set_value
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_report_handler::cci_value_failure("Set cci value not implemented for not specialized parameter types.");
      // TODO: this could use a cci value's json representation to set the parameter
    }
    /// Causes error by default; called by gs_cci_param::get_value
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_report_handler::cci_value_failure("Get cci value not implemented for not specialized parameter types.");
      // TODO: this could use a cci value's json representation to get the parameter
      cci::cnf::cci_value val;
      return val;    
    }
  };

  /// Helper class UNSIGNED INT template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<unsigned int, TM> {
    typedef unsigned int my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::partype_number:
          // TODO one could implement further checks, e.g. overflows or implement conversions from type partype_real
          param.set(val.get_int64());
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val((sc_dt::int64)param.get());
      return val;    
    }
  };

  /// Helper class INT template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<int, TM> {
    typedef int my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::partype_number:
          // TODO one could implement further checks, e.g. overflows or implement conversions from type partype_real
          param.set(val.get_int());
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get());
      return val;    
    }
  };

  /// Helper class BOOL template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<bool, TM> {
    typedef bool my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::partype_bool:
          // TODO one could implement further checks or conversions from int etc.
          param.set(val.get_bool());
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get());
      return val;    
    }
  };

  /// Helper class DOUBLE template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<double, TM> {
    typedef double my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::partype_real:
          // TODO one could implement further checks or conversions from int etc.
          param.set(val.get_real());
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get());
      return val;    
    }
  };

  /// Helper class FLOAT template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<float, TM> {
    typedef float my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::partype_real:
          // TODO one could implement further checks or conversions from int etc.
          param.set(val.get_real());
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get());
      return val;    
    }
  };

  /// Helper class STRING template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<std::string, TM> {
    typedef std::string my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::partype_string:
          param.set(val.get_string());
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get());
      return val;    
    }
  };

  /// Helper class LONG LONG template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<long long, TM> {
    typedef long long my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::partype_number:
          // TODO one could implement further checks or conversions from int etc.
          param.set(val.get_int64());
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get());
      return val;    
    }
  };


  /// Helper class UNSIGNED CHAR template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<unsigned char, TM> {
    typedef unsigned char my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::partype_number:
          // TODO one could implement further checks or conversions from int etc.
          param.set(val.get_int());
          break;
        case cci::cnf::partype_string:
          // TODO one could implement further checks or conversions from int etc.
          //TODO
          //break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get());
      return val;    
    }
  };

  /// Helper class SIGNED CHAR template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<signed char, TM> {
    typedef signed char my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::partype_number:
          // TODO one could implement further checks or conversions from int etc.
          param.set(val.get_int());
          break;
        case cci::cnf::partype_string:
          // TODO one could implement further checks or conversions from int etc.
          //TODO
          //break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get());
      return val;    
    }
  };


  /// Helper class CHAR template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<char, TM> {
    typedef char my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      std::string str;
      switch (val.type() ) {
        case cci::cnf::partype_number:
          // TODO one could implement further checks or conversions from int etc.
          param.set(val.get_int());
          break;
        case cci::cnf::partype_string:
          // TODO one could implement further checks or conversions from int etc.
          //TODO
          //break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get());
      return val;    
    }
  };

  /// Helper class UNSIGNED SHORT template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<unsigned short, TM> {
    typedef unsigned short my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::partype_number:
          // TODO one could implement further checks or conversions from int etc.
          param.set(val.get_int());
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get());
      return val;    
    }
  };

//TODO weitere datentypen



  template<typename T, cci::cnf::param_mutable_type TM = cci::cnf::mutable_parameter>
  class gs_cci_param 
  : public gs_cci_param_t <T, TM>
  {
  protected:
    /// Typedef for the value.
    typedef T val_type;
    /// Typedef for the param itself.
    typedef gs_cci_param<val_type, TM> my_type;
    /// Typedef for base type
    typedef gs_cci_param_t<val_type, TM> base_type;

  public:

    using base_type::return_string;
    //using base_type::my_value;
    using base_type::operator=;
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Construction / Destruction   ///////////////////// //

    gs_cci_param(cci::cnf::cci_param<val_type, TM>& owner_par, const char* nam, const char* val    , const bool is_top_level_name, cci::cnf::cci_cnf_broker_if* broker_accessor) : base_type(owner_par, nam, std::string(val), is_top_level_name, /*register_at_db=*/true                 , broker_accessor)   { /* TODO: Remove this check to allow empty default values!! This check is to ensure the framework internally uses the correct constructor*/ if (std::string(val).length()==0) assert(false && "This shall not happen, use other constructor!"); /*base_type::init(); Done with InitParam function*/ }
    gs_cci_param(cci::cnf::cci_param<val_type, TM>& owner_par, const char* nam, const val_type& val, const bool is_top_level_name, cci::cnf::cci_cnf_broker_if* broker_accessor) : base_type(owner_par, nam, val,              is_top_level_name, /*register_at_db=*/true, /*(dummy)*/true, broker_accessor)   { /*base_type::init(); Done with InitParam function*/ }
    gs_cci_param(cci::cnf::cci_param<val_type, TM>& owner_par, const char* nam,                      const bool is_top_level_name, cci::cnf::cci_cnf_broker_if* broker_accessor) : base_type(owner_par, nam,                   is_top_level_name, /*register_at_db=*/true                 , broker_accessor)   { /*base_type::init(); Done with InitParam function*/ }
  
    ~gs_cci_param() {
    }
    
    /*my_type& operator +=  (val_type);
    my_type& operator -=  (val_type);
    my_type& operator /=  (val_type);
    my_type& operator *=  (val_type);
    val_type operator +  (val_type);
    val_type operator -  (val_type);
    val_type operator /  (val_type);
    val_type operator *  (val_type);
    my_type& operator %=  (val_type);
    my_type& operator ^=  (val_type);
    my_type& operator &=  (val_type);
    my_type& operator |=  (val_type);
    my_type& operator <<= (val_type);
    my_type& operator >>= (val_type);
    
    my_type& operator -- ();    // prefix
    val_type operator -- (int); // postfix
    my_type& operator ++ ();    // prefix
    val_type operator ++ (int); // postfix */ 

    // //////////////// JSON (DE)SERIALIZE /////////////////////////// //

    std::string json_serialize() const {
      return_string = json_serialize(base_type::get());
      return return_string;
    }
    
    void json_deserialize(const std::string& str) {
      val_type t;
      json_deserialize(t, str);
      base_type::set(t);
    }

    std::string json_serialize(const val_type& val) const {
      // TODO: this is currently not a JSON but a GreenConfig specific string
      // TODO: throw exception on error
      return base_type::m_gs_param.serialize(val);
    }

    void json_deserialize(val_type& target_val, const std::string& str) {
      // TODO: this is currently not a JSON but a GreenConfig specific string
      if (!base_type::m_gs_param.deserialize(target_val, str))
        cci::cnf::cci_report_handler::set_param_failed("String conversion failed.");
    }

    // //////////////// CCI VALUE HANDLING /////////////////////////// //

    const val_type& get_default_value() {
      if (!base_type::m_gs_param.has_default_value())
        cci::cnf::cci_report_handler::get_param_failed("Param has no default value.");
      return base_type::m_gs_param.get_default_value();
    }

    // //////////////// CCI VALUE HANDLING /////////////////////////// //
    
    void set_value(const cci::cnf::cci_value& val) {
      cci_value_helper<val_type, TM>::from_value(val, *this);
    }

    cci::cnf::cci_value get_value() {
      return cci_value_helper<val_type, TM>::to_value(*this);
    }
    
  };
      
  

  template<cci::cnf::param_mutable_type TM>
  class gs_cci_param<std::string, TM>
  : public gs_cci_param_t<std::string, TM>
  {
  protected:
    /// Typedef for the value.
    typedef std::string val_type;
    /// Typedef for the param itself.
    typedef gs_cci_param<val_type, TM> my_type;
    /// Typedef for base type
    typedef gs_cci_param_t<val_type, TM> base_type;
    
  public:
    
    using base_type::return_string;
    //using base_type::my_value;
    using base_type::operator=;
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Construction / Destruction   ///////////////////// //
    
    gs_cci_param(cci::cnf::cci_param<val_type, TM>& owner_par, const char* nam, const char* val    , const bool is_top_level_name, cci::cnf::cci_cnf_broker_if* broker_accessor) : base_type(owner_par, nam, std::string(val), is_top_level_name, true, (std::string(val).length()>0) ? true : false, broker_accessor) { /*base_type::init(); Done with InitParam function*/ }
    gs_cci_param(cci::cnf::cci_param<val_type, TM>& owner_par, const char* nam, const val_type& val, const bool is_top_level_name, cci::cnf::cci_cnf_broker_if* broker_accessor) : base_type(owner_par, nam, val, is_top_level_name, true, broker_accessor)   { /*base_type::init(); Done with InitParam function*/ }
    gs_cci_param(cci::cnf::cci_param<val_type, TM>& owner_par, const char* nam,                      const bool is_top_level_name, cci::cnf::cci_cnf_broker_if* broker_accessor) : base_type(owner_par, nam, std::string(""), is_top_level_name, true, false, broker_accessor) { /*base_type::init(); Done with InitParam function*/ }
    
    ~gs_cci_param() {
    }
    
    /*my_type& operator +=  (val_type);
     my_type& operator -=  (val_type);
     my_type& operator /=  (val_type);
     my_type& operator *=  (val_type);
     val_type operator +  (val_type);
     val_type operator -  (val_type);
     val_type operator /  (val_type);
     val_type operator *  (val_type);
     my_type& operator %=  (val_type);
     my_type& operator ^=  (val_type);
     my_type& operator &=  (val_type);
     my_type& operator |=  (val_type);
     my_type& operator <<= (val_type);
     my_type& operator >>= (val_type);
     
     my_type& operator -- ();    // prefix
     val_type operator -- (int); // postfix
     my_type& operator ++ ();    // prefix
     val_type operator ++ (int); // postfix */ 
    
    std::string json_serialize() const {
      return_string = json_serialize(base_type::get());
      return return_string;
    }
    
    void json_deserialize(const std::string& str) {
      val_type t;
      json_deserialize(t, str);
      base_type::set(t);
    }
    
    std::string json_serialize(const val_type& val) const {
      return base_type::m_gs_param.serialize(val);
    }
    
    void json_deserialize(val_type& target_val, const std::string& str) {
      base_type::m_gs_param.deserialize(target_val, str);
    }    
    
    const val_type& get_default_value() {
      if (!base_type::m_gs_param.has_default_value())
        cci::cnf::cci_report_handler::get_param_failed("Param has no default value.");
      return base_type::m_gs_param.get_default_value();
    }
    
    // TODO: check for update_latest_write_originator() for each successfull set (when adding new sets)
    void set_value(const cci::cnf::cci_value& val) {
      cci_value_helper<val_type, TM>::from_value(val, *this);
    }
    
    cci::cnf::cci_value get_value() {
      return cci_value_helper<val_type, TM>::to_value(*this);
    }
    
    const cci::cnf::basic_param_type get_basic_type() const { return cci::cnf::partype_string; }

  };
  

__CLOSE_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__


#endif
