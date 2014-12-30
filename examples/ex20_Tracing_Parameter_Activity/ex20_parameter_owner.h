/*********************************************************************************
 *   The following code is derived, directly or indirectly, from the SystemC
 *   source code Copyright (c) 1996-2010 by all Contributors.
 *   All Rights reserved.
 *
 *   The contents of this file are subject to the restrictions and limitations
 *   set forth in the SystemC Open Source License Version 2.2.0 (the "License");
 *   One may not use this file except in compliance with such restrictions and
 *   limitations.  One may obtain instructions on how to receive a copy of the
 *   License at http://www.systemc.org/.  Software distributed by Contributors
 *   under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 *   ANY KIND, either express or implied. See the License for the specific
 *   language governing rights and limitations under the License.
 *******************************************************************************/

#ifndef EXAMPLES_EX20_TRACING_PARAMETER_ACTIVITY_EX20_PARAMETER_OWNER_H_
#define EXAMPLES_EX20_TRACING_PARAMETER_ACTIVITY_EX20_PARAMETER_OWNER_H_

#include <cci>
#include <systemc.h>

#include "xreport.hpp"

/// This class declares and defines 'int' and 'float' type cci-parameters
SC_MODULE(ex20_parameter_owner) {
 public:
  SC_CTOR(ex20_parameter_owner) {
    XREPORT("Prior to " << sc_time_stamp());

    XREPORT("[OWNER C_TOR] : Creating new integer type cci-parameter with"
            " default value 10");
    int_param = new cci::cnf::cci_param<int>("int_param", 10);

    SC_THREAD(run_owner);
  }

/// Implementation of SC_THREAD. Sets new value to the int type cci-parameter
/// and sets a default value to the float type cci-parameter
  void run_owner(void) {
    while (1) {
      XREPORT("@ " << sc_time_stamp());
      XREPORT("[OWNER] : Setting new value to the 'int' type param to '15'");
      *int_param = 15;

      wait(5.0, SC_NS);
      XREPORT("@ " << sc_time_stamp());

      XREPORT("[OWNER] : Creating new float type cci-parameter with default"
              " value : 12.345");
      float_param = new cci::cnf::cci_param<float>("float_param", 12.345);

      wait(15.0, SC_NS);
    }
  }

 private:
  /// Integer-type cci-parameter
  cci::cnf::cci_param<int>* int_param;
  /// Float-type cci-parameter
  cci::cnf::cci_param<float>* float_param;
};
/// ex20_parameter_owner

#endif  // EXAMPLES_EX20_TRACING_PARAMETER_ACTIVITY_EX20_PARAMETER_OWNER_H_
