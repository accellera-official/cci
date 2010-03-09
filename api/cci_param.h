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


namespace cci {

  
  /// Compiler Check Interface for the parameters class
  template<typename T, param_mutable_type TM>
  class cci_param 
  : public cci_param_if<T, TM>
  {
  protected:
    typedef T val_type;
  public:
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Construction / Destruction   ///////////////////// //
    
    // Constructors shall be provided by the derived class:
    /// Empty constructor. Name will be set in base
    explicit cci_param(); // TODO: make private?
    explicit cci_param(const val_type& val); // TODO: allow this one?

    /// Constructor with (local/hierarchical) name.
    explicit cci_param(const std::string& nam) { m_pImpl = cci_impl::CreateParam<T, TM>(nam, false); }
    explicit cci_param(const char* nam       ) { m_pImpl = cci_impl::CreateParam<T, TM>(nam, false); }

    /// Constructor with (local/hierarchical) name and string representation of initial value.
    explicit cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name = false) { m_pImpl = cci_impl::CreateParam<T, TM>(nam, val, force_top_level_name); }
    explicit cci_param(const char* nam,        const char* val       , const bool force_top_level_name = false) { m_pImpl = cci_impl::CreateParam<T, TM>(nam, val, force_top_level_name); }
    explicit cci_param(const std::string& nam, const char* val       , const bool force_top_level_name = false) { m_pImpl = cci_impl::CreateParam<T, TM>(nam, val, force_top_level_name); }
    explicit cci_param(const char* nam,        const std::string& val, const bool force_top_level_name = false) { m_pImpl = cci_impl::CreateParam<T, TM>(nam, val, force_top_level_name); }

    /// Constructor with (local/hierarchical) name and initial value.
    explicit cci_param(const std::string& nam, const val_type& val, const bool force_top_level_name = false) { m_pImpl = cci_impl::CreateParam<T, TM>(nam, val, force_top_level_name); }
    explicit cci_param(const char* nam,        const val_type& val, const bool force_top_level_name = false) { m_pImpl = cci_impl::CreateParam<T, TM>(nam, val, force_top_level_name); }

    // Constructors with register_at_db bool
    //explicit cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name, const bool register_at_db) { m_pImpl = cci_impl::CreateParam<T, TM>(nam, val, force_top_level_name, register_at_db); }

    /// Destructor
    virtual ~cci_param() { cci_impl::DestroyParam<T, TM>(m_pImpl); };
    
    
    virtual operator const T& () const { return m_pImpl->operator&(); }
    virtual void set(const T& val) { m_pImpl->set(val); }
    virtual const T& get() const { return m_pImpl->get(); }
    virtual void set(const T& val, void* lock_pwd) { m_pImpl->set(val, lock_pwd); }
    virtual std::string json_serialize(const T& val) const { return m_pImpl->json_serialize(val); }
    virtual void json_deserialize(T& target_val, const std::string& str) { m_pImpl->json_deserialize(target_val, str); }

  protected:
    cci_param_if<T, TM> *m_pImpl;

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
