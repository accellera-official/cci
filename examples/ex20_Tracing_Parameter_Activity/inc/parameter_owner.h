/*********************************************************************************
 *   The following code is derived, directly or indirectly, from the SystemC
 *   source code Copyright (c) 1996-2010 by all Contributors.
 *   All Rights reserved.
 *
 *   The contents of this file are subject to the restrictions and limitations
 *   set forth in the SystemC Open Source License Version 2.2.0 (the "License");
 *   One may not use this file except in compliance with such restrictions and
 *   limitations.  One may obtain instructions on how to receive a copy of the
 *   License at http://www.systemc.org/.  Software distributed by Contributors
 *   under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 *   ANY KIND, either express or implied. See the License for the specific
 *   language governing rights and limitations under the License.
 *******************************************************************************/
#ifndef PARAMETER_OWNER_H
#define PARAMETER_OWNER_H

#include <cci.h>              // Include the cci header in all cci-based applications
#include "systemc.h"

/*!
 * \class   parameter_owner  parameter_owner.h
 * \brief   This class declares and defines 'int' and 'float' type cci-parameters
 * \author  P V S Phaneendra, CircuitSutra Technologies <pvs@circuitsutra.com>
 * \date    11th October, 2011
 */
class parameter_owner : public sc_module
{
	public	:

		/// Default constructor
		SC_CTOR(parameter_owner)
		{	
			std::cout << "\nPrior to " << sc_time_stamp() << std::endl;				

			std::cout << "\n\t[OWNER C_TOR] : Defining new integer type cci-parameter with default value 10" << std::endl;
			int_param	=	new cci::cnf::cci_param<int>("int_param", 10);			

			SC_THREAD(run_owner);
		}

		/// Destructor
		~parameter_owner()
		{
			// Nothing to destruct
		}


		/*!
		 * \fn     void run_owner (void)
		 * \brief  Implementation of SC_THREAD. Sets new value to the int type cci-parameter
		 *         and sets a default value to the float type cci-parameter
		 */
		void run_owner (void)
		{
			while(1)
			{
				std::cout << "\n@ " << sc_time_stamp() << std::endl;				
				*int_param = 15;
				
				wait(5.0, SC_NS);
				std::cout << "@ " << sc_time_stamp() << std::endl;

				std::cout << "\n\t[OWNER] : Defining new float type cci-parameter with no default value" << std::endl;
				float_param	=	new cci::cnf::cci_param<float>("float_param", 12.345);	

				wait(15.0, SC_NS);

			}// End of WHILE

		}// End of SC_THREAD

		
	private	:

		cci::cnf::cci_param<int>*            int_param;    //!< Integer-type cci-parameter
		cci::cnf::cci_param<float>*          float_param;  //!< Float-type cci-parameter

};// End of PARAMETER_OWNER SC_MODULE

#endif	// End of PARAMETER_OWNER_H
