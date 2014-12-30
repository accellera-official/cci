/*******************************************************************************
 * The following code is derived, directly or indirectly, from the SystemC
 * source code Copyright (c) 1996-2010 by all Contributors.
 * All Rights reserved.
 * 
 * The contents of this file are subject to the restrictions and limitations
 * set forth in the SystemC Open Source License Version 2.2.0 (the "License");
 * One may not use this file except in compliance with such restrictions and
 * limitations.  One may obtain instructions on how to receive a copy of the
 * License at http://www.systemc.org/.  Software distributed by Contributors
 * under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 * ANY KIND, either express or implied. See the License for the specific
 * language governing rights and limitations under the License.
 * *******************************************************************************/

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
