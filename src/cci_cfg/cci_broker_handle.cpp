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
 * @author Guillaume Delbergue, Ericsson / GreenSocs
 */

#include "cci_cfg/cci_broker_handle.h"
#include "cci_cfg/cci_report_handler.h"

CCI_OPEN_NAMESPACE_

cci_broker_handle::cci_broker_handle(cci_broker_if& orig_broker,
                                     const cci_originator& originator)
    : m_originator(originator), m_orig_broker(orig_broker)
{
}

cci_broker_handle::cci_broker_handle(const cci_broker_handle& broker_handle)
    : m_originator(broker_handle.m_originator),
      m_orig_broker(broker_handle.m_orig_broker)
{
}

cci_broker_handle::~cci_broker_handle()
{
}

cci_broker_handle cci_broker_handle::create_broker_handle(
  const cci_originator &originator)
{
    return m_orig_broker.create_broker_handle(originator);
}

cci_originator cci_broker_handle::get_originator() const
{
    return m_originator;
}

cci_broker_handle& cci_broker_handle::operator=(
  const cci_broker_handle& broker_handle)
{
    cci_originator originator(broker_handle.m_originator);
    std::swap(m_originator, originator);
    m_orig_broker = broker_handle.m_orig_broker;
    return *this;
}

const std::string& cci_broker_handle::name() const
{
    return m_orig_broker.name();
}

void cci_broker_handle::set_initial_cci_value(
  const std::string &parname,
  const cci::cci_value &cci_value)
{
    m_orig_broker.set_initial_cci_value(parname,
                                         cci_value,
                                         m_originator);
}

cci::cci_value cci_broker_handle::get_initial_cci_value(
  const std::string &parname) const
{
    return m_orig_broker.get_initial_cci_value(parname);
}

std::vector<cci_name_value_pair>
cci_broker_handle::get_unconsumed_initial_values() const
{
    return m_orig_broker.get_unconsumed_initial_values();
}

cci_initial_value_range cci_broker_handle::get_unconsumed_initial_values(
  const cci_initial_value_predicate &pred) const
{
    return m_orig_broker.get_unconsumed_initial_values(pred);
}

void cci_broker_handle::ignore_unconsumed_initial_values(
  const cci_initial_value_predicate &pred)
{
    m_orig_broker.ignore_unconsumed_initial_values(pred);
}

cci_originator cci_broker_handle::get_latest_write_originator(
  const std::string &parname) const
{
    return m_orig_broker.get_latest_write_originator(parname);
}

void cci_broker_handle::lock_initial_value(const std::string &parname)
{
    m_orig_broker.lock_initial_value(parname);
}

cci::cci_value cci_broker_handle::get_cci_value(
  const std::string &parname) const
{
    return m_orig_broker.get_cci_value(parname);
}

cci_param_untyped_handle cci_broker_handle::get_param_handle(
  const std::string &parname) const
{
    return m_orig_broker.get_param_handle(parname, m_originator);
}
bool cci_broker_handle::param_exists(const std::string &parname) const
{
    return m_orig_broker.param_exists(parname);
}

bool cci_broker_handle::is_used(const std::string &parname) const
{
    return m_orig_broker.is_used(parname);
}

void cci_broker_handle::add_param(cci_param_if *par)
{
    m_orig_broker.add_param(par);
}

void cci_broker_handle::remove_param(cci_param_if *par)
{
    m_orig_broker.remove_param(par);
}

const std::vector <cci_param_untyped_handle> cci_broker_handle::get_param_handles() const
{
    return m_orig_broker.get_param_handles(m_originator);
}

cci_param_range cci_broker_handle::get_param_handles(
        cci_param_predicate& pred) const
{
    return m_orig_broker.get_param_handles(pred, m_originator);
}

bool cci_broker_handle::is_private_broker() const
{
    return m_orig_broker.is_private_broker();
}

cci_param_create_callback_handle cci_broker_handle::register_create_callback(
  const cci_param_create_callback& cb)
{
    return m_orig_broker.register_create_callback(cb, m_originator);
}

bool cci_broker_handle::unregister_create_callback(
  const cci_param_create_callback_handle& cb)
{
    return m_orig_broker.unregister_create_callback(cb, m_originator);
}

cci_param_destroy_callback_handle cci_broker_handle::register_destroy_callback(
  const cci_param_destroy_callback& cb)
{
    return m_orig_broker.register_destroy_callback(cb, m_originator);
}

bool cci_broker_handle::unregister_destroy_callback(
  const cci_param_destroy_callback_handle& cb)
{
    return m_orig_broker.unregister_destroy_callback(cb, m_originator);
}

bool cci_broker_handle::unregister_all_callbacks()
{
    return m_orig_broker.unregister_all_callbacks(m_originator);
}

bool cci_broker_handle::has_callbacks() const
{
    return m_orig_broker.has_callbacks();
}

CCI_CLOSE_NAMESPACE_
