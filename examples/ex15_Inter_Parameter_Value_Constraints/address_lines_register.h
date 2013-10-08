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

#include <cci>  // Include the 'cci' header in all cci-based applications

/**
 * @class      address_lines_register address_lines_register.h
 * @brief      The Address  Lines Register initializes a cci-parameter
 *             for maintaining the number of address lines
 */
class address_lines_register : public sc_core::sc_module
{
	public:

		SC_HAS_PROCESS(address_lines_register);

		/**
		 * @fn    address_lines_register::address_lines_register(sc_module_name )
		 * @brief Constructor
		 * @param sc_module_name  SC_MODULE name
		 */
		address_lines_register(sc_module_name _name)
		: sc_module(_name)
		, no_of_addr_lines("curr_addr_lines", 9)
		{
			std::cout << "\n\t[ADDR_LINES_REG C_TOR] : Default Address Lines : " << no_of_addr_lines.get() << std::endl;

		}// End of Constructor
		
	
		/**
		 * @fn    address_lines_register::~address_lines_register
		 * @brief Destructor
		 */
		~address_lines_register()
		{
			// Nothing to destruct
		}


	private	:
	
		/** Declare a cci-parameter for specifying the number of address lines (defines the max' addressable range)*/
		cci::cnf::cci_param<int>            no_of_addr_lines; //!< Number of address lines

};// End of SC_MODULE

#endif // End of ADDRESS_LINES_REGISTER_H
