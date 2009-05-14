//   OSCI CCI WG
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// 
// ENDLICENSETEXT


#ifndef __GS_CCI_PARAM_H__
#define __GS_CCI_PARAM_H__


#include <string>
#include <iostream>
#include <sstream>

#include <systemc>


namespace cci {

  template<typename T>
  class gs_cci_param 
  : public gs_cci_param_base
  , public cci_param<T>
  {
  public: // protected:
    /// Typedef for the value.
    typedef T val_type;
    /// Typedef for the param itself.
    typedef gs_cci_param<T> my_type;
    using gs_cci_param_base::get_name;
  public:

    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Construction / Destruction   ///////////////////// //

    // Explicit constructors to avoid implicit construction of parameters.
    
    /// Empty constructor. Name will be set in base
    explicit gs_cci_param()                    : gs_cci_param_base("") { init(); }
    explicit gs_cci_param(const val_type& val) : gs_cci_param_base("") { init(); }
    
    /// Constructor with (local/hierarchical) name.
    explicit gs_cci_param(const std::string& nam) : gs_cci_param_base(nam) { init(); }
    explicit gs_cci_param(const char* nam       ) : gs_cci_param_base(nam) { init(); }
    
    /// Constructor with (local/hierarchical) name and string representation of initial value.
    explicit gs_cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name = false)  : gs_cci_param_base(nam, true, force_top_level_name) { init(); } 
    explicit gs_cci_param(const char* nam,        const char* val       , const bool force_top_level_name = false)  : gs_cci_param_base(nam, true, force_top_level_name) { init(); } 
    explicit gs_cci_param(const std::string& nam, const char* val       , const bool force_top_level_name = false)  : gs_cci_param_base(nam, true, force_top_level_name) { init(); } 
    explicit gs_cci_param(const char* nam,        const std::string& val, const bool force_top_level_name = false)  : gs_cci_param_base(nam, true, force_top_level_name) { init(); } 
    
    /// Constructor with (local/hierarchical) name and initial value.
    explicit gs_cci_param(const std::string& nam, const val_type& val, const bool force_top_level_name = false) : gs_cci_param_base(nam, true, force_top_level_name)   { init(val); }    
    explicit gs_cci_param(const char* nam,        const val_type& val, const bool force_top_level_name = false) : gs_cci_param_base(nam, true, force_top_level_name)   { init(val); }    
    
    // Constructors with register_at_db bool
    explicit gs_cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name, const bool register_at_db) : gs_cci_param_base(nam, register_at_db, force_top_level_name)  { init();         } 

    ~gs_cci_param() {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
    }
    
    void init(const val_type& default_val) {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
    }
    
    void init() {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
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
       
    void set(const val_type& val) {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
    }
    
    const val_type& get() const {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
      return my_value;
    }
    
    const std::string& json_serialize() const {
      serialize(return_string);
      return return_string;
    }
    
    bool json_deserialize(const std::string& str) {
      return deserialize(my_value, str);
    }

    std::string serialize(const val_type& val) const {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
      return std::string("default");
    }

    const bool deserialize(val_type& target_val, const std::string& str) {
      SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "not implemented");
      return false;
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
    
  protected:
    my_type my_value;

    /// String whose reference can be returned as string value
    mutable std::string return_string;
    
  };
      
} // end namespace cci

#endif
