/*******************************************************************************
 The following code is derived, directly or indirectly, from the SystemC
 source code Copyright (c) 1996-2010 by all Contributors.
 All Rights reserved.

 The contents of this file are subject to the restrictions and limitations
 set forth in the SystemC Open Source License Version 3.0 (the "License");
 You may not use this file except in compliance with such restrictions and
 limitations. You may obtain instructions on how to receive a copy of the
 License at http://www.systemc.org/. Software distributed by Contributors
 under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 ANY KIND, either express or implied. See the License for the specific
 language governing rights and limitations under the License.
 ******************************************************************************/

/**
 *  @file   simple_ip.h
 *  @brief  A simple IP that demonstartes the use of elaboration_time_param
 *  @author Udaya Ranga, TI
 */

#ifndef EXAMPLES_EX03_ELAB_TIME_PARAM_EX03_SIMPLE_IP_H_
#define EXAMPLES_EX03_ELAB_TIME_PARAM_EX03_SIMPLE_IP_H_

#include <cci>
#include "xreport.hpp"

/// The SimpleIP owns a CCI param
SC_MODULE(ex03_simple_ip) {
 public:
  /// Ctor
  SC_CTOR(ex03_simple_ip)
  // Initialize struc_param with 0
      : struc_param("struc_param", 0) {
    SC_THREAD(execute);
    XREPORT("Ctor: Default value of " << struc_param.get_name() << " is "
            << struc_param);
  }

  /// Dtor
  ~ex03_simple_ip() {
    XREPORT_PLAIN("Dtor: Current value of " << struc_param.get_name() << " is "
                  << struc_param);
  }

  /// Before End of Elaboration
  void before_end_of_elaboration() {
    // Update struc_param value to 1 (valid)
    XREPORT("Before_End_Elab: Set value of struc_param to 1");
    struc_param = 1;
    if (0 == struc_param) {
      XREPORT_ERROR("Before_End_Elab: Value of struc_param"
                    " remains unchanged " << struc_param);
    } else if (1 == struc_param) {
      XREPORT("Before_End_Elab: Value of struc_param changed to: "
              << struc_param);
    } else {
      XREPORT_ERROR("Before_End_Elab: Invalid update to struc_param,"
                    " value changed to: " << struc_param);
      assert(0);
    }
  }

  /// Start Of Simulation->TODO
  // void start_of_simulation()    {
  //}

  /// Main execution block (no functionality)
  void execute() {
    // Wait for 10ns to allow config_ip to update int_param value FIXME
    wait(10, sc_core::SC_NS);

    // Update struc_param value to 2 (invalid)
    XREPORT("execute: Set value of struc_param to 2");
    struc_param = 2;
    if (1 == struc_param) {
      XREPORT("execute: Value of struc_param unchanged: " << struc_param);
    } else if (2 == struc_param) {
      XREPORT_WARNING("execute: Value of struc_param changed to: "
                      << struc_param);
    } else {
      XREPORT_ERROR("execute: Invalid update to struc_param,value changed to: "
                    << struc_param);
      assert(0);
    }

    /// Create an elab-time param after end_of elaboration()
    cci::cnf::cci_param<int, cci::cnf::elaboration_time_param>
      struc_param_post_eoe("struc_param_post_eoe", 0);
    XREPORT("execute: Creating a struc_param_post_eoe after"
            " end_of elaboration(), with a default value:"
            << struc_param_post_eoe);

    // Wait for 10ns
    wait(10, sc_core::SC_NS);

    // Update struc_param_post_eoe value to 1 (invalid)
    XREPORT("execute: Set value of struc_param_post_eoe to 1");
    struc_param_post_eoe = 1;
    if (0 == struc_param_post_eoe) {
      XREPORT("execute: Value of struc_param_post_eoe remains unchanged "
              << struc_param_post_eoe);
    } else if (1 == struc_param_post_eoe) {
      XREPORT_WARNING("execute: Value of struc_param_post_eoe changed to: "
                      << struc_param_post_eoe);
    } else {
      XREPORT_ERROR("execute: Invalid update to struc_param_post_eoe,"
                    " value changed to: " << struc_param_post_eoe);
      assert(0);
    }
  }

 private:
  /// CCI param
  cci::cnf::cci_param<int, cci::cnf::elaboration_time_param> struc_param;
};
// ex03_simple_ip

#endif  // EXAMPLES_EX03_ELAB_TIME_PARAM_EX03_SIMPLE_IP_H_
