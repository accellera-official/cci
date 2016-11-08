/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 ****************************************************************************/
/**
 * @author Enrico Galli, Intel
 * @author Guillaume Delbergue, GreenSocs / Ericsson
 */

#include "cci_param_untyped.h"
#include "cci_broker_if.h"
#include "cci_core/cci_name_gen.h"

CCI_OPEN_NAMESPACE_

cci_param_untyped::cci_param_untyped(const std::string& name,
                                     cci_name_type name_type,
                                     cci_broker_if* broker_handle,
                                     const std::string& desc,
                                     const cci_originator& originator)
: m_is_default_value(true), m_is_initial_value(false),
  m_description(desc), m_init_called(false), m_locked(false),
  m_lock_pwd(NULL), m_broker_handle(broker_handle),
  m_value_originator(originator),
  m_originator(originator)
{
    if(name_type == CCI_ABSOLUTE_NAME) {
        m_name = name;
    } else {
        sc_core::sc_object* current_obj = sc_core::sc_get_current_object();
        for (sc_core::sc_process_handle current_proc(current_obj);
             current_proc.valid();
             current_proc = sc_core::sc_process_handle(current_obj)) { 
            current_obj = current_proc.get_parent_object(); 
        }
        if(current_obj) {
            m_name = std::string(current_obj->name()) +
                sc_core::SC_HIERARCHY_CHAR + name;
        } else {
            m_name = name;
        }
    }

    // Handle name collision and destruction / resurrection
    std::string unique_name = std::string(cci_gen_unique_name(m_name.c_str()));
    if (unique_name != m_name
        && (sc_core::sc_hierarchical_name_exists(m_name.c_str())
            || broker_handle->param_exists(m_name))) {
        m_name = unique_name;
    }
}

cci_param_untyped::~cci_param_untyped()
{
    if(m_init_called) {
        m_broker_handle->remove_param(this);
        for (std::vector<cci_param_untyped_handle*>::iterator
                     it = m_param_handles.begin();
             it != m_param_handles.end(); it++) {
            (*it)->invalidate();
        }
    }
    if(!m_name.empty()) {
        cci_unregister_name(get_name().c_str());
    }
}

void cci_param_untyped::set_description(const std::string& desc)
{
    m_description = desc;
}

std::string cci_param_untyped::get_description() const
{
    return m_description;
}

void cci_param_untyped::add_metadata(const std::string &name,
                                     const cci_value &value,
                                     const std::string &desc)
{
    metadata.push_entry(name, cci_value_list().push_back(value)
            .push_back(desc));
}

cci_value_map cci_param_untyped::get_metadata() const
{
    return metadata;
}

bool cci_param_untyped::is_default_value()
{
    return m_is_default_value;
}

bool cci_param_untyped::is_initial_value() const
{
    return m_is_initial_value;
}

const cci_originator& cci_param_untyped::get_latest_write_originator() const
{
    return m_value_originator;
}

void cci_param_untyped::update_latest_write_originator(
        const cci_originator& originator) const
{
    m_value_originator = originator;
}

#define CCI_PARAM_UNTYPED_CALLBACK_IMPL_(name)                                 \
cci_callback_untyped_handle                                                    \
cci_param_untyped::register_##name##_callback(                                 \
        const cci_callback_untyped_handle &cb,                                 \
        const cci_originator &orig)                                            \
{                                                                              \
    m_##name##_callbacks.vec.push_back(name##_callback_obj_t(cb, orig));       \
    return cb;                                                                 \
}                                                                              \
                                                                               \
cci_callback_untyped_handle                                                    \
cci_param_untyped::register_##name##_callback(                                 \
        const cci_param_##name##_callback_untyped& cb,                         \
        cci_untyped_tag)                                                       \
{                                                                              \
    return cci_param_untyped::register_##name##_callback(cb, m_originator);    \
}                                                                              \
                                                                               \
