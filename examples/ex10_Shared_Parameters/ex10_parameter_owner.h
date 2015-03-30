/*****************************************************************************
  Copyright 2006-2014 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2010-2015 CircuitSutra Technologies Pvt. Ltd.
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

/*!
 * \file      parameter_owner.h
 * \brief     This file defines the PARAMETER_OWNER modules
 * \author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * \date      16th May, 2011 (Monday)
 */

#ifndef EXAMPLES_EX10_SHARED_PARAMETERS_EX10_PARAMETER_OWNER_H_
#define EXAMPLES_EX10_SHARED_PARAMETERS_EX10_PARAMETER_OWNER_H_

#include <cci>
#include "xreport.hpp"

class parameter_configurator;

/// This module declares a CCI parameter. It also declares and makes the
/// configurator class a friend.
SC_MODULE(ex10_parameter_owner) {
 public:
  friend class ex10_parameter_configurator;

  SC_CTOR(ex10_parameter_owner)
      : int_param("mutable_int_param", 5) {
    XREPORT("[OWNER C_TOR] : Default Value : " << int_param.get());

    /// SC_THREAD declaration
    SC_THREAD(run_owner);
  }

  void run_owner(void) {
    while (1) {
      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[OWNER] : Parameter Value : " << int_param.get());

      wait(5.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[OWNER] : Set parameter value to 15.");
      int_param = 15;

      wait(15.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[OWNER] : Parameter Value   : " << int_param.get());

      wait(50.0, sc_core::SC_NS);
    }
  }

 private:
  // Integer type mutable cci-parameter of type
  cci::cnf::cci_param<int> int_param;
};
// ex10_parameter_owner

#endif  // EXAMPLES_EX10_SHARED_PARAMETERS_EX10_PARAMETER_OWNER_H_
