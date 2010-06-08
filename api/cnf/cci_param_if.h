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



#ifndef __CCI_PARAM_IF_H__
#define __CCI_PARAM_IF_H__


#include <string>
#include <iostream>
#include <sstream>

#include <systemc>


__CCI_OPEN_CONFIG_NAMESPACE__

  
  /// Compiler Check Interface for the parameters class
  template<typename T, param_mutable_type TM>
  class cci_param_if 
  : virtual public cci_base_param_if
  {
  /*protected:
    /// Typedef for the value.
    typedef T val_type;
    /// Typedef for the param itself.
    typedef cci_param<T, TM> my_type;*/

  public:

    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Construction / Destruction   ///////////////////// //

    // Constructors shall be provided by the derived class:
/*    
    /// Empty constructor. Name will be set in base
    explicit cci_param(); // TODO: make private?
    explicit cci_param(const val_type& val); // TODO: allow this one?
    
    /// Constructor with (local/hierarchical) name.
    explicit cci_param(const std::string& nam);
    explicit cci_param(const char* nam       );
    
    /// Constructor with (local/hierarchical) name and string representation of initial value.
    explicit cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name = false);
    explicit cci_param(const char* nam,        const char* val       , const bool force_top_level_name = false);
    explicit cci_param(const std::string& nam, const char* val       , const bool force_top_level_name = false);
    explicit cci_param(const char* nam,        const std::string& val, const bool force_top_level_name = false);
    
    /// Constructor with (local/hierarchical) name and initial value.
    explicit cci_param(const std::string& nam, const val_type& val, const bool force_top_level_name = false);
    explicit cci_param(const char* nam,        const val_type& val, const bool force_top_level_name = false);
    
    // Constructors with register_at_db bool
    explicit cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name, const bool register_at_db);
*/
    
    /// Destructor
    virtual ~cci_param_if() { };
    

    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Set and Get Operators   ////////////////////////// //

    
    /// Set the value of this parameter to the value of another cci_param.
    /**
     * @exception cci::cnf::cci_report_types::set_param_failed Setting value failed
     * @param v  Parameter where the value should be read from.
     * @return   Pointer to this.
     */
    virtual cci_param<T, TM>& operator = (const cci_param<T, TM>& v) = 0;
    /* { 
      set(v.get());
      return *this;
    }*/
    
    /// Set the value of this parameter.
    /**
     * @exception cci::cnf::cci_report_types::set_param_failed Setting value failed
     * @param v  Value which has to be set.
     * @return   Pointer to this.
     */
    virtual cci_param<T, TM>& operator = (const T& v) = 0;
    /* { 
      set(v);
      return *this;
    }*/
    
    /// Get the value of this parameter.
    /**
     * @exception cci::cnf::cci_report_types::get_param_failed Getting value failed
     * @return Value of the parameter.
     */
    virtual operator const T& () const = 0;/* { 
      return get(); 
    }*/
    
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Set and Get with Value   ///////////////////////// //

    
    /// Set the value of this parameter.
    /**
     * @exception cci::cnf::cci_report_types::set_param_failed Setting value failed
     * @param val  The new value for this parameter.
     */
    virtual void set(const T& val) = 0;

    /// Returns the value of this parameter.
    /**
     * @exception cci::cnf::cci_report_types::get_param_failed Getting value failed
     * @return Value
     */
    virtual const T& get() const = 0;
    
    /// Set the value of this parameter overriding a lock.
    /**
     * @exception cci::cnf::cci_report_types::set_param_failed Setting value failed
     * @param val  The new value for this parameter.
     * @param lock_pwd  Password needed for the lock (if needed, else NULL)
     */
    virtual void set(const T& val, void* lock_pwd) = 0;
    
    
    // //////////////////////////////////////////////////////////////////// //
    // //////////   Conversion Methods JSON string <-> Value   //////////// //

    // These functions allow access to the conversion even without changing
    // the parameter.
    
    
    /// Conversion value type --> JSON string. (Not using the parameter value as source)
    /**
     * Should not make use of m_par_name because it is possibly called inside constructor!
     *
     * Direct parameter access @see cci_base_param::json_serialize
     *
     * @exception cci::cnf::cci_report_types::get_param_failed Converting value failed
     * @param val  Value that should be converted.
     * @return JSON string representation of the value.
     */
    virtual std::string json_serialize(const T& val) const = 0;

    /// Convertion JSON string --> value type (without affecting the parameter value directly)
    /**
     * Direct parameter access @see cci_base_param::json_deserialize
     *
     * Guidelines:
     * - Do not write to target_val if deserialization fails!
     * - Set target_val to the default value if str is empty (=="").
     *
     * @exception cci::cnf::cci_report_types::set_param_failed Converting value failed
     * @param  target_val  Reference to the value that should be set.
     * @param  str         JSON string that should be converted to a value.
     */
    virtual void json_deserialize(T& target_val, const std::string& str) = 0;
    
  };
    
  //template<class T, param_mutable_type TM> bool operator == (cci_param<T, TM>& p1, cci_param<T, TM>& p2);
  /* {
    return p1.get() == p2.getValue();
  }*/

  //template<class T, param_mutable_type TM> bool operator == (cci_param<T, TM>& p1, T& p2);
  /* {
    return p1.get() == p2;
  }*/

  //template<class T, param_mutable_type TM> bool operator == (T& p1, cci_param<T, TM>& p2);
  /* {
    return p1 == p2.get();
  }*/
  
  
__CCI_CLOSE_CONFIG_NAMESPACE__

#endif
