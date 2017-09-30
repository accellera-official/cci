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

#include "cci_cfg/cci_cfg_broker.h"
#include "cci_cfg/cci_config_macros.h"
#include "cci_cfg/cci_originator.h"
#include "cci_cfg/cci_report_handler.h"

CCI_OPEN_NAMESPACE_
/* anonymous */ namespace {

class cci_cfg_private_global_broker : public cci_cfg_broker
{
  friend cci_broker_handle CCI_NAMESPACE::cci_get_global_broker(const cci_originator &);
  explicit cci_cfg_private_global_broker(const std::string& nm) : cci_cfg_broker(_globalonly_) {}

  virtual bool is_global_broker() const { return true; }

};

/// Internal pointer to the one global default fallback broker instance
static cci_broker_if* singleton_broker = NULL;

} // anonymous namespace

/**
 * Creates or returns the one non-private global config broker provided
 * by the broker implementation.
 *
 * @return The one non-private global config broker
 */
cci_broker_handle cci_get_global_broker(const cci_originator &originator)
{
  if (!singleton_broker) {
    singleton_broker = new cci_cfg_private_global_broker(CCI_DEFAULT_BROKER_STRING_);
  } else {
    // this is a weak test, as you could lie about your originator, but
    // hopefully it protects errors...
    if (sc_core::sc_get_current_object() &&
        (!originator.get_parent_originator().is_unknown())) {
      CCI_REPORT_ERROR("cci_get_global_broker","You may not request the global broker from within the SystemC heirarchy");
    }
  }
  return singleton_broker->create_broker_handle(originator);
}


CCI_CLOSE_NAMESPACE_
