//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007-2010 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//   Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// doxygen comments

#ifndef __SCML_PROPERTY_H__
#define __SCML_PROPERTY_H__

#include <systemc>
#include <string>
#include <iostream>

#include "boost/shared_ptr.hpp"

#include "scml_api.h"
#include "operator_macros.h"

// //////////////////////////////////////////////////////////////////////////// //

// TODO: IF_VERBOSE applys to GC_TRACE, not GCNF_DUMP_N

namespace gs {
namespace cnf {

/// Base class for scml_property parameters.
/**
 * Has functions which are not type dependent.
 */
class gcnfScmlProperty_base
: public sc_object
{

public:
  virtual ~gcnfScmlProperty_base();

  virtual          int getIntValue()    const;
  virtual unsigned int getUIntValue()   const;
  virtual         bool getBoolValue()   const;
  virtual       double getDoubleValue() const;
  virtual  std::string getStringValue() const;
  
  virtual  std::string getType()        const;

  virtual void    setIntValue(int          val);
  virtual void   setBoolValue(bool         val);
  virtual void setDoubleValue(double       val);
  virtual void setStringValue(const        std::string &val);
  virtual void   setUIntValue(unsigned int val);

  std::string getName() const;

  /// Exception class for wrong types in scml_properties.
  class WrongTypeException {
  public:
    WrongTypeException(const string &bad_type,const string &good_type)
      : m_bad_type(bad_type), 
        m_good_type(good_type)
    {  }
    
    std::string getBadType() const  { return m_bad_type; }
    
    std::string getGoodType() const { return m_good_type; }
    
    std::string getDescription() const {
      std::ostringstream sstream;
      sstream << "Incorrect type (" << m_bad_type << ") for scml_property<"
              << m_good_type << ">";
      return sstream.str();
    }
    
  private:
    std::string m_bad_type;
    std::string m_good_type;
  };
  
protected:
  explicit gcnfScmlProperty_base(const std::string& par_name);
  
  const char * name() {
#if SYSTEMC_API == 210
    sc_core::sc_module *mod = dynamic_cast<sc_module*>(this->get_parent());
#else
    sc_core::sc_module *mod = dynamic_cast<sc_module*>(this->get_parent_object());
#endif
    if (mod == NULL)
      SC_REPORT_ERROR("scml_property", "Parent module is not a sc_core::sc_module!");
    const char* n = mod->name();
    return n;
    //return "TODOscml_property";//(dynamic_cast<sc_core::sc_object*>(this))->name();
  }

  std::string m_local_par_name;
  std::string m_full_par_name;
  shared_ptr<Scml_Api> m_api;  // Pointer to Scml API

};


// //////////////////////////////////////////////////////////////////////////// //

/// Base class for scml_property parameters, templated.
/**
 * Has the type dependent functions.
 */
template<typename T>
class gcnfScmlProperty_tbase 
: public gcnfScmlProperty_base
{

public:
  typedef T val_type;
  typedef gcnfScmlProperty_tbase<val_type> my_type;
  typedef gcnfScmlProperty_base::WrongTypeException WrongTypeException;
  
  // templated operators
  my_type& operator +=  (val_type);
  my_type& operator -=  (val_type);
  my_type& operator /=  (val_type);
  my_type& operator *=  (val_type);
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
  
  // templated
  operator T() const;
  const T& getValue() const;
  T&       getValue();
    
protected:
  explicit gcnfScmlProperty_tbase(const std::string& par_name);
  explicit gcnfScmlProperty_tbase(const std::string& par_name, const val_type& init_val);
  virtual ~gcnfScmlProperty_tbase();

protected:
  val_type m_ret_value; // parameter itself doesn't hold the value
  
};

} // end namespace cnf
} // end namespace gs

