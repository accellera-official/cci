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
#include "cci_cnf_api.h"
#include "cci_param_base.h"


namespace cci {

  /// The parameters class
  template<typename T>
  class cci_param 
  : public cci_param_base
  {
  public:

    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Construction / Destruction   ///////////////////// //

    // Explicit constructors to avoid implicit construction of parameters.
    
    /// Empty constructor. Name will be set in base
    explicit cci_param()                    : cci_param_t<val_type>(false, true) { cci_param_t<val_type>::init(); }
    explicit cci_param(const val_type& val) : cci_param_t<val_type>(false, true) { cci_param_t<val_type>::init(); }
    
    /// Constructor with (local/hierarchical) name.
    explicit cci_param(const std::string& nam) : cci_param_t<val_type>(nam             , false, true) { cci_param_t<val_type>::init(); }
    explicit cci_param(const char* nam       ) : cci_param_t<val_type>(std::string(nam), false, true) { cci_param_t<val_type>::init(); }
    
    /// Constructor with (local/hierarchical) name and string representation of initial value.
    explicit cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name = false) : cci_param_t<val_type>(nam             , force_top_level_name, true ) { cci_param_t<val_type>::init(convertStringToValue(val));         } 
    explicit cci_param(const char* nam,        const char* val       , const bool force_top_level_name = false) : cci_param_t<val_type>(std::string(nam), force_top_level_name, true ) { cci_param_t<val_type>::init(convertStringToValue(std::string(val))); } 
    explicit cci_param(const std::string& nam, const char* val       , const bool force_top_level_name = false) : cci_param_t<val_type>(nam             , force_top_level_name, true ) { cci_param_t<val_type>::init(convertStringToValue(std::string(val))); } 
    explicit cci_param(const char* nam,        const std::string& val, const bool force_top_level_name = false) : cci_param_t<val_type>(std::string(nam), force_top_level_name, true ) { cci_param_t<val_type>::init(convertStringToValue(val));         } 
    
    /// Constructor with (local/hierarchical) name and initial value.
    explicit cci_param(const std::string& nam, const val_type& val, const bool force_top_level_name = false) : cci_param_t<val_type>(nam             , force_top_level_name, true )  { cci_param_t<val_type>::init(val); }    
    explicit cci_param(const char* nam,        const val_type& val, const bool force_top_level_name = false) : cci_param_t<val_type>(std::string(nam), force_top_level_name, true )  { cci_param_t<val_type>::init(val); }    
    
    // Constructors with register_at_db bool
    explicit cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name, const bool register_at_db) : cci_param_t<val_type>(nam, force_top_level_name, register_at_db) { cci_param_t<val_type>::init(convertStringToValue(val));         } 
    
    /// Destructor
    virtual ~cci_param() { cci_param_t<val_type>::destruct_cci_param(); }
    
    /// Init method to set the value and add the parameter to the plugin db.
    /** 
     * Has to be called by the cci_param class after construction!
     * Does set the value and add the parameter to the plugin db.
     */
    void init(const val_type& default_val) = 0;
    
    /// Init method without value @see init(val_type &default_val)
    void init() = 0;

    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////////////   Operators   ////////////////////////////// //

    
    // templated operators
    virtual my_type& operator +=  (val_type);
    virtual my_type& operator -=  (val_type);
    virtual my_type& operator /=  (val_type);
    virtual my_type& operator *=  (val_type);
    virtual val_type operator +  (val_type);
    virtual val_type operator -  (val_type);
    virtual val_type operator /  (val_type);
    virtual val_type operator *  (val_type);
    virtual my_type& operator %=  (val_type);
    virtual my_type& operator ^=  (val_type);
    virtual my_type& operator &=  (val_type);
    virtual my_type& operator |=  (val_type);
    virtual my_type& operator <<= (val_type);
    virtual my_type& operator >>= (val_type);
    
    // templated decrement and increment
    virtual my_type& operator -- ();    // prefix
    virtual val_type operator -- (int); // postfix
    virtual my_type& operator ++ ();    // prefix
    virtual val_type operator ++ (int); // postfix  
    
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Set and Get Operators   ////////////////////////// //

    
    /// Set the value of this parameter to the value of another cci_param.
    /**
     * To resolve the correct = operator in the (not specialized) class
     * which inherits from this class has to use 
     * \code using cci_param_t<val_type>::operator =; \endcode
     *
     * @param v  Parameter where the value should be read from.
     * @return   Pointer to this.
     */
    virtual my_type& operator = (const my_type& v) { 
      setValue(v.getValue());
      return *this;
    }
    
    /// Set the value of this parameter.
    /**
     * To resolve the correct = operator in the (not specialized) class
     * which inherits from this class has to use 
     * \code using cci_param_t<val_type>::operator =; \endcode
     *
     * @param v  Value which has to be set.
     * @return   Pointer to this.
     */
    my_type& operator = (const val_type& v) { 
      setValue(v);
      return *this;
    }
    
    /// Get the value of this parameter.
    /**
     * @return Value of the parameter.
     */
    operator const val_type& () const { 
      return getValue(); 
    }
    
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Set and Get with Value   ///////////////////////// //

    
    /// Set the value of this parameter.
    /**
     * Calls conversion value type --> string.
     *
     * @param val  The new value for this parameter.
     */
    virtual void set_value(const val_type& val) = 0;
    
    /// Returns the value of this parameter.
    /**
     * @return Value
     */
    virtual const val_type& get_value() const = 0;
    
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////   Conversion Methods String <-> Value   //////////////////// //
    
    
    /// Conversion method value type --> string. To be implemented by the specialization.
    /**
     * May not make use of m_par_name because it is called inside constructor!
     *
     * @param val  Value that should be converted to string.
     * @return String representation of the value.
     */
    virtual std::string serialize(const val_type& val) const = 0;

    /// Deserialize for this parameter. Calls the static_deserialize function
    virtual const bool deserialize(val_type& target_val, const std::string& str) { return static_deserialize(target_val, str); }
    
    /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
    /**
     * Conversion string --> value type.
     *
     * User implemented for each template specialization of cci_param:
     * Do not write to target_val if deserialization fails!
     *
     * Set target_val to the default value if str is empty (=="").
     *
     * @param  target_val  Reference to the value that should be set.
     * @param  str         String that should be converted to a value.
     * @return If the convertion was successfull
     */
    virtual static bool static_deserialize(val_type& target_val, const std::string& str) = 0;

  };
    
  template<class T> bool operator == (cci_param<T>& p1, cci_param<T>& p2) {
    return p1.getValue() == p2.getValue();
  }

  template<class T> bool operator == (cci_param<T>& p1, T& p2) {
    return p1.getValue() == p2;
  }

  template<class T> bool operator == (T& p1, cci_param<T>& p2) {
    return p1 == p2.getValue();
  }
  
  
} // end namespace cci

#endif
