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

#ifndef CCI_CCI_BROKER_MANAGER_H_INCLUDED_
#define CCI_CCI_BROKER_MANAGER_H_INCLUDED_

#include "cci_cfg/cci_broker_handle.h"

#include <map>

CCI_OPEN_NAMESPACE_

class cci_broker_if;

/// Class managing broker hierarchy

/**
 * To get access to a broker, use static cci_broker_manager::get_broker
 * function.
 */
class cci_broker_manager
{
public:
    /// Returns a handle to the broker currently on top of broker
    /**
     * Returns a handle to a private or the global broker.
     * Returns a handle to the global broker if no registered broker.
     *
     * @param originator Originator the handle shall point to
     * @return Broker (private or global) handle
     */
    static cci_broker_handle get_broker(
            const cci_originator &originator = cci_originator());

    /// Register a broker handle in the broker hierarchy
    /**
     * This can be used to register a private broker handle in the current
     * hierarchy.
     *
     * In case a broker is already registered at the current hierarchy, an
     * error will be generated.
     *
     * @param broker Broker handle to register
     */
    static cci_broker_if& register_broker(cci_broker_if& broker);

protected:
    /// Returns a handle to the parent broker
    /**
     * This can be used by a private broker to identify its parent's broker -
     * which is the broker it needs to forward the public actions to.
     *
     * Returns a handle to a private or the global broker.
     * Returns a handle to the global broker if no parent broker.
     *
     * @param originator Originator the handle shall point to
     * @return Broker (private or global) handle
     */
    static cci_broker_handle get_parent_broker(
            const cci_originator &originator = cci_originator());

private:
    /// Public broker hierarchy
    static std::map<cci_originator, cci_broker_handle> m_brokers;
};


CCI_CLOSE_NAMESPACE_

#endif
