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
#ifndef MEMORY_BLOCK_H
#define MEMORY_BLOCK_H

/// Include the 'cci.h' header in all cci-based applications
#include <cci.h>

/**
 * @brief      The 'Memory Stack' initializes a cci-parameter for defining the memory size
 */
SC_MODULE(memory_block)
{
	public:

		SC_CTOR(memory_block)
		: mem_size("mem_size", 500)
		{
			std::cout << "\n\t[MEMORY_BLOCK C_TOR] : Default Memory Size : " << mem_size.get() << "\n" << std::endl;

		}// End of Constructor
		
	private	:
	
		/// Declare a cci-parameter to hold an address value
		cci::cnf::cci_param<int>                     mem_size;

};// End of SC_MODULE

#endif // End of MEMORY_BLOCK_H
