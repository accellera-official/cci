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


#include "cci_param_if.h"
#include "cci_cfg_private_broker.h"
#include "cci_core/cci_name_gen.h"


CCI_OPEN_NAMESPACE_
#define ORIGINATOR
#define CHOOSEFUNCTION(method, parname, ...)                        \
  if (expose.find(parname) != expose.end()) {                           \
    if (!is_global_broker()) {                                          \
      cci_broker_handle p=cci_broker_manager::get_parent_broker(m_originator); \
      std::string _parname=m_originator.name() + std::string(".") + parname; \
      return p.method(__VA_ARGS__);                                     \
    } else {                                                            \
      std::string _parname=parname;                                     \
      return cci_cfg_broker::method(__VA_ARGS__ ORIGINATOR);            \
    }                                                                   \
  } else {                                                              \
    std::string _parname=parname;                                       \
    return cci_cfg_broker::method(__VA_ARGS__ ORIGINATOR);             \
  }                                                                     \
  

cci_cfg_private_broker::cci_cfg_private_broker(const std::string& name)
    : cci_cfg_broker(name)
{ 
  assert (name.length() > 0 && "Name must not be empty");
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
  CHOOSEFUNCTION(get_latest_write_originator, parname,  _parname);
}

const cci_value cci_cfg_private_broker::get_initial_cci_value(const std::string &parname)
{
CHOOSEFUNCTION(get_initial_cci_value, parname,  _parname);
}

void cci_cfg_private_broker::lock_initial_value(const std::string &parname)
{
  CHOOSEFUNCTION(lock_initial_value, parname,  _parname);
}

const cci_value cci_cfg_private_broker::get_cci_value(const std::string &parname)
{
  CHOOSEFUNCTION(get_cci_value, parname,  _parname);
}

bool cci_cfg_private_broker::param_exists(const std::string &parname)
{
  CHOOSEFUNCTION(param_exists, parname,  _parname);
}

bool cci_cfg_private_broker::is_used(const std::string &parname)
{
  CHOOSEFUNCTION(is_used, parname,  _parname);
}

void cci_cfg_private_broker::add_param(cci_param_if* par) {
  CHOOSEFUNCTION(add_param, par->get_name(),  par);
}

void cci_cfg_private_broker::remove_param(cci_param_if* par) {
  CHOOSEFUNCTION(remove_param, par->get_name(),  par);
}

// Functions below here require an orriginator to be passed to the local
// method variant.
#undef ORIGINATOR
#define ORIGINATOR ,originator
void cci_cfg_private_broker::set_initial_cci_value(
  const std::string &parname,
  const cci_value &cci_value,
  const cci_originator& originator)
{
  CHOOSEFUNCTION(set_initial_cci_value, parname,  _parname, cci_value);
}
cci_param_untyped_handle cci_cfg_private_broker::get_param_handle(
        const std::string &parname,
        const cci_originator& originator)
{
  CHOOSEFUNCTION(get_param_handle, parname,  _parname);
}



CCI_CLOSE_NAMESPACE_
