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
 *  @file     parameter_configurator.h
 *  @brief    This file declares and defines configurator and implements the
 *            locking and unlocking mechnanisms on the owner parameter
 *  @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date     10th May, 2011 (Tuesday)
 */

#ifndef EXAMPLES_EX13_PARAMETER_VALUE_LOCKING_EX13_PARAMETER_CONFIGURATOR_H_
#define EXAMPLES_EX13_PARAMETER_VALUE_LOCKING_EX13_PARAMETER_CONFIGURATOR_H_

#include <cci_configuration>
#include "xreport.hpp"

/**
 *  @class  ex13_parameter_configurator
 *  @brief  This module lists and defines the locking/unlocking mechanism by a
 *          parameter-setter (configurator)
 */
SC_MODULE(ex13_parameter_configurator) {
 public:
  /**
   *  @fn     ex13_parameter_configurator
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex13_parameter_configurator):
      m_broker(cci::cci_get_broker())
  {
      // Getting handle for the integer parameter of onwer module
      // by the configurator
      int_param_handle = m_broker.get_param_handle("param_owner.mutable_int_param"); 
      if (!int_param_handle.is_valid()) {
      XREPORT("[CFGR C_TOR] : int_param not found");
    }

    // Registering SC_THREAD with the SystemC kernel
    SC_THREAD(run_mutable_cfgr);
}

  /**
   *  @fn     void run_mutable_cfgr(void)
   *  @brief  This function illustrates various locking and unlocking mechanisms
   *          of a parameter using 'lock()', 'lock(&password)', 'unlock()',
   *          'unlock(&password)' APIs. 'is_locked() is used to query the locking status
   *  @return void
   */
  void run_mutable_cfgr(void) {
    while (1) {
      wait(2.0, sc_core::SC_NS);

      // Before configurator tries to assign a new value to the parameter, it
      // is always recommended to check the locking status of the parameter
      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[CFGR] : Parameter locking status : "
              << int_param_handle.is_locked() << "\tValue : "
              << int_param_handle.get_cci_value().to_json());

      // 'locked' API returns TRUE when a parameter is in LOCKED STATE and
      // FALSE when in UNLOCKED STATE
      if (int_param_handle.is_locked()) {
        XREPORT("[CFGR] :  Cannot assign new value to the parameter as it"
                " is already locked!!");
      } else {
        int_param_handle.set_cci_value(cci::cci_value(2));
      }

      XREPORT("[CFGR] : Parameter locking status : "
              << int_param_handle.is_locked() << "\tValue : "
              << int_param_handle.get_cci_value().to_json());

      wait(5.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[CFGR] : Parameter locking status : "
              << int_param_handle.is_locked() << "\tValue : "
              << int_param_handle.get_cci_value().to_json());
      XREPORT("[CFGR] : Parameter is not locked!! Assign a new value '3'"
              " to it");
      int_param_handle.set_cci_value(cci::cci_value(3));
      XREPORT("[CFGR] : Parameter locking status : "
              << int_param_handle.is_locked() << "\tValue : "
              << int_param_handle.get_cci_value().to_json());

      // Demonstrating 'lock' API to lock a parameter without a password
      XREPORT("[CFGR] : Lock parameter without password");
      int_param_handle.lock();
      XREPORT("[CFGR] : Parameter locking status : "
              << int_param_handle.is_locked() << "\tValue : "
              << int_param_handle.get_cci_value().to_json());

      // Demonstrating 'unlock' API to lock a parameter without a password
      XREPORT("[CFGR] : Unlock parameter without password");
      int_param_handle.unlock();
      XREPORT("[CFGR] : Parameter locking status : "
              << int_param_handle.is_locked() << "\tValue : "
              << int_param_handle.get_cci_value().to_json());

      // Demonstrating 'lock' API to lock a parameter with a password
      // 'Lock' API returns TRUE if parameter is locked successfully and
      // FALSE if it could not be locked
      psswd = NULL;
      XREPORT("[CFGR] : Lock with password");

      if (int_param_handle.lock(&psswd)) {
        XREPORT("[CFGR] : Parameter locked with password (password not known"
                " to OWNER)");
      } else {
        XREPORT("[CFGR] : Could not lock the parameter.");
      }

      XREPORT("[CFGR] : Parameter locking status : "
              << int_param_handle.is_locked() << "\tValue : "
              << int_param_handle.get_cci_value().to_json());

      try {
        XREPORT("[CFGR] : Set parameter value to '4'");
        int_param_handle.set_cci_value(cci::cci_value(4));
      } catch (const sc_core::sc_report& exception) {
        XREPORT("[CFGR] : Caught Exception : " << exception.what());
      }

      XREPORT("[CFGR] : Parameter locking status : "
              << int_param_handle.is_locked() << "\tValue : "
              << int_param_handle.get_cci_value().to_json());

      // 'Unlock' API returns TRUE if parameter is unlocked successfully and
      // FALSE if it could not be unlocked. Demonstrating unlocking with the
      // right password
      XREPORT("[CFGR] : Now, unlock with the correct password");
      int_param_handle.unlock(&psswd);
      XREPORT("[CFGR] : Parameter locking status : "
              << int_param_handle.is_locked() << "\tValue : "
              << int_param_handle.get_cci_value().to_json());

      wait(50.0, sc_core::SC_NS);
    }
  }

 private:
  cci::cci_broker_handle m_broker;  ///< CCI configuration broker handle
  cci::cci_param_handle int_param_handle;  ///< CCI parameter handle to access the actual owner's parameter

  char* paramName;  ///< The parameter name

  // Some passwords local to the configurator
  void* psswd;  ///< local password
  void* newpsswd; ///< local password
};
// ex13_parameter_configurator

#endif  // EXAMPLES_EX13_PARAMETER_VALUE_LOCKING_EX13_PARAMETER_CONFIGURATOR_H_
