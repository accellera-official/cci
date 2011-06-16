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
 * @brief     This file contains the code which defines the OWNER module and its contents.
 *            The owner module is instantiated by the TOP_MODULE
 * @author    P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date      9th June, 2011 (Thursday)
 */
#ifndef PARAMETER_OWNER_H
#define PARAMETER_OWNER_H

/// Include the 'cci.h' header in all cci-based applications
#include <cci.h>
#include <string.h>

/**
 * @brief      This sc_module declares the cci_parameters and they are created
 *             with the names passed from the TOP_MODULE
 * @author     P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date       9th June, 2011 (Thursday)
 */
SC_MODULE(parameter_owner)
{
	public:
		
		/// Default Constructor		
		SC_HAS_PROCESS(parameter_owner);
		
		/**
 			* @brief    The constructor SC_HAS_PROCESS takes in two arguments, one among them being
 			*           name of the parameter passed from the TOP_MODULE
 			* @param    _name         Name of the SC_MODULE
 			* @param    _param_name   (std::string) Name of the cci_parameter
 			*/
		parameter_owner(sc_module_name _name, std::string  _param_name = "clock_speed_Hz")
		{
		
			clk	=	new cci::cnf::cci_param<int>(_param_name.c_str(), 1000);
	
			std::cout << "\n\t[OWNER C_TOR] : Parameter Name   : " << clk->get_name() << "\tParameter Value : " << clk->get() << std::endl; 

		}// End of constructor


	private	:
	
		/// Declare an instance of mutable CCI parameter of type 'int'
		cci::cnf::cci_param<int>*	clk;

};// End of SC_MODULE

#endif // End of PARAMETER_OWNER_H
