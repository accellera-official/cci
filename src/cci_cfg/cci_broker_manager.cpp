/*****************************************************************************
  Copyright 2016 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2010-2011 GreenSocs
  All rights reserved.
  Authors:
      Christian Schroeder <schroeder@eis.cs.tu-bs.de>
      Mark Burton <mark@greensocs.com>

  Copyright 2016 Ericsson
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


#include "cci_cfg/cci_debug.h"
#include "cci_cfg/cci_datatypes.h"
#include "cci_cfg/cci_originator.h"
#include "cci_cfg/cci_param.h"
#include "cci_cfg/cci_broker_manager.h"
#include "cci_cfg/cci_broker_stack.h"
#include "cci_cfg/cci_report_handler.h"

CCI_OPEN_NAMESPACE_


// Implementation cci_broker_manager

// static get function
cci_broker_if& cci_broker_manager::get_current_broker(const cci_originator& originator) {
  if (std::string(originator.name()).empty()) {
#ifdef CCI_CNF_VERBOSE
    SC_REPORT_INFO("CCI/get_current_broker", "It is recommended not to get a broker without originator information (NULL pointer or empty string)!");
#endif
  }
  if (cci_broker_stack::stack().size() > 0) {
    return cci_broker_stack::stack().top()->get_accessor(originator);
  }
  // else return global broker
  else {
    return cci::create_global_cnf_broker().get_accessor(originator);
  }
}

// static get function
cci_broker_if& cci_broker_manager::get_current_parent_broker(const cci_originator& originator) {
  if (std::string(originator.name()).empty()) {
#ifdef CCI_CNF_VERBOSE
    SC_REPORT_INFO("CCI/get_current_broker", "It is recommended not to get a broker without originator information (NULL pointer or empty string)!");
#endif
  }
  if (cci_broker_stack::stack().size() > 1) {
    return cci_broker_stack::stack().second_top()->get_accessor(originator);
  }
  // else return global broker
  else {
    return cci::create_global_cnf_broker().get_accessor(originator);
  }
}

/*cci_broker_manager::cci_broker_manager(const cci_broker_manager&) {
  assert(false && "Not allowed to copy manager!");
}*/

cci_broker_manager::cci_broker_manager(cci_broker_if* broker)
{
  // Set m_broker either to own private broker or the one responsible upwards the hierarchy
  if (broker) {
    CCI_CNF_DUMP("Broker manager with private broker (\""<< broker->name() <<"\")");
    // push to broker stack
    cci_broker_stack::stack().push(broker);
    if (broker->get_originator()) {
      m_broker = &broker->get_accessor(*broker->get_originator());
    } else {
      std::cout << "The private broker (\""<<broker->name()<<"\") given to this broker manager IS NOT an accessor." << std::endl;
      SC_REPORT_INFO("CCI/cci_broker_manager", "It is recommended to provide a broker accessor to the broker manager!");
      m_broker = &broker->get_accessor(cci_originator("unknown broker manager")); // TODO: what string is reasonable here?
    }
  } else {
	m_broker = NULL;
    assert(false && "no private broker error");
    // cci_report_handler::TODO("no private broker!");
  }
}

cci_broker_manager::~cci_broker_manager() {
  // pop private broker from broker stack
  cci_broker_stack::stack().pop();
}

cci_broker_manager::operator cci::cci_broker_if&() {
  return *m_broker;
}

cci_broker_manager::operator cci::cci_broker_if*() {
  return m_broker;
}

CCI_CLOSE_NAMESPACE_
