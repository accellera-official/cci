// LICENSETEXT
//
//   Copyright (C) 2009-2010 : GreenSocs Ltd
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
  #include "gs_cci_param_t.h"
  #undef __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
#endif


#include <string>
#include <iostream>
#include <sstream>

#include <systemc>

#include "gs_param_implementation/json_spirit/json_spirit.h"


namespace cci_impl {

  template<typename T, cci::cnf::param_mutable_type TM = cci::cnf::mutable_parameter>
  class cci_param 
  : public gs_cci_param_t <T, TM>
  {
  protected:
    /// Typedef for the value.
    typedef T val_type;
    /// Typedef for the param itself.
    typedef cci_param<val_type, TM> my_type;
    /// Typedef for base type
    typedef gs_cci_param_t<val_type, TM> base_type;

  public:
    
    using base_type::return_string;
    //using base_type::my_value;
    using base_type::operator=;
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Construction / Destruction   ///////////////////// //

    // Explicit constructors to avoid implicit construction of parameters.
    
    /// Empty constructor. Name will be set in base
    /*explicit cci_param()                    : base_type("", std::string(""), false, true, false) { base_type::init(); }
    explicit cci_param(const val_type& val) : base_type("", val, false, true) { base_type::init(); }
    
    /// Constructor with (local/hierarchical) name.
    explicit cci_param(const std::string& nam) : base_type(nam, std::string(""), false, true, false) { base_type::init(); }
    explicit cci_param(const char* nam       ) : base_type(nam, std::string(""), false, true, false) { base_type::init(); }
    
    /// Constructor with (local/hierarchical) name and string representation of initial value.
    explicit cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name = false)  : base_type(nam, val, force_top_level_name, true, true) { base_type::init(); }
    explicit cci_param(const char* nam,        const char* val       , const bool force_top_level_name = false)  : base_type(nam, std::string(val), force_top_level_name, true, true) { base_type::init(); }
    explicit cci_param(const std::string& nam, const char* val       , const bool force_top_level_name = false)  : base_type(nam, std::string(val), force_top_level_name, true, true) { base_type::init(); }
    explicit cci_param(const char* nam,        const std::string& val, const bool force_top_level_name = false)  : base_type(nam, val, force_top_level_name, true, true) { base_type::init(); }
    
    /// Constructor with (local/hierarchical) name and initial value.
    explicit cci_param(const std::string& nam, const val_type& val, const bool force_top_level_name = false) : base_type(nam, val, force_top_level_name, true)   { base_type::init(); }
    explicit cci_param(const char* nam,        const val_type& val, const bool force_top_level_name = false) : base_type(nam, val, force_top_level_name, true)   { base_type::init(); }
    
    // Constructors with register_at_db bool
    explicit cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name, const bool register_at_db) : base_type(nam, val, force_top_level_name, register_at_db, true) { base_type::init(); }
*/

    //explicit cci_param(cci::cnf::cci_param<val_type, TM>& owner_par, const char* nam, const char* val    , const bool force_top_level_name = false) : base_type(owner_par, nam, std::string(val), force_top_level_name, /*register_at_db=*/true, (std::string(val).length()>0) ? true : false) { /*base_type::init(); Done with InitParam function*/ }
    explicit cci_param(cci::cnf::cci_param<val_type, TM>& owner_par, const char* nam, const char* val    , const bool force_top_level_name) : base_type(owner_par, nam, std::string(val), force_top_level_name, /*register_at_db=*/true                 )   { if (std::string(val).length()==0) assert(false && "Dies darf nicht auftreten, den anderen Konstruktor benutzen!"); /*base_type::init(); Done with InitParam function*/ }
    explicit cci_param(cci::cnf::cci_param<val_type, TM>& owner_par, const char* nam, const val_type& val, const bool force_top_level_name) : base_type(owner_par, nam, val,              force_top_level_name, /*register_at_db=*/true, /*(dummy)*/true)   { /*base_type::init(); Done with InitParam function*/ }
    explicit cci_param(cci::cnf::cci_param<val_type, TM>& owner_par, const char* nam,                      const bool force_top_level_name) : base_type(owner_par, nam,                   force_top_level_name, /*register_at_db=*/true                 )   { /*base_type::init(); Done with InitParam function*/ }
  
    ~cci_param() {
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

    const std::string& json_serialize() const {
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
      return base_type::m_gs_param.convertValueToString(val);
    }

    void json_deserialize(val_type& target_val, const std::string& str) {
      // TODO: this is currently not a JSON but a GreenConfig specific string
      if (!base_type::m_gs_param.deserialize(target_val, str))
        CCI_THROW_ERROR(cci::cnf::cci_report_types::type().set_param_failed, "String conversion failed.");
    }

    // //////////////// CCI VALUE HANDLING /////////////////////////// //

    const val_type& get_default_value() {
      if (!base_type::m_gs_param.has_default_value())
        CCI_THROW_ERROR(cci::cnf::cci_report_types::type().get_param_failed, "Param has no default value.");
      return base_type::m_gs_param.get_default_value();
    }

    // //////////////// CCI VALUE HANDLING /////////////////////////// //
    
    void set_value(const cci::cnf::cci_value& val) {
      CCI_THROW_ERROR(cci::cnf::cci_report_types::type().cci_value_failure, "Set cci value not implemented for not specialized parameter types.");
      // TODO: this could use a cci value's json representation to set the parameter
    }

    cci::cnf::cci_value get_value() {
      CCI_THROW_ERROR(cci::cnf::cci_report_types::type().cci_value_failure, "Get cci value not implemented for not specialized parameter types.");
      // TODO: this could use a cci value's json representation to get the parameter
      cci::cnf::cci_value val;
      return val;
    }
    
  };
      
  

  template<cci::cnf::param_mutable_type       TM>
  class cci_param<std::string, TM>
  : public gs_cci_param_t<std::string, TM>
  {
  protected:
    /// Typedef for the value.
    typedef std::string val_type;
    /// Typedef for the param itself.
    typedef cci_param<val_type, TM> my_type;
    /// Typedef for base type
    typedef gs_cci_param_t<val_type, TM> base_type;
    
  public:
    
    using base_type::return_string;
    //using base_type::my_value;
    using base_type::operator=;
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Construction / Destruction   ///////////////////// //
    
    // Explicit constructors to avoid implicit construction of parameters.

    /// Empty constructor. Name will be set in base
    /*explicit cci_param()                    : base_type("", std::string(""), false, true, false) { base_type::init(); }
    
    /// Constructor with (local/hierarchical) name.
    explicit cci_param(const std::string& nam) : base_type(nam, std::string(""), false, true, false) { base_type::init(); }
    explicit cci_param(const char* nam       ) : base_type(nam, std::string(""), false, true, false) { base_type::init(); }
    
    /// Constructor with (local/hierarchical) name and string representation of initial value.
    explicit cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name = false)  : base_type(nam, val, force_top_level_name, true, true) { base_type::init(); }
    explicit cci_param(const char* nam,        const char* val       , const bool force_top_level_name = false)  : base_type(nam, std::string(val), force_top_level_name, true, true) { base_type::init(); }
    explicit cci_param(const std::string& nam, const char* val       , const bool force_top_level_name = false)  : base_type(nam, std::string(val), force_top_level_name, true, true) { base_type::init(); }
    explicit cci_param(const char* nam,        const std::string& val, const bool force_top_level_name = false)  : base_type(nam, val, force_top_level_name, true, true) { base_type::init(); }
    
    /// Constructor with (local/hierarchical) name and initial value.
    
    // Constructors with register_at_db bool
    explicit cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name, const bool register_at_db) : base_type(nam, val, force_top_level_name, register_at_db, true) { base_type::init(); }
     */
    explicit cci_param(cci::cnf::cci_param<val_type, TM>& owner_par, const char* nam, const char* val    , const bool force_top_level_name = false) : base_type(owner_par, nam, std::string(val), force_top_level_name, true, (std::string(val).length()>0) ? true : false) { /*base_type::init(); Done with InitParam function*/ }
    explicit cci_param(cci::cnf::cci_param<val_type, TM>& owner_par, const char* nam, const val_type& val, const bool force_top_level_name = false) : base_type(owner_par, nam, val, force_top_level_name, true)   { /*base_type::init(); Done with InitParam function*/ }
    
    ~cci_param() {
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
    
    const std::string& json_serialize() const {
      return_string = json_serialize(base_type::get());
      return return_string;
    }
    
    void json_deserialize(const std::string& str) {
      val_type t;
      json_deserialize(t, str);
      base_type::set(t);
    }
    
    std::string json_serialize(const val_type& val) const {
      return base_type::m_gs_param.convertValueToString(val);
    }
    
    void json_deserialize(val_type& target_val, const std::string& str) {
      base_type::m_gs_param.deserialize(target_val, str);
    }    
    
    const val_type& get_default_value() {
      if (!base_type::m_gs_param.has_default_value())
        CCI_THROW_ERROR(cci::cnf::cci_report_types::type().get_param_failed, "Param has no default value.");
      return base_type::m_gs_param.get_default_value();
    }
    
    void set_value(const cci::cnf::cci_value& val) {
      if (val.type() != get_basic_type()) {
        CCI_THROW_ERROR(cci::cnf::cci_report_types::type().cci_value_failure, "Wrong cci value type applied to parameter.");
        CCI_THROW_INFO(cci::cnf::cci_report_types::type().cci_value_failure, "  cci value type: "<<val.type()<<" != parameter type: "<<get_basic_type());
      }
      switch (val.type()) {
        case cci::cnf::partype_not_available:
          CCI_THROW_WARNING(cci::cnf::cci_report_types::type().cci_value_failure, "Applied cci value has no type.");
          break;
        case cci::cnf::partype_number:
        case cci::cnf::partype_real:
        case cci::cnf::partype_bool:
        case cci::cnf::partype_list:
        case cci::cnf::partype_other:
          CCI_THROW_ERROR(cci::cnf::cci_report_types::type().cci_value_failure, "Applied cci value not available for this param type.");
          break;
        case cci::cnf::partype_string:
          set(val.get_string());
          break;
        default:
          assert(false && "This should never happen!");
          CCI_THROW_ERROR(cci::cnf::cci_report_types::type().cci_value_failure, "Not implemented.");
      }
    }
    
    cci::cnf::cci_value get_value() {
      cci::cnf::cci_value val(base_type::get());
      return val;
    }
    
    const cci::cnf::basic_param_type get_basic_type() const { return cci::cnf::partype_string; }

  };
  
} // end namespace cci_impl

#endif
