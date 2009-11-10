// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//    Mark Burton, mark@greensocs.com
//
//
//   This program is free software.
// 
//   If you have no applicable agreement with GreenSocs Ltd, this software
//   is licensed to you, and you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   If you have a applicable agreement with GreenSocs Ltd, the terms of that
//   agreement prevail.
// 
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
//   02110-1301  USA 
// 
// ENDLICENSETEXT


#ifndef __GS_CCI_PARAM_H__
#define __GS_CCI_PARAM_H__


#include <string>
#include <iostream>
#include <sstream>

#include <systemc>

#include "json_spirit/json_spirit.h"


namespace cci {

  template<typename T>
  class gs_cci_param 
  : public gs_cci_param_t<T>
  {
  protected:
    /// Typedef for the value.
    typedef T val_type;
    /// Typedef for the param itself.
    typedef gs_cci_param<val_type> my_type;

  public:
    
    using gs_cci_param_t<val_type>::return_string;
    //using gs_cci_param_t<val_type>::my_value;
    using gs_cci_param_t<val_type>::operator=;
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Construction / Destruction   ///////////////////// //

    // Explicit constructors to avoid implicit construction of parameters.
    
    /// Empty constructor. Name will be set in base
    explicit gs_cci_param()                    : gs_cci_param_t<val_type>("", std::string(""), false, true, false) {  }
    explicit gs_cci_param(const val_type& val) : gs_cci_param_t<val_type>("", val, false, true) {  }
    
    /// Constructor with (local/hierarchical) name.
    explicit gs_cci_param(const std::string& nam) : gs_cci_param_t<val_type>(nam, std::string(""), false, true, false) {  }
    explicit gs_cci_param(const char* nam       ) : gs_cci_param_t<val_type>(nam, std::string(""), false, true, false) {  }
    
    /// Constructor with (local/hierarchical) name and string representation of initial value.
    explicit gs_cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name = false)  : gs_cci_param_t<val_type>(nam, val, force_top_level_name, true, true) {  }
    explicit gs_cci_param(const char* nam,        const char* val       , const bool force_top_level_name = false)  : gs_cci_param_t<val_type>(nam, std::string(val), force_top_level_name, true, true) {  }
    explicit gs_cci_param(const std::string& nam, const char* val       , const bool force_top_level_name = false)  : gs_cci_param_t<val_type>(nam, std::string(val), force_top_level_name, true, true) {  }
    explicit gs_cci_param(const char* nam,        const std::string& val, const bool force_top_level_name = false)  : gs_cci_param_t<val_type>(nam, val, force_top_level_name, true, true) {  }
    
    /// Constructor with (local/hierarchical) name and initial value.
    explicit gs_cci_param(const std::string& nam, const val_type& val, const bool force_top_level_name = false) : gs_cci_param_t<val_type>(nam, val, force_top_level_name, true)   {  }
    explicit gs_cci_param(const char* nam,        const val_type& val, const bool force_top_level_name = false) : gs_cci_param_t<val_type>(nam, val, force_top_level_name, true)   {  }
    
