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
 * @file     parameter_configurator.h
 * @brief    This header declares and defines configurator 
 * @author   P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date     18th May, 2011 (Monday)
 */
#ifndef PARAMETER_CONFIGURATOR_H
#define PARAMETER_CONFIGURATOR_H

/// Include the "cci.h" header file in all cci-based applications
#include <cci.h>
#include <assert.h>

/**
 * @brief    This class either reads or sets values to the parameter-owner's
 *           parameters initialization
 * @author   P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date     18th May, 2011 (Monday)
 */ 
class parameter_configurator : public ::sc_core::sc_module
{
	public:
		
		/// Default constructor
		SC_CTOR(parameter_configurator)
		{
			myCfgrBrokerIF	=	&cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));

			assert(myCfgrBrokerIF != NULL && "Parameter-Setter handle is NULL");

			// Get handle of the 'int_param' cci-parameter
			if(myCfgrBrokerIF->exists_param("param_owner.mutable_int_param"))
			{
				std::cout << "\n\t[CFGR C_TOR] : Integer parameter exists" << std::endl; 
				
				int_param_ptr	=	myCfgrBrokerIF->get_param("param_owner.mutable_int_param");
			
				assert(int_param_ptr != NULL && "Parameter Handle is NULL");
			}		
			else
				std::cout << "\n\t[CFGR C_TOR] : Integer parameter doesn't exists." << std::endl;


			// Get handle of the 'float_param' cci-parameter
			if(myCfgrBrokerIF->exists_param("param_owner.mutable_float_param"))
			{
				std::cout << "\n\t[CFGR C_TOR] : Float parameter exists" << std::endl; 
				
				float_param_ptr	=	myCfgrBrokerIF->get_param("param_owner.mutable_float_param");
			
				assert(float_param_ptr != NULL && "Parameter Handle is NULL");
			}		
			else
				std::cout << "\n\t[CFGR C_TOR] : Float parameter doesn't exists." << std::endl;


			// Get handle of the 'string_param' cci-parameter
			if(myCfgrBrokerIF->exists_param("param_owner.mutable_string_param"))
			{
				std::cout << "\n\t[CFGR C_TOR] : String parameter exists" << std::endl; 
				
				str_param_ptr	=	myCfgrBrokerIF->get_param("param_owner.mutable_string_param");
			
				assert(str_param_ptr != NULL && "Parameter Handle is NULL");
			}		
			else
				std::cout << "[CFGR C_TOR] : String parameter doesn't exists." << std::endl;


			/// Registering SC_THREAD with the SystemC kernel
			SC_THREAD(run_mutable_cfgr);

		}// End of Constructor	


		/// SC_THREAD implementation
		void run_mutable_cfgr (void)
		{
			while(1)
			{
				/// Set value to the 'integer' parameter of the owner module
				std::cout << "\n@ " << sc_core::sc_time_stamp() << std::endl;
				std::cout << "\t[CFGR] : Set integer parameter value to '20' using cci_base_parameter" << std::endl;
				int_param_ptr->json_deserialize("20");

				/// Set value to the 'string' parameter of the owner module
				std::cout << "\t[CFGR] : Set string  parameter value to 'configure' using cci_base_parameter" << std::endl;
				str_param_ptr->json_deserialize("configure");

				wait(50.0, sc_core::SC_NS);	

			}// End of WHILE

		}// End of SC_THREAD


	private	:

	/// Declare a configuration broker	
	cci::cnf::cci_cnf_broker_if* myCfgrBrokerIF;

	/// Declare cci_base_param for each of the owner's cci-parameters 
	cci::cnf::cci_base_param* int_param_ptr;
	cci::cnf::cci_base_param* float_param_ptr;
	cci::cnf::cci_base_param* str_param_ptr;

};// End of SC_MODULE

#endif	// End of PARAMETER_CONFIGURATOR_H

