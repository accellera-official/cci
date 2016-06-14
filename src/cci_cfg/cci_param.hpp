/*****************************************************************************
  Copyright 2016 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2010-2011 GreenSocs Ltd
  All rights reserved.
  Author(s): Mark Burton <mark@greensocs.com>
             Christian Schroeder <schroeder@eis.cs.tu-bs.de>

  Copyright 2016 Ericsson
  All rights reserved.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 *****************************************************************************/


#include "cci_cnf/cci_factory.h"
#include "cci_cnf/cci_broker_if.h"
#include "cci_cnf/cci_broker_manager.h"

CCI_OPEN_NAMESPACE_
  

// -- ------------------------------------------------------- --
// General Parameter Implementation

// private contructor
template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const cci_param<value_type, TM>& param)            : m_pImpl(NULL), m_is_accessor(false), m_broker_accessor(&cci::cci_broker_manager::get_current_broker(m_originator)) { assert(false && "Not allowed to copy parameter objects!"); }

// Constructors without options

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(cci_param_impl_if<value_type, TM>* param)            : m_pImpl(NULL), m_is_accessor(false), m_broker_accessor(&cci::cci_broker_manager::get_current_broker(m_originator)) { m_pImpl = param;                                                         m_broker_accessor->add_param(this); init_cci_param(this); }

// Constructor with (local/hierarchical) name.

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const std::string& nam)                            : m_pImpl(NULL), m_is_accessor(false), m_broker_accessor(&cci::cci_broker_manager::get_current_broker(m_originator)) { m_pImpl = create_cci_param<T, TM>(this, nam, false, m_broker_accessor); m_broker_accessor->add_param(this); init_cci_param(this); }

// Constructor with (local/hierarchical) name and string representation of default value.

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const std::string& nam, const std::string& val)    : m_pImpl(NULL), m_is_accessor(false), m_broker_accessor(&cci::cci_broker_manager::get_current_broker(m_originator)) { m_pImpl = create_cci_param<T, TM>(this, nam, val, false, m_broker_accessor); m_broker_accessor->add_param(this); init_cci_param(this); }

// Constructor with (local/hierarchical) name and initial value.

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const std::string& nam, const T& val          )    : m_pImpl(NULL), m_is_accessor(false), m_broker_accessor(&cci::cci_broker_manager::get_current_broker(m_originator)) { m_pImpl = create_cci_param<T, TM>(this, nam, val, false, m_broker_accessor); m_broker_accessor->add_param(this); init_cci_param(this); }

// Constructor with (local/hierarchical) name and NO initial value and top-level name.

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const std::string& nam, cci_top_level_name                                   )                            : m_pImpl(NULL), m_is_accessor(false), m_broker_accessor(&cci::cci_broker_manager::get_current_broker(m_originator)) { m_pImpl = create_cci_param<T, TM>(this, nam, true , m_broker_accessor); m_broker_accessor->add_param(this); init_cci_param(this); }

/// Constructor with (local/hierarchical) name and NO initial value and private broker.

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const std::string& nam,                     cci_broker_if& private_broker)                            : m_pImpl(NULL), m_is_accessor(false), m_broker_accessor(&private_broker)                                                { m_pImpl = create_cci_param<T, TM>(this, nam, false, m_broker_accessor); m_broker_accessor->add_param(this); init_cci_param(this); }

/// Constructor with (local/hierarchical) name and NO initial value and top-level name and private broker.

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const std::string& nam, cci_top_level_name, cci_broker_if& private_broker)                            : m_pImpl(NULL), m_is_accessor(false), m_broker_accessor(&private_broker)                                                { m_pImpl = create_cci_param<T, TM>(this, nam, true , m_broker_accessor); m_broker_accessor->add_param(this); init_cci_param(this); }