    // Constructors with register_at_db bool
    explicit gs_cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name, const bool register_at_db) : gs_cci_param_t<val_type>(nam, val, force_top_level_name, register_at_db, true) {  }

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

    const std::string& json_serialize() const {
      return_string = json_serialize(gs_cci_param_t<val_type>::get());
      return return_string;
    }
    
    void json_deserialize(const std::string& str) {
      val_type t;
      json_deserialize(t, str);
      gs_cci_param_t<val_type>::set(t);
    }

    std::string json_serialize(const val_type& val) const {
      // TODO: this is currently not a JSON but a GreenConfig specific string
      // TODO: throw exception on error
      return gs::gs_param<val_type>::convertValueToString(val);
    }

    void json_deserialize(val_type& target_val, const std::string& str) {
      // TODO: this is currently not a JSON but a GreenConfig specific string
      if (!gs::gs_param<val_type>::deserialize(target_val, str))
        CCI_THROW_ERROR(cci_report_types::type().set_param_failed, "String conversion failed.");
    }

    // //////////////// CCI VALUE HANDLING /////////////////////////// //
    
    void set_value(const cci_value& val) {
      CCI_THROW_ERROR(cci::cci_report_types::type().cci_value_failure, "Set cci value not implemented for not specialized parameter types.");
      // TODO: this could use a cci value's json representation to set the parameter
    }

    cci_value get_value() {
      CCI_THROW_ERROR(cci::cci_report_types::type().cci_value_failure, "Get cci value not implemented for not specialized parameter types.");
      // TODO: this could use a cci value's json representation to get the parameter
      cci_value val;
      return val;
    }
    
  };
      
  

  template<>
  class gs_cci_param<std::string>
  : public gs_cci_param_t<std::string>
  {
  protected:
    /// Typedef for the value.
    typedef std::string val_type;
    /// Typedef for the param itself.
    typedef gs_cci_param<val_type> my_type;
    
  public:
    
    using gs_cci_param_t<val_type>::return_string;
    //using gs_cci_param_t<val_type>::my_value;
    using gs_cci_param_t<val_type>::operator=;
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Construction / Destruction   ///////////////////// //
    
    // Explicit constructors to avoid implicit construction of parameters.

    /// Empty constructor. Name will be set in base
    explicit gs_cci_param()                    : gs_cci_param_t<val_type>("", std::string(""), false, true, false) {  }
    
    /// Constructor with (local/hierarchical) name.
    explicit gs_cci_param(const std::string& nam) : gs_cci_param_t<val_type>(nam, std::string(""), false, true, false) {  }
    explicit gs_cci_param(const char* nam       ) : gs_cci_param_t<val_type>(nam, std::string(""), false, true, false) {  }
    
    /// Constructor with (local/hierarchical) name and string representation of initial value.
    explicit gs_cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name = false)  : gs_cci_param_t<val_type>(nam, val, force_top_level_name, true, true) {  }
    explicit gs_cci_param(const char* nam,        const char* val       , const bool force_top_level_name = false)  : gs_cci_param_t<val_type>(nam, std::string(val), force_top_level_name, true, true) {  }
    explicit gs_cci_param(const std::string& nam, const char* val       , const bool force_top_level_name = false)  : gs_cci_param_t<val_type>(nam, std::string(val), force_top_level_name, true, true) {  }
    explicit gs_cci_param(const char* nam,        const std::string& val, const bool force_top_level_name = false)  : gs_cci_param_t<val_type>(nam, val, force_top_level_name, true, true) {  }
    
    /// Constructor with (local/hierarchical) name and initial value.
    
    // Constructors with register_at_db bool
    explicit gs_cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name, const bool register_at_db) : gs_cci_param_t<val_type>(nam, val, force_top_level_name, register_at_db, true) {  }
    
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
    
    const std::string& json_serialize() const {
      return_string = json_serialize(gs_cci_param_t<val_type>::get());
      return return_string;
    }
    
    void json_deserialize(const std::string& str) {
      val_type t;
      json_deserialize(t, str);
      gs_cci_param_t<val_type>::set(t);
    }
    
    std::string json_serialize(const val_type& val) const {
      return gs::gs_param<val_type>::convertValueToString(val);
    }
    
    void json_deserialize(val_type& target_val, const std::string& str) {
      gs::gs_param<val_type>::deserialize(target_val, str);
    }    
    
    void set_value(const cci_value& val) {
      if (val.type() != get_basic_type()) {
        CCI_THROW_ERROR(cci::cci_report_types::type().cci_value_failure, "Wrong cci value type applied to parameter.");
        CCI_THROW_INFO(cci::cci_report_types::type().cci_value_failure, "  cci value type: "<<val.type()<<" != parameter type: "<<get_basic_type());
      }
      switch (val.type()) {
        case cci::partype_not_available:
          CCI_THROW_WARNING(cci::cci_report_types::type().cci_value_failure, "Applied cci value has no type.");
          break;
        case cci::partype_number:
        case cci::partype_real:
        case cci::partype_bool:
        case cci::partype_list:
        case cci::partype_other:
          CCI_THROW_ERROR(cci::cci_report_types::type().cci_value_failure, "Applied cci value not available for this param type.");
          break;
        case cci::partype_string:
          set(val.get_string());
          break;
        default:
          assert(false && "This should never happen!");
          CCI_THROW_ERROR(cci::cci_report_types::type().cci_value_failure, "Not implemented.");
      }
    }
    
    cci_value get_value() {
      cci_value val(get());
      return val;
    }
    
    const basic_param_type get_basic_type() const { return partype_string; }

  };
  
} // end namespace cci

#endif
