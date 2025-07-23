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
 *  @file      parameter_owner.h
 *  @brief     This file defines the sc_module which is the owner class of a
 *             cci- parameter
 *  @author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date      10th May, 2011 (Tuesday)
 */

#ifndef EXAMPLES_EX13_PARAMETER_VALUE_LOCKING_EX13_PARAMETER_OWNER_H_
#define EXAMPLES_EX13_PARAMETER_VALUE_LOCKING_EX13_PARAMETER_OWNER_H_

#include <cci_configuration>
#include "xreport.hpp"

/**
 *  @class  ex13_parameter_owner
 *  @brief  This module declares an integer parameter and also illustrates usage
 *          of CCI infrastructure in locking/unlocking of the parameter
 */
SC_MODULE(ex13_parameter_owner) {
 public:
  /**
   *  @fn     ex13_parameter_owner
   *  @brief  The class constructor
   */
  SC_CTOR(ex13_parameter_owner)
     : int_param("mutable_int_param", 0) {
    XREPORT("[OWNER C_TOR] : Parameter locking status : "
            << int_param.is_locked() << "\tValue : " << int_param.get_value());

    // SC_THREAD declaration
    SC_THREAD(run_owner);
  }

  /**
   *  @fn     void run_owner(void)
   *  @brief  This process demonstrates various locking/unlocking APIs
   *  @return void
   */
  void run_owner(void) {
    while (1) {
      XREPORT("@ " << sc_core::sc_time_stamp());

      XREPORT("[OWNER] : Locking the parameter without password");
      int_param.lock();
      XREPORT("[OWNER] : Parameter locking status : "
              << int_param.is_locked() << "\tValue : " << int_param.get_value());

      // Illustrating usage of try and catch mechanism in order to catch the
      // exception raised when trying to set a value to the parameter if it
      // is already locked
      try {
        XREPORT("[OWNER] : Try to set parameter value to 1 from 0");
        int_param = 1;
      } catch (const sc_core::sc_report& e) {
        XREPORT("[OWNER] : " << name()
                << "\tFailed to set parameter value to 1 from 0.");
        XREPORT("[OWNER] : Caught Exception : " << e.what());
      }

      XREPORT("[OWNER] : Parameter locking status : "
              << int_param.is_locked() << "\tValue : " << int_param.get_value());

      wait(5.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[OWNER] : Unlocking parameter (locked earlier) without"
              " password");
      int_param.unlock();
      XREPORT("[OWNER] : Parameter locking status : "
              << int_param.is_locked() << "\tValue : " << int_param.get_value());

      wait(5.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[OWNER] : Parameter locking status : "
              << int_param.is_locked() << "\tValue : " << int_param.get_value());
      key = NULL;
      XREPORT("[OWNER] : Directly lock the unlocked parameter with password");
      int_param.lock(&key);

      XREPORT("[OWNER] : Parameter locking status : "
              << int_param.is_locked() << "\tValue : " << int_param.get_value());

      // This 'set' API llustrating assigning new value to an already locked
      // parameter without unlocking it
      XREPORT("[OWNER] : Overwrite value of the (locked!) parameter to 4"
              " without unlocking");
      int_param.set_value(4, &key);
      XREPORT("[OWNER] : Parameter locking status : "
              << int_param.is_locked() << "\tValue : " << int_param.get_value());

      wait(30.0, sc_core::SC_NS);
    }
  }

 private:
  cci::cci_param<int> int_param; ///< an int CCI parameter
  void* key;  ///< Arbitrary password to lock the parameter with
};
// ex13_parameter_owner

#endif  // EXAMPLES_EX13_PARAMETER_VALUE_LOCKING_EX13_PARAMETER_OWNER_H_
