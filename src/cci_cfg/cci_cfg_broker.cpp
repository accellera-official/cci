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


#include "cci_cfg/cci_broker_if.h"
#include "cci_cfg/cci_broker_manager.h"
#include "cci_cfg/cci_param_if.h"
#include "cci_cfg/cci_cfg_broker.h"
#include "cci_core/cci_name_gen.h"
#include "cci_cfg/cci_report_handler.h"

CCI_OPEN_NAMESPACE_


cci_cfg_broker::cci_cfg_broker(const std::string& name)
  : m_name(cci_gen_unique_name(name.c_str())),
    m_originator(cci_originator(m_name))
{ 
  sc_assert (name.length() > 0 && "Name must not be empty");
}

cci_cfg_broker::~cci_cfg_broker()
{
}

cci_broker_handle cci_cfg_broker::create_broker_handle(const cci_originator& originator)
{
  return cci_broker_handle(*this, originator);
}

const std::string &cci_cfg_broker::name() const
{
  return m_name;
}

void cci_cfg_broker::set_initial_cci_value(
        const std::string &parname,
        const cci_value &cci_value,
        const cci_originator& originator)
{
      
  if (locked.find(parname) != locked.end()) {
    cci_report_handler::set_param_failed("Setting initial value failed (parameter locked).");
  } else {
    std::map<std::string,cci::cci_value>::const_iterator iter =
      m_used_value_registry.find(parname);
    if (iter != m_used_value_registry.end() ) {
      m_used_value_registry[parname]=cci_value; // kiss a zombee
      if (sc_core::sc_get_status()==sc_core::SC_ELABORATION) {
        // In this special case, we permit direct writes to existing params
        cci_param_if* p = get_orig_param(parname);
        if (p) {
          p->set_cci_value(cci_value);
        }
      }
    } else {
      m_unused_value_registry[parname] = cci_value;
    }
    m_implicit_originator_map.insert(
            std::pair<std::string, cci::cci_originator>(parname, originator));
  }
}

std::vector<cci_name_value_pair> cci_cfg_broker::get_unconsumed_initial_values()
{
  std::vector<cci_name_value_pair> unconsumed_initial_cci_values;
  std::map<std::string, cci_value>::iterator iter;
  for( iter = m_unused_value_registry.begin(); iter != m_unused_value_registry.end(); ++iter ) {
    unconsumed_initial_cci_values.push_back(std::make_pair(iter->first, iter->second));
  }
  return unconsumed_initial_cci_values;
}

cci_initial_value_range cci_cfg_broker::get_unconsumed_initial_values(
        const cci_initial_value_predicate &pred)
{
  return cci_initial_value_range(pred, get_unconsumed_initial_values());
}

void cci_cfg_broker::ignore_unconsumed_initial_values(const cci_initial_value_predicate &pred)
{
  std::vector< std::pair<std::string, cci_value> >
          unconsumed_initial_cci_values = get_unconsumed_initial_values();
  for(std::vector< std::pair<std::string, cci_value> >::iterator it =
          unconsumed_initial_cci_values.begin();
      it != unconsumed_initial_cci_values.end(); it++) {
    if(pred(*it)) {
      m_ignored_unconsumed_initial_cci_values.push_back(it->first);
    }
  }
}

cci_originator cci_cfg_broker::get_latest_write_originator(const std::string &parname) const
{
  cci_param_if* p = get_orig_param(parname);
  if (p) {
      return p->get_latest_write_originator();
  }
  std::map<std::string, cci_originator>::const_iterator it;
  it = m_implicit_originator_map.find(parname);
  if (it != m_implicit_originator_map.end()) {
    return it->second;
  }
  // if the param doesn't exist, we should return 'unkown_originator'
  return cci_originator(__CCI_UNKNOWN_ORIGINATOR_STRING__);
}

const cci_value cci_cfg_broker::get_initial_cci_value(const std::string &parname)
{
  {
    std::map<std::string,cci_value>::const_iterator iter =
      m_used_value_registry.find(parname);
    if (iter != m_used_value_registry.end() ) {
      return iter->second;
    }
  }
  {
    std::map<std::string,cci_value>::const_iterator iter =
      m_unused_value_registry.find(parname);
    if (iter != m_unused_value_registry.end() ) {
      return iter->second;
    }
  }
// If there is nothing in the database, return NULL.
  return cci_value::from_json("null");
}

