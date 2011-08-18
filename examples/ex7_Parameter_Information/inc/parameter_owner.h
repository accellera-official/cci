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
  * @author  P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
  * @date    18th April, 2011 (Monday) : 17:13 hrs IST
  */
#ifndef PARAMETER_OWNER_H
#define PARAMETER_OWNER_H

/// Include cci.h header in all CCI-based applications
#include "cci.h"

/**
 	* @brief   The various CCI-based parameters aire initialized within this sc_module
 	* @author  P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 	* @date    18th April, 2011 (Monday) : 15:34 hrs IST
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
			/// Registering 'pre_read' callback onto the 'str_param' cci-parameter
			str_pre_read_cb	=	string_param.register_callback(cci::cnf::pre_read, this,
			                      cci::bind(&parameter_owner::read_callbacks, this, _1, _2));

			/// Registering 'post_write' callback onto the 'int_param' cci-parameter
			int_post_write_cb	=	int_param.register_callback(cci::cnf::post_write, this,
			                      cci::bind(&parameter_owner::write_callbacks, this, _1, _2));


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
			//	std::cout << "\n\t[OWNER] : Value has not been modified since it is initialized by CCI tool." << endl;
			//else
			//	std::cout << "\n\t[OWNER] : is_initial_value() - Value has been modified." << endl;
			//
			//Used something like : int_param = 1; (see code within the SC_THREAD)

			/// Query default value of a parameter using 'get_default_type()' API
			std::cout << "\n\t[OWNER -> Retrieve] : Using 'get_default_value()' : " << int_param.get_default_value() << endl;			
			std::cout << "\n\t[OWNER -> Set] : Param doc - 'Initial documentation'" << endl;
			
			const std::string init_doc = "Initial documentation";
			int_param.set_documentation(init_doc);
		}

		/// Destructor
		~parameter_owner()
		{
			// Destructor does nothing here
		}
	
	
		/// Callbacks implementation
		cci::cnf::callback_return_type
			read_callbacks	(cci::cnf::cci_base_param & _selected_base_param, const cci::cnf::callback_type & cb_reason)
			{
				const cci::cnf::cci_originator* myOriginator = cci::cnf::cci_originator::get_global_originator();

				std::cout << "\n\t[OWNER pre_read_cb] :  Parameter Name : " << _selected_base_param.get_name()
					<<	"\tOriginator info : " << myOriginator->name() << std::endl;
		
				return cci::cnf::return_nothing;
			}


		/// Callbacks implementation
		cci::cnf::callback_return_type
			write_callbacks	(cci::cnf::cci_base_param & _selected_base_param, const cci::cnf::callback_type & cb_reason)
			{
				const cci::cnf::cci_originator* myOriginator = cci::cnf::cci_originator::get_global_originator();

				std::cout << "\n\t[OWNER post_write_cb] :  Parameter Name : " << _selected_base_param.get_name()
					<<	"\tOriginator info : " << myOriginator->name() << std::endl;
		
				return cci::cnf::return_nothing;
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
				
				int_param = 1;
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
			}
		}

	private	:
		
		/// CCI parameters declarations
		cci::cnf::cci_param<int>         int_param;    /*!Integer Parameter*/
		cci::cnf::cci_param<std::string> string_param; /*!String Parameter*/

		/// Declaration of callback adaptor objects
		cci::shared_ptr<cci::cnf::callb_adapt_b>    str_pre_read_cb; 
		cci::shared_ptr<cci::cnf::callb_adapt_b>    int_post_write_cb; 
};

#endif	// End of PARAMETER_OWNER_H

