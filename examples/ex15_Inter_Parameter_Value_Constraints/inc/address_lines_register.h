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
#ifndef ADDRESS_LINES_REGISTER_H
#define ADDRESS_LINES_REGISTER_H

/// Include the 'cci.h' header in all cci-based applications
#include <cci.h>

/**
 * @brief      The Address  Lines Register initializes a cci-parameter
 *             for maintaining the number of address lines
 */
SC_MODULE(address_lines_register)
{
	public:

		SC_CTOR(address_lines_register)
		: no_of_addr_lines("curr_addr_lines", 9)
		{
			std::cout << "\n\t[ADDR_LINES_REG C_TOR] : Default Address Lines : " << no_of_addr_lines.get() << std::endl;

		}// End of Constructor
		
	private	:
	
		/// Declare a cci-parameter for specifying the number of address lines (defines the max' addressable range) 
		cci::cnf::cci_param<int>            no_of_addr_lines;

};// End of SC_MODULE

#endif // End of ADDRESS_LINES_REGISTER_H
