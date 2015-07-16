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

/**
 *  @file      parameter_owner.h
 *  @brief     The file specifies 'number of address lines' parameter that will be
 *             used by the 'processor' module
 *  @author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date      4th August, 2011 (Thursday)
 */

#ifndef EXAMPLES_EX15_INTER_PARAMETER_VALUE_CONSTRAINTS_EX15_ADDRESS_LINES_REGISTER_H_
#define EXAMPLES_EX15_INTER_PARAMETER_VALUE_CONSTRAINTS_EX15_ADDRESS_LINES_REGISTER_H_

#include <cci_configuration>

#include "xreport.hpp"

/**
 *  @class  ex15_address_lines_register
 *  @brief  This module initializes a cci_parameter for maintaining the number
 *          of address lines
 */
SC_MODULE(ex15_address_lines_register) {
 public:
  /**
   *  @fn     ex15_address_lines_register
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex15_address_lines_register)
      : no_of_addr_lines("curr_addr_lines", 9) {
    XREPORT("[ADDR_LINES_REG C_TOR] : Default Address Lines : "
            << no_of_addr_lines.get());
  }

 private:
  cci::cci_param<int> no_of_addr_lines;  ///< The number of address lines.
};
// ex15_address_lines_register

#endif  // EXAMPLES_EX15_INTER_PARAMETER_VALUE_CONSTRAINTS_EX15_ADDRESS_LINES_REGISTER_H_
