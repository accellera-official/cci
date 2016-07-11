/*****************************************************************************
  Copyright 2016 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2011 GreenSocs
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

#ifndef CCI_CCI_BROKER_STACK_H_INCLUDED_
#define CCI_CCI_BROKER_STACK_H_INCLUDED_

#include <vector>

CCI_OPEN_NAMESPACE_

class cci_broker_if;

/// Broker stack, for internal use only
/**
 * Used by cci_broker_manager.
 * Stack for system instantiation, similar to sc_module_stack
 * Note there is a broker registry as well. @see cci::cci_broker_registry
 */
class cci_broker_stack
: public std::vector<cci_broker_if*> {

protected:
  typedef std::vector<cci_broker_if*> base;

  /// Friend class, this shall only be used by the broker manager
  friend class cci_broker_manager;

private:
  /// Private copy constructor to prevent stack from being copied
  cci_broker_stack(const cci_broker_stack&);
  //private assignment operator for preventing stack from copying
  cci_broker_stack& operator=(const cci_broker_stack &originator );


public:

  cci_broker_if* top();
  const cci_broker_if* top() const;

  cci_broker_if* second_top();
  const cci_broker_if* second_top() const;

protected:

  /// Push a broker, protected to be called only by broker manager
  void push(cci_broker_if* broker);
  /// Pop the top broker, protected to be called only by broker manager
  void pop();

  explicit cci_broker_stack() {}

  ~cci_broker_stack() {}

  /// Internal function; returns the broker stack used during construction
  /**
   * Contains a static variable for the broker stack.
   * Used instead of a global variable to ease dynamic linking.
   * @return Reference to the broker stack
   */
  static cci_broker_stack& stack() {
    static cci_broker_stack stack;
    return stack;
  }

};


CCI_CLOSE_NAMESPACE_

#endif
