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

/**
  * @file    parameter_owner.h
  * @brief   This file instantiates defines a 'PARAMETER_OWNER' sc_module which,
  *          in turn, 'int-type' & 'string-type'cci-parameters with default values
  * @author  P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
  * @date    18th April, 2011 (Monday) : 17:13 hrs IST
  */
#ifndef PARAMETER_OWNER_H
#define PARAMETER_OWNER_H

/// Include 'cci.h' header in all CCI-based applications
#include "cci.h"

/**
 	* @brief   The various CCI-based parameters are initialized within this sc_module
  */ 
SC_MODULE(parameter_owner)
{
	public	:

		/// Default Constructor
		SC_CTOR(parameter_owner)

			/// Assign name and value to the cci 'int-type' parameter.
		: int_param("mutable_int_param", 0)
			/// Assign name to the cci 'string-type' parameter (Value NOT ASSIGNED)
		, string_param("mutable_string_param","Default_Value")
		{
			// Registering SC_THREAD process
			SC_THREAD(run_thread);

			std::cout << "\nPrior to " << sc_time_stamp() << "\tdemonstrating 'get_default_value()'" << endl;			
			std::cout << "\n\t[OWNER -> Retrieve] : Parameter name  : " << int_param.get_name() << endl;
			
			//TODO :  The following code generates a warning :
			//
			//Warning: GreenSocs/cci/not_implemented: not implemented
			//In file: /home/pvs/MyInstallations/greenstarcore/greenstarcore_rev_622/gs_param_implementation/gs_cci_base_param.h:210 
			//
			//Also, the status value returned is 'FALSE' (in this case) for integer type parameter
			//
			//if(int_param.is_initial_value())
			//	itd::cout << "\n\t[OWNER] : Value has not been modified since it is initialized by CCI tool." << endl;
			//else
			//	std::cout << "\n\t[OWNER] : is_initial_value() - Value has been modified." << endl;
			//
			//Setting parameter value using 'cci_value' not implemented
			//Used something like : int_param = 1; (see code within the SC_THREAD)

			/// Query default value of a parameter using 'get_default_type()' API
			std::cout << "\n\t[OWNER -> Retrieve] : Using 'get_default_value()' : " << int_param.get_default_value() << endl;			
			std::cout << "\n\t[OWNER -> Set] : Param doc - 'Initial documentation'" << endl;
			
			const std::string init_doc = "Initial documentation";
			int_param.set_documentation(init_doc);

		}// End of Constructor

		/// Destructor
		~parameter_owner()
		{
			// Destructor does nothing here
		}

	
		/// SC_THREAD process definition
		void run_thread (void)
		{
			while(1)
			{
				wait(1.0, SC_NS);
				
				// Override the default value
				std::cout << "@ " << sc_time_stamp() << endl;
				std::cout << "\n\t[OWNER -> Set] : Overriding default value of " << int_param.get_name() << " to 2" << endl;
			
				/// Setting value to the integer type parameter
				int_param = 1;

				///
				const std::string doc2 = "Modified by initial value";					

				std::cout << "\n\t[OWNER -> Set] : Int param doc - 'Modified by initial value'" << endl;
				int_param.set_documentation(doc2);
		
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
				if(int_param.is_invalid_value())
					std::cout << "\n\t[OWNER] : " << int_param.get_name() << " value is invalid : " << int_param.get() << endl;
				else
					std::cout << "\n\t[OWNER] : " << int_param.get_name() << " value is not invalid." << endl;					

				wait(20.0, SC_NS); 

			}// End of WHILE

		}// End of SC_THREAD


	private	:
		
		/// CCI parameters declarations
		cci::cnf::cci_param<int>         int_param;    /*!Integer Parameter*/
		cci::cnf::cci_param<std::string> string_param; /*!String Parameter*/

};

#endif	// End of PARAMETER_OWNER_H

