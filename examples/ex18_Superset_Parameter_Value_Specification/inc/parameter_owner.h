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
 * @brief          This module implements the owner module cci-parameters with default values
 * @author         P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date           15th June, 2011 (Wednesday)
 */
#ifndef PARAMETER_OWNER_H
#define PARAMETER_OWNER_H

/*!This header must be included in all the 'cci' header files*/
#include <cci.h>

/**
 * @brief          This module defines the owner module which instantiates the
 *                 'int type' and 'string type' cci-parameters.  The other two parameters
 *                 of 'float type' and 'double type' have not been instantiated
 * @author         P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date           15th June, 2011 (Wednesday)
 */
class parameter_owner : public sc_module
{
	public	:

		/// Default constructor
		SC_CTOR(parameter_owner)
		:	int_param("int_param", 10)
		,	string_param("string_param", "default_value")
		{
			// Constructor does nothing other than initializing parameters with default values
		}

	private	:

		cci::cnf::cci_param<int>            int_param;
		cci::cnf::cci_param<std::string>    string_param;

};// End of SC_MODULE

#endif	// End of PARAMETER_OWNER_H