#ifdef ENABLE_SCML_NAMESPACE
  namespace gs {
  namespace cnf {
#else
  using gs::cnf::gcnfScmlProperty_tbase;
  using gs::cnf::gcnfScmlProperty_base;
#endif

/// scml_property class, templated.
/**
 * GreenControl version of CoWare's scml_property.
 *
 * Each supported data type has a template specialisation.
 * See specialisations.
 *
 * Enable namespace to match GreenControl conventions, 
 * disable if needed to be linked as original scml_property
 * (which originally is in no namespace)
 *
 * \see scml_property<std::string>
 * \see scml_property<int>
 * \see scml_property<unsigned int>
 * \see scml_property<bool>
 * \see scml_property<double>
 */
template<typename T>
class scml_property 
{
};


// //////////////////////////////////////////////////////////////////////////// //
/**
 * gcnfScmlProperty_tbase implementations (inline)
 */

/// Constructor(par_name)
template<typename T>
inline gcnfScmlProperty_tbase<T>::gcnfScmlProperty_tbase(const std::string& par_name) 
  : gcnfScmlProperty_base(par_name) 
{
  GCNF_DUMP_N(name(), "gcnfScmlProperty_tbase<T>("<<par_name.c_str()<<") constructor");

  sc_core::sc_module *mod = dynamic_cast<sc_module*>(this->get_parent());
  if (!mod) {
    SC_REPORT_ERROR(name(), "gcnfScmlProperty_tbase: parent is no module!");
  }
  this->m_api = Scml_Api::getApiInstance(mod);  // get only existing API instance
  std::stringstream ss;
  ss << name() << SC_NAME_DELIMITER << par_name;
  this->m_api->addParam(ss.str().c_str());    // add this parameter to the API
}

/// Constructor(par_name, init_value)
template<typename T>
inline gcnfScmlProperty_tbase<T>::gcnfScmlProperty_tbase(const std::string& par_name, 
                                                     const val_type& init_val) 
  : gcnfScmlProperty_base(par_name) 
{
  std::stringstream val_ss;
  val_ss << init_val;

  GCNF_DUMP_N(name(), "gcnfScmlProperty_tbase<T>("<<par_name.c_str()<<", "<<val_ss.str().c_str()<<") constructor");

  sc_core::sc_module *mod = dynamic_cast<sc_module*>(this->get_parent());
  if (!mod) {
    SC_REPORT_ERROR(name(), "gcnfScmlProperty_tbase: parent is no module!");
  }
  this->m_api = Scml_Api::getApiInstance(mod);  // get only existing API instance
  IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR(name(), "m_api is NULL. Error in constructing this instance."); } );
  std::stringstream ss;
  ss << name() << SC_NAME_DELIMITER << par_name;
  this->m_api->addParam(ss.str().c_str(), val_ss.str().c_str());    // add this parameter to the API
}

/// Destructor
template<typename T>
inline gcnfScmlProperty_tbase<T>::~gcnfScmlProperty_tbase()
{
}



// //////////////////////////////////////////////////////////////////////////// //
// template specializations for scml_property<type>

// TODO in all specialisations:    const T& getValue() const;
//                             and       T& getValue();



/// Template specialization scml_property<int>.
/**
 * Template specialization scml_property<int>.
 */
