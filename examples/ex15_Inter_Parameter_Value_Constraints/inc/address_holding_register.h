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
 * @brief     The file specifies a cci-parameter to hold the current value
 *            of a memory location that will later be used by the processor
 * @author    P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date      4th August, 2011 (Thursday)
 */
#ifndef ADDRESS_HOLDING_REGISTER_H
#define ADDRESS_HOLDING_REGISTER_H

/// Include the 'cci.h' header in all cci-based applications
#include <cci.h>

/**
 * @brief      The Address Holding Register initializes a cci-parameter
 *             for holding a address value
 * @author     P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date       4th August, 2011 (Thursday)
 */
SC_MODULE(address_holding_register)
{
	public:

		SC_CTOR(address_holding_register)
		: addr_pos("curr_addr_pos", 999)
		{
			std::cout << "\n\t[ADDR_HOLDING_REG C_TOR] : Default Address Value : " << addr_pos.get() << "\n" << std::endl;

		}// End of Constructor
		
	private	:
	
		/// Declare a cci-parameter to hold an address value
		cci::cnf::cci_param<int>                     addr_pos;

};// End of SC_MODULE

#endif // End of ADDRESS_HOLDING_REGISTER_H
