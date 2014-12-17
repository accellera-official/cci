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
 * @brief     The file specifies 'number of address lines' parameter that will be
 *            used by the 'processor' module
 * @author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * @date      4th August, 2011 (Thursday)
 */

#ifndef EXAMPLES_EX15_INTER_PARAMETER_VALUE_CONSTRAINTS_EX15_ADDRESS_LINES_REGISTER_H_
#define EXAMPLES_EX15_INTER_PARAMETER_VALUE_CONSTRAINTS_EX15_ADDRESS_LINES_REGISTER_H_

#include <cci>

#include "xreport.hpp"

/// This module initializes a cci_parameter for maintaining the number
/// of address lines
SC_MODULE(ex15_address_lines_register) {
 public:
  SC_CTOR(ex15_address_lines_register)
      : no_of_addr_lines("curr_addr_lines", 9) {
    XREPORT("[ADDR_LINES_REG C_TOR] : Default Address Lines : "
            << no_of_addr_lines.get());
  }

 private:
  /// Declare a cci-parameter for specifying the number of address lines
  /// (defines the max' addressable range)
  cci::cnf::cci_param<int> no_of_addr_lines;
};
/// ex15_address_lines_register

#endif  // EXAMPLES_EX15_INTER_PARAMETER_VALUE_CONSTRAINTS_EX15_ADDRESS_LINES_REGISTER_H_