template<>
class scml_property<int>
: public gcnfScmlProperty_tbase<int>
{
public:

  /// Type of the saved value.
  typedef int                val_type;
  /// Type of this object.
  typedef scml_property<int> my_type;

  SPECIALISATIONS_EQUAL_OPERATORS;

  /// Constructor without default value.
  /**
   * @param par_name Name of the parameter.
   */
  explicit scml_property(const ::std::string& par_name)
    : gcnfScmlProperty_tbase<int>(par_name)
  {
    GCNF_DUMP_N(name(), "scml_property<int>("<<par_name.c_str()<<") constructor");
  }

  /// Constructor with default value.
  /**
   * @param par_name    Name of the parameter.
   * @param init_val    Initial value of the parameter.
   */
  scml_property(const std::string& par_name, val_type init_val)
    : gcnfScmlProperty_tbase<int>(par_name, init_val)
  {
    GCNF_DUMP_N(name(), "scml_property<int>("<<par_name.c_str()<<", "<<init_val<<") constructor");
  }

  /// Destructor.
  // TODO: original virtual
  //virtual ~scml_property();
  //~scml_property() {
  //}
  
  /// Get the int value.
  /**
   * @return Integer value of the parameter.
   */
  int getIntValue() const
  {
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR("scml_property<...>", "m_api is NULL. Error in constructing this instance."); } );
    return this->m_api->getIntParam(m_full_par_name);
  }
  
  /// Set the int value.
  /**
   * @param value Value to be set in the parameter.
   */
  void setIntValue(int value)
  {
    std::stringstream ss;
    ss << value;
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR(name(), "m_api is NULL. Error in constructing this instance."); } );
    this->m_api->setParam(m_full_par_name, ss.str());
  }
  
  /// Returns a string representation of the type.
  /**
   * @return Type string representation.
   */
  std::string getType() const
  {
    return "int";
  }

  /// Returns the value.
  /**
   * @return Value of the parameter
   */
  /*inline const val_type& getValue() const
  {
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR(name(), "m_api is NULL. Error in constructing this instance."); } );
    m_ret_value = this->m_api->getIntParam(m_full_par_name);
    return m_ret_value;
    }*/
  
  // getValue()
  inline int& getValue()
  {
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR(name(), "m_api is NULL. Error in constructing this instance."); } );
    m_ret_value = this->m_api->getIntParam(m_full_par_name);
    return m_ret_value;
  }
  
  // ///////////////////////
  //  operators
  
  SPECIALISATIONS_ARITHMETIC_OPERATORS;
  
  SPECIALISATIONS_BINARY_OPERATORS;
  
  SPECIALISATIONS_INCREMENT_OPERATORS;
  SPECIALISATIONS_DECREMENT_OPERATORS;
  
  
  /// operator ()
  inline operator val_type() //const  // TODO
  {
    return getValue();
  }
  
};



/// Template specialization scml_property<unsigned int>.
/**
 * Template specialization scml_property<unsigned int>.
 */
template<>
class scml_property<unsigned int>
: public gcnfScmlProperty_tbase<unsigned int>
{
public:

  /// Type of the saved value.
  typedef unsigned int                val_type;
  /// Type of this object.
  typedef scml_property<unsigned int> my_type;

  SPECIALISATIONS_EQUAL_OPERATORS;

  /// Constructor without default value.
  /**
   * @param par_name Name of the parameter.
   */
  explicit scml_property(const ::std::string& par_name)
    : gcnfScmlProperty_tbase<unsigned int>(par_name)
  {
    GCNF_DUMP_N(name(), "scml_property<unsigned int>("<<par_name.c_str()<<") constructor");
  }

  /// Constructor with default value.
  /**
   * @param par_name    Name of the parameter.
   * @param init_val    Initial value of the parameter.
   */
  scml_property(const std::string& par_name, val_type init_val)
    : gcnfScmlProperty_tbase<unsigned int>(par_name, init_val)
  {
    GCNF_DUMP_N(name(), "scml_property<unsigned int>("<<par_name.c_str()<<", "<<init_val<<") constructor");
  }

  // /// Destructor.
  // TODO: original virtual
  //virtual ~scml_property();
  //~scml_property() {
  //}
  
  /// Get the unsigned int value.
  /**
   * @return Unsigned integer value of the parameter.
   */
  unsigned int getUIntValue() const
  {
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR("scml_property<...>", "m_api is NULL. Error in constructing this instance."); } );
    return this->m_api->getUIntParam(m_full_par_name);
  }
  
  /// Set the unsigned int value.
  /**
   * @param value Value to be set in the parameter.
   */
  void setUIntValue(unsigned int value)
  {
    std::stringstream ss;
    ss << value;
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR(name(), "m_api is NULL. Error in constructing this instance."); } );
    this->m_api->setParam(m_full_par_name, ss.str());
  }
  
  /// Returns a string representation of the type.
  /**
   * @return Type string representation.
   */
  std::string getType() const
  {
    return "unsigned int";
  }

  /// Returns the value.
  /** TODO
   * @return Value of the parameter
   */
  /*inline const val_type& getValue() const
  {
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR(name(), "m_api is NULL. Error in constructing this instance."); } );
    m_ret_value = this->m_api->getUIntParam(m_full_par_name);
    return m_ret_value;
    }*/
  
  // getValue()
  inline unsigned int& getValue()
  {
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR(name(), "m_api is NULL. Error in constructing this instance."); } );
    m_ret_value = this->m_api->getUIntParam(m_full_par_name);
    return m_ret_value;
  }
  
  // ///////////////////////
  //  operators
  
  SPECIALISATIONS_ARITHMETIC_OPERATORS;
  
  SPECIALISATIONS_BINARY_OPERATORS;
  
  SPECIALISATIONS_INCREMENT_OPERATORS;
  SPECIALISATIONS_DECREMENT_OPERATORS;
  
  /// operator ()
  inline operator val_type() const
  {
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR("scml_property<...>", "m_api is NULL. Error in constructing this instance."); } );
    return this->m_api->getUIntParam(m_full_par_name);
  }
  
};