// Constructor with (local/hierarchical) name and string representation of default value and top-level name.

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const std::string& nam, const std::string& val, cci_top_level_name)                                       : m_pImpl(NULL), m_is_accessor(false), m_broker_accessor(&cci::cci_broker_manager::get_current_broker(m_originator)) { m_pImpl = create_cci_param<T, TM>(this, nam, val.c_str(), true , m_broker_accessor); m_broker_accessor->add_param(this); init_cci_param(this); }

// Constructor with (local/hierarchical) name and string representation of default value and private broker.

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const std::string& nam, const std::string& val,                     cci_broker_if& private_broker)    : m_pImpl(NULL), m_is_accessor(false), m_broker_accessor(&private_broker)                                                { m_pImpl = create_cci_param<T, TM>(this, nam, val.c_str(), false, m_broker_accessor); m_broker_accessor->add_param(this); init_cci_param(this); }

// Constructor with (local/hierarchical) name and string representation of default value and top-level name and private broker.

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const std::string& nam, const std::string& val, cci_top_level_name, cci_broker_if& private_broker)    : m_pImpl(NULL), m_is_accessor(false), m_broker_accessor(&private_broker)                                                { m_pImpl = create_cci_param<T, TM>(this, nam, val.c_str(), true , m_broker_accessor); m_broker_accessor->add_param(this); init_cci_param(this); }

// Constructor with (local/hierarchical) name and initial value and top-level name.

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const std::string& nam, const T& val          , cci_top_level_name                                   )    : m_pImpl(NULL), m_is_accessor(false), m_broker_accessor(&cci::cci_broker_manager::get_current_broker(m_originator)) { m_pImpl = create_cci_param<T, TM>(this, nam, val, true , m_broker_accessor); m_broker_accessor->add_param(this); init_cci_param(this); }

// Constructor with (local/hierarchical) name and initial value and private broker.

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const std::string& nam, const T& val          ,                     cci_broker_if& private_broker)    : m_pImpl(NULL), m_is_accessor(false), m_broker_accessor(&cci::cci_broker_manager::get_current_broker(m_originator)) { m_pImpl = create_cci_param<T, TM>(this, nam, val, false, m_broker_accessor); m_broker_accessor->add_param(this); init_cci_param(this); }

// Constructor with (local/hierarchical) name and initial value and top-level name and private broker.

template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(const std::string& nam, const T& val          , cci_top_level_name, cci_broker_if& private_broker)    : m_pImpl(NULL), m_is_accessor(false), m_broker_accessor(&private_broker)                                                { m_pImpl = create_cci_param<T, TM>(this, nam, val, true , m_broker_accessor); m_broker_accessor->add_param(this); init_cci_param(this); }

/// Constructor for a parameter accessor object
template<typename T, param_mutable_type TM>
cci_param<T,TM>::cci_param(cci_param<T,TM>& param, const cci_originator& originator) : m_pImpl(param.m_pImpl), m_is_accessor(true), m_originator(originator) { }



// Destructor
template<typename T, param_mutable_type TM>
cci_param<T,TM>::~cci_param() { 
  if (!m_is_accessor) {
    const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
    destroy_cci_param<T, TM>(this); 
    cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  }
}
  
// Type independent functions
template<typename T, param_mutable_type TM>
void cci_param<T,TM>::json_deserialize(const std::string& json_string) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  get_pImpl()->json_deserialize(json_string); 
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

