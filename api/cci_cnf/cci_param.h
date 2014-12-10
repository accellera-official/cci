// LICENSETEXT
//
//   Copyright (C) 2010-2011 : GreenSocs Ltd
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

#ifndef CCI_CNF_CCI_PARAM_H_INCLUDED_
#define CCI_CNF_CCI_PARAM_H_INCLUDED_


#include "cci_config_macros.h"
#include "cci_base_param.h"
#include "cci_param_impl_if.h"
#include "cci_originator.h"

CCI_OPEN_CONFIG_NAMESPACE_

class cci_originator;


// @TODO: This file contains much redundant code within the two implementations

  
/// Parameter class, internally forwarding calls to the implementation
/**
 * The implementation is in file cci_param.hpp, which has to be included 
 * in the headers as well because the class is a template class.
 *
 * Documentation for the undocumented functions can be found in
 * cci_base_param_impl_if and cci_param_impl_if
 */
template<typename T, param_mutable_type TM = mutable_param>
class cci_param 
: public cci_base_param
, public cci_param_impl_if<T, TM> // TODO: Just to make sure they are compatible // needed to be removed due to originator difference
{
private:
  /// Private copy constructor to prevent parameters from being copied
  cci_param(const cci_param<T, TM>&);
  
public:
  /// The parameter's value type.
  typedef T value_type;
  
  // //////////////////////////////////////////////////////////////////// //
  // ///////////////   Construction / Destruction   ///////////////////// //
  
  /// Constructor with a complete implementation parameter
  explicit cci_param(cci_param_impl_if<value_type, TM>* param);

  /// Constructor with (local/hierarchical) name.
  explicit cci_param(const std::string& nam);
  
  /// Constructor with (local/hierarchical) name and string representation of default value.
  cci_param(const std::string& nam, const std::string& val);

  /// Constructor with (local/hierarchical) name and initial value.
  cci_param(const std::string& nam, const value_type& val);

  /// Constructor with (local/hierarchical) name and NO initial value and top-level name.
  cci_param(const std::string& nam, cci_top_level_name);

  /// Constructor with (local/hierarchical) name and NO initial value and private broker.
  cci_param(const std::string& nam, cci_cnf_broker_if& private_broker);

  /// Constructor with (local/hierarchical) name and NO initial value and top-level name and private broker.
  cci_param(const std::string& nam, cci_top_level_name, cci_cnf_broker_if& private_broker);

  /// Constructor with (local/hierarchical) name and string representation of default value and top-level name.
  cci_param(const std::string& nam, const std::string& val, cci_top_level_name);

  /// Constructor with (local/hierarchical) name and string representation of default value and private broker.
  cci_param(const std::string& nam, const std::string& val,                                  cci_cnf_broker_if& private_broker);

  /// Constructor with (local/hierarchical) name and string representation of default value and top-level name and private broker.
  cci_param(const std::string& nam, const std::string& val, cci_top_level_name, cci_cnf_broker_if& private_broker);

  /// Constructor with (local/hierarchical) name and initial value and top-level name.
  cci_param(const std::string& nam, const value_type& val, cci_top_level_name);

  /// Constructor with (local/hierarchical) name and initial value and private broker.
  cci_param(const std::string& nam, const value_type& val,                                  cci_cnf_broker_if& private_broker);

  /// Constructor with (local/hierarchical) name and initial value and top-level name and private broker.
  cci_param(const std::string& nam, const value_type& val, cci_top_level_name, cci_cnf_broker_if& private_broker);

  /// Constructor for a parameter accessor object
  /**
   * @param original_param  Parameter that shall be accessed with the constructed accessor
   * @param originator      Originator the constructed accessor shall hold
   */
  cci_param(cci_param<value_type, TM>& param, const cci_originator& originator);

  /// Destructor
  virtual ~cci_param();

  // Type independent functions
  virtual void json_deserialize(const std::string& json_string);
  virtual std::string json_serialize() const;
  virtual const basic_param_type get_basic_type() const;
  virtual void set_value(const cci_value& val);
  virtual cci_value get_value();
  virtual void set_documentation(const std::string& doc);
  virtual std::string get_documentation() const;
  virtual bool is_default_value();
  virtual bool is_invalid_value();
  virtual void set_invalid_value();
  virtual bool is_initial_value() const;
  virtual const std::string& get_name() const;
  virtual cci::shared_ptr<callb_adapt> register_callback(const callback_type type, void* observer, param_callb_func_ptr function);
  virtual cci::shared_ptr<callb_adapt> register_callback(const callback_type type, cci::shared_ptr<callb_adapt> callb);
  virtual void unregister_all_callbacks(void* observer);
  virtual bool unregister_callback(cci::shared_ptr<callb_adapt> callb);
  virtual bool unregister_callback(callb_adapt* callb);
  virtual bool has_callbacks();
  virtual bool lock(void* pwd = NULL);
  virtual bool unlock(void* pwd = NULL);
  virtual bool is_locked() const;
  virtual const cci_originator* get_latest_write_originator() const;
  
  // Type dependent functions
  virtual cci_param<value_type, TM>& operator = (const cci_param<value_type, TM>& v);
  virtual cci_param<value_type, TM>& operator = (const value_type& v);
  virtual operator const value_type& () const;
  virtual void set(const value_type& val);
  virtual const value_type& get() const;
  virtual void set(const value_type& val, void* lock_pwd);
  virtual std::string json_serialize(const value_type& val) const;
  virtual void json_deserialize(value_type& target_val, const std::string& str);
  virtual const value_type& get_default_value();
  
  virtual cci_base_param_impl_if* get_pImpl() const;

  /// If this parameter is a parameter accessor (if not it is an original parameter)
  /**
   * @return If this is a parameter accessor
   */
  bool is_accessor() const;
  
  /// Copy myself; The copy is a parameter accessor object holding the originator information and pointing to the same parameter
  /**
   * This shall be used by the broker when returning a not yet created parameter accessor.
   *
   * @param originator  Originator object the returned parameter accessor shall represent
   * @return  A newed copy pointing to the same implementation parameter.
   *          Memory management has to be done by the caller!
   */
  cci_base_param* create_accessor(const cci_originator& originator);

protected:

  /// Pointer to the parameter object with the actual implementation
  cci_param_impl_if<value_type, TM> *m_pImpl; // TODO: this was mutable, why?

  /// If this is a parameter accessor object
  bool m_is_accessor;

  /// In the case this is a parameter accessor object: this accessor's originator object, set during construction by broker
  const cci_originator m_originator;

private:
  /// The broker accessor that is responsible for this parameter; This must not be accessible by anyone except given to created implementation param
  cci_cnf_broker_if* m_broker_accessor;

};

