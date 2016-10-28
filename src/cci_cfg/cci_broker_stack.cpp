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
#include "cci_cfg/cci_broker_stack.h"

CCI_OPEN_NAMESPACE_

#ifdef CCI_CNF_VERBOSE
  static void cci_debug_show_broker_stack(cci_broker_stack* stack, const char* action) {
    std::cout << " | cci_broker_stack result (after action "<< action << "):" << std::endl;
    if (stack->empty()) std::cout << " |   <empty>" << std::endl;
    std::vector<cci_broker_if*>::reverse_iterator rit;
    for ( rit=stack->rbegin() ; rit < stack->rend(); ++rit ) {
      std::cout << " |   " << (*rit)->name();
      if ((*rit)->is_private_broker()) std::cout << " (priv)";
      std::cout << std::endl;
    }
  }
  #define CCI_SHOW_BROKER_STACK(stack, action) cci::cci_debug_show_broker_stack(stack, action);
#else
  #define CCI_SHOW_BROKER_STACK(stack, action)
#endif

cci_broker_if* cci_broker_stack::top() {
  return base::back();
}

const cci_broker_if* cci_broker_stack::top() const  {
  return base::back();
}

cci_broker_if* cci_broker_stack::second_top() {
  return base::operator[](base::size()-2);
}

const cci_broker_if* cci_broker_stack::second_top() const {
  return base::operator[](base::size()-2);
}

void cci_broker_stack::push(cci_broker_if* broker) {
  base::push_back(broker);
  CCI_SHOW_BROKER_STACK(this, "push")
}

void cci_broker_stack::pop() {
  assert(!base::empty() && "Stack is empty, pop not possible");
  base::pop_back();
  CCI_SHOW_BROKER_STACK(this, "pop")
}

cci_broker_stack::cci_broker_stack(const cci_broker_stack&) {
  assert(false && "Not allowed to copy stack!");
}

cci_broker_stack& cci_broker_stack::operator=(const cci_broker_stack &originator )
{
	 assert(false && "Not allowed to copy stack!");
	 return (*this);
}

CCI_CLOSE_NAMESPACE_

