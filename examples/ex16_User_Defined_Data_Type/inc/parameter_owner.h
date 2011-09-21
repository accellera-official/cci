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
  * @brief   This header instantiates various CCI parameters with default values
  * @author  Girish Verma, CircuitSutra Technologies   <girish@circuitsutra.com>
  *          P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
  * @date    12th September, 2011 (Monday)
  */
#ifndef PARAMETER_OWNER_H
#define PARAMETER_OWNER_H

/// Include cci.h header in all CCI-based applications
#include "cci.h"

#include "user_datatype.h"

/**
 	* @brief   This sc_module instantiates an object of user-defined data type of
 	*          CCI-parameter
  */ 
SC_MODULE(parameter_owner)
{
	public	:
		/// Default Constructor
		SC_CTOR(parameter_owner)

			/// Assign name and value to the cci 'user-defined data type' parameter
		: udt_param("User_data_type_param",route_table_ut(0x200,0x300,1))
		{

			// Registering SC_THREAD process
			SC_THREAD(run_thread);

			std::cout << "\nPrior to " << sc_time_stamp() << "\tdemonstrating 'get_default_value()'" << endl;			
			std::cout << "\n\t[OWNER -> Retrieve] : Parameter name  : " << udt_param.get_name() << endl;
			
			/// Query default value of a parameter using 'get_default_type()' API
			std::cout << "\n\t[OWNER -> Retrieve] : Using 'get_default_value()' : " << udt_param.get_default_value() << endl;			
			std::cout << "\n\t[OWNER -> Retrieve] : Parameter Value' : " << udt_param.get() << endl;			

			/// Set documentation for the user-defined data type
			std::cout << "\n\t[OWNER -> Set] : Param doc - 'This is user-defined data type" << endl;			
			const std::string init_doc = "This is user-defined data type";
			udt_param.set_documentation(init_doc);
		}

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
				std::cout << "@ " << sc_time_stamp() << endl;
				std::cout << "\n\t[OWNER -> Retrieve] : Documentation : " << udt_param.get_documentation() << std::endl;

				// Override the default value
				std::cout << "\n\t[OWNER -> Set] : New Value " << udt_param.get_name()
					<< "\t's_address:768,d_address:1024,index:2'" << std::endl;				
				udt_param.json_deserialize("{\"s_address\":768,\"d_address\":1024,\"index\":2}");


				wait(2.0, SC_NS);		
				std::cout << "@ " << sc_time_stamp() << endl;

				// Access parameter's value
				std::cout << "\n\t[OWNER -> Retrieve] : UDT Value : " << udt_param.get() << std::endl;


				wait(8.0, SC_NS);

				std::cout << "\n@ " << sc_time_stamp() << " demonstrating 'set_value_invalid()'" << endl;	
				std::cout << "\n\t[OWNER -> Set] : " << udt_param.get_name() << " value invalid." << endl;
				
				/// Set the cci parameter to invalid state using 'set_invalid_state()' API
				udt_param.set_invalid_value();				
			
				/// Query a cci parameter value validity using 'is_invalid_value()' API	
				if(udt_param.is_invalid_value())
				{
					std::cout << "\n\t[OWNER] : " << udt_param.get_name() << " value is invalid : " << udt_param.get() << endl;
					std::cout << "\n\t[OWNER] : Is Invalid Value ? " << "\tReturned status : "\
						<< std::boolalpha << udt_param.is_invalid_value() << endl;
				}
				else
				{
					std::cout << "\n\t[OWNER] : " << udt_param.get_name() << " value is not invalid." << endl;					
					std::cout << "\n\t[OWNER] : Is Invalid Value ? " << "\tReturned status : "\
						<< std::boolalpha << udt_param.is_invalid_value() << endl;
				}// End of IF-ELSE

				wait(20.0, SC_NS); 

			}// End of WHILE

		}//End of SC_THREAD


	private	:
		
		/// CCI parameters declarations
		cci::cnf::cci_param<route_table_ut, cci::cnf::mutable_parameter> udt_param; /*!<User-defined data type parameter*/

};//End of PARAMETER_OWNER

#endif	// End of PARAMETER_OWNER_H
