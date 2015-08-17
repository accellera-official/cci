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
 *  @file      memory_block.h
 *  @brief     The file specifies a cci-parameter to list the size of a
 * 	 					memory block that will later be used by the processor
 *  @author    P V S Phaneendra, CircuitSutra Technologies    <pvs@circuitsutra.com>
 *  @date      4th August, 2011 (Thursday)
 */

#ifndef EXAMPLES_EX15_INTER_PARAMETER_VALUE_CONSTRAINTS_EX15_MEMORY_BLOCK_H_
#define EXAMPLES_EX15_INTER_PARAMETER_VALUE_CONSTRAINTS_EX15_MEMORY_BLOCK_H_

#include <cci_configuration>

#include "xreport.hpp"

/**
 *  @class  ex15_memory_block
 *  @brief  This module initializes a cci_parameter for defining the memory size
 */
SC_MODULE(ex15_memory_block) {
 public:
  /**
   *  @fn     ex15_memory_block
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex15_memory_block)
      : mem_size("mem_size", 500) {
    XREPORT("[MEMORY_BLOCK C_TOR] : Default Memory Size : "
            << mem_size.get() << "\n");
  }

 private:
  cci::cci_param<int> mem_size;  ///< CCI parameter to set the memory size
};
// ex15_memory_block

#endif  // EXAMPLES_EX15_INTER_PARAMETER_VALUE_CONSTRAINTS_EX15_MEMORY_BLOCK_H_