/// Template specialization scml_property<bool>.
/**
 * Template specialization scml_property<bool>.
 */
template<>
class scml_property<bool>
: public gcnfScmlProperty_tbase<bool>
{
public:

  /// Type of the saved value.
  typedef bool                val_type;
  /// Type of this object.
  typedef scml_property<bool> my_type;

  SPECIALISATIONS_EQUAL_OPERATORS;

  /// Constructor without default value.
  /**
   * @param par_name Name of the parameter.
   */
  explicit scml_property(const ::std::string& par_name)
    : gcnfScmlProperty_tbase<bool>(par_name)
  {
    GCNF_DUMP_N(name(), "scml_property<bool>("<<par_name.c_str()<<") constructor");
  }

  /// Constructor with default value.
  /**
   * @param par_name    Name of the parameter.
   * @param init_val    Initial value of the parameter.
   */
  scml_property(const std::string& par_name, val_type init_val)
    : gcnfScmlProperty_tbase<bool>(par_name, init_val)
  {
    GCNF_DUMP_N(name(), "scml_property<bool>("<<par_name.c_str()<<", "<<init_val<<") constructor");
  }

  // /// Destructor.
  // TODO: original virtual
  //virtual ~scml_property();
               //~scml_property() {
               //}
  
  /// Get the bool value.
  /**
   * @return Bool value of the parameter.
   */
  bool getBoolValue() const
  {
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR("scml_property<...>", "m_api is NULL. Error in constructing this instance."); } );
    return this->m_api->getBoolParam(m_full_par_name);
  }
  
  /// Set the bool value.
  /**
   * @param value Value to be set in the parameter.
   */
  void setBoolValue(bool value)
  {
    std::stringstream ss;
    ss << value;
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR(name(), "m_api is NULL. Error in constructing this instance."); } );
    this->m_api->setParam(m_full_par_name, ss.str());
  }
  
  /// Returns a string representation of the type.
  /**
   * @return Type string representation.
   */
  std::string getType() const
  {
    return "bool";
  }

  /// Returns the value.
  /** TODO
   * @return Value of the parameter
   */
  /*inline const val_type& getValue() const
  {
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR(name(), "m_api is NULL. Error in constructing this instance."); } );
    m_ret_value = this->m_api->getBoolParam(m_full_par_name);
    return m_ret_value;
    }*/
  
  // getValue()
  inline bool& getValue()
  {
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR(name(), "m_api is NULL. Error in constructing this instance."); } );
    m_ret_value = this->m_api->getBoolParam(m_full_par_name);
    return m_ret_value;
  }

  // ///////////////////////
  //  operators
  
  SPECIALISATIONS_ARITHMETIC_OPERATORS;
  
  SPECIALISATIONS_BINARY_OPERATORS;  
  
  // operator -- prefix
  // not allowed for bool
  
  // operator -- postfix
  // not allowed for bool
  
  SPECIALISATIONS_INCREMENT_OPERATORS;
  
  /// operator ()
  inline operator val_type() const
  {
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR("scml_property<...>", "m_api is NULL. Error in constructing this instance."); } );
    return this->m_api->getBoolParam(m_full_par_name);
  }
  
};



/// Template specialization scml_property<double>.
/**
 * Template specialization scml_property<double>.
 */
