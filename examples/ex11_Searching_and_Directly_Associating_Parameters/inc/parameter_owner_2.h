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
 * @file      parameter_owner_2.h
 * @brief     This sc_module is the owner class of a cci-based parameter
 * @author    P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date      2nd June, 2011 (Thursday)
 */
#ifndef PARAMETER_OWNER_2_H
#define PARAMETER_OWNER_2_H

/// Include the 'cci.h' header in all cci-based applications
#include <cci.h>

/**
 * @brief      This sc_module declares a cci-parameter.
 * @author     P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date       2nd June, 2011 (Thursday)
 */
SC_MODULE(parameter_owner_2)
{
	public:
		
		/// Default Constructor		
		SC_CTOR(parameter_owner_2)
		: clock_speed_Hz("clock_speed_Hz", 1500)
		//, clock_speed_KHz("clock_speed_KHz", 2500)
		//, clock_speed_MHz("clock_speed_MHz", 3500)
		{
			std::cout << "\n\t[OWNER_2 C_TOR] : Parameter Name   : " << clock_speed_Hz.get_name() << "\tParameter Value : " << clock_speed_Hz.get() << std::endl; 
			//std::cout << "\t[OWNER_2 C_TOR] : Parameter Name   : " << clock_speed_KHz.get_name() << "\tParameter Value : " << clock_speed_KHz.get() << std::endl; 
			//std::cout << "\t[OWNER_2 C_TOR] : Parameter Name   : " << clock_speed_MHz.get_name() << "\tParameter Value : " << clock_speed_MHz.get() << std::endl; 
			
			/// SC_THREAD declaration
			SC_THREAD(run_owner);
	
		}// End of constructor

		/**
		 * @brief         This process reads the values of the cci-parameters
		 * @param         void
		 * @return        void
		 */
		void run_owner (void)
		{
			while(1)
			{
				wait(10.0, sc_core::SC_NS);

				std::cout << "\n@ " << sc_core::sc_time_stamp() << std::endl;
				std::cout << "\t[OWNER_2] : Parameter Name   : " << clock_speed_Hz.get_name() << "\tParameter Value : " << clock_speed_Hz.get() << std::endl; 
				//std::cout << "\t[OWNER_2] : Parameter Name   : " << clock_speed_KHz.get_name() << "\tParameter Value : " << clock_speed_KHz.get() << std::endl; 
				//std::cout << "\t[OWNER_2] : Parameter Name   : " << clock_speed_MHz.get_name() << "\tParameter Value : " << clock_speed_MHz.get() << std::endl; 

				wait(10.0, sc_core::SC_NS);

			}// End of WHILE	
		}// End of SC_THREAD


	private	:
	
		/// Declare an instance of mutable CCI parameter of type 'int'
		cci::cnf::cci_param<int>	clock_speed_Hz;
		//cci::cnf::cci_param<int>	clock_speed_KHz;
		//cci::cnf::cci_param<int>	clock_speed_MHz;

};// End of SC_MODULE

#endif // End of PARAMETER_OWNER_H
