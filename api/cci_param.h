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



#ifndef __CCI_PARAM_H__
#define __CCI_PARAM_H__


#include <string>
#include <iostream>
#include <sstream>

#include <systemc>


namespace cci {

  
  /// The parameters class
  template<typename T, param_mutable_type TM>
  class cci_param 
  : public cci_base_param
  {
  protected:
    /// Typedef for the value.
    typedef T val_type;
    /// Typedef for the param itself.
    typedef cci_param<T, TM> my_type;

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
    virtual ~cci_param() { };
    

    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Set and Get Operators   ////////////////////////// //

    
    /// Set the value of this parameter to the value of another cci_param.
    /**
     * @exception cci::cci_report_types::set_param_failed Setting value failed
     * @param v  Parameter where the value should be read from.
     * @return   Pointer to this.
     */
    virtual my_type& operator = (const my_type& v) { 
      set(v.get());
      return *this;
    }
    
    /// Set the value of this parameter.
    /**
     * @exception cci::cci_report_types::set_param_failed Setting value failed
     * @param v  Value which has to be set.
     * @return   Pointer to this.
     */
    virtual my_type& operator = (const val_type& v) { 
      set(v);
      return *this;
    }
    
    /// Get the value of this parameter.
    /**
     * @exception cci::cci_report_types::get_param_failed Getting value failed
     * @return Value of the parameter.
     */
    virtual operator const val_type& () const { 
      return get(); 
    }
    
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Set and Get with Value   ///////////////////////// //

    
    /// Set the value of this parameter.
    /**
     * @exception cci::cci_report_types::set_param_failed Setting value failed
     * @param val  The new value for this parameter.
     */
    virtual void set(const val_type& val) = 0;

    /// Returns the value of this parameter.
    /**
     * @exception cci::cci_report_types::get_param_failed Getting value failed
     * @return Value
     */
    virtual const val_type& get() const = 0;
    
    /// Set the value of this parameter overriding a lock.
    /**
     * @exception cci::cci_report_types::set_param_failed Setting value failed
     * @param val  The new value for this parameter.
     * @param lock_pwd  Password needed for the lock (if needed, else NULL)
     */
    virtual void set(const val_type& val, void* lock_pwd) = 0;
    
    
    // //////////////////////////////////////////////////////////////////// //
    // //////////   Conversion Methods JSON string <-> Value   //////////// //

    // These functions allow access to the conversion even without changing
    // the parameter.
    
    
    /// Conversion value type --> JSON string. To be implemented by the specialization.
    /**
     * Should not make use of m_par_name because it is possibly called inside constructor!
     *
     * @exception cci::cci_report_types::get_param_failed Converting value failed
     * @param val  Value that should be converted.
     * @return JSON string representation of the value.
     */
    virtual std::string json_serialize(const val_type& val) const = 0;

    /// Convertion JSON string --> value type
    /**
     * Implemented for each template specialization of cci_param:
     *
     * Guidelines:
     * - Do not write to target_val if deserialization fails!
     * - Set target_val to the default value if str is empty (=="").
     *
     * @exception cci::cci_report_types::set_param_failed Converting value failed
     * @param  target_val  Reference to the value that should be set.
     * @param  str         JSON string that should be converted to a value.
     */
    virtual void json_deserialize(val_type& target_val, const std::string& str) = 0;

  };
    
  template<class T, param_mutable_type TM> bool operator == (cci_param<T, TM>& p1, cci_param<T, TM>& p2) {
    return p1.get() == p2.getValue();
  }

  template<class T, param_mutable_type TM> bool operator == (cci_param<T, TM>& p1, T& p2) {
    return p1.get() == p2;
  }

  template<class T, param_mutable_type TM> bool operator == (T& p1, cci_param<T, TM>& p2) {
    return p1 == p2.get();
  }
  
  
} // end namespace cci

#endif
