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
 * @author GreenSocs
 */


#include "cci_cfg/cci_param_if.h"
#include "cci_cfg/cci_cfg_private_broker.h"
#include "cci_core/cci_name_gen.h"


CCI_OPEN_NAMESPACE_


/*
 * private function to determin if we send to the parent broker or not
 */
bool cci_cfg_private_broker::sendToParent(const std::string &parname) const
{
  return  ((expose.find(parname) != expose.end()) && (!is_global_broker()));
}

/*
 * public interface functions
 */
cci_cfg_private_broker::cci_cfg_private_broker(const std::string& name)
  : cci_cfg_broker( _privateonly_, name),
    m_parent(get_parent_broker(m_originator))
{ 
  sc_assert (name.length() > 0 && "Name must not be empty");
}

cci_cfg_private_broker::~cci_cfg_private_broker()
{
}

cci_broker_handle cci_cfg_private_broker::create_broker_handle(const cci_originator& originator)
{
  return cci_broker_handle(*this, originator);
}

cci_originator cci_cfg_private_broker::get_latest_write_originator(const std::string &parname) const
{
  if (sendToParent(parname)) {
    return m_parent.get_latest_write_originator(parname);
  } else {
    return cci_cfg_broker::get_latest_write_originator(parname);
  }
}
  
  cci_value cci_cfg_private_broker::get_preset_cci_value(const std::string &parname) const
{
  if (sendToParent(parname)) {
    return m_parent.get_preset_cci_value(parname);
  } else {
    return cci_cfg_broker::get_preset_cci_value(parname);
  }
}

void cci_cfg_private_broker::lock_preset_value(const std::string &parname)
{
  if (sendToParent(parname)) {
    return m_parent.lock_preset_value(parname);
  } else {
    return cci_cfg_broker::lock_preset_value(parname);
  }
}

cci_value cci_cfg_private_broker::get_cci_value(const std::string &parname) const
{
  if (sendToParent(parname)) {
    return m_parent.get_cci_value(parname);
  } else {
    return cci_cfg_broker::get_cci_value(parname);
  }
}

bool cci_cfg_private_broker::param_exists(const std::string &parname) const
{
  if (sendToParent(parname)) {
    return m_parent.param_exists(parname);
  } else {
    return cci_cfg_broker::param_exists(parname);
  }
}

bool cci_cfg_private_broker::is_used(const std::string &parname) const
{
  if (sendToParent(parname)) {
    return m_parent.is_used(parname);
  } else {
    return cci_cfg_broker::is_used(parname);
  }
}

void cci_cfg_private_broker::add_param(cci_param_if* par)
{
  if (sendToParent(par->get_name())) {
    return m_parent.add_param(par);
  } else {
    return cci_cfg_broker::add_param(par);
  }
}

void cci_cfg_private_broker::remove_param(cci_param_if* par) {
 if (sendToParent(par->get_name())) {
    return m_parent.remove_param(par);
  } else {
    return cci_cfg_broker::remove_param(par);
  }
}

// Functions below here require an orriginator to be passed to the local
// method variant.

void cci_cfg_private_broker::set_preset_cci_value(
  const std::string &parname,
  const cci_value &cci_value,
  const cci_originator& originator)
{
  if (sendToParent(parname)) {
    return m_parent.set_preset_cci_value(parname,cci_value);
  } else {
    return cci_cfg_broker::set_preset_cci_value(parname,cci_value,originator);
  }
}
cci_param_untyped_handle cci_cfg_private_broker::get_param_handle(
        const std::string &parname,
        const cci_originator& originator) const
{
  if (sendToParent(parname)) {
    return m_parent.get_param_handle(parname);
  } else {
      cci_param_if* orig_param = get_orig_param(parname);
      if (orig_param) {
        return cci_cfg_broker::get_param_handle(*orig_param, originator);
      } else {
        return m_parent.get_param_handle(parname);
      }
  }
}


std::vector<cci_param_untyped_handle>
cci_cfg_private_broker::get_param_handles(const cci_originator& originator) const
{
  std::vector<cci_param_untyped_handle> p_param_handles=m_parent.get_param_handles();
  std::vector<cci_param_untyped_handle> param_handles=cci_cfg_broker::get_param_handles(originator);
  // this is likely to be more efficient the other way round, but it keeps
  // things consistent and means the local (mre useful) params will be at the
  // head of the list.
  param_handles.insert(param_handles.end(),p_param_handles.begin(), p_param_handles.end());
  return param_handles;
}


CCI_CLOSE_NAMESPACE_
