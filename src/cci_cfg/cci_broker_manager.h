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


#ifndef CCI_CCI_BROKER_MANAGER_H_INCLUDED_
#define CCI_CCI_BROKER_MANAGER_H_INCLUDED_

#include "cci_cfg/cci_broker_if.h"

CCI_OPEN_NAMESPACE_


/// Class taking a private broker within a user module's constructor

// OLD, semantic changed!!!: identifying a module which provides a (private) broker
/**
 * To get access to a (possibly private) broker, use static 
 * cci_broker_manager::get_current_broker function.
 *
 * If an sc_module shall be owner of a private broker, it must derive from this class.
 * It is highly recommended to use this as a base class for only sc_modules 
 * (and modules derived from sc_modules but not as a base for non-modules). 
 * @todo: Check if this works as a base of a non-sc_module as well.
 */
class cci_broker_manager {

public:
  
  //
  // Functions for static usage:
  //
  
  /// Returns a handle to the broker currently on top of broker stack
  /**
   * Returns a handle to a private or the global broker.
   * Returns a handle to the global broker if no broker on the stack.
   *
   * @param originator Originator the handle shall point to
   * @return Broker (private or global) handle
   */
  // old name: "search_for_broker"
  static cci_broker_if& get_current_broker(const cci_originator& originator);

  /// Returns a handle to the broker currently on second top of broker stack
  /**
   * This can be used by a private broker to identify its parent's broker -
   * which is the broker it needs to forward the public actions to.
   *
   * Returns a handle to a private or the global broker.
   * Returns a handle to the global broker if not two brokers on the stack.
   *
   * @param originator Originator the handle shall point to
   * @return Broker (private or global) handle
   */
  static cci_broker_if& get_current_parent_broker(const cci_originator& originator);
  
private:
  //TODO
  /// Private copy constructor to prevent manager from being copied
  //cci_broker_manager(const cci_broker_manager&);
  
public:
  
  /// Constructor for ONLY temporary object; taking an optional (private) broker instance (recommended to provide a handle)
  /**
   * @todo: Memory management for the private broker given to the broker manager: Is this left for the user to be solved? The broker must not be deleted as long as parameters exist using it.
   *
   * The constructor does a push to the private broker stack, which is poped by 
   * the destructor again. That's the reason why this object is only allowed to 
   * be used as temporary object within user module constructors.
   *
   * @param broker Broker that should use forwarded to user module and placed on stack
   */
  cci_broker_manager(cci_broker_if* broker);
  
  /// Destructor
  /**
   * Pops the given private broker from private broker stack.
   * This happens when the temporary object in user module contructor is destructed, which 
   * happens after completed construction of the user module.
   */
  virtual ~cci_broker_manager();

  /// Conversion operator
  /**
   * Can be used by user module to access the broker
   */
  operator cci_broker_if&();

  /// Conversion operator
  /**
   * Can be used by user module to access the broker
   */
  operator cci_broker_if*();
  
// Private to prevent from modification
private:

  /// Private broker handle pointer this manager returns (own or upwards one)
  /**
   * This broker is always valid. 
   * This is either a handle to the own private broker
   * or a handle to another (private or not private) broker from upwards the hierarchy.
   */
  cci_broker_if* m_broker;

};


CCI_CLOSE_NAMESPACE_

#endif
