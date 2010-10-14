// LICENSETEXT
//
//   Copyright (C) 2010 : GreenSocs Ltd
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

#ifndef __CCI_PARAM_H___
#define __CCI_PARAM_H___


__CCI_OPEN_CONFIG_NAMESPACE__
  
  
/// Parameter class, internally forwarding calls to the implementation
/**
 * The implementation is in file cci_param.hpp, which has to be included 
 * in the headers as well because the class is a template class.
 */
template<typename T, param_mutable_type TM = mutable_parameter>
class cci_param 
: public cci_base_param
, public cci_param_impl_if<T, TM> // TODO: Just to make sure they are compatible
{
protected:
  typedef T val_type;

  explicit cci_param();

public:
  
  // //////////////////////////////////////////////////////////////////// //
  // ///////////////   Construction / Destruction   ///////////////////// //
  
  /// Constructor with a complete implementation parameter
  explicit cci_param(cci_param_impl_if<val_type, TM>* param);

  /// Constructor with (local/hierarchical) name.
  explicit cci_param(const std::string& nam);
  explicit cci_param(const char* nam       );
  
  /// Constructor with (local/hierarchical) name and string representation of default value.
  explicit cci_param(const std::string& nam, const std::string& val);
  explicit cci_param(const char* nam,        const char* val       );
  explicit cci_param(const std::string& nam, const char* val       );
  explicit cci_param(const char* nam,        const std::string& val);

  /// Constructor with (local/hierarchical) name and initial value.
  explicit cci_param(const std::string& nam, const val_type& val);
  explicit cci_param(const char* nam,        const val_type& val);

  /// Constructor with (local/hierarchical) name and NO initial value and options.
  explicit cci_param(const std::string& nam, const bool force_top_level_name, cci_cnf_broker* private_broker);
  explicit cci_param(const char* nam,        const bool force_top_level_name, cci_cnf_broker* private_broker);

  /// Constructor with (local/hierarchical) name and string representation of default value and options.
  explicit cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name, cci_cnf_broker* private_broker);
  explicit cci_param(const char* nam,        const char* val       , const bool force_top_level_name, cci_cnf_broker* private_broker);
  explicit cci_param(const std::string& nam, const char* val       , const bool force_top_level_name, cci_cnf_broker* private_broker);
  explicit cci_param(const char* nam,        const std::string& val, const bool force_top_level_name, cci_cnf_broker* private_broker);

  /// Constructor with (local/hierarchical) name and initial value and options.
  explicit cci_param(const std::string& nam, const val_type& val, const bool force_top_level_name, cci_cnf_broker* private_broker);
  explicit cci_param(const char* nam,        const val_type& val, const bool force_top_level_name, cci_cnf_broker* private_broker);

  /// Destructor
  virtual ~cci_param();

  // Type independent functions
  virtual void json_deserialize(const std::string& json_string) {get_pImpl()->json_deserialize(json_string); }
  virtual const std::string& json_serialize() const {return get_pImpl()->json_serialize();}
  virtual const basic_param_type get_basic_type() const {return get_pImpl()->get_basic_type();}
  virtual void set_value(const cci_value& val) {get_pImpl()->set_value(val);}
  virtual cci_value get_value() {return get_pImpl()->get_value();}
  virtual void set_documentation(const std::string& doc) {get_pImpl()->set_documentation(doc);}
  virtual std::string get_documentation() const {return get_pImpl()->get_documentation();}
  virtual bool is_default_value() {return get_pImpl()->is_default_value();}
  virtual bool is_invalid_value() {return get_pImpl()->is_invalid_value();}
  virtual void set_invalid_value() {get_pImpl()->set_invalid_value();}
  virtual bool is_initial_value() {return get_pImpl()->is_initial_value();};
  virtual const std::string& get_name() const {return get_pImpl()->get_name();}
  virtual cci::shared_ptr<callb_adapt_b> register_callback(const callback_type type, void* observer, callb_func_ptr function) {return get_pImpl()-> register_callback(type, observer, function);}
  virtual cci::shared_ptr<callb_adapt_b> register_callback(const callback_type type, cci::shared_ptr<callb_adapt_b> callb) {return get_pImpl()-> register_callback(type, callb);}
  virtual void unregister_all_callbacks(void* observer) {get_pImpl()->unregister_all_callbacks(observer);}
  virtual bool unregister_param_callback(cci::shared_ptr<callb_adapt_b> callb) {return get_pImpl()->unregister_param_callback(callb);}
  virtual bool unregister_param_callback(callb_adapt_b* callb) {return get_pImpl()->unregister_param_callback(callb);}
  virtual bool has_callbacks(){return get_pImpl()->has_callbacks();}
  virtual bool lock(void* pwd = NULL){return get_pImpl()->lock(pwd);}
  virtual bool unlock(void* pwd = NULL){return get_pImpl()->unlock(pwd);}
  virtual bool locked() const {return get_pImpl()->locked();}

  // Type dependent functions
  virtual cci_param<val_type, TM>& operator = (const cci_param<val_type, TM>& v);
  virtual cci_param<val_type, TM>& operator = (const val_type& v);
  virtual operator const val_type& () const;
  virtual void set(const val_type& val);
  virtual const val_type& get() const;
  virtual void set(const val_type& val, void* lock_pwd);
  virtual std::string json_serialize(const val_type& val) const;
  //using cci_base_param::json_serialize;
  virtual void json_deserialize(val_type& target_val, const std::string& str);
  //using cci_base_param::json_deserialize;
  virtual const val_type& get_default_value();

  virtual cci_base_param_impl_if* get_pImpl() const;