bool                                                                           \
cci_param_untyped::unregister_##name##_callback(                               \
        const cci_callback_untyped_handle &cb,                                 \
        const cci_originator &orig)                                            \
{                                                                              \
    std::vector<name##_callback_obj_t>::iterator it;                           \
    for(it=m_##name##_callbacks.vec.begin() ;                                  \
        it < m_##name##_callbacks.vec.end();                                   \
        it++)                                                                  \
    {                                                                          \
        if(it->callback == cb && it->originator == orig) {                     \
            m_##name##_callbacks.vec.erase(it);                                \
            return true;                                                       \
        }                                                                      \
    }                                                                          \
    return false;                                                              \
}

CCI_PARAM_UNTYPED_CALLBACK_IMPL_(pre_write)

CCI_PARAM_UNTYPED_CALLBACK_IMPL_(post_write)

CCI_PARAM_UNTYPED_CALLBACK_IMPL_(pre_read)

CCI_PARAM_UNTYPED_CALLBACK_IMPL_(post_read)

bool cci_param_untyped::unregister_all_callbacks(const cci_originator &orig)
{
    bool result = false;
    for (std::vector<pre_write_callback_obj_t>::iterator it =
            m_pre_write_callbacks.vec.begin();
         it < m_pre_write_callbacks.vec.end(); it++)
    {
        if(it->originator == orig) {
            m_pre_write_callbacks.vec.erase(it);
            result = true;
        }
    }
    for(std::vector<post_write_callback_obj_t>::iterator it =
            m_post_write_callbacks.vec.begin();
        it < m_post_write_callbacks.vec.end(); it++)
    {
        if(it->originator == orig) {
            m_post_write_callbacks.vec.erase(it);
            result = true;
        }
    }
    for(std::vector<pre_read_callback_obj_t>::iterator it =
            m_pre_read_callbacks.vec.begin();
        it < m_pre_read_callbacks.vec.end(); it++)
    {
        if(it->originator == orig) {
            m_pre_read_callbacks.vec.erase(it);
            result = true;
        }
    }
    for(std::vector<post_read_callback_obj_t>::iterator it =
            m_post_read_callbacks.vec.begin();
        it < m_post_read_callbacks.vec.end(); it++)
    {
        if(it->originator == orig) {
            m_post_read_callbacks.vec.erase(it);
            result = true;
        }
    }
    return result;
}

bool cci_param_untyped::unregister_all_callbacks()
{
    return unregister_all_callbacks(m_originator);
}

bool cci_param_untyped::has_callbacks() const
{
    return (!m_post_write_callbacks.vec.empty() ||
            !m_pre_write_callbacks.vec.empty());
}

bool cci_param_untyped::lock(void* pwd)
{
    if(m_locked && pwd != m_lock_pwd && m_lock_pwd != NULL) {
        return false;
    } else {
        m_lock_pwd = pwd;
        m_locked = true;
        return true;
    }
}

bool cci_param_untyped::unlock(void* pwd)
{
    if(pwd == m_lock_pwd) {
        m_locked = false;
    }
    return !m_locked;
}

bool cci_param_untyped::is_locked() const
{
    return m_locked;
}

const std::string& cci_param_untyped::get_name() const
{
    return m_name;
}

cci_originator cci_param_untyped::get_originator() const
{
    return m_originator;
}

void cci_param_untyped::init()
{
    assert(m_init_called == false && "init() function called more than once!");
    m_init_called = true;
}

void cci_param_untyped::add_param_handle(cci_param_untyped_handle* param_handle)
{
    m_param_handles.push_back(param_handle);
}

void cci_param_untyped::remove_param_handle(
        cci_param_untyped_handle* param_handle)
{
    m_param_handles.erase(std::remove(m_param_handles.begin(),
                                      m_param_handles.end(),
                                      param_handle),
                          m_param_handles.end());
}

CCI_CLOSE_NAMESPACE_
