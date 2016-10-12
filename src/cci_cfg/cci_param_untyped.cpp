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
 */

#include "cci_param_untyped.h"
#include <greencontrol/config.h>

CCI_OPEN_NAMESPACE_

cci_param_untyped::cci_param_untyped(bool is_top_level_name,
                               cci_broker_if* broker_handle,
                               const std::string& desc,
                               const cci_originator& originator)
: m_gs_param_base(NULL), m_is_default_value(0), m_is_invalid_value(!0), // TODO
  m_description(desc), m_init_called(false),
  m_broker_handle(broker_handle),
  m_value_originator(originator),
  m_originator(originator)
{
}

cci_param_untyped::~cci_param_untyped()
{
    m_broker_handle->remove_param(this);
    assert(m_init_called && "If this happens, the construction did not call the base param init function!");
    sc_core::sc_unregister_hierarchical_name(get_name().c_str());
    for (std::vector<cci_param_untyped_handle*>::iterator it = m_param_handles.begin();
         it != m_param_handles.end(); it++) {
        (*it)->invalidate();
    }
    delete m_gs_param_base;
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
    assert(m_gs_param_base != NULL && "This must been set immediately after construction!");
    return m_gs_param_base->is_initial_value();
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
    m_##name##_callbacks.push_back(name##_callback_obj_t(cb, orig));           \
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
    for(it=m_##name##_callbacks.begin() ; it < m_##name##_callbacks.end();     \
        it++)                                                                  \
    {                                                                          \
        if(it->callback == cb && it->originator == orig) {    \
            m_##name##_callbacks.erase(it);                                    \
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
            m_pre_write_callbacks.begin();
         it < m_pre_write_callbacks.end(); it++)
    {
        if(it->originator == orig) {
            m_pre_write_callbacks.erase(it);
            result = true;
        }
    }
    for(std::vector<post_write_callback_obj_t>::iterator it =
            m_post_write_callbacks.begin();
        it < m_post_write_callbacks.end(); it++)
    {
        if(it->originator == orig) {
            m_post_write_callbacks.erase(it);
            result = true;
        }
    }
    for(std::vector<pre_read_callback_obj_t>::iterator it =
            m_pre_read_callbacks.begin();
        it < m_pre_read_callbacks.end(); it++)
    {
        if(it->originator == orig) {
            m_pre_read_callbacks.erase(it);
            result = true;
        }
    }
    for(std::vector<post_read_callback_obj_t>::iterator it =
            m_post_read_callbacks.begin();
        it < m_post_read_callbacks.end(); it++)
    {
        if(it->originator == orig) {
            m_post_read_callbacks.erase(it);
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
    return (!m_post_write_callbacks.empty() ||
            !m_pre_write_callbacks.empty());
}

bool cci_param_untyped::lock(void* pwd)
{
    assert(m_gs_param_base != NULL && "This must been set immediately after construction!");
    return m_gs_param_base->lock(pwd);
}

bool cci_param_untyped::unlock(void* pwd)
{
    assert(m_gs_param_base != NULL && "This must been set immediately after construction!");
    return m_gs_param_base->unlock(pwd);
}

bool cci_param_untyped::is_locked() const
{
    assert(m_gs_param_base != NULL && "This must been set immediately after construction!");
    return m_gs_param_base->locked();
}

const std::string& cci_param_untyped::get_name() const
{
    assert(m_gs_param_base != NULL && "This must been set immediately after construction!");
    return m_gs_param_base->getName();
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

void cci_param_untyped::remove_param_handle(cci_param_untyped_handle* param_handle)
{
    m_param_handles.erase(std::remove(m_param_handles.begin(), m_param_handles.end(), param_handle), m_param_handles.end());
}

CCI_CLOSE_NAMESPACE_
