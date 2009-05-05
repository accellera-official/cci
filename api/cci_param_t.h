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


#ifndef __CCI_PARAM_T_h__
#define __CCI_PARAM_T_h__

// included by cci_params.h

namespace cci {


/// Template specialized base class for configuration parameters.
template <typename T>
class cci_param_t 
: public cci_param_base
{
  /// Typedef for this specialized class.
  typedef cci_param_t<T> my_type;

  /// Typedef for the value.
  typedef T val_type;

protected:
  /// Value of this parameter
  val_type my_value;

  /// String whose reference can be returned as string value
  mutable std::string return_string;

public:

  // templated operators
  my_type& operator +=  (val_type);
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

  // templated decrement and increment
  my_type& operator -- ();    // prefix
  val_type operator -- (int); // postfix
  my_type& operator ++ ();    // prefix
  val_type operator ++ (int); // postfix  


  // //////////////////////////////////////////////////////////////////// //
  // //////////////   constructors and destructor   ///////////////////// //


  /// Empty constructor. Name will be set in base. Avoid using it! You MUST call init() after instantiation!
  /**
   */
  explicit cci_param_t()
  : cci_param_base("", true, NULL, false)
  {  }
  
  /// Constructor with the special parameters. Name will be set in base. Avoid using it! You MUST call init() after instantiation!
  /**
   * @param register_at_db  see gs::cnf::cci_param_base, default = true.
   * @param force_top_level_name If the given name n should be a top-level name (then no prefeix is attached to the name),
   *                             default false,
   *                             be carefull in using this.
   */
  explicit cci_param_t(const bool force_top_level_name, const bool register_at_db)
    : cci_param_base("", register_at_db, force_top_level_name)
  {  }

  /// Constructor with (local or hierarchical) name. You MUST call init() after instantiation!
  /**
   * This constructor may be called with local or hierarchical name.
   *
   * Explicit constructor to avoid implicit construction of parameters.
   *
   * cci_param MUST call init() or init(value) after construction to 
   * add the parameter to the plugin database!!!
   *
   * @param nam  The local (or full hierarchical) parameter name (local: not including points) 
   *             (local: unique inside a module, hierarchical: unique in the system).
   */
  explicit cci_param_t(const std::string &nam)
    : cci_param_base(nam, true, NULL, false)
  {  }

  /// Constructor with (local or hierarchical) name and special parameters. You MUST call init() after instantiation!
  /**
   * This constructor may be called with local or hierarchical name.
   *
   * Explicit constructor to avoid implicit construction of parameters.
   *
   * cci_param MUST call init() or init(value) after construction to 
   * add the parameter to the plugin database!!!
   *
   * @param nam  The local (or full hierarchical) parameter name (local: not including points) 
   *             (local: unique inside a module, hierarchical: unique in the system).
   * @param register_at_db  see gs::cnf::cci_param_base, default = true.
   * @param parent_array    see gs::cnf::cci_param_base, default=NULL.
   * @param force_top_level_name If the given name n should be a top-level name (then no prefeix is attached to the name),
   *                             default false,
   *                             be carefull in using this.
   */
  explicit cci_param_t(const std::string &nam, const bool force_top_level_name,
                      const bool register_at_db)
  : cci_param_base(nam, register_at_db, force_top_level_name)
  {  }
  
  /// Init method to set the value and add the parameter to the plugin db.
  /** 
   * Has to be called by the cci_param class after construction!
   * Does set the value and add the parameter to the plugin db.
   */
  void init(const val_type &default_val) {
    CCI_PARAM_DUMP("Init cci_param_t "<< m_par_name.c_str());
    // set my_value
    my_value = default_val;
    // add to plugin database
    if (m_register_at_db) {
      m_api->addPar(this);
    }
  }
  /// Init method without value @see gs::cnf::cci_param_t::init(val_type &default_val)
  void init() {
    CCI_PARAM_DUMP("Init cci_param_t "<< m_par_name.c_str());
    my_value = convertStringToValue(std::string(""));
    // add to plugin database
    if (m_register_at_db) {
      m_api->addPar(this);
    }
  }

  /// Destructor
  virtual ~cci_param_t() {
  }


  // //////////////////////////////////////////////////////////////////// //
  // ///////   set and get with value   ///////////////////////////////// //


  /// Set the value of this parameter to the value of another cci_param.
  /**
   * To resolve the correct = operator in the (not specialized) class
   * which inherits from this class has to use 
   * \code using cci_param_t<val_type>::operator =; \endcode
   *
   * @param v  Parameter where the value should be read from.
   * @return   Pointer to this.
   */
  my_type& operator = (const my_type& v) { 
    setValue(v.getValue()); //m_api->setParam(m_par_name, const_cast<my_type&>(v).get());
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

  /// Set the value of this parameter.
  /**
   * Calls conversion value type --> string.
   *
   * @param val  The new value for this parameter.
   */
  void setValue(const val_type &val) {
    my_value = val;
    makeCallbacks();
  }
  
  /// Returns the value of this parameter.
  /**
   * @return Value
   */
  const val_type& getValue() const {
    return my_value;
  }
  
  /// @see gs::cnf::cci_param_base::get_value_pointer
  const void* get_value_pointer() const{
    return &my_value;
  }

  // //////////////////////////////////////////////////////////////////// //
  // ///////   set and get with string representation   ///////////////// //


  /// Set the value of this parameter with a string.
  /**
   * @param str  The new value for this parameter, represented as a string.
   * @return If setting was successful
   */ 
  bool setString(const std::string &str) {
    bool success = deserialize(my_value, str);
    if (success)
      makeCallbacks();
    return success;
  }
  
  /// Get the value of this parameter as a string.
  /**
   * @return String representation of the current value of this parameter.
   */
  const std::string& getString() const {
    return_string = convertValueToString(my_value);
    return return_string;
  }
  
  // //////////////////////////////////////////////////////////////////// //
  // ///////   virtual conversion methods string <-> value   //////////// //


  /// Conversion method value type --> string. To be implemented by the specialization.
  /**
   * May not make use of m_par_name because it is called inside constructor!
   *
   * @param val  Value that should be converted to string.
   * @return String representation of the value.
   */
  virtual std::string convertValueToString(const val_type &val) const = 0;

  /// Deserialize for this parameter. To be implemented by the specialization.
  /**
   * Conversion string --> value type.
   *
   * User implemented for each template specialization of cci_param:
   * Do not write to target_val if deserialization fails!
   *
   * Set target_val to the default value if str is empty (=="").
   *
   * Do not use ANY member variables. Act as if the method is static.
   *
   * @param  target_val  Reference to the value that should be set.
   * @param  str         String that should be converted to a value.
   * @return If the convertion was successfull
   */
  virtual const bool deserialize(val_type &target_val, const std::string& str) = 0;

protected:
  /// Get the value the string. Needed for construction of cci_param.
  /**
   * Conversion string --> value type.
   *
   * @param  str String that should be converted to a value
   * @return If the convert was successfull
   */
  const val_type& convertStringToValue(const std::string& str) {    
    deserialize(my_value, str);                   
    return my_value;                              
  }                                                                 

};


} // end namespace cci

#endif