template<>
class scml_property<double>
: public gcnfScmlProperty_tbase<double>
{
public:

  /// Type of the saved value.
  typedef double                val_type;
  /// Type of this object.
  typedef scml_property<double> my_type;

  SPECIALISATIONS_EQUAL_OPERATORS;

  /// Constructor without default value.
  /**
   * @param par_name Name of the parameter.
   */
  explicit scml_property(const ::std::string& par_name)
    : gcnfScmlProperty_tbase<double>(par_name)
  {
    GCNF_DUMP_N(name(), "scml_property<double>("<<par_name.c_str()<<") constructor");
  }

  /// Constructor with default value.
  /**
   * @param par_name    Name of the parameter.
   * @param init_val    Initial value of the parameter.
   */
  scml_property(const std::string& par_name, val_type init_val)
    : gcnfScmlProperty_tbase<double>(par_name, init_val)
  {
    GCNF_DUMP_N(name(), "scml_property<double>("<<par_name.c_str()<<", "<<init_val<<") constructor");
  }

  // /// Destructor.
  // TODO: original virtual
  //virtual ~scml_property();
               //~scml_property() {
               //}
  
  /// Get the double value.
  /**
   * @return Double value of the parameter.
   */
  double getDoubleValue() const
  {
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR("scml_property<...>", "m_api is NULL. Error in constructing this instance."); } );
    return this->m_api->getBoolParam(m_full_par_name);
  }
  
  /// Set the double value.
  /**
   * @param value Value to be set in the parameter.
   */
  void setDoubleValue(double value)
  {
    std::stringstream ss;
    ss << value;
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR(name(), "m_api is NULL. Error in constructing this instance."); } );
    this->m_api->setParam(m_full_par_name, ss.str());
  }
  
  /// Returns a string representation of the type.
  /**
   * @return Type string representation.
   */
  std::string getType() const
  {
    return "double";
  }

  /// Returns the value.
  /** TODO
   * @return Value of the parameter
   */
  /*inline const val_type& getValue() const
  {
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR(name(), "m_api is NULL. Error in constructing this instance."); } );
    m_ret_value = this->m_api->getDoubleParam(m_full_par_name);
    return m_ret_value;
    }*/
  
  // getValue()
  inline double& getValue()
  {
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR(name(), "m_api is NULL. Error in constructing this instance."); } );
    m_ret_value = this->m_api->getDoubleParam(m_full_par_name);
    return m_ret_value;
  }

  // ///////////////////////
  //  operators
  
  SPECIALISATIONS_ARITHMETIC_OPERATORS;
  
  // not allowed for double 
  // SPECIALISATIONS_BINARY_OPERATORS;
  
  SPECIALISATIONS_DECREMENT_OPERATORS;
  SPECIALISATIONS_INCREMENT_OPERATORS;

  /// operator ()
  inline operator val_type() const
  {
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR("scml_property<...>", "m_api is NULL. Error in constructing this instance."); } );
    return this->m_api->getDoubleParam(m_full_par_name);
  }
  
};

using std::string;
using std::ostream;
using std::istream;

/// Template specialization scml_property<std::string>.
/**
 * Template specialization scml_property<std::string>.
 */
