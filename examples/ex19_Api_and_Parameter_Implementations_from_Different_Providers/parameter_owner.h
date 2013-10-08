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
 * @brief     This sc_module is the owner class of a cci-based parameter
 * @author    Girish Verma, CircuitSutra Technologies     <girish@circuitsutra.com>
 *            P V S Phaneendra, CircuitSutra Technologies <pvs@circuitsutra.com>
 * @date      14th December, 2011 (Wednesday);
 *            (Last Modifed On) 16th May, 2011 (Monday)
 */
#ifndef PARAMETER_OWNER_H
#define PARAMETER_OWNER_H

/// Include the 'cci' header in all cci-based applications
#include <cci>

#include "parameter_impl_by_abc.h"


class parameter_configurator;  //Forward declaration of the configurator class


/**
 * @brief      This sc_module declares a cci-parameter.  Also, it declares and makes
 *             the configurator class a friend class of this sc_module (class)
 */
SC_MODULE(parameter_owner)
{
	public:
		
		friend class parameter_configurator;   /*!Making configurator a friend class to the owner*/

		/// Default Constructor		
		SC_CTOR(parameter_owner)
		:	int_param("mutable_int_param", 5)
		,	udtype_param("mutable_udtype_param", user_data_type(0x100,0x200,1))
		{
			std::cout << "[OWNER C_TOR] : Default Value : " << int_param.get() << std::endl;
			std::cout << "[OWNER C_TOR] : Default Value udtype_param : " << udtype_param.get() << std::endl;
			
			/// SC_THREAD declaration
			SC_THREAD(run_owner);
	
		}// End of constructor


		/**
		 * @brief         This process sets and reads value of the cci-parameter
		 */
		void run_owner (void)
		{
			while(1)
			{
				std::cout << "\n@ " << sc_core::sc_time_stamp() << std::endl;
				std::cout << "\t[OWNER] : Parameter Value : " << int_param.get() << std::endl; 

				wait(5.0, sc_core::SC_NS);

				std::cout << "\n@ " << sc_core::sc_time_stamp() << std::endl;
				std::cout << "\t[OWNER] : Set parameter value to 15." << std::endl; 
				int_param = 15;

				wait(15.0, sc_core::SC_NS);

				std::cout << "\n@ " << sc_core::sc_time_stamp() << std::endl;
				std::cout << "\t[OWNER] : Parameter Value   : " << int_param.get()	<< std::endl; 

				wait(50.0, sc_core::SC_NS);

			}// End of WHILE	
		}// End of SC_THREAD


	private	:
	
		/// Declare an instance of mutable CCI parameter of type 'int'
		cci::cnf::cci_param<int>	int_param;	
		cci::cnf::cci_param<user_data_type>	udtype_param;	
	
};// End of SC_MODULE

#endif // End of PARAMETER_OWNER_H
