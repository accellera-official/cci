/*******************************************************************************
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

/**
 * @file    parameter_owner.h
 * @brief   This header instantiates various CCI parameters with default values
 * @author  Girish Verma, CircuitSutra Technologies   <girish@circuitsutra.com>
 *          P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * @date    12th September, 2011 (Monday)
 */

#ifndef EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_PARAMETER_OWNER_H_
#define EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_PARAMETER_OWNER_H_

#include <cci>
#include <string>

#include "ex16_user_datatype.h"
#include "xreport.hpp"

/// This module instantiates an object of user-defined data type of
/// cci_parameter
SC_MODULE(ex16_parameter_owner) {
 public:
  SC_CTOR(ex16_parameter_owner)
        /// Assign name and value to the cci 'user-defined data type' parameter
      : udt_param("User_data_type_param", route_table_ut(0x200, 0x300, 1)) {
    // Registering SC_THREAD process
    SC_THREAD(run_thread);

    XREPORT("Prior to " << sc_time_stamp()
            << "\tdemonstrating 'get_default_value()'");
    XREPORT("[OWNER -> Retrieve] : Parameter name  : " << udt_param.get_name());

    /// Query default value of a parameter using 'get_default_type()' API
    XREPORT("[OWNER -> Retrieve] : Using 'get_default_value()' : "
            << udt_param.get_default_value());
    XREPORT("[OWNER -> Retrieve] : Parameter Value' : " << udt_param.get());

    /// Set documentation for the user-defined data type
    XREPORT("[OWNER -> Set] : Param doc - 'This is user-defined data type");
    const std::string init_doc = "This is user-defined data type";
    udt_param.set_documentation(init_doc);
  }

  /// SC_THREAD process definition
  void run_thread(void) {
    while (1) {
      XREPORT("@ " << sc_time_stamp());
      XREPORT("[OWNER -> Retrieve] : Documentation : "
              << udt_param.get_documentation());

      // Override the default value
      XREPORT("[OWNER -> Set] : New Value " << udt_param.get_name()
              << "\t's_address:768,d_address:1024,index:2'");
      udt_param.json_deserialize("{\"s_address\":768,\"d_address\":1024,"
                                 "\"index\":2}");

      wait(2.0, SC_NS);
      XREPORT("@ " << sc_time_stamp());

      // Access parameter's value
      XREPORT("[OWNER -> Retrieve] : UDT Value : " << udt_param.get());

      wait(8.0, SC_NS);

      XREPORT("@ " << sc_time_stamp()
              << " demonstrating 'set_value_invalid()'");
      XREPORT("[OWNER -> Set] : " << udt_param.get_name() << " value invalid.");

      /// Set the cci parameter to invalid state using 'set_invalid_state()' API
      udt_param.set_invalid_value();

      /// Query a cci parameter value validity using 'is_invalid_value()' API
      if (udt_param.is_invalid_value()) {
        XREPORT("OWNER] : " << udt_param.get_name()
                << " value is invalid : " << udt_param.get());
        XREPORT("[OWNER] : Is Invalid Value ? " << "\tReturned status : "
                << std::boolalpha << udt_param.is_invalid_value());
      } else {
        XREPORT("[OWNER] : " << udt_param.get_name()
                << " value is not invalid.");
        XREPORT("[OWNER] : Is Invalid Value ? " << "\tReturned status : "
                << std::boolalpha << udt_param.is_invalid_value());
      }

      wait(20.0, SC_NS);
    }
  }

 private:
  /// CCI parameters declarations
  cci::cnf::cci_param<route_table_ut, cci::cnf::mutable_param> udt_param;
};
/// ex16_parameter_owner

#endif  // EXAMPLES_EX16_USER_DEFINED_DATA_TYPE_EX16_PARAMETER_OWNER_H_
