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
  * @file    parameter_configurer.h
  * @brief   This class demonstrates means to access and set various  
  *          attributes of a cci parameter
  * @author  P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
  *          Girish Verma, CircuitSutra Technologies   <girish@circuitsutra.com>
  * @date    12th September, 2011 (Monday)
  */
#ifndef PARAMETER_CONFIGURER_H
#define PARAMETER_CONFIGURER_H

/// Include 'cci.h' header in all CCI-based applications
#include <cci.h>
#include <assert.h>

/**
  * @brief   This sc_module instantiates a cci configuration broker which accessess
  *          and sets the various attributes of a cci parameter 
  */
SC_MODULE(parameter_configurer)
{
	public	:

		/// Default constructor
		SC_CTOR(parameter_configurer)
		{
			/// Get the broker responsible for this module using 'get_current_broker' API
			myBrokerInterface = &cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));

			// Assert if broker handle returned is NULL
			assert(myBrokerInterface != NULL && "Broker Handle Returned is NULL");


			/// Check for the broker type (default or private) using 'is_private_broker()' API
			if(myBrokerInterface->is_private_broker())
				/// Access broker's name using 'name()' 
				std::cout << "\n\t[CFGR C_TOR] : Broker Type : " << myBrokerInterface->name() << endl;
			else
				std::cout << "\n\t[CFGR C_TOR] : Broker Type : " << myBrokerInterface->name() << " - is not a private broker." << endl; 


			udt_param_str = "param_owner.User_data_type_param";

			/// Check the existence of the user-defined data type cci-parameter
			if(myBrokerInterface->exists_param(udt_param_str))
			{
				/// If parameter exists, get handle of the parameter using 'get_param' API
				udt_param_ptr = myBrokerInterface->get_param(udt_param_str);		
					
				// Report if parameter handle is returned NULL
				assert(str_param_ptr != NULL && "User define data type CCI Parameter Handle returned NULL");
			}
			else
				std::cout << "\n\t[CFGR C_TOR] : User define datatype  parameter does not exist" << endl;


 			/// Registering SC_THREAD process 
			SC_THREAD(run_accessor);

		}// End of Constructor


		/// Destructor
		~parameter_configurer()
		{
			// Destructor does nothing
		}


		/// Within SC_THREAD process definition
		void run_accessor(void)
		{
			while(1) 
			{
				wait(4.0, SC_NS);
				
				std::cout << "\n@ " << sc_time_stamp() << endl;

				/// Access parameter's name using 'get_name()' API
				std::cout << "\n\t[CFGR -> Retrieve] : Parameter name : " << udt_param_ptr->get_name() << endl;

				/// Access parameter's value using 'json_serialize' API
				std::cout << "\n\t[CFGR -> Retrieve] : Parameter value: " << udt_param_ptr->json_serialize() << endl;

				/// Access parameter's documentation using 'get_documentation()' API
				std::cout << "\n\t[CFGR -> Retrieve] : Parameter doc: " << udt_param_ptr->get_documentation() << endl;

				wait(2.0, SC_NS);

				std::cout << "\n@ " << sc_time_stamp() << endl;
				std::cout << "\t[CFGR -> Set] : Value - 's_address:1024,d_address:1280,index:3" << std::endl; 

				std::string set_string ("{\"s_address\":1024,\"d_address\":1280,\"index\":3}");
				udt_param_ptr->json_deserialize(set_string);


				wait(2.0, SC_NS);
				std::cout << "\n@ " << sc_time_stamp() << std::endl;

				/// Access parameter's name using 'get_name()' API
				std::cout << "\n\t[CFGR -> Retrieve] : Parameter name : " << udt_param_ptr->get_name() << endl;

				/// Access parameter's value using 'json_serialize' API
				std::cout << "\n\t[CFGR -> Retrieve] : Parameter value: " << udt_param_ptr->json_serialize() << endl;

				wait(20.0, SC_NS);

			}// End of WHILE

	}// End of SC_THREAD


	private	:

		/// CCI configuration broker instance
		cci::cnf::cci_cnf_broker_if* myBrokerInterface;

		/// std::string types for storing parameters hierarchical paths
		std::string udt_param_str;
			
		/// Declaring cci_base_parameters
		cci::cnf::cci_base_param* udt_param_ptr;			

};//End of SC_MODULE

#endif	// End of PARAMETER_CONFIGURER_H