void cci_cfg_broker::lock_initial_value(const std::string &parname)
{
  // no error is possible. Even if the parameter does not yet exist.
  locked.insert(parname);
}

const cci_value cci_cfg_broker::get_cci_value(const std::string &parname)
{
  cci_param_if* p = get_orig_param(parname);
  if(p) {
    return p->get_cci_value();
  } else {
    std::map<std::string,cci_value>::const_iterator iter =
      m_unused_value_registry.find(parname);
    if (iter != m_unused_value_registry.end() ) {
      return iter->second;
    }
    cci_report_handler::get_param_failed("Unable to find the parameter to get value");
    return cci_value::from_json("null");
  }
}

cci_param_if* cci_cfg_broker::get_orig_param(
        const std::string &parname) const
{
  std::map<std::string,cci_param_if*>::const_iterator iter =
          m_param_registry.find(parname);
  if( iter != m_param_registry.end() ) {
    cci_param_if* ret = iter->second;
    sc_assert(ret != NULL && "This param shall be a cci_param_if!");
    return ret;
  }
  else return NULL;
}

/*
 * This entire broker can be re-used as a 'greedy' private broker, the broker
 * should remain the same, except for when a model asks for a param handle for a
 * param that this broker has no knowldegd of, at which point it should ask up
 * the broker tree. This case is handled below. The intention is that a well
 * behaved private broker can then 'wrap' this broker, passing 'public' params up
 * the broker tree and bypassing this broker, while this broker will handle all
 * 'private' params
 */

cci_param_untyped_handle cci_cfg_broker::get_param_handle(
        const std::string &parname,
        const cci_originator& originator)
{
  cci_param_if* orig_param = get_orig_param(parname);
  if (orig_param) {
    return get_param_handle(*orig_param, originator);
  } else {
    cci_originator parent_originator = m_originator.get_parent_originator();
    if(parent_originator.is_unknown()) {
      return cci_param_untyped_handle(originator);
    } else {
      cci_broker_handle parent=cci_broker_manager::get_broker(parent_originator).create_broker_handle(originator);
      if (parent.name() == name()) {
        return cci_param_untyped_handle(originator);
      } else {
        return parent.get_param_handle(parname);
      }
    }
  }
}

bool cci_cfg_broker::param_exists(const std::string &parname)
{
  {
    std::map<std::string,cci_param_if*>::iterator iter =
      m_param_registry.find(parname);
    if( iter != m_param_registry.end() ) {
      return true;
    }
  }
  {
    std::map<std::string,cci_value>::const_iterator iter =
      m_unused_value_registry.find(parname);
    if (iter != m_unused_value_registry.end()  ) {
      return true;
    }
  }
  return false;
}

bool cci_cfg_broker::is_used(const std::string &parname) {
  std::map<std::string,cci_param_if*>::iterator iter =
          m_param_registry.find(parname);  
  if(iter != m_param_registry.end() ) {
    return true;
  } else {
    return false;
  }
}

cci_param_create_callback_handle
cci_cfg_broker::register_create_callback(
        const cci_param_create_callback &cb,
        const cci_originator &orig) {
  m_create_callbacks.push_back(create_callback_obj_t(cb, orig));
  return cb;
}

bool
cci_cfg_broker::unregister_create_callback(
        const cci_param_create_callback_handle &cb,
        const cci_originator &orig) {
  std::vector<create_callback_obj_t>::iterator it;
  for(it=m_create_callbacks.begin() ; it < m_create_callbacks.end(); it++ )
  {
    if(it->callback == cb && it->originator == orig) {
      m_create_callbacks.erase(it);
      return true;
    }
  }
  return false;
}

cci_param_destroy_callback_handle
cci_cfg_broker::register_destroy_callback(
        const cci_param_destroy_callback &cb,
        const cci_originator& orig) {
  m_destroy_callbacks.push_back(destroy_callback_obj_t(cb, orig));
  return cb;
}