template<>
class scml_property<std::string>
: public gcnfScmlProperty_tbase<std::string>
{
public:

  /// Type of the saved value.
  typedef std::string                val_type;
  /// Type of this object.
  typedef scml_property<std::string> my_type;

  SPECIALISATIONS_EQUAL_OPERATORS;

  /// Constructor without default value.
  /**
   * @param par_name Name of the parameter.
   */
  explicit scml_property(const ::std::string& par_name)
    : gcnfScmlProperty_tbase<std::string>(par_name)
  {
    GCNF_DUMP_N(name(), "scml_property<string>("<<par_name.c_str()<<") constructor");
  }

  /// Constructor with default value.
  /**
   * @param par_name    Name of the parameter.
   * @param init_val    Initial value of the parameter.
   */
  scml_property(const std::string& par_name, val_type init_val)
    : gcnfScmlProperty_tbase<std::string>(par_name, init_val)
  {
    GCNF_DUMP_N(name(), "scml_property<string>("<<par_name.c_str()<<", "<<init_val.c_str()<<") constructor");
  }

  // /// Destructor.
  // TODO: original virtual
  //virtual ~scml_property();
               //~scml_property() {
               //}
  
  /// Get the std::string value.
  /**
   * @return String value of the parameter.
   */
  std::string getStringValue() const
  {
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR("scml_property<...>", "m_api is NULL. Error in constructing this instance."); } );
    return this->m_api->getStringParam(m_full_par_name);
  }
  
  /// Set the std::string value.
  /**
   * @param value Value to be set in the parameter.
   */
  void setStringValue(std::string value)
  {
    std::stringstream ss;
    ss << value;
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR(name(), "m_api is NULL. Error in constructing this instance."); } );
    this->m_api->setParam(m_full_par_name, ss.str());
  }
  
  /// Returns a string representation of the type.
  /**
   * @return Type string representation.
   */
  string getType() const
  {
    return "string";
  }

  /// Returns the value.
  /** TODO
   * This function may not change this object. Needed e.g. for operators
   * @return Value of the parameter
   */
  /*inline const val_type& getValue() const
    {    
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR(name(), "m_api is NULL. Error in constructing this instance."); } );
    val_type tmp = this->m_api->getStringParam(m_full_par_name);
    return tmp;
    }*/
  
  // getValue()
  inline val_type& getValue()
  {
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR(name(), "m_api is NULL. Error in constructing this instance."); } );
    m_ret_value = this->m_api->getStringParam(m_full_par_name);
    return m_ret_value;
  }

  /// operator ()
  inline operator val_type() const
  {
    IF_VERBOSE( if (!this->m_api) { SC_REPORT_ERROR("scml_property<...>", "m_api is NULL. Error in constructing this instance."); } );
    return this->m_api->getStringParam(m_full_par_name);
  }

  // not allowed for string:
  /// operator --,++ pre- u. postfix


};

  // ///////////////////////
  //  operators

  // TODO

  
#define CNSC_STRING_OPERATOR_IMPLEMENTATIONS(otype)                     \
  inline bool operator otype (const scml_property<string>& lhs,         \
                              const scml_property<string>& rhs)         \
  {                                                                     \
    return const_cast<scml_property<string>& >(lhs).getValue() otype    \
      const_cast<scml_property<string>& >(rhs).getValue();              \
  }                                                                     \
                                                                        \
  inline bool operator otype (const string& lhs, const scml_property<string>& rhs)  \
  {                                                                       \
    return lhs otype const_cast<scml_property<string>& >(rhs).getValue(); \
  }                                                                       \
                                                                          \
  inline bool operator otype (const scml_property<string>& lhs, const string& rhs)  \
  {                                                                       \
    return const_cast<scml_property<string>& >(lhs).getValue() otype rhs; \
  }
    
  CNSC_STRING_OPERATOR_IMPLEMENTATIONS(==)
  CNSC_STRING_OPERATOR_IMPLEMENTATIONS(!=)
  CNSC_STRING_OPERATOR_IMPLEMENTATIONS(<)
  CNSC_STRING_OPERATOR_IMPLEMENTATIONS(<=)
  CNSC_STRING_OPERATOR_IMPLEMENTATIONS(>)
  CNSC_STRING_OPERATOR_IMPLEMENTATIONS(>=)
  
#undef CNSC_STRING_OPERATOR_IMPLEMENTATIONS

  
inline ostream& operator << (ostream& os, const scml_property<string>& str)
{
  os << const_cast<scml_property<string>& >(str).getValue();
  return os;
}

// TODO: check if works!
inline istream& operator>>(istream& is, scml_property<string>& str)
{
  is >> const_cast<scml_property<string>& >(str).getValue();
  return is;
}
  
inline string operator+ (const scml_property<string>& lhs, const scml_property<string>& rhs)
{
  return const_cast<scml_property<string>& >(lhs).getValue() + const_cast<scml_property<string>& >(rhs).getValue();
}
  
inline string operator+ (const scml_property<string>& lhs, const string& rhs)
{
  return const_cast<scml_property<string>& >(lhs).getValue() + rhs;
}

inline string operator+ (const string& lhs, const scml_property<string>& rhs)
{
  return lhs + const_cast<scml_property<string>& >(rhs).getValue();
}
  
 
#include "scml_property.tpp"

#ifdef ENABLE_SCML_NAMESPACE
} // end namespace cnf
} // end namespace gs
#endif

#endif
