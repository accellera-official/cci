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

#include "cci_cfg/cci_debug.h"
#include "cci_cfg/cci_originator.h"
#include "cci_cfg/cci_param_typed.h"
#include "cci_cfg/cci_broker_manager.h"
#include "cci_cfg/cci_report_handler.h"

CCI_OPEN_NAMESPACE_

std::map<cci_originator, cci_broker_if *> cci_broker_manager::m_brokers;

cci_broker_if&
cci_broker_manager::get_current_broker(const cci_originator &originator)
{
    std::map<cci_originator, cci_broker_if *>::iterator it = m_brokers.find(
            originator);

    if (it != m_brokers.end()) {
        return *(it->second);
    } else {
        cci_broker_if *broker_handle = &get_current_parent_broker(originator).
                create_broker_handle(originator);
        m_brokers[originator] = broker_handle;
        return *broker_handle;
    }
}

cci_broker_if&
cci_broker_manager::get_current_parent_broker(const cci_originator &originator)
{
    const sc_core::sc_object *current_object = originator.get_object();

    if (current_object != NULL) {
        const sc_core::sc_object *parent_object =
                current_object->get_parent_object();
        if (parent_object) {
            cci_originator parent_originator(*parent_object);
            std::map<cci_originator, cci_broker_if *>::iterator it =
                    m_brokers.find(parent_originator);
            if (it != m_brokers.end()) {
                return *(it->second);
            } else {
                cci_broker_if *broker_handle =
                        &get_current_parent_broker(parent_originator).
                                create_broker_handle(parent_originator);
                m_brokers[parent_originator] = broker_handle;
                return *broker_handle;
            }
        }
    }

    std::map<cci_originator, cci_broker_if *>::iterator it =
            m_brokers.find(cci_originator(""));

    if(it != m_brokers.end()) {
        return *(it->second);
    } else {
        cci_broker_if *broker_handle = &(create_global_cnf_broker().
                create_broker_handle(cci_originator("")));
        m_brokers[originator] = broker_handle;
        return *broker_handle;
    }
}

CCI_CLOSE_NAMESPACE_