bool
cci_cfg_broker::unregister_destroy_callback(
        const cci_param_destroy_callback_handle &cb,
        const cci_originator &orig) {
  std::vector<destroy_callback_obj_t>::iterator it;
  for(it=m_destroy_callbacks.begin() ; it < m_destroy_callbacks.end(); it++ )
  {
    if(it->callback == cb && it->originator == orig) {
      m_destroy_callbacks.erase(it);
      return true;
    }
  }
  return false;
}

bool cci_cfg_broker::unregister_all_callbacks(
        const cci_originator &orig) {
  bool result = false;
  std::vector<create_callback_obj_t>::iterator it;
  for(it=m_create_callbacks.begin() ; it < m_create_callbacks.end(); it++ )
  {
    if(it->originator == orig) {
      m_create_callbacks.erase(it);
      result = true;
    }
  }
  std::vector<destroy_callback_obj_t>::iterator itt;
  for (itt = m_destroy_callbacks.begin();
       itt < m_destroy_callbacks.end(); itt++)
  {
    if(itt->originator == orig) {
      m_destroy_callbacks.erase(itt);
      result = true;
    }
  }
  return result;
}

bool cci_cfg_broker::has_callbacks() const {
  return (!m_create_callbacks.empty() ||
          !m_destroy_callbacks.empty());
}

void cci_cfg_broker::add_param(cci_param_if* par) {
  sc_assert(par != NULL && "Unable to add a NULL parameter");
  bool new_element = m_param_registry.insert(
          std::pair<std::string, cci_param_if*>(par->get_name(), par)).second;
  sc_assert(new_element && "The same parameter had been added twice!!");

  std::map<std::string,cci_value>::const_iterator iter =
    m_unused_value_registry.find(par->get_name());
  if (iter != m_unused_value_registry.end()  ) {
    m_used_value_registry.insert(std::make_pair(iter->first, iter->second));
    m_unused_value_registry.erase(iter);
  }
  // Create callbacks
  for (unsigned i = 0; i < m_create_callbacks.size(); ++i) {
    m_create_callbacks[i].callback.invoke(
            par->create_param_handle(par->get_originator()));
  }
}

void cci_cfg_broker::remove_param(cci_param_if* par) {
  sc_assert(par != NULL && "Unable to remove a NULL parameter");
  m_param_registry.erase(par->get_name());

  std::map<std::string,cci_value>::const_iterator iter =
    m_used_value_registry.find(par->get_name());
  if (iter != m_used_value_registry.end()  ) {
    m_unused_value_registry.insert(std::make_pair(iter->first, iter->second));
    m_used_value_registry.erase(iter);    
  }

  // Destroy callbacks
  for (unsigned i = 0; i < m_destroy_callbacks.size(); ++i) {
    m_destroy_callbacks[i].callback.invoke(
            par->create_param_handle(par->get_originator()));
  }
}

const std::vector<cci_param_untyped_handle>
cci_cfg_broker::get_param_handles(const cci_originator& originator) {
  std::vector<cci_param_untyped_handle> param_handles;
  std::map<std::string,cci_param_if*>::const_iterator it;
  for (it=m_param_registry.begin(); it != m_param_registry.end(); ++it) {
    cci_param_if* p = it->second;
    param_handles.push_back(get_param_handle(*p, originator));
  }
  return param_handles;
}

cci_param_range cci_cfg_broker::get_param_handles(
        cci_param_predicate& pred,
        const cci_originator& originator) {
  return cci_param_range(pred,
        get_param_handles(originator));
}


cci_param_untyped_handle cci_cfg_broker::get_param_handle(
        cci_param_if& orig_param,
        const cci_originator& originator) const {
    cci_param_untyped_handle param_handle = orig_param.create_param_handle(
            originator);
    sc_assert(param_handle.is_valid());
    return param_handle;
}


bool cci_cfg_broker::is_global_broker() const
{
  return  name()  == cci_get_global_broker().name();
}
bool cci_cfg_broker::is_private_broker() const
{
  return !is_global_broker();
}

CCI_CLOSE_NAMESPACE_
