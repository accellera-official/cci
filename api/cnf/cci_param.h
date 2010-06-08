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
: public cci_base_param,
  public cci_param_if<T, TM>
{
protected:
  typedef T val_type;

  explicit cci_param();

public:
  
  // //////////////////////////////////////////////////////////////////// //
  // ///////////////   Construction / Destruction   ///////////////////// //
  
  /// Constructor with a complete parameter
  explicit cci_param(cci_param_if<val_type, TM>* param);

  /// Constructor with (local/hierarchical) name.
  explicit cci_param(const std::string& nam);
  explicit cci_param(const char* nam       );
  
  /// Constructor with (local/hierarchical) name and string representation of initial value.
  explicit cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name = false, cci_cnf_broker* private_broker = NULL);
  explicit cci_param(const char* nam,        const char* val       , const bool force_top_level_name = false, cci_cnf_broker* private_broker = NULL);
  explicit cci_param(const std::string& nam, const char* val       , const bool force_top_level_name = false, cci_cnf_broker* private_broker = NULL);
  explicit cci_param(const char* nam,        const std::string& val, const bool force_top_level_name = false, cci_cnf_broker* private_broker = NULL);
  
  /// Constructor with (local/hierarchical) name and initial value.
  explicit cci_param(const std::string& nam, const val_type& val, const bool force_top_level_name = false, cci_cnf_broker* private_broker = NULL);
  explicit cci_param(const char* nam,        const val_type& val, const bool force_top_level_name = false, cci_cnf_broker* private_broker = NULL);
  
  /// Destructor
  virtual ~cci_param();
  
  virtual cci_param<val_type, TM>& operator = (const cci_param<val_type, TM>& v);
  virtual cci_param<val_type, TM>& operator = (const val_type& v);
  virtual operator const val_type& () const;
  virtual void set(const val_type& val);
  virtual const val_type& get() const;
  virtual void set(const val_type& val, void* lock_pwd);
  virtual std::string json_serialize(const val_type& val) const;
  using cci_base_param::json_serialize;
  virtual void json_deserialize(val_type& target_val, const std::string& str);
  using cci_base_param::json_deserialize;

  virtual cci_base_param_if* get_pImpl() const;

protected:

  mutable cci_param_if<val_type, TM> *m_pImpl;
  
};

template<param_mutable_type TM>
class cci_param<std::string, TM>
: public cci_base_param,
  public cci_param_if<std::string, TM>
{
protected:
  typedef std::string val_type;

  explicit cci_param(); // TODO: make private?

public:
  
  // //////////////////////////////////////////////////////////////////// //
  // ///////////////   Construction / Destruction   ///////////////////// //

  /// Constructor with a complete parameter
  explicit cci_param(cci_param_if<val_type, TM>* param);
  
  /// Constructor with (local/hierarchical) name.
  explicit cci_param(const std::string& nam);
  explicit cci_param(const char* nam       );
  
  /// Constructor with (local/hierarchical) name and string representation of initial value.
  explicit cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name = false, cci_cnf_broker* private_broker = NULL);
  explicit cci_param(const char* nam,        const char* val       , const bool force_top_level_name = false, cci_cnf_broker* private_broker = NULL);
  explicit cci_param(const std::string& nam, const char* val       , const bool force_top_level_name = false, cci_cnf_broker* private_broker = NULL);
  explicit cci_param(const char* nam,        const std::string& val, const bool force_top_level_name = false, cci_cnf_broker* private_broker = NULL);
  
  /// Destructor
  virtual ~cci_param();
  
  virtual cci::cnf::cci_param<val_type, TM>& operator = (const cci::cnf::cci_param<val_type, TM>& v);
  virtual cci::cnf::cci_param<val_type, TM>& operator = (const val_type& v);
  virtual operator const val_type& () const;
  virtual void set(const val_type& val);
  virtual const val_type& get() const;
  virtual void set(const val_type& val, void* lock_pwd);
  virtual std::string json_serialize(const val_type& val) const;
  using cci_base_param::json_serialize;
  virtual void json_deserialize(val_type& target_val, const std::string& str);
  using cci_base_param::json_deserialize;
  
  virtual cci_base_param_if* get_pImpl() const;

protected:

  cci_param_if<val_type, TM> *m_pImpl;
  
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
