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
#include "cci_cfg/cci_originator.h"
#include "cci_cfg/cci_param_typed.h"
#include "cci_cfg/cci_report_handler.h"

CCI_OPEN_NAMESPACE_

std::map<cci_originator, cci_broker_handle> cci_broker_manager::m_brokers;

cci_broker_handle
cci_broker_manager::get_broker(const cci_originator &originator)
{
    std::map<cci_originator, cci_broker_handle>::iterator it =
            m_brokers.find(originator);
    if(it != m_brokers.end()) {
        return it->second;
    } else {
        return get_parent_broker(originator).create_broker_handle(originator);
    }
}

cci_broker_handle
cci_broker_manager::get_parent_broker(const cci_originator &originator)
{
    // Get parent originator
    cci_originator parent_originator = originator.get_parent_originator();

    // Return handle to the broker found for the parent originator
    std::map<cci_originator, cci_broker_handle>::iterator it =
            m_brokers.find(parent_originator);
    if(it != m_brokers.end()) {
        return it->second;
    } else {
        if(parent_originator.is_unknown()) {
            return cci_get_global_broker().create_broker_handle(originator);
        } else {
            return get_parent_broker(parent_originator).
                    create_broker_handle(parent_originator);
        }
    }
}

cci_broker_if&
cci_broker_manager::register_broker(cci_broker_if& broker)
{
    cci_originator originator;
    std::map<cci_originator, cci_broker_handle>::iterator it =
            m_brokers.find(originator);
    if(it != m_brokers.end()) {
        CCI_REPORT_ERROR("cci_broker_manager/register_broker",
                         "A broker is already registered in the current"
                                 " hierarchy.");
        /* abort here, if error is suppressed? */
    } else {
        cci_broker_handle broker_handle =
                broker.create_broker_handle(originator);
        m_brokers.insert(std::make_pair(originator, broker_handle));
    }

    return broker;
}

CCI_CLOSE_NAMESPACE_
