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

#ifndef __CCI_PARAM_H___
#define __CCI_PARAM_H___


__CCI_OPEN_CONFIG_NAMESPACE__
  
  
  /// Parameter class, internally forwarding calls to the implementation
  template<typename T, param_mutable_type TM = mutable_parameter>
  class cci_param 
  : public cci_base_param,
    public cci_param_if<T, TM>
  {
protected:
  typedef T val_type;
public:
  
  // //////////////////////////////////////////////////////////////////// //
  // ///////////////   Construction / Destruction   ///////////////////// //
  
  // Constructors shall be provided by the derived class:
  /// Empty constructor. Name will be set in base
  explicit cci_param(); // TODO: make private?
  //explicit cci_param(const val_type& val); // TODO: allow this one?
  
  /// Constructor with (local/hierarchical) name.
  explicit cci_param(const std::string& nam) : m_pImpl(NULL) { m_pImpl = cci_impl::CreateParam<val_type, TM>(this, nam, false); cci_impl::InitParam(m_pImpl); }
  explicit cci_param(const char* nam       ) : m_pImpl(NULL) { m_pImpl = cci_impl::CreateParam<val_type, TM>(this, nam, false); cci_impl::InitParam(m_pImpl); }
  
  /// Constructor with (local/hierarchical) name and string representation of initial value.
  explicit cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name = false) : m_pImpl(NULL) { m_pImpl = cci_impl::CreateParam<val_type, TM>(this, nam, val, force_top_level_name); cci_impl::InitParam(m_pImpl); }
  explicit cci_param(const char* nam,        const char* val       , const bool force_top_level_name = false) : m_pImpl(NULL) { m_pImpl = cci_impl::CreateParam<val_type, TM>(this, nam, val, force_top_level_name); cci_impl::InitParam(m_pImpl); }
  explicit cci_param(const std::string& nam, const char* val       , const bool force_top_level_name = false) : m_pImpl(NULL) { m_pImpl = cci_impl::CreateParam<val_type, TM>(this, nam, val, force_top_level_name); cci_impl::InitParam(m_pImpl); }
  explicit cci_param(const char* nam,        const std::string& val, const bool force_top_level_name = false) : m_pImpl(NULL) { m_pImpl = cci_impl::CreateParam<val_type, TM>(this, nam, val, force_top_level_name); cci_impl::InitParam(m_pImpl); }
  
  /// Constructor with (local/hierarchical) name and initial value.
  explicit cci_param(const std::string& nam, const val_type& val, const bool force_top_level_name = false) : m_pImpl(NULL) { m_pImpl = cci_impl::CreateParam<val_type, TM>(this, nam, val, force_top_level_name); cci_impl::InitParam(m_pImpl); }
  explicit cci_param(const char* nam,        const val_type& val, const bool force_top_level_name = false) : m_pImpl(NULL) { m_pImpl = cci_impl::CreateParam<val_type, TM>(this, nam, val, force_top_level_name); cci_impl::InitParam(m_pImpl); }
  
  // Constructors with register_at_db bool
  //explicit cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name, const bool register_at_db) : m_pImpl(NULL) { m_pImpl = cci_impl::CreateParam<val_type, TM>(this, nam, val, force_top_level_name, register_at_db); cci_impl::InitParam(m_pImpl); }

  /// Destructor
  virtual ~cci_param() { cci_impl::DestroyParam<val_type, TM>(m_pImpl); };
  
  
  virtual cci_param<val_type, TM>& operator = (const cci_param<val_type, TM>& v) { return m_pImpl->operator=(v);}
  virtual cci_param<val_type, TM>& operator = (const val_type& v) { return m_pImpl->operator=(v);}
  virtual operator const val_type& () const { return (val_type)(*m_pImpl); }
  virtual void set(const val_type& val) { m_pImpl->set(val); }
  virtual const val_type& get() const { return m_pImpl->get(); }
  virtual void set(const val_type& val, void* lock_pwd) { m_pImpl->set(val, lock_pwd); }
  virtual std::string json_serialize(const val_type& val) const { return m_pImpl->json_serialize(val); }
  using cci_base_param::json_serialize;
  virtual void json_deserialize(val_type& target_val, const std::string& str) { m_pImpl->json_deserialize(target_val, str); }
  using cci_base_param::json_deserialize;

protected:
  
  virtual cci_base_param_if* get_pImpl() const { assert(m_pImpl != NULL && "must not be called before m_pImpl has been set in constructor!"); std::cout << (std::hex) << "m_pImpl=" << m_pImpl << std::endl; return static_cast<cci_base_param_if* >(m_pImpl); }

  mutable cci_param_if<val_type, TM> *m_pImpl;
  
};

