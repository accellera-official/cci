/*****************************************************************************
  Copyright 2006-2015 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2006-2014 OFFIS Institute for Information Technology
  All rights reserved.

  Copyright 2006-2015 Intel Corporation
  All rights reserved.

  Copyright 2016 Ericsson AB
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
  m_latest_write_access_originator_cp("NONE"),
  m_latest_write_access_originator_valid(false),
  m_originator(originator)
{
}

cci_param_untyped::~cci_param_untyped()
{
    m_broker_handle->remove_param(this);
    assert(m_init_called && "If this happens, the construction did not call the base param init function!");
    sc_core::sc_unregister_hierarchical_name(get_name().c_str());
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

const cci_originator* cci_param_untyped::get_latest_write_originator() const
{
    if (!m_latest_write_access_originator_valid) {
        return NULL;
    }
    return &m_latest_write_access_originator_cp;
}

void cci_param_untyped::update_latest_write_originator(const cci_originator& originator) const
{
    m_latest_write_access_originator_valid = true;
    m_latest_write_access_originator_cp = originator;
}

cci_callback_untyped_handle cci_param_untyped::register_write_callback(
        const cci_callback_untyped_handle &cb,
        const cci_originator &orig)
{
    write_callback_obj_t test(cb, orig);
    m_write_callbacks.push_back(write_callback_obj_t(cb, orig));
    return cb;
}

bool cci_param_untyped::unregister_write_callback(
        const cci_callback_untyped_handle &cb,
        const cci_originator &orig)
{
    std::vector<write_callback_obj_t>::iterator it;
    for(it=m_write_callbacks.begin() ; it < m_write_callbacks.end(); it++ )
    {
        if(it->callback == cb /*&& it->originator == orig TODO: FIXME */) {
            m_write_callbacks.erase(it);
            return true;
        }
    }
    return false;
}

cci_callback_untyped_handle
cci_param_untyped::register_validate_write_callback(
        const cci_callback_untyped_handle &cb,
        const cci_originator &orig)
{
    m_validate_write_callbacks.push_back(
            validate_write_callback_obj_t(cb, orig));
    return cb;
}

bool cci_param_untyped::unregister_validate_write_callback(
        const cci_callback_untyped_handle &cb,
        const cci_originator &orig)
{
    std::vector<validate_write_callback_obj_t>::iterator it;
    for (it = m_validate_write_callbacks.begin();
         it < m_validate_write_callbacks.end(); it++)
    {
        if(it->callback == cb /*&& it->originator == orig TODO: FIXME */) {
            m_validate_write_callbacks.erase(it);
            return true;
        }
    }
    return false;
}

cci_callback_untyped_handle cci_param_untyped::register_read_callback(
        const cci_callback_untyped_handle &cb,
        const cci_originator &orig)
{
    m_read_callbacks.push_back(read_callback_obj_t(cb, orig));
    return cb;
}

bool cci_param_untyped::unregister_read_callback(
        const cci_callback_untyped_handle &cb,
        const cci_originator &orig)
{
    std::vector<read_callback_obj_t>::iterator it;
    for(it=m_read_callbacks.begin() ; it < m_read_callbacks.end(); it++ )
    {
        if(it->callback == cb /*&& it->originator == orig TODO: FIXME */) {
            m_read_callbacks.erase(it);
            return true;
        }
    }
    return false;
}

bool cci_param_untyped::unregister_all_callbacks(const cci_originator &orig)
{
    bool result = false;
    std::vector<write_callback_obj_t>::iterator it;
    for(it=m_write_callbacks.begin() ; it < m_write_callbacks.end(); it++ )
    {
        if(1/*it->originator == orig TODO: FIXME */) {
            m_write_callbacks.erase(it);
            result = true;
        }
    }
    std::vector<validate_write_callback_obj_t>::iterator itt;
    for (itt = m_validate_write_callbacks.begin();
         itt < m_validate_write_callbacks.end(); itt++)
    {
        if(1/*itt->originator == orig TODO: FIXME */) {
            m_validate_write_callbacks.erase(itt);
            result = true;
        }
    }
    return result;
}

cci_callback_untyped_handle
cci_param_untyped::register_write_callback(
        const cci_param_write_callback_untyped& cb,
        cci_untyped_tag)
{
    return register_write_callback(cb, m_originator);
}

template<typename C>
cci_callback_untyped_handle
cci_param_untyped::register_write_callback(
        cci_param_write_callback_untyped::signature(C::*cb), C* obj,
        cci_untyped_tag)
{
    return register_write_callback(sc_bind(cb, obj, sc_unnamed::_1),
                                   m_originator);
}

cci_callback_untyped_handle
cci_param_untyped::register_validate_write_callback(
        const cci_param_validate_write_callback_untyped& cb,
        cci_untyped_tag)
{
    return register_validate_write_callback(cb, m_originator);
}

template<typename C>
cci_callback_untyped_handle
cci_param_untyped::register_validate_write_callback(
        cci_param_validate_write_callback_untyped::signature(C::*cb),
        C* obj, cci_untyped_tag)
{
    return register_validate_write_callback(sc_bind(cb, obj, sc_unnamed::_1),
                                            m_originator);
}


cci_callback_untyped_handle
cci_param_untyped::register_read_callback(
        const cci_param_read_callback_untyped& cb, cci_untyped_tag)
{
    return register_read_callback(cb, m_originator);
}

template<typename C>
cci_callback_untyped_handle
cci_param_untyped::register_read_callback(
        cci_param_read_callback_untyped::signature (C::*cb), C* obj,
        cci_untyped_tag)
{
    return register_read_callback(sc_bind(cb, obj, sc_unnamed::_1),
                                  m_originator);
}

bool cci_param_untyped::unregister_all_callbacks()
{
    return unregister_all_callbacks(m_originator);
}

bool cci_param_untyped::has_callbacks() const
{
    return (!m_write_callbacks.empty() ||
            !m_validate_write_callbacks.empty());
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

CCI_CLOSE_NAMESPACE_