template<param_mutable_type TM>
class cci_param<std::string, TM>
: public cci_base_param
, public cci_param_impl_if<std::string, TM> // TODO: Just to make sure they are compatible
{
public:
  // The parameter's value type.
  typedef std::string value_type;
  
  // //////////////////////////////////////////////////////////////////// //
  // ///////////////   Construction / Destruction   ///////////////////// //

  /// Constructor with a complete implementation parameter
  explicit cci_param(cci_param_impl_if<value_type, TM>* param);
  
  /// Constructor with (local/hierarchical) name.
  explicit cci_param(const std::string& nam);
  
  /// Constructor with (local/hierarchical) name and string representation of default value.
  cci_param(const std::string& nam, const std::string& val);
  
  /// Constructor with (local/hierarchical) name and NO initial value and top-level name.
  cci_param(const std::string& nam, cci_top_level_name);
  
  /// Constructor with (local/hierarchical) name and NO initial value and private broker.
  cci_param(const std::string& nam, cci_cnf_broker_if& private_broker);
  
  /// Constructor with (local/hierarchical) name and NO initial value and top-level name and private broker.
  cci_param(const std::string& nam, cci_top_level_name, cci_cnf_broker_if& private_broker);
  
  /// Constructor with (local/hierarchical) name and string representation of default value and top-level name.
  cci_param(const std::string& nam, const std::string& val, cci_top_level_name);

  /// Constructor with (local/hierarchical) name and string representation of default value and private broker.
  cci_param(const std::string& nam, const std::string& val,                                  cci_cnf_broker_if& private_broker);

  /// Constructor with (local/hierarchical) name and string representation of default value and top-level name and private broker.
  cci_param(const std::string& nam, const std::string& val, cci_top_level_name, cci_cnf_broker_if& private_broker);
  
  /// Constructor for a parameter accessor object
  /**
   * @param original_param  Parameter that shall be accessed with the constructed accessor
   * @param originator      Originator the constructed accessor shall hold
   */
  cci_param(cci_param<value_type, TM>& param, const cci_originator& originator);
  
  /// Destructor
  virtual ~cci_param();
  
  // Type independent functions
  virtual void json_deserialize(const std::string& json_string);
  virtual std::string json_serialize() const;
  virtual const basic_param_type get_basic_type() const;
  virtual void set_value(const cci_value& val);
  virtual cci_value get_value();
  virtual void set_documentation(const std::string& doc);
  virtual std::string get_documentation() const;
  virtual bool is_default_value();
  virtual bool is_invalid_value();
  virtual void set_invalid_value();
  virtual bool is_initial_value() const;
  virtual const std::string& get_name() const;
  virtual cci::shared_ptr<callb_adapt> register_callback(const callback_type type, void* observer, param_callb_func_ptr function);
  virtual cci::shared_ptr<callb_adapt> register_callback(const callback_type type, cci::shared_ptr<callb_adapt> callb);
  virtual void unregister_all_callbacks(void* observer);
  virtual bool unregister_callback(cci::shared_ptr<callb_adapt> callb);
  virtual bool unregister_callback(callb_adapt* callb);
  virtual bool has_callbacks();
  virtual bool lock(void* pwd = NULL);
  virtual bool unlock(void* pwd = NULL);
  virtual bool is_locked() const;
  virtual const cci_originator* get_latest_write_originator() const;
  
  // Type dependent functions
  virtual cci::cnf::cci_param<value_type, TM>& operator = (const cci::cnf::cci_param<value_type, TM>& v);
  virtual cci::cnf::cci_param<value_type, TM>& operator = (const value_type& v);
  virtual operator const value_type& () const;
  virtual void set(const value_type& val);
  virtual const value_type& get() const;
  virtual void set(const value_type& val, void* lock_pwd);
  virtual std::string json_serialize(const value_type& val) const;
  virtual void json_deserialize(value_type& target_val, const std::string& str);
  virtual const value_type& get_default_value();
  
  virtual cci_base_param_impl_if* get_pImpl() const;
  
  // Param accessor functions
  
  /// If this parameter is a parameter accessor (if not it is an original parameter)
  /**
   * @return If this is a parameter accessor
   */
  bool is_accessor() const;
  
  /// Copy myself; The copy is a parameter accessor object holding the originator information and pointing to the same parameter
  /**
   * This shall be used by the broker when returning a not yet created parameter accessor.
   *
   * @param originator  Originator object the returned parameter accessor shall represent
   * @return  A newed copy pointing to the same implementation parameter.
   *          Memory management has to be done by the caller!
   */
  cci_base_param* create_accessor(const cci_originator& originator);
  
protected:

  /// Pointer to the parameter object with the actual implementation
  mutable cci_param_impl_if<value_type, TM> *m_pImpl;
  
  /// If this is a parameter accessor object
  bool m_is_accessor;

  /// In the case this is a parameter accessor object: this accessor's originator object, set during construction by broker
  const cci_originator m_originator;
  
private:
  /// The broker accessor that is responsible for this parameter; This must not be accessible by anyone except given to created implementation param
  cci_cnf_broker_if* m_broker_accessor;
  
};

/// Equals operator comparing two params
/**
 * This operator compares the value returned by the get-function
 */
template<class T, param_mutable_type TM, param_mutable_type TM2> bool operator == (cci_param<T, TM>& p1, cci_param<T, TM2>& p2);

/// Equals operator comparing a param and a value
/**
 * This operator compares the value returned by the get-function
 */
template<class T, param_mutable_type TM> bool operator == (cci_param<T, TM>& p1, T& p2);

/// Equals operator comparing a value and a param
/**
 * This operator compares the value returned by the get-function
 */
template<class T, param_mutable_type TM> bool operator == (T& p1, cci_param<T, TM>& p2);


CCI_CLOSE_CONFIG_NAMESPACE_

#include "cci_param.hpp"   // fold into curr file?

#endif
