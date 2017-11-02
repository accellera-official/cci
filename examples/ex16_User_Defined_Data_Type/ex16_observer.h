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
 *  @file    observer.h
 *  @brief   This file defines an observer class demonstrates
 *  @author  P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *           Girish Verma, CircuitSutra Technologies   <girish@circuitsutra.com>
 *  @date    12th September, 2011 (Monday)
 */

#ifndef EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_OBSERVER_H_
#define EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_OBSERVER_H_

#include <cci_configuration>
#include <vector>

/**
 *  @class  ex16_observer
 *  @brief  This observer class registers callbacks on the cci_parameter values
 *          of interest in order to find originator for the appropriate actions
 *          on the respective cci_parameters
 */
class ex16_observer {
 public:
  /**
   *  @fn     ex16_observer
   *  @brief  The class constructor
   *  @return void
   */
  ex16_observer():
      m_originator(cci::cci_originator("ex16_observer")),
      m_broker(cci::cci_get_global_broker(m_originator)),
      obsv_udt_base_handle(m_originator)
  {
    // Check for the broker type (default or private) using
    // 'is_global_broker()' API
    if (!m_broker.is_global_broker()) {
      // Access broker's name using 'name()'
      std::cout << "\n\t[OBSERVER C_TOR] : Broker Type : "
                << m_broker.name() << endl;
    } else {
      std::cout << "\n\t[OBSERVER C_TOR] : Broker Type : "
                << m_broker.name() << " - is not a private broker."
                << endl;
    }

    // Gets the reference to the 'udt' type cci-parameter of OWNER module
    obsv_udt_base_handle =
        m_broker.get_param_handle("param_owner.User_data_type_param");

    sc_assert(obsv_udt_base_handle.is_valid()
           && "Returned Handle of 'integer type' cci-parameter is NULL");

    // Observer registering 'PRE_READ', 'PRE_WRITE' & 'POST_WRITE' callbacks
    // on the UDT parameter to monitor all actions on it
    udt_pre_read_cb = obsv_udt_base_handle.register_pre_read_callback(
        &ex16_observer::untyped_pre_read_callback,this);

    udt_post_read_cb = obsv_udt_base_handle.register_post_read_callback(
        &ex16_observer::untyped_post_read_callback,this);

    udt_pre_write_cb = obsv_udt_base_handle.register_pre_write_callback(
        &ex16_observer::untyped_pre_write_callback,this);

    udt_post_write_cb = obsv_udt_base_handle.register_post_write_callback(
        &ex16_observer::untyped_post_write_callback,this);
  }

  /**
   *  @fn     void typed_pre_read_callback(const cci::cci_param_read_event<int> & ev)
   *  @brief  Pre Callback function for reads on parameters
   *  @return void
   */
  void untyped_pre_read_callback(const cci::cci_param_read_event<> & ev)
  {
    std::cout << "\n\t[OBSERVER pre_read_cb] :  Parameter Name : "
              << ev.param_handle.get_name() << "\tOriginator info : "
              << ev.originator.name() << std::endl;
  }

  /**
   *  @fn     void typed_pre_read_callback(const cci::cci_param_read_event<int> & ev)
   *  @brief  Post Callback function for reads on parameters
   *  @return void
   */
  void untyped_post_read_callback(const cci::cci_param_read_event<> & ev)
  {
    std::cout << "\n\t[OBSERVER post_read_cb]:  Parameter Name : "
              << ev.param_handle.get_name() << "\tOriginator info : "
              << ev.originator.name() << std::endl;
  }

  /**
   *  @fn     bool typed_pre_write_callback(const cci::cci_param_write_event<int> & ev)
   *  @brief  Pre Callback function for writes on parameters
   *  @return True to indicate the write is valid
   */
  bool untyped_pre_write_callback(const cci::cci_param_write_event<> & ev)
  {
    std::cout << "\n\t[OBSERVER pre_write_cb] :  Parameter Name : "
              << ev.param_handle.get_name() << "\tOriginator info : "
              << ev.originator.name() << std::endl;
    return true;
  }

  /**
   *  @fn     void typed_post_write_callback(const cci::cci_param_write_event<int> & ev)
   *  @brief  Post Callback function for writes on parameters
   *  @return void
   */
  void untyped_post_write_callback(const cci::cci_param_write_event<> & ev)
  {
    std::cout << "\n\t[OBSERVER post_write_cb] :  Parameter Name : "
              << ev.param_handle.get_name() << "\tOriginator info : "
              << ev.originator.name() << std::endl;
  }

 private:
  cci::cci_originator m_originator;
  cci::cci_broker_handle m_broker;  ///< CCI configuration broker handle

  cci::cci_param_handle obsv_udt_base_handle;  ///< Declare CCI param handle for int type cci-parameter

  // Callback handle Objects for 'int' type parameter
  cci::cci_callback_untyped_handle udt_pre_read_cb;   ///< Callback handle object for pre-read
  cci::cci_callback_untyped_handle udt_post_read_cb;  ///< Callback handle object for pre-read
  cci::cci_callback_untyped_handle udt_pre_write_cb;  ///< Callback handle object for pre-write
  cci::cci_callback_untyped_handle udt_post_write_cb; ///< Callback handle object for post-write
};
// ex16_observer.h

#endif  // EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_OBSERVER_H_
