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

/**
 * @file      parameter_owner.h
 * @brief     This sc_module is the owner class of a cci-based parameter
 * @author    Girish Verma, CircuitSutra Technologies     <girish@circuitsutra.com>
 *            P V S Phaneendra, CircuitSutra Technologies <pvs@circuitsutra.com>
 * @date      14th December, 2011 (Wednesday);
 *            (Last Modifed On) 16th May, 2011 (Monday)
 */

#ifndef EXAMPLES_EX19_API_AND_PARAMETER_IMPLEMENTATIONS_FROM_DIFFERENT_PROVIDERS_EX19_PARAMETER_OWNER_H_
#define EXAMPLES_EX19_API_AND_PARAMETER_IMPLEMENTATIONS_FROM_DIFFERENT_PROVIDERS_EX19_PARAMETER_OWNER_H_

#include <cci>

#include "ex19_parameter_impl_by_abc.h"
#include "xreport.hpp"

/// Forward declaration of the configurator class
class ex19_parameter_configurator;

/// This sc_module declares a cci-parameter.  Also, it declares and makes
/// the configurator class a friend class of this sc_module (class)
SC_MODULE(ex19_parameter_owner) {
 public:
  friend class ex19_parameter_configurator;

  SC_CTOR(ex19_parameter_owner)
      : int_param("mutable_int_param", 5),
        udtype_param("mutable_udtype_param", user_data_type(0x100, 0x200, 1)) {
    XREPORT("[OWNER C_TOR] : Default Value : " << int_param.get());
    XREPORT("[OWNER C_TOR] : Default Value udtype_param : "
            << udtype_param.get());

    /// SC_THREAD declaration
    SC_THREAD(run_owner);
  }

/// This process sets and reads value of the cci-parameter
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
  /// Declare an instance of mutable CCI parameter of type 'int'
  cci::cnf::cci_param<int> int_param;
  cci::cnf::cci_param<user_data_type> udtype_param;
};
/// ex19_parameter_owner

#endif  // EXAMPLES_EX19_API_AND_PARAMETER_IMPLEMENTATIONS_FROM_DIFFERENT_PROVIDERS_EX19_PARAMETER_OWNER_H_
