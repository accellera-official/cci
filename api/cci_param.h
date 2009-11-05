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


#ifndef __CCI_PARAM_H__
#define __CCI_PARAM_H__


#include <string>
#include <iostream>
#include <sstream>

#include <systemc>


namespace cci {

  /// The parameters class
  template<typename T>
  class cci_param 
  : public cci_base_param
  {
  protected:
    /// Typedef for the value.
    typedef T val_type;
    /// Typedef for the param itself.
    typedef cci_param<T> my_type;

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
     * @exception cci_exception_set_param Setting value failed
     * @param v  Parameter where the value should be read from.
     * @return   Pointer to this.
     */
    virtual my_type& operator = (const my_type& v) { 
      set(v.get());
      return *this;
    }
    
    /// Set the value of this parameter.
    /**
     * @exception cci_exception_set_param Setting value failed
     * @param v  Value which has to be set.
     * @return   Pointer to this.
     */
    virtual my_type& operator = (const val_type& v) { 
      set(v);
      return *this;
    }
    
    /// Get the value of this parameter.
    /**
     * @exception cci_exception_get_param Getting value failed
     * @return Value of the parameter.
     */
    virtual operator const val_type& () const { 
      return get(); 
    }
    
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Set and Get with Value   ///////////////////////// //

    
    /// Set the value of this parameter.
    /**
     * @exception cci_exception_set_param Setting value failed
     * @param val  The new value for this parameter.
     */
    virtual void set(const val_type& val) = 0;
    
    /// Returns the value of this parameter.
    /**
     * @exception cci_exception_get_param Getting value failed
     * @return Value
     */
    virtual const val_type& get() const = 0;
    
    
    // //////////////////////////////////////////////////////////////////// //
    // //////////   Conversion Methods JSON string <-> Value   //////////// //

    // These functions allow access to the conversion even without changing
    // the parameter.
    
    
    /// Conversion value type --> JSON string. To be implemented by the specialization.
    /**
     * Should not make use of m_par_name because it is possibly called inside constructor!
     *
     * @exception cci_exception_get_param Converting value failed
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
     * @exception cci_exception_set_param Converting value failed
     * @param  target_val  Reference to the value that should be set.
     * @param  str         JSON string that should be converted to a value.
     */
    virtual void json_deserialize(val_type& target_val, const std::string& str) = 0;

  };
    
  template<class T> bool operator == (cci_param<T>& p1, cci_param<T>& p2) {
    return p1.get() == p2.getValue();
  }

  template<class T> bool operator == (cci_param<T>& p1, T& p2) {
    return p1.get() == p2;
  }

  template<class T> bool operator == (T& p1, cci_param<T>& p2) {
    return p1 == p2.get();
  }
  
  
} // end namespace cci

#endif
