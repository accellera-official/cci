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
  : public cci_param_base
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
    explicit cci_param();
    explicit cci_param(const val_type& val);
    
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
     * @param v  Parameter where the value should be read from.
     * @return   Pointer to this.
     */
    virtual my_type& operator = (const my_type& v) { 
      set(v.get());
      return *this;
    }
    
    /// Set the value of this parameter.
    /**
     * @param v  Value which has to be set.
     * @return   Pointer to this.
     */
    virtual my_type& operator = (const val_type& v) { 
      set(v);
      return *this;
    }
    
    /// Get the value of this parameter.
    /**
     * @return Value of the parameter.
     */
    virtual operator const val_type& () const { 
      return get(); 
    }
    
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Set and Get with Value   ///////////////////////// //

    
    /// Set the value of this parameter.
    /**
     * @param val  The new value for this parameter.
     */
    virtual void set(const val_type& val) = 0;
    
    /// Returns the value of this parameter.
    /**
     * @return Value
     */
    virtual const val_type& get() const = 0;
    
    
    // //////////////////////////////////////////////////////////////////// //
    // //////////   Conversion Methods String <-> Value   ///////////////// //
    
    
    /// Conversion value type --> string. To be implemented by the specialization.
    /**
     * May not make use of m_par_name because it is called inside constructor!
     *
     * @param val  Value that should be converted to string.
     * @return String representation of the value.
     */
    virtual std::string serialize(const val_type& val) const = 0;

    /// Convertion string --> value type
    /**
     * Implemented for each template specialization of cci_param:
     *
     * Guidelines:
     * - Do not write to target_val if deserialization fails!
     * - Set target_val to the default value if str is empty (=="").
     *
     * @param  target_val  Reference to the value that should be set.
     * @param  str         String that should be converted to a value.
     * @return If the convertion was successfull
     */
    virtual const bool deserialize(val_type& target_val, const std::string& str) = 0;

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