template<typename T, param_mutable_type TM>
std::string cci_param<T,TM>::json_serialize() const {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const std::string& ret = get_pImpl()->json_serialize();
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<typename T, param_mutable_type TM>
const basic_param_type cci_param<T,TM>::get_basic_type() const {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const basic_param_type& ret = get_pImpl()->get_basic_type();
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<typename T, param_mutable_type TM>
void cci_param<T,TM>::set_value(const cci_value& val) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  get_pImpl()->set_value(val);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

template<typename T, param_mutable_type TM>
cci_value cci_param<T,TM>::get_value() const {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const cci_value& ret = get_pImpl()->get_value();
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<typename T, param_mutable_type TM>
void cci_param<T,TM>::set_documentation(const std::string& doc) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  get_pImpl()->set_documentation(doc);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

template<typename T, param_mutable_type TM>
std::string cci_param<T,TM>::get_documentation() const {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const std::string& ret = get_pImpl()->get_documentation();
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<typename T, param_mutable_type TM>
bool cci_param<T,TM>::is_default_value() {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = get_pImpl()->is_default_value();
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<typename T, param_mutable_type TM>
const std::string& cci_param<T,TM>::get_name() const {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const std::string& ret = get_pImpl()->get_name();
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<typename T, param_mutable_type TM>
cci::shared_ptr<callb_adapt> cci_param<T,TM>::register_callback(const callback_type type, void* observer, param_callb_func_ptr function) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const cci::shared_ptr<callb_adapt>& ret = get_pImpl()-> register_callback(type, observer, function);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<typename T, param_mutable_type TM>
cci::shared_ptr<callb_adapt> cci_param<T,TM>::register_callback(const callback_type type, cci::shared_ptr<callb_adapt> callb) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const cci::shared_ptr<callb_adapt>& ret = get_pImpl()-> register_callback(type, callb);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<typename T, param_mutable_type TM>
void cci_param<T,TM>::unregister_all_callbacks(void* observer) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  get_pImpl()->unregister_all_callbacks(observer);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

template<typename T, param_mutable_type TM>
bool cci_param<T,TM>::unregister_callback(cci::shared_ptr<callb_adapt> callb) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = get_pImpl()->unregister_callback(callb);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<typename T, param_mutable_type TM>
bool cci_param<T,TM>::unregister_callback(callb_adapt* callb) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = get_pImpl()->unregister_callback(callb);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<typename T, param_mutable_type TM>
bool cci_param<T,TM>::has_callbacks() {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = get_pImpl()->has_callbacks();
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<typename T, param_mutable_type TM>
bool cci_param<T,TM>::lock(void* pwd) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = get_pImpl()->lock(pwd);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<typename T, param_mutable_type TM>
bool cci_param<T,TM>::unlock(void* pwd) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = get_pImpl()->unlock(pwd);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<typename T, param_mutable_type TM>
bool cci_param<T,TM>::is_locked() const {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = get_pImpl()->is_locked();
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<typename T, param_mutable_type TM>
const cci_originator* cci_param<T,TM>::get_latest_write_originator() const {
  //const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  return get_pImpl()->get_latest_write_originator();
  //cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  //return ret;
}

// Other
template<typename T, param_mutable_type TM>
cci_param<T, TM>& cci_param<T,TM>::operator = (const cci_param<T, TM>& v) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_pImpl->set(v.get());
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return *this; 
}

template<typename T, param_mutable_type TM>
cci_param<T, TM>& cci_param<T,TM>::operator = (const T& v) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_pImpl->set(v);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return *this; 
}

template<typename T, param_mutable_type TM>
cci_param<T,TM>::operator const T& () const {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const T& ret = m_pImpl->get(); 
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<typename T, param_mutable_type TM>
void cci_param<T,TM>::set(const T& val) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_pImpl->set(val); 
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

template<typename T, param_mutable_type TM>
const T& cci_param<T,TM>::get() const {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const T& ret = m_pImpl->get(); 
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<typename T, param_mutable_type TM>
void cci_param<T,TM>::set(const T& val, void* lock_pwd) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_pImpl->set(val, lock_pwd); 
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

template<typename T, param_mutable_type TM>
std::string cci_param<T,TM>::json_serialize(const T& val) const {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const std::string& ret = m_pImpl->json_serialize(val); 
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<typename T, param_mutable_type TM>
void cci_param<T,TM>::json_deserialize(T& target_val, const std::string& str) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_pImpl->json_deserialize(target_val, str); 
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

template<typename T, param_mutable_type TM>
const T& cci_param<T,TM>::get_default_value() {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const T& ret = m_pImpl->get_default_value(); 
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<typename T, param_mutable_type TM>
cci_base_param_impl_if* cci_param<T,TM>::get_pImpl() const {
  assert(m_pImpl != NULL && "must not be called before m_pImpl has been set in constructor!"); /*std::cout << (std::hex) << "m_pImpl=" << m_pImpl << std::endl;*/ 
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  cci_base_param_impl_if* ret = m_pImpl; 
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

// Param accessor functions
template<typename T, param_mutable_type TM>
bool cci_param<T,TM>::is_accessor() const { return m_is_accessor; }

template<typename T, param_mutable_type TM>
cci_base_param* cci_param<T,TM>::create_accessor(const cci_originator& originator) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  cci_base_param* ret = new cci_param<T,TM>(*this, originator);
  add_param_accessor(ret);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}


// -- ------------------------------------------------------- --
// String Parameter Specialization Implementation

// Constructors without options

template<param_mutable_type TM>
cci_param<std::string,TM>::cci_param(cci_param_impl_if<std::string, TM>* param)         : m_pImpl(NULL), m_is_accessor(false), m_broker_accessor(&cci::cci_broker_manager::get_current_broker(m_originator)) { m_pImpl = param;                                                                  m_broker_accessor->add_param(this); init_cci_param(this); }

// Constructor with (local/hierarchical) name.

template<param_mutable_type TM>
cci_param<std::string,TM>::cci_param(const std::string& nam)                            : m_pImpl(NULL), m_is_accessor(false), m_broker_accessor(&cci::cci_broker_manager::get_current_broker(m_originator)) { m_pImpl = create_cci_param<std::string, TM>(this, nam, false, m_broker_accessor); m_broker_accessor->add_param(this); init_cci_param(this); }

// Constructor with (local/hierarchical) name and string representation of default value.

template<param_mutable_type TM>
cci_param<std::string,TM>::cci_param(const std::string& nam, const std::string& val)    : m_pImpl(NULL), m_is_accessor(false), m_broker_accessor(&cci::cci_broker_manager::get_current_broker(m_originator)) { m_pImpl = create_cci_param<std::string, TM>(this, nam, val, false, m_broker_accessor); m_broker_accessor->add_param(this); init_cci_param(this); }

// Constructor with (local/hierarchical) name and NO initial value and top-level name.

template<param_mutable_type TM>
cci_param<std::string,TM>::cci_param(const std::string& nam, cci_top_level_name                                   )                            : m_pImpl(NULL), m_is_accessor(false), m_broker_accessor(&cci::cci_broker_manager::get_current_broker(m_originator)) { m_pImpl = create_cci_param<std::string, TM>(this, nam, true , m_broker_accessor); m_broker_accessor->add_param(this); init_cci_param(this); }

// Constructor with (local/hierarchical) name and NO initial value and private broker.

template<param_mutable_type TM>
cci_param<std::string,TM>::cci_param(const std::string& nam,                     cci_broker_if& private_broker)                            : m_pImpl(NULL), m_is_accessor(false), m_broker_accessor(&private_broker)                                                { m_pImpl = create_cci_param<std::string, TM>(this, nam, false, m_broker_accessor); m_broker_accessor->add_param(this); init_cci_param(this); }

// Constructor with (local/hierarchical) name and NO initial value and top-level name and private broker.

template<param_mutable_type TM>
cci_param<std::string,TM>::cci_param(const std::string& nam, cci_top_level_name, cci_broker_if& private_broker)                            : m_pImpl(NULL), m_is_accessor(false), m_broker_accessor(&private_broker)                                                { m_pImpl = create_cci_param<std::string, TM>(this, nam, true , m_broker_accessor); m_broker_accessor->add_param(this); init_cci_param(this); }

// Constructor with (local/hierarchical) name and string representation of default value and top-level name.

template<param_mutable_type TM>
cci_param<std::string,TM>::cci_param(const std::string& nam, const std::string& val, cci_top_level_name                                   )    : m_pImpl(NULL), m_is_accessor(false), m_broker_accessor(&cci::cci_broker_manager::get_current_broker(m_originator)) { m_pImpl = create_cci_param<std::string, TM>(this, nam, val, true , m_broker_accessor); m_broker_accessor->add_param(this); init_cci_param(this); }

/// Constructor with (local/hierarchical) name and string representation of default value and private broker.

template<param_mutable_type TM>
cci_param<std::string,TM>::cci_param(const std::string& nam, const std::string& val,                     cci_broker_if& private_broker)    : m_pImpl(NULL), m_is_accessor(false), m_broker_accessor(&private_broker)                                                { m_pImpl = create_cci_param<std::string, TM>(this, nam, val, false, m_broker_accessor); m_broker_accessor->add_param(this); init_cci_param(this); }

// Constructor with (local/hierarchical) name and string representation of default value and top-level name and private broker.

template<param_mutable_type TM>
cci_param<std::string,TM>::cci_param(const std::string& nam, const std::string& val, cci_top_level_name, cci_broker_if& private_broker)    : m_pImpl(NULL), m_is_accessor(false), m_broker_accessor(&private_broker)                                                { m_pImpl = create_cci_param<std::string, TM>(this, nam, val, true , m_broker_accessor); m_broker_accessor->add_param(this); init_cci_param(this); }

/// Constructor for a parameter accessor object
template<param_mutable_type TM>
cci_param<std::string,TM>::cci_param(cci_param<std::string,TM>& param, const cci_originator& originator) : m_pImpl(param.m_pImpl), m_is_accessor(true), m_originator(originator) { }



// Destructor
template<param_mutable_type TM>
cci_param<std::string,TM>::~cci_param() {
  if (!m_is_accessor) {
    const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
    destroy_cci_param<std::string, TM>(this); 
    cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  }
}

template<param_mutable_type TM>
void cci_param<std::string,TM>::json_deserialize(const std::string& json_string) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  get_pImpl()->json_deserialize(json_string); 
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

template<param_mutable_type TM>
std::string cci_param<std::string,TM>::json_serialize() const {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const std::string& ret = get_pImpl()->json_serialize();
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<param_mutable_type TM>
const basic_param_type cci_param<std::string,TM>::get_basic_type() const {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const basic_param_type& ret = get_pImpl()->get_basic_type();
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<param_mutable_type TM>
void cci_param<std::string,TM>::set_value(const cci_value& val) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  get_pImpl()->set_value(val);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

template<param_mutable_type TM>
cci_value cci_param<std::string,TM>::get_value() const {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const cci_value& ret = get_pImpl()->get_value();
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<param_mutable_type TM>
void cci_param<std::string,TM>::set_documentation(const std::string& doc) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  get_pImpl()->set_documentation(doc);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

template<param_mutable_type TM>
std::string cci_param<std::string,TM>::get_documentation() const {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const std::string& ret = get_pImpl()->get_documentation();
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<param_mutable_type TM>
bool cci_param<std::string,TM>::is_default_value() {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = get_pImpl()->is_default_value();
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<param_mutable_type TM>
const std::string& cci_param<std::string,TM>::get_name() const {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const std::string& ret = get_pImpl()->get_name();
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<param_mutable_type TM>
cci::shared_ptr<callb_adapt> cci_param<std::string,TM>::register_callback(const callback_type type, void* observer, param_callb_func_ptr function) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const cci::shared_ptr<callb_adapt>& ret = get_pImpl()-> register_callback(type, observer, function);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<param_mutable_type TM>
cci::shared_ptr<callb_adapt> cci_param<std::string,TM>::register_callback(const callback_type type, cci::shared_ptr<callb_adapt> callb) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const cci::shared_ptr<callb_adapt>& ret = get_pImpl()-> register_callback(type, callb);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<param_mutable_type TM>
void cci_param<std::string,TM>::unregister_all_callbacks(void* observer) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  get_pImpl()->unregister_all_callbacks(observer);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

template<param_mutable_type TM>
bool cci_param<std::string,TM>::unregister_callback(cci::shared_ptr<callb_adapt> callb) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = get_pImpl()->unregister_callback(callb);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<param_mutable_type TM>
bool cci_param<std::string,TM>::unregister_callback(callb_adapt* callb) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = get_pImpl()->unregister_callback(callb);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<param_mutable_type TM>
bool cci_param<std::string,TM>::has_callbacks() {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = get_pImpl()->has_callbacks();
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<param_mutable_type TM>
bool cci_param<std::string,TM>::lock(void* pwd) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = get_pImpl()->lock(pwd);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<param_mutable_type TM>
bool cci_param<std::string,TM>::unlock(void* pwd) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = get_pImpl()->unlock(pwd);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<param_mutable_type TM>
bool cci_param<std::string,TM>::is_locked() const {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  bool ret = get_pImpl()->is_locked();
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<param_mutable_type TM>
const cci_originator* cci_param<std::string,TM>::get_latest_write_originator() const {
  //const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  return get_pImpl()->get_latest_write_originator();
  //cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  //return ret;
}

// Other
template<param_mutable_type TM>
cci::cci_param<std::string,TM>& cci_param<std::string,TM>::operator = (const cci::cci_param<std::string, TM>& v) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_pImpl->set(v.get()); 
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return *this; 
}

template<param_mutable_type TM>
cci::cci_param<std::string,TM>& cci_param<std::string,TM>::operator = (const std::string& v) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_pImpl->set(v);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return *this;
}

template<param_mutable_type TM>
cci_param<std::string,TM>::operator const std::string& () const {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const std::string& ret = m_pImpl->get(); 
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<param_mutable_type TM>
void cci_param<std::string,TM>::set(const std::string& val) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_pImpl->set(val); 
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

template<param_mutable_type TM>
const std::string& cci_param<std::string,TM>::get() const {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const std::string& ret = m_pImpl->get(); 
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<param_mutable_type TM>
void cci_param<std::string,TM>::set(const std::string& val, void* lock_pwd) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_pImpl->set(val, lock_pwd); 
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

template<param_mutable_type TM>
std::string cci_param<std::string,TM>::json_serialize(const std::string& val) const {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const std::string& ret = m_pImpl->json_serialize(val); 
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<param_mutable_type TM>
void cci_param<std::string,TM>::json_deserialize(std::string& target_val, const std::string& str) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  m_pImpl->json_deserialize(target_val, str); 
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
}

template<param_mutable_type TM>
const std::string& cci_param<std::string,TM>::get_default_value() {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  const std::string& ret = m_pImpl->get_default_value(); 
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

template<param_mutable_type TM>
cci_base_param_impl_if* cci_param<std::string,TM>::get_pImpl() const {
  assert(m_pImpl != NULL && "must not be called before m_pImpl has been set in constructor!"); 
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  cci_base_param_impl_if* ret = m_pImpl; 
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

// Param accessor functions
template<param_mutable_type TM>
bool cci_param<std::string,TM>::is_accessor() const { return m_is_accessor; }

template<param_mutable_type TM>
cci_base_param* cci_param<std::string,TM>::create_accessor(const cci_originator& originator) {
  const cci_originator* originator_backup = cci_originator::set_global_originator(&m_originator); // backup global originator pointer and set local one
  cci_base_param* ret = new cci_param<std::string,TM>(*this, originator);
  add_param_accessor(ret);
  cci_originator::set_global_originator(originator_backup); // restore original global originator pointer
  return ret;
}

// == Operators

template<class T, param_mutable_type TM, param_mutable_type TM2> bool operator == (cci_param<T, TM>& p1, cci_param<T, TM2>& p2) {
  return p1.get() == p2.get();
}

template<class T, param_mutable_type TM> bool operator == (cci_param<T, TM>& p1, T& p2) {
  return p1.get() == p2;
}

template<class T, param_mutable_type TM> bool operator == (T& p1, cci_param<T, TM>& p2) {
  return p1 == p2.get();
}


CCI_CLOSE_NAMESPACE_