template<param_mutable_type TM>
class cci_param<std::string, TM>
: public cci_base_param,
  public cci_param_if<std::string, TM>
{
protected:
  typedef std::string val_type;
public:
  
  // //////////////////////////////////////////////////////////////////// //
  // ///////////////   Construction / Destruction   ///////////////////// //
  
  // Constructors shall be provided by the derived class:
  /// Empty constructor. Name will be set in base
  explicit cci_param(); // TODO: make private?
  //explicit cci_param(const val_type& val); // TODO: allow this one?
  
  /// Constructor with (local/hierarchical) name.
  explicit cci_param(const std::string& nam) : m_pImpl(NULL) { m_pImpl = cci_impl::CreateParam<val_type, TM>(this, nam, false); cci_impl::InitParam(m_pImpl); }
  explicit cci_param(const char* nam       ) : m_pImpl(NULL) { m_pImpl = cci_impl::CreateParam<val_type, TM>(this, nam, false); cci_impl::InitParam(m_pImpl); }
  
  /// Constructor with (local/hierarchical) name and string representation of initial value.
  explicit cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name = false) : m_pImpl(NULL) { m_pImpl = cci_impl::CreateParam<val_type, TM>(this, nam, val, force_top_level_name); cci_impl::InitParam(m_pImpl); }
  explicit cci_param(const char* nam,        const char* val       , const bool force_top_level_name = false) : m_pImpl(NULL) { m_pImpl = cci_impl::CreateParam<val_type, TM>(this, nam, val, force_top_level_name); cci_impl::InitParam(m_pImpl); }
  explicit cci_param(const std::string& nam, const char* val       , const bool force_top_level_name = false) : m_pImpl(NULL) { m_pImpl = cci_impl::CreateParam<val_type, TM>(this, nam, val, force_top_level_name); cci_impl::InitParam(m_pImpl); }
  explicit cci_param(const char* nam,        const std::string& val, const bool force_top_level_name = false) : m_pImpl(NULL) { m_pImpl = cci_impl::CreateParam<val_type, TM>(this, nam, val, force_top_level_name); cci_impl::InitParam(m_pImpl); }
  
  /// Constructor with (local/hierarchical) name and initial value.
  //explicit cci_param(const std::string& nam, const val_type& val, const bool force_top_level_name = false) : m_pImpl(NULL) { m_pImpl = cci_impl::CreateParam<val_type, TM>(this, nam, val, force_top_level_name); cci_impl::InitParam(m_pImpl); }
  //explicit cci_param(const char* nam,        const val_type& val, const bool force_top_level_name = false) : m_pImpl(NULL) { m_pImpl = cci_impl::CreateParam<val_type, TM>(this, nam, val, force_top_level_name); cci_impl::InitParam(m_pImpl); }
  
  // Constructors with register_at_db bool
  //explicit cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name, const bool register_at_db) : m_pImpl(NULL) { m_pImpl = cci_impl::CreateParam<val_type, TM>(this, nam, val, force_top_level_name, register_at_db); cci_impl::InitParam(m_pImpl); }
  
  /// Destructor
  virtual ~cci_param() { cci_impl::DestroyParam<val_type, TM>(m_pImpl); };
  
  
  virtual cci::cnf::cci_param<val_type, TM>& operator = (const cci::cnf::cci_param<val_type, TM>& v) { return m_pImpl->operator=(v);}
  virtual cci::cnf::cci_param<val_type, TM>& operator = (const val_type& v) { return m_pImpl->operator=(v);}
  virtual operator const val_type& () const { return (val_type)(*m_pImpl); }
  virtual void set(const val_type& val) { m_pImpl->set(val); }
  virtual const val_type& get() const { return m_pImpl->get(); }
  virtual void set(const val_type& val, void* lock_pwd) { m_pImpl->set(val, lock_pwd); }
  virtual std::string json_serialize(const val_type& val) const { return m_pImpl->json_serialize(val); }
  using cci_base_param::json_serialize;
  virtual void json_deserialize(val_type& target_val, const std::string& str) { m_pImpl->json_deserialize(target_val, str); }
  using cci_base_param::json_deserialize;
  
protected:
  
  virtual cci_base_param_if* get_pImpl() const { assert(m_pImpl != NULL && "must not be called before m_pImpl has been set in constructor!"); return m_pImpl; }

  cci_param_if<val_type, TM> *m_pImpl;
  
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


__CCI_CLOSE_CONFIG_NAMESPACE__

#endif
