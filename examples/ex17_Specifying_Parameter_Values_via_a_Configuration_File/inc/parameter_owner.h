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
 * @brief     This sc_module is the owner class of cci-parameters
 * @author    P V S Phaneendra, CircuitSutra Technologies    <pvs@circuitsutra.com>
 *            Girish Verman, CircuitSutra Technologies   <girish@circuitsutra.com>
 * @date      18th May, 2011 (Monday)
 */
#ifndef PARAMETER_OWNER_H
#define PARAMETER_OWNER_H

/// Include the 'cci.h' header in all cci-based applications
#include <cci.h>

class parameter_configurator;

/**
 * @brief      This sc_module gives default values to all its cci-parameters.
 *             The parameters that are overridden with their initial values
 *             appear with their initial values rather than default values.
 */
SC_MODULE(parameter_owner)
{
	public:
		
		friend class parameter_configurator;

		/// Default Constructor		
		SC_CTOR(parameter_owner)
		:	int_param("mutable_int_param", 5)
		, float_param("mutable_float_param", 123.45)
		, string_param("mutable_string_param","default")
		{
			std::cout << "\n\t[OWNER C_TOR] : Int Parameter Value : " << int_param.get() << std::endl;
			std::cout << "\n\t[OWNER C_TOR] : Float Parameter Value : " << float_param.get() << std::endl;
			std::cout << "\n\t[OWNER C_TOR] : String Parameter Value : " << string_param.get() << std::endl;
			
			/// SC_THREAD declaration
			SC_THREAD(run_owner);
	
		}// End of constructor

		
		// SC_THREAD Implementation
		void run_owner (void)
		{
			while(1)
			{
				wait(5.0, sc_core::SC_NS);
				std::cout << "\n@ " << sc_core::sc_time_stamp() << std::endl;	
				std::cout << "\t[OWNER] : Int Parameter Value : " << int_param.get() << std::endl;
				std::cout << "\t[OWNER] : Float Parameter Value : " << float_param.get() << std::endl;
				std::cout << "\t[OWNER] : String Parameter Value : " << string_param.json_serialize() << std::endl;

				wait(50.0, sc_core::SC_NS);

			}// End of WHILE	
		}// End of SC_THREAD


	private	:
	
		/// CCI parameters
		cci::cnf::cci_param<int>	       int_param;	
		cci::cnf::cci_param<float>	     float_param;	
		cci::cnf::cci_param<std::string> string_param;
	
};// End of SC_MODULE

#endif // End of PARAMETER_OWNER_H
