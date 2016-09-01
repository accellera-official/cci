/*****************************************************************************
  Copyright 2016 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2016 Ericsson AB
  All rights reserved.

  Copyright 2016 GreenSocs
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

#include "cci_param_untyped_handle.h"

CCI_OPEN_NAMESPACE_

cci_param_untyped_handle::cci_param_untyped_handle(cci_param_if & orig_param, const cci_originator & originator)
        : m_originator(originator), m_orig_param(orig_param)
{}

cci_param_untyped_handle::~cci_param_untyped_handle()
{}

std::string cci_param_untyped_handle::get_description() const
{
    return m_orig_param.get_description();
}

void cci_param_untyped_handle::set_cci_value(const cci_value& val)
{
    m_orig_param.set_cci_value(val, m_originator);
}

cci_value cci_param_untyped_handle::get_cci_value() const
{
    return m_orig_param.get_cci_value();
}

bool cci_param_untyped_handle::is_default_value()
{
    return m_orig_param.is_default_value();
}

bool cci_param_untyped_handle::is_initial_value() const
{
    return m_orig_param.is_initial_value();
}

const cci_originator* cci_param_untyped_handle::get_latest_write_originator() const
{
    return m_orig_param.get_latest_write_originator();
}

shared_ptr<callb_adapt> cci_param_untyped_handle::register_callback(const callback_type type, void* observer, param_callb_func_ptr function)
{
    return m_orig_param.register_callback(type, observer, function, *this);
}

shared_ptr<callb_adapt> cci_param_untyped_handle::register_callback(const callback_type type, shared_ptr<callb_adapt> callb)
{
    return m_orig_param.register_callback(type, callb, *this);
}

void cci_param_untyped_handle::unregister_all_callbacks(void* observer)
{
    m_orig_param.unregister_all_callbacks(observer);
}

bool cci_param_untyped_handle::unregister_callback(cci::shared_ptr<callb_adapt> callb)
{
    return m_orig_param.unregister_callback(callb);
}

bool cci_param_untyped_handle::unregister_callback(callb_adapt* callb)
{
    return m_orig_param.unregister_callback(callb);
}

bool cci_param_untyped_handle::has_callbacks()
{
    return m_orig_param.has_callbacks();
}

bool cci_param_untyped_handle::lock(void* pwd)
{
    return m_orig_param.lock(pwd);
}

bool cci_param_untyped_handle::unlock(void* pwd)
{
    return m_orig_param.unlock(pwd);
}

bool cci_param_untyped_handle::is_locked() const
{
    return m_orig_param.is_locked();
}

basic_param_type cci_param_untyped_handle::get_basic_type() const
{
    return m_orig_param.get_basic_type();
}

const std::string& cci_param_untyped_handle::get_name() const
{
    return m_orig_param.get_name();
}

cci_originator cci_param_untyped_handle::get_originator() const
{
    return m_originator;
}

const void* cci_param_untyped_handle::get_raw_value() const
{
    return m_orig_param.get_raw_value();
}

const void* cci_param_untyped_handle::get_default_value_raw() const
{
    return m_orig_param.get_default_value_raw();
}

void cci_param_untyped_handle::set_raw_value(const void* vp)
{
    m_orig_param.set_raw_value(vp, m_originator);
}

void cci_param_untyped_handle::set_raw_value(const void* vp, const void* pwd)
{
    m_orig_param.set_raw_value(vp, pwd, m_originator);
}

bool cci_param_untyped_handle::equals(const cci_param_untyped_handle& rhs) const
{
    return m_orig_param.equals(rhs.m_orig_param);
}

bool cci_param_untyped_handle::equals(const cci_param_if& rhs) const
{
    return m_orig_param.equals(rhs);
}

bool cci_param_untyped_handle::is_handle() const
{
    return true;
}

void cci_param_untyped_handle::init()
{
    m_orig_param.init();
}

void cci_param_untyped_handle::destroy()
{
    m_orig_param.destroy();
}

CCI_CLOSE_NAMESPACE_
