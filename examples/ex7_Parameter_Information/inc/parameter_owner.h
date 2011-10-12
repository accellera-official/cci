/*******************************************************************************
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

/*!
 * \file    parameter_owner.h
 * \brief   This file defines the OWNER module's functionality.
 *          This file defines a 'PARAMETER_OWNER' sc_module which, in turn,
 *          'int-type' & 'string-type'cci-parameters with default values
 * \author  P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * \date    12th September, 2011 (Monday)
 */
#ifndef PARAMETER_OWNER_H
#define PARAMETER_OWNER_H

#include "cci.h"    // Include 'cci.h' header in all CCI-based applications

/*!
 * \class   parameter_owner parameter_owner.h
 * \brief   The various CCI-based parameters are initialized within this sc_module
 */ 
class parameter_owner : public sc_core::sc_module
{
	public	:
		
		SC_HAS_PROCESS(parameter_owner);

		/*!
		 * \fn    parameter_owner::parameter_owner
		 * \brief Default Constructor
		 */
		parameter_owner(sc_module_name _name)
		: sc_module(_name)
			// Assign name and default value to the cci 'int-type' parameter.
		, int_param("mutable_int_param", 0)
			// Assign name to the cci 'string-type' parameter (Default value NOT ASSIGNED)
		, string_param("mutable_string_param","Default_Value")
		{
			/// Register a SC_THREAD process
			SC_THREAD(run_thread);

			std::cout << "\nPrior to " << sc_time_stamp() << "\tdemonstrating 'is_initial_value' API" << std::endl;

			std::cout << "\n\t[OWNER] : Parameter : " << int_param.get_name()\
				<< "\tValue : " << int_param.get() << std::endl;

			std::cout << "\tIs initial value ?\t" << std::boolalpha	<< int_param.is_initial_value() << std::endl;


			std::cout << "\nPrior to " << sc_time_stamp() << "\tdemonstrating 'get_default_value()'" << endl;			
			std::cout << "\n\t[OWNER -> Retrieve] : Parameter name  : " << int_param.get_name() << endl;

			//Setting parameter value using 'cci_value' not implemented
			//Used something like : int_param = 1; (see code within the SC_THREAD)

			/// Query default value of a parameter using 'get_default_type()' API
			std::cout << "\n\t[OWNER -> Retrieve] : Using 'get_default_value()' : " << int_param.get_default_value() << endl;			

			/// Set Documentation - This gives the meta-data about the parameter
			std::cout << "\n\t[OWNER -> Set] : Param documentation - 'This is a mutable type integer parameter'" << endl;			
			const std::string init_doc = "This is a mutable type integer parameter";
			int_param.set_documentation(init_doc);

		}// End of Constructor

		/*!
		 * \fn    paramter_owner::~parameter_owner
		 * \brief Default Destructor
		 */
		~parameter_owner()
		{
			// Destructor does nothing here
		}

	
		/*!
		 * \fn     void run_thread (void)
		 * \brief  SC_THREAD process definition
		 */
		void run_thread (void)
		{
			while(1)
			{
				wait(1.0, SC_NS);
				
				// Override the default value
				std::cout << "@ " << sc_time_stamp() << endl;
				std::cout << "\n\t[OWNER -> Set] : Overriding default value of " << int_param.get_name() << " to 1" << endl;
			
				/// Setting value to the integer type parameter
				int_param = 1;

				wait(17.0, SC_NS);

				/*****************************************************************************************************************
				// The code (commented) below notifies the following error :
				//
				// Error: /OSCI/CCI/cci_value_failure: Set cci value not implemented for not specialized parameter types.
				// In file: /home/pvs/MyInstallations/greenstarcore/greenstarcore_rev_622/gs_param_implementation/gs_cci_param.h:125
				// In process: param_acc.run_accessor @ 20 ns
					
				std::cout << "\n@ " << sc_time_stamp() << "\tdemonstrating 'set_value' using cci_value for int" << endl;
				
				cci::cnf::cci_value int_value(55);
				int_param.set_value(int_value);
			
				std::cout << "\n\t[OWNER -> Retrieve] : " << int_param.get_name() << " value is " << int_param.get() << endl;
							
				wait(2.0, SC_NS);
				******************************************************************************************************************/

				std::cout << "\n@ " << sc_time_stamp() << " demonstrating 'set_value_invalid()'" << endl;	
				std::cout << "\n\t[OWNER -> Set] : " << int_param.get_name() << " value invalid." << endl;
				
				/// Set the cci parameter to invalid state using 'set_invalid_state()' API
				int_param.set_invalid_value();				
			
				/// Query a cci parameter value validity using 'is_invalid_value()' API	
				if(int_param.is_invalid_value())	{
					std::cout << "\t[OWNER] : Is Invalid Value ? " << "\tReturned Status : "\
						<< std::boolalpha << int_param.is_invalid_value() << endl;
					std::cout << "\n\t[OWNER] : " << int_param.get_name() << "\tValue : " << int_param.get() << endl;
				}
				else
				{
					std::cout << "\t[OWNER] : Is Invalid Value ? " << "\tReturned Status : "\
						<< std::boolalpha << int_param.is_invalid_value() << endl;
					std::cout << "\n\t[OWNER] : " << int_param.get_name() << "\tValue : " << int_param.get() << endl;
				}//End of IF-ELSE

				wait(10.0, SC_NS);

			}// End of WHILE

		}// End of SC_THREAD


	private	:
		
		// CCI parameters declarations
		cci::cnf::cci_param<int>         int_param;    //!< Integer type cci-parameter
		cci::cnf::cci_param<std::string> string_param; //!< String type cci-parameter

};// End of (SC_MODULE) class

#endif	// End of PARAMETER_OWNER_H

