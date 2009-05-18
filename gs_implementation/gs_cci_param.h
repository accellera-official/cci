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


#ifndef __GS_CCI_PARAM_H__
#define __GS_CCI_PARAM_H__


#include <string>
#include <iostream>
#include <sstream>

#include <systemc>


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
    explicit gs_cci_param()                    : gs_cci_param_t<val_type>("") {  }
    explicit gs_cci_param(const val_type& val) : gs_cci_param_t<val_type>("") {  }
    
    /// Constructor with (local/hierarchical) name.
    explicit gs_cci_param(const std::string& nam) : gs_cci_param_t<val_type>(nam) {  }
    explicit gs_cci_param(const char* nam       ) : gs_cci_param_t<val_type>(nam) {  }
    
    /// Constructor with (local/hierarchical) name and string representation of initial value.
    explicit gs_cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name = false)  : gs_cci_param_t<val_type>(nam, force_top_level_name, true) {  }
    explicit gs_cci_param(const char* nam,        const char* val       , const bool force_top_level_name = false)  : gs_cci_param_t<val_type>(nam, force_top_level_name, true) {  }
    explicit gs_cci_param(const std::string& nam, const char* val       , const bool force_top_level_name = false)  : gs_cci_param_t<val_type>(nam, force_top_level_name, true) {  }
    explicit gs_cci_param(const char* nam,        const std::string& val, const bool force_top_level_name = false)  : gs_cci_param_t<val_type>(nam, force_top_level_name, true) {  }
    
    /// Constructor with (local/hierarchical) name and initial value.
    explicit gs_cci_param(const std::string& nam, const val_type& val, const bool force_top_level_name = false) : gs_cci_param_t<val_type>(nam, force_top_level_name, true)   {  }
    explicit gs_cci_param(const char* nam,        const val_type& val, const bool force_top_level_name = false) : gs_cci_param_t<val_type>(nam, force_top_level_name, true)   {  }
    
    // Constructors with register_at_db bool
    explicit gs_cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name, const bool register_at_db) : gs_cci_param_t<val_type>(nam, force_top_level_name, register_at_db)  {  }

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
      // TODO
      return_string = json_serialize(gs_cci_param_t<val_type>::get());
      return return_string;
    }
    
    bool json_deserialize(const std::string& str) {
      // TODO
      val_type t;
      bool ret = json_deserialize(t, str);
      if (ret) gs_cci_param_t<val_type>::set(t);
      return ret;
    }

    std::string json_serialize(const val_type& val) const {
      return gs::gs_param<val_type>::convertValueToString(val);
    }

    const bool json_deserialize(val_type& target_val, const std::string& str) {
      return gs::gs_param<val_type>::deserialize(target_val, str);
    }

    
    /* To be implemented in specializations
     const Param_JSON_type get_JSON_type() const { return partype_not_available; }
    bool set_number(const long long value)   { return false; }
    bool set_double(const double value)      { long long llval = (long long)value; return set_number(llval); }
    bool set_string(const std::string value) { return false; }
    bool set_bool(const bool value)          { return false; }
    bool get_number(long long& retvalue)     { return false; }
    bool get_double(double& retvalue)        { long long llval; bool res=set_number(llval); if (res) retval=(double)llval; return res; }
    bool get_string(std::string& retvalue)   { return false; }
    bool get_bool(bool& retvalue)            { return false; }
     */
    
    
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
  private:   
    /// Empty constructor. Name will be set in base
    explicit gs_cci_param()                    : gs_cci_param_t<val_type>("") {  }
  public:
    
    /// Constructor with (local/hierarchical) name.
    explicit gs_cci_param(const std::string& nam) : gs_cci_param_t<val_type>(nam) {  }
    explicit gs_cci_param(const char* nam       ) : gs_cci_param_t<val_type>(nam) {  }
    
    /// Constructor with (local/hierarchical) name and string representation of initial value.
    explicit gs_cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name = false)  : gs_cci_param_t<val_type>(nam, force_top_level_name, true) {  }
    explicit gs_cci_param(const char* nam,        const char* val       , const bool force_top_level_name = false)  : gs_cci_param_t<val_type>(nam, force_top_level_name, true) {  }
    explicit gs_cci_param(const std::string& nam, const char* val       , const bool force_top_level_name = false)  : gs_cci_param_t<val_type>(nam, force_top_level_name, true) {  }
    explicit gs_cci_param(const char* nam,        const std::string& val, const bool force_top_level_name = false)  : gs_cci_param_t<val_type>(nam, force_top_level_name, true) {  }
    
    // Constructors with register_at_db bool
    explicit gs_cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name, const bool register_at_db) : gs_cci_param_t<val_type>(nam, force_top_level_name, register_at_db)  {  }
    
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
    
    bool json_deserialize(const std::string& str) {
      val_type t;
      bool ret = json_deserialize(t, str);
      if (ret) gs_cci_param_t<val_type>::set(t);
      return ret;
    }
    
    std::string json_serialize(const val_type& val) const {
      return gs::gs_param<val_type>::convertValueToString(val);
    }
    
    const bool json_deserialize(val_type& target_val, const std::string& str) {
      return gs::gs_param<val_type>::deserialize(target_val, str);
    }    
    
    /* To be implemented in specializations
     const Param_JSON_type get_JSON_type() const { return partype_not_available; }
     bool set_number(const long long value)   { return false; }
     bool set_double(const double value)      { long long llval = (long long)value; return set_number(llval); }
     bool set_string(const std::string value) { return false; }
     bool set_bool(const bool value)          { return false; }
     bool get_number(long long& retvalue)     { return false; }
     bool get_double(double& retvalue)        { long long llval; bool res=set_number(llval); if (res) retval=(double)llval; return res; }
     bool get_string(std::string& retvalue)   { return false; }
     bool get_bool(bool& retvalue)            { return false; }
     */
    
    
  };
  
} // end namespace cci

#endif