protected:

  mutable cci_param_impl_if<val_type, TM> *m_pImpl;
  
};

template<param_mutable_type TM>
class cci_param<std::string, TM>
: public cci_base_param
//, public cci_param_impl_if<std::string, TM> // TODO: Just to make sure they are compatible
{
protected:
  typedef std::string val_type;

  explicit cci_param(); // TODO: make private?

public:
  
  // //////////////////////////////////////////////////////////////////// //
  // ///////////////   Construction / Destruction   ///////////////////// //

  /// Constructor with a complete implementation parameter
  explicit cci_param(cci_param_impl_if<val_type, TM>* param);
  
  /// Constructor with (local/hierarchical) name.
  explicit cci_param(const std::string& nam);
  explicit cci_param(const char* nam       );
  
  /// Constructor with (local/hierarchical) name and string representation of default value.
  explicit cci_param(const std::string& nam, const std::string& val);
  explicit cci_param(const char* nam,        const char* val       );
  explicit cci_param(const std::string& nam, const char* val       );
  explicit cci_param(const char* nam,        const std::string& val);
  
  /// Constructor with (local/hierarchical) name and NO initial value and options.
  explicit cci_param(const std::string& nam, const bool force_top_level_name, cci_cnf_broker* private_broker);
  explicit cci_param(const char* nam,        const bool force_top_level_name, cci_cnf_broker* private_broker);
  
  /// Constructor with (local/hierarchical) name and string representation of default value and options.
  explicit cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name, cci_cnf_broker* private_broker);
  explicit cci_param(const char* nam,        const char* val       , const bool force_top_level_name, cci_cnf_broker* private_broker);
  explicit cci_param(const std::string& nam, const char* val       , const bool force_top_level_name, cci_cnf_broker* private_broker);
  explicit cci_param(const char* nam,        const std::string& val, const bool force_top_level_name, cci_cnf_broker* private_broker);
  
  /// Destructor
  virtual ~cci_param();
  
  // Type independent functions
  virtual void json_deserialize(const std::string& json_string) {get_pImpl()->json_deserialize(json_string); }
  virtual const std::string& json_serialize() const {return get_pImpl()->json_serialize();}
  virtual const basic_param_type get_basic_type() const {return get_pImpl()->get_basic_type();}
  virtual void set_value(const cci_value& val) {get_pImpl()->set_value(val);}
  virtual cci_value get_value() {return get_pImpl()->get_value();}
  virtual void set_documentation(const std::string& doc) {get_pImpl()->set_documentation(doc);}
  virtual std::string get_documentation() const {return get_pImpl()->get_documentation();}
  virtual bool is_default_value() {return get_pImpl()->is_default_value();}
  virtual bool is_invalid_value() {return get_pImpl()->is_invalid_value();}
  virtual void set_invalid_value() {get_pImpl()->set_invalid_value();}
  virtual bool is_initial_value() {return get_pImpl()->is_initial_value();};
  virtual const std::string& get_name() const {return get_pImpl()->get_name();}
  virtual cci::shared_ptr<callb_adapt_b> register_callback(const callback_type type, void* observer, callb_func_ptr function) {return get_pImpl()-> register_callback(type, observer, function);}
  virtual cci::shared_ptr<callb_adapt_b> register_callback(const callback_type type, cci::shared_ptr<callb_adapt_b> callb) {return get_pImpl()-> register_callback(type, callb);}
  virtual void unregister_all_callbacks(void* observer) {get_pImpl()->unregister_all_callbacks(observer);}
  virtual bool unregister_param_callback(cci::shared_ptr<callb_adapt_b> callb) {return get_pImpl()->unregister_param_callback(callb);}
  virtual bool unregister_param_callback(callb_adapt_b* callb) {return get_pImpl()->unregister_param_callback(callb);}
  virtual bool has_callbacks(){return get_pImpl()->has_callbacks();}
  virtual bool lock(void* pwd = NULL){return get_pImpl()->lock(pwd);}
  virtual bool unlock(void* pwd = NULL){return get_pImpl()->unlock(pwd);}
  virtual bool locked() const {return get_pImpl()->locked();}
  
  // Type dependent functions
  virtual cci::cnf::cci_param<val_type, TM>& operator = (const cci::cnf::cci_param<val_type, TM>& v);
  virtual cci::cnf::cci_param<val_type, TM>& operator = (const val_type& v);
  virtual operator const val_type& () const;
  virtual void set(const val_type& val);
  virtual const val_type& get() const;
  virtual void set(const val_type& val, void* lock_pwd);
  virtual std::string json_serialize(const val_type& val) const;
  //using cci_base_param::json_serialize;
  virtual void json_deserialize(val_type& target_val, const std::string& str);
  //using cci_base_param::json_deserialize;
  virtual const val_type& get_default_value();
  
  virtual cci_base_param_impl_if* get_pImpl() const;

protected:

  cci_param_impl_if<val_type, TM> *m_pImpl;
  
};

/// Equals operator comparing two params
template<class T, param_mutable_type TM> bool operator == (cci_param<T, TM>& p1, cci_param<T, TM>& p2);

/// Equals operator comparing a param and a value
template<class T, param_mutable_type TM> bool operator == (cci_param<T, TM>& p1, T& p2);

/// Equals operator comparing a value and a param
template<class T, param_mutable_type TM> bool operator == (T& p1, cci_param<T, TM>& p2);


__CCI_CLOSE_CONFIG_NAMESPACE__

#include "cci_param.hpp"

#endif
