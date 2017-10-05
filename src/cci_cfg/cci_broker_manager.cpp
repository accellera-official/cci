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

#include "cci_cfg/cci_broker_manager.h"
#include "cci_cfg/cci_broker_if.h"
#include "cci_cfg/cci_originator.h"
#include "cci_cfg/cci_report_handler.h"

CCI_OPEN_NAMESPACE_

std::map<cci_originator, cci_broker_handle> cci_broker_manager::m_brokers;

cci_broker_handle
cci_broker_manager::get_broker()
{
  const cci_originator top_originator=cci_originator(CCI_BROKER_HANDLE_ORIGINATOR_STRING_);
  cci_originator originator=top_originator;
  while (true) {
    std::map<cci_originator, cci_broker_handle>::iterator it;
    for (it=m_brokers.begin();it!=m_brokers.end();++it) {
      if (originator.get_object()==it->first.get_object()) {
        break;
      }
    }
    if(it != m_brokers.end()) {
      return (it->second).create_broker_handle(top_originator);
    }

    if(originator.is_unknown()) {
      CCI_REPORT_ERROR("cci_broker_manager","No global broker found!");
      // We will abort at this point, unless the report is caught.
    }
    originator = originator.get_parent_originator();
  }
}

cci_broker_handle
cci_broker_manager::register_broker(cci_broker_if& broker)
{
    cci_originator originator(CCI_BROKER_ORIGINATOR_STRING_);
    cci_broker_handle broker_handle =
      broker.create_broker_handle(originator);

    std::map<cci_originator, cci_broker_handle>::iterator it =
            m_brokers.find(originator);    
    if(it != m_brokers.end()) {
        CCI_REPORT_ERROR("cci_broker_manager/register_broker",
                         "A broker is already registered in the current"
                                 " hierarchy.");
        /* abort here, if error is suppressed? */
    } else {
        m_brokers.insert(std::make_pair(originator, broker_handle));
    }

    return broker_handle;
}



cci_broker_handle cci_get_broker() {
  return cci_broker_manager::get_broker();
}
cci_broker_handle cci_register_broker(cci_broker_if& broker) 
{
  return cci_broker_manager::register_broker(broker);
}
cci_broker_handle cci_register_broker(cci_broker_if* broker) 
{
  return cci_broker_manager::register_broker(broker);
}
cci_broker_handle cci_get_global_broker(const cci_originator &originator)
{
  if (sc_core::sc_get_current_object() ||
      (!originator.get_parent_originator().is_unknown())) {
    CCI_REPORT_ERROR("cci_get_global_broker","You may not request the global broker from within the SystemC heirarchy");
  }
  return cci_broker_manager::get_broker().create_broker_handle(originator);
}

CCI_CLOSE_NAMESPACE_
