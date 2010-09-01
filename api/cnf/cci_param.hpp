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

//#include "cci.h"

__CCI_OPEN_CONFIG_NAMESPACE__
  

// -- ------------------------------------------------------- --
// General Parameter Implementation

// Constructors without options

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(cci_param_if<val_type, TM>* param) : m_pImpl(param) { cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const std::string& nam) : m_pImpl(NULL) { m_pImpl = create_cci_param<T, TM>(this, nam, /*force_top_level_name=*/false); cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const char* nam       ) : m_pImpl(NULL) { m_pImpl = create_cci_param<T, TM>(this, nam, /*force_top_level_name=*/false); cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }
  
template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const std::string& nam, const std::string& val) : m_pImpl(NULL) { m_pImpl = create_cci_param<T, TM>(this, nam, val, /*force_top_level_name=*/false); cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const char* nam,        const char* val       ) : m_pImpl(NULL) { m_pImpl = create_cci_param<T, TM>(this, nam, val, /*force_top_level_name=*/false); cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const std::string& nam, const char* val       ) : m_pImpl(NULL) { m_pImpl = create_cci_param<T, TM>(this, nam, val, /*force_top_level_name=*/false); cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const char* nam,        const std::string& val) : m_pImpl(NULL) { m_pImpl = create_cci_param<T, TM>(this, nam, val, /*force_top_level_name=*/false); cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const std::string& nam, const T& val          ) : m_pImpl(NULL) { m_pImpl = create_cci_param<T, TM>(this, nam, val, /*force_top_level_name=*/false); cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const char* nam,        const T& val          ) : m_pImpl(NULL) { m_pImpl = create_cci_param<T, TM>(this, nam, val, /*force_top_level_name=*/false); cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

// Constructors with options

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name, cci_cnf_broker* private_broker) : m_pImpl(NULL) { m_pImpl = create_cci_param<T, TM>(this, nam, val, force_top_level_name); if (private_broker) private_broker->add_param(this); else cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const char* nam,        const char* val       , const bool force_top_level_name, cci_cnf_broker* private_broker) : m_pImpl(NULL) { m_pImpl = create_cci_param<T, TM>(this, nam, val, force_top_level_name); if (private_broker) private_broker->add_param(this); else cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const std::string& nam, const char* val       , const bool force_top_level_name, cci_cnf_broker* private_broker) : m_pImpl(NULL) { m_pImpl = create_cci_param<T, TM>(this, nam, val, force_top_level_name); if (private_broker) private_broker->add_param(this); else cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const char* nam,        const std::string& val, const bool force_top_level_name, cci_cnf_broker* private_broker) : m_pImpl(NULL) { m_pImpl = create_cci_param<T, TM>(this, nam, val, force_top_level_name); if (private_broker) private_broker->add_param(this); else cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const std::string& nam, const T& val          , const bool force_top_level_name, cci_cnf_broker* private_broker) : m_pImpl(NULL) { m_pImpl = create_cci_param<T, TM>(this, nam, val, force_top_level_name); if (private_broker) private_broker->add_param(this); else cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const char* nam,        const T& val          , const bool force_top_level_name, cci_cnf_broker* private_broker) : m_pImpl(NULL) { m_pImpl = create_cci_param<T, TM>(this, nam, val, force_top_level_name); if (private_broker) private_broker->add_param(this); else cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const std::string& nam, const bool force_top_level_name, cci_cnf_broker* private_broker) : m_pImpl(NULL) { m_pImpl = create_cci_param<T, TM>(this, nam, force_top_level_name); if (private_broker) private_broker->add_param(this); else cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const char* nam       , const bool force_top_level_name, cci_cnf_broker* private_broker) : m_pImpl(NULL) { m_pImpl = create_cci_param<T, TM>(this, nam, force_top_level_name); if (private_broker) private_broker->add_param(this); else cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

// Destructor
template<typename T, param_mutable_type TM>
cci_param<T,TM>::~cci_param() { destroy_cci_param<T, TM>(this); }
  
// Other
template<typename T, param_mutable_type TM>
cci_param<T, TM>& cci_param<T,TM>::operator = (const cci_param<T, TM>& v) { return m_pImpl->operator=(v);}
template<typename T, param_mutable_type TM>
cci_param<T, TM>& cci_param<T,TM>::operator = (const T& v) { return m_pImpl->operator=(v);}
template<typename T, param_mutable_type TM>
cci_param<T,TM>::operator const T& () const { return m_pImpl->get(); }
template<typename T, param_mutable_type TM>
void cci_param<T,TM>::set(const T& val) { m_pImpl->set(val); }
template<typename T, param_mutable_type TM>
const T& cci_param<T,TM>::get() const { return m_pImpl->get(); }
template<typename T, param_mutable_type TM>
void cci_param<T,TM>::set(const T& val, void* lock_pwd) { m_pImpl->set(val, lock_pwd); }
template<typename T, param_mutable_type TM>
std::string cci_param<T,TM>::json_serialize(const T& val) const { return m_pImpl->json_serialize(val); }
template<typename T, param_mutable_type TM>
void cci_param<T,TM>::json_deserialize(T& target_val, const std::string& str) { m_pImpl->json_deserialize(target_val, str); }
template<typename T, param_mutable_type TM>
const T& cci_param<T,TM>::get_default_value() { return m_pImpl->get_default_value(); }

template<typename T, param_mutable_type TM>
cci_base_param_if* cci_param<T,TM>::get_pImpl() const { assert(m_pImpl != NULL && "must not be called before m_pImpl has been set in constructor!"); /*std::cout << (std::hex) << "m_pImpl=" << m_pImpl << std::endl;*/ return static_cast<cci_base_param_if* >(m_pImpl); }


// -- ------------------------------------------------------- --
// String Parameter Specialization Implementation

// Constructors without options

template<param_mutable_type TM>
cci_param<std::string,TM>::cci_param(cci_param_if<std::string, TM>* param) : m_pImpl(param) { cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<param_mutable_type TM>
cci_param<std::string,TM>::cci_param(const std::string& nam) : m_pImpl(NULL) { m_pImpl = create_cci_param<std::string, TM>(this, nam, /*force_top_level_name=*/false); cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<param_mutable_type TM>
cci_param<std::string,TM>::cci_param(const char* nam       ) : m_pImpl(NULL) { m_pImpl = create_cci_param<std::string, TM>(this, nam, /*force_top_level_name=*/false); cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<param_mutable_type TM>
cci_param<std::string,TM>::cci_param(const std::string& nam, const std::string& val) : m_pImpl(NULL) { m_pImpl = create_cci_param<std::string, TM>(this, nam, val, /*force_top_level_name=*/false); cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<param_mutable_type TM>
cci_param<std::string,TM>::cci_param(const char* nam,        const char* val       ) : m_pImpl(NULL) { m_pImpl = create_cci_param<std::string, TM>(this, nam, val, /*force_top_level_name=*/false); cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<param_mutable_type TM>
cci_param<std::string,TM>::cci_param(const std::string& nam, const char* val       ) : m_pImpl(NULL) { m_pImpl = create_cci_param<std::string, TM>(this, nam, val, /*force_top_level_name=*/false); cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<param_mutable_type TM>
cci_param<std::string,TM>::cci_param(const char* nam,        const std::string& val) : m_pImpl(NULL) { m_pImpl = create_cci_param<std::string, TM>(this, nam, val, /*force_top_level_name=*/false); cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

// Constructors with options

template<param_mutable_type TM>
cci_param<std::string,TM>::cci_param(const std::string& nam, const std::string& val, const bool force_top_level_name, cci_cnf_broker* private_broker) : m_pImpl(NULL) { m_pImpl = create_cci_param<std::string, TM>(this, nam, val, force_top_level_name); if (private_broker) private_broker->add_param(this); else cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<param_mutable_type TM>
cci_param<std::string,TM>::cci_param(const char* nam,        const char* val       , const bool force_top_level_name, cci_cnf_broker* private_broker) : m_pImpl(NULL) { m_pImpl = create_cci_param<std::string, TM>(this, nam, val, force_top_level_name); if (private_broker) private_broker->add_param(this); else cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<param_mutable_type TM>
cci_param<std::string,TM>::cci_param(const std::string& nam, const char* val       , const bool force_top_level_name, cci_cnf_broker* private_broker) : m_pImpl(NULL) { m_pImpl = create_cci_param<std::string, TM>(this, nam, val, force_top_level_name); if (private_broker) private_broker->add_param(this); else cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<param_mutable_type TM>
cci_param<std::string,TM>::cci_param(const char* nam,        const std::string& val, const bool force_top_level_name, cci_cnf_broker* private_broker) : m_pImpl(NULL) { m_pImpl = create_cci_param<std::string, TM>(this, nam, val, force_top_level_name); if (private_broker) private_broker->add_param(this); else cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }
  
template<param_mutable_type TM>
cci_param<std::string,TM>::cci_param(const std::string& nam, const bool force_top_level_name, cci_cnf_broker* private_broker) : m_pImpl(NULL) { m_pImpl = create_cci_param<std::string, TM>(this, nam, force_top_level_name); if (private_broker) private_broker->add_param(this); else cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

template<param_mutable_type TM>
cci_param<std::string,TM>::cci_param(const char* nam,        const bool force_top_level_name, cci_cnf_broker* private_broker) : m_pImpl(NULL) { m_pImpl = create_cci_param<std::string, TM>(this, nam, force_top_level_name); if (private_broker) private_broker->add_param(this); else cci::cnf::get_cnf_broker_instance(NULL)->add_param(this); init_cci_param(this); }

// Destructor
template<param_mutable_type TM>
cci_param<std::string,TM>::~cci_param() { destroy_cci_param<std::string, TM>(this); }

// Other
template<param_mutable_type TM>
cci::cnf::cci_param<std::string, TM>& cci_param<std::string,TM>::operator = (const cci::cnf::cci_param<std::string, TM>& v) { return m_pImpl->operator=(v);}
template<param_mutable_type TM>
cci::cnf::cci_param<std::string, TM>& cci_param<std::string,TM>::operator = (const std::string& v) { return m_pImpl->operator=(v);}
template<param_mutable_type TM>
cci_param<std::string,TM>::operator const std::string& () const { return m_pImpl->get(); }
template<param_mutable_type TM>
void cci_param<std::string,TM>::set(const std::string& val) { m_pImpl->set(val); }
template<param_mutable_type TM>
const std::string& cci_param<std::string,TM>::get() const { return m_pImpl->get(); }
template<param_mutable_type TM>
void cci_param<std::string,TM>::set(const std::string& val, void* lock_pwd) { m_pImpl->set(val, lock_pwd); }
template<param_mutable_type TM>
std::string cci_param<std::string,TM>::json_serialize(const std::string& val) const { return m_pImpl->json_serialize(val); }
template<param_mutable_type TM>
void cci_param<std::string,TM>::json_deserialize(std::string& target_val, const std::string& str) { m_pImpl->json_deserialize(target_val, str); }
template<param_mutable_type TM>
const std::string& cci_param<std::string,TM>::get_default_value() { return m_pImpl->get_default_value(); }

template<param_mutable_type TM>
cci_base_param_if* cci_param<std::string,TM>::get_pImpl() const { assert(m_pImpl != NULL && "must not be called before m_pImpl has been set in constructor!"); return m_pImpl; }


// == Operators

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
