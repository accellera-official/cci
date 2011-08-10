/* The following code is derived, directly or indirectly, from the SystemC
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
 *  @file     parameter_owner.h
 *  @brief    This class declares few of the cci-parameters which are
 *            initialized with initial values either from the testbench (main.cpp)
 *            file or the configuration file reader & parser.
 *  @author   P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 *  @date     21st July, 2011 (Thursday)
 */
#ifndef PARAMETER_OWNER_H
#define PARAMETER_OWNER_H

#include <cci.h>  /*!This header must be included wherever cci-infrastructure is used*/

/**
 *  @brief    This SC_MODULE class instantiates two 'integer' type cci-parameters along
 *            with default values.  In this case, both the parameters have been provided with
 *            initial values from the testbench file 
 *            @see example#17 : Specifying Parameter Values via a Configuration File for further details
 *  @author   P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 *  @date     21st July, 2011 (Thursday)
 */
class parameter_owner : public sc_module
{
	public	:

		/// Default Constructor
		SC_CTOR(parameter_owner)
		:	int_param_1("int_param_1", 10)
		,	int_param_2("int_param_2", 20)
		{
			// Constructor does nothing here other than initializing parameters with default values

		}// End of Constructor

	private	:

		/// Declare the cci-parameters of type 'int' and 'std::string'
		cci::cnf::cci_param<int>            int_param_1;
		cci::cnf::cci_param<int>            int_param_2;

};// End of SC_MODULE

#endif	// End of PARAMETER_OWNER_H
