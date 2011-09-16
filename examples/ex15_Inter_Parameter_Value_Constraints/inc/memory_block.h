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

#include <cci.h> // Include the 'cci.h' header in all cci-based applications

/**
 * @class      memory_block  memory_block.h
 * @brief      The 'Memory Stack' initializes a cci-parameter for defining the memory size
 */
class memory_block : public sc_core::sc_module
{
	public:

		SC_HAS_PROCESS(memory_block);

		/**
		 * \fn    memory_block::memory_block(sc_module_name )
		 * \brief Constructor
		 * \param sc_module_name  SC_MODULE name
		 */
		memory_block(sc_module_name _name)
		: sc_module(_name)
		, mem_size("mem_size", 500)
		{
			std::cout << "\n\t[MEMORY_BLOCK C_TOR] : Default Memory Size : " << mem_size.get() << "\n" << std::endl;

		}// End of Constructor
		

		/**
		 * @fn    memory_block::~memory_block
		 * @brief Destructor
		 */
		~memory_block()
		{
			// Nothing to destruct
		}


	private	:
	
		/** Declare a cci-parameter to set memory size*/
		cci::cnf::cci_param<int>                     mem_size; //!< Memory Size

};// End of SC_MODULE

#endif // End of MEMORY_BLOCK_H
