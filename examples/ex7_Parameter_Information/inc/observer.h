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
  * @brief   This class demonstrates implements a simple observer module's functionality
  * @author  P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
  * @date    17th August, 2011 (Wednesday) : 17:43 hrs IST
  */
#ifndef OBSERVER_H
#define OBSERVER_H

/// Include 'cci.h' header in all CCI-based applications
#include <cci.h>
#include <assert.h>
#include <vector>

/**
  * @brief   This observer class reads in the cci-parameters values 
  * @author  P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
  * @date    17th August, 2011 (Wednesday) : 17:55 hrs IST
  */
class observer
{
	public	:

		/// Default constructor
		observer()
		{
			/// Instantiate a cci_originator in order to get hold of the configuration broker interface
			cci::cnf::cci_originator  observerOriginator("observerOriginator");
		
			/// Get the broker responsible for this module using 'get_current_broker' API
			observerBrokerIF = &cci::cnf::cci_broker_manager::get_current_broker(observerOriginator);

			// Assert if broker handle returned is NULL
			assert(observerBrokerIF != NULL && "Observer Broker Handle Returned is NULL");


			/// Check for the broker type (default or private) using 'is_private_broker()' API
			if(observerBrokerIF->is_private_broker())
				/// Access broker's name using 'name()' 
				std::cout << "\n\t[OBSERVER] : Broker Type : " << observerBrokerIF->name() << endl;
			else
				std::cout << "\n\t[OBSERVER] : " << observerBrokerIF->name() << " is not a private broker." << endl; 


			/// Gets the list of all cci-parameters within the model
			complete_param_list = observerBrokerIF->get_param_list();


			// NOTE :  All cci-parameters will be available here as this class has been instantiated after
			//         PARAMETER_OWNER and PARAMETER_CONFIGURATOR modules
			//for(unsigned int i = 0; i < base_param_ptr_list.size(); i++)
			for(unsigned int i = 0; i < complete_param_list.size(); i++)
			{
				// Get reference to the owner cci-parameters
				base_param_ptr_list.push_back(observerBrokerIF->get_param(complete_param_list[i]));
	
				std::cout << "\n\t[OBSERVER] : Parameter Name : " << base_param_ptr_list[i]->get_name()
					<< "\tParameter Value : " << base_param_ptr_list[i]->json_serialize() << std::endl;
			}
	
		}// End of Constructor


		/// Destructor
		~observer()
		{
			// Destructor does nothing
		}


	private	:

		/// CCI configuration broker instance
		cci::cnf::cci_cnf_broker_if* observerBrokerIF;

		/// Declare a std::vector to hold the string names of all the cci-parameters
		std::vector<std::string> complete_param_list;			

		/// Declaring cci_base_parameters
		std::vector<cci::cnf::cci_base_param *> base_param_ptr_list;			

};// End of Class

#endif	// End of OBSERVER_H
