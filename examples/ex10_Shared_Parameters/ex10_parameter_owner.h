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
 *  @brief     This file defines the PARAMETER_OWNER modules
 *  @author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date      16th May, 2011 (Monday)
 */

#ifndef EXAMPLES_EX10_SHARED_PARAMETERS_EX10_PARAMETER_OWNER_H_
#define EXAMPLES_EX10_SHARED_PARAMETERS_EX10_PARAMETER_OWNER_H_

#include <cci_configuration>
#include "xreport.hpp"

class parameter_configurator;

/**
 *  @class  ex10_parameter_owner
 *  @brief  This module decalrse a CCI parameter. It also makes the configurator class a friend
 */
SC_MODULE(ex10_parameter_owner) {
 public:
  friend class ex10_parameter_configurator;

  /**
   *  @fn     ex10_parameter_owner
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex10_parameter_owner)
      : int_param("mutable_int_param", 5) {
    XREPORT("[OWNER C_TOR] : Default Value : " << int_param.get_value());

    // SC_THREAD declaration
    SC_THREAD(run_owner);
  }

  /**
   *  @fn     void run_owner(void)
   *  @brief  The main thread that prints information regarding the parameter
   *  @reutrn void
   */
  void run_owner(void) {
    while (1) {
      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[OWNER] : Parameter Value : " << int_param.get_value());

      wait(5.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[OWNER] : Set parameter value to 15.");
      int_param = 15;

      wait(15.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[OWNER] : Parameter Value   : " << int_param.get_value());

      wait(50.0, sc_core::SC_NS);
    }
  }

 private:
  cci::cci_param<int> int_param; ///< Integer type mutable cci-parameter of type
};
// ex10_parameter_owner

#endif  // EXAMPLES_EX10_SHARED_PARAMETERS_EX10_PARAMETER_OWNER_H_
