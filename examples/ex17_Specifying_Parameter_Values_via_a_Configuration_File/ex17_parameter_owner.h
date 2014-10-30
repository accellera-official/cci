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
 * @brief     This sc_module is the owner class of cci-parameters
 * @author    P V S Phaneendra, CircuitSutra Technologies    <pvs@circuitsutra.com>
 *            Girish Verman, CircuitSutra Technologies   <girish@circuitsutra.com>
 * @date      18th May, 2011 (Monday)
 */

#ifndef EXAMPLES_EX17_SPECIFYING_PARAMETER_VALUES_VIA_A_CONFIGURATION_FILE_EX17_PARAMETER_OWNER_H_
#define EXAMPLES_EX17_SPECIFYING_PARAMETER_VALUES_VIA_A_CONFIGURATION_FILE_EX17_PARAMETER_OWNER_H_

#include <cci>
#include <string>

#include "xreport.hpp"

class ex17_parameter_configurator;

/// This sc_module gives default values to all its cci-parameters.
/// The parameters that are overridden with their initial values
/// appear with their initial values rather than default values.
SC_MODULE(ex17_parameter_owner) {
 public:
  friend class ex17_parameter_configurator;

  SC_CTOR(ex17_parameter_owner)
      : int_param("mutable_int_param", 5),
        float_param("mutable_float_param", 123.45),
        string_param("mutable_string_param", "default") {
    XREPORT("[OWNER C_TOR] : Int Parameter Value : " << int_param.get());
    XREPORT("[OWNER C_TOR] : Float Parameter Value : " << float_param.get());
    XREPORT("[OWNER C_TOR] : String Parameter Value : " << string_param.get());

    /// SC_THREAD declaration
    SC_THREAD(run_owner);
  }

  // SC_THREAD Implementation
  void run_owner(void) {
    while (1) {
      wait(5.0, sc_core::SC_NS);
      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[OWNER] : Int Parameter Value : " << int_param.get());
      XREPORT("[OWNER] : Float Parameter Value : " << float_param.get());
      XREPORT("[OWNER] : String Parameter Value : "
              << string_param.json_serialize());

      wait(50.0, sc_core::SC_NS);
    }
  }

 private:
  /// CCI parameters
  cci::cnf::cci_param<int> int_param;
  cci::cnf::cci_param<float> float_param;
  cci::cnf::cci_param<std::string> string_param;
};
/// ex17_parameter_owner

#endif  // EXAMPLES_EX17_SPECIFYING_PARAMETER_VALUES_VIA_A_CONFIGURATION_FILE_EX17_PARAMETER_OWNER_H_
