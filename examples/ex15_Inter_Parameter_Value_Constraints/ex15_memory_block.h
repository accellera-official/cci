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
 * @file      memory_block.h
 * @brief     The file specifies a cci-parameter to list the size of a
 * 						memory block that will later be used by the processor
 * @author    P V S Phaneendra, CircuitSutra Technologies    <pvs@circuitsutra.com>
 * @date      4th August, 2011 (Thursday)
 */

#ifndef EXAMPLES_EX15_INTER_PARAMETER_VALUE_CONSTRAINTS_EX15_MEMORY_BLOCK_H_
#define EXAMPLES_EX15_INTER_PARAMETER_VALUE_CONSTRAINTS_EX15_MEMORY_BLOCK_H_

#include <cci>

#include "xreport.hpp"

/// This module initializes a cci_parameter for defining the memory size
SC_MODULE(ex15_memory_block) {
 public:
  SC_CTOR(ex15_memory_block)
      : mem_size("mem_size", 500) {
    XREPORT("[MEMORY_BLOCK C_TOR] : Default Memory Size : "
            << mem_size.get() << "\n");
  }

 private:
  /** Declare a cci-parameter to set memory size*/
  cci::cnf::cci_param<int> mem_size;
};
/// ex15_memory_block

#endif  // EXAMPLES_EX15_INTER_PARAMETER_VALUE_CONSTRAINTS_EX15_MEMORY_BLOCK_H_
