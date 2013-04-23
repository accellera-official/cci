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
 * @file     top_module.h
 * @brief    This header declares and defines the top module which instantiates two owner modules
 * @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * @date     9th June, 2011 (Thursday)
 */
#ifndef TOP_MODULE_H
#define TOP_MODULE_H

#include <cci>      // Include the "cci" header file in all cci-based applications
#include <assert.h>
#include <vector>

#include "parameter_owner.h"
#include "param_value_sync.h"

/**
 * @class    top_module top_module.h
 * @brief    The configurator class registers 'post_write' callbacks for the
 *           owner's parameters in order to update an owner cci_parameter directly
 *           when another cci_parameter value is modified 
 */ 
class top_module : public sc_module
{
	public:
		
		param_value_sync*  param_sync;  //!< Instance of the 'param_value_sync' class

		// Pointers to the owner module
		parameter_owner*   param_owner1; //!< owner module#1
		parameter_owner*   param_owner2; //!< owner module#2

		/**
		 * @fn     SC_CTOR(top_module)
		 * @brief  Default constructor
		 */
		SC_CTOR(top_module)
		{
			/// Get handle of the broker responsible for the class/module
			myTopModBrokerIF	=	&cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));
		
			std::string str1,str2;		//!< Strings to store the names of the owner's parameters
			str1="clk_freq_Hz";
			str2="clock_speed_Hz";	
			param_owner1	= new parameter_owner("param_owner1", str1, 1000);
			param_owner2	= new parameter_owner("param_owner2", str2, 2000);

			// Report if handle returned is NULL
			assert(myTopModBrokerIF != NULL && "Configuration Broker handle is NULL");
			
			std::string param1_str = "top_mod.param_owner1.clk_freq_Hz";
			std::string param2_str = "top_mod.param_owner2.clock_speed_Hz";

			/// Check for existence of the owner cci_parameter using name-based look up access
			/// and then assign their reference to respective cci_base_param
			if(myTopModBrokerIF->exists_param(param1_str))
			{
				cci::cnf::cci_base_param *temp = myTopModBrokerIF->get_param(param1_str);
				selected_base_param_list.push_back(temp);
			
				std::cout << "\n\t[TOP_MODULE C_TOR] : Parameter Name : " << temp->get_name()	\
					<< "\tParameter Value : " << temp->json_serialize() << std::endl;
			}
			else
				std::cout << "\t[TOP_MODULE C_TOR] : Parameter Name : " << param1_str << "\tnot found." << std::endl;

			/// Check for existence of the owner cci_parameter using name-based look up access
			/// and then assign their reference to respective cci_base_param
			if(myTopModBrokerIF->exists_param(param2_str))
			{
				cci::cnf::cci_base_param *temp = myTopModBrokerIF->get_param(param2_str);
				selected_base_param_list.push_back(temp);
			
				std::cout << "\n\t[TOP_MODULE C_TOR] : Parameter Name : " << temp->get_name()	\
					<< "\tParameter Value : " << temp->json_serialize() << std::endl;
			}
			else
				std::cout << "\t[TOP_MODULE C_TOR] : Parameter Name : " << param2_str << "\tnot found." << std::endl;


			/// Pass on the list of selected cci_base_parameters to the 'param_value_sync'
			/// that looks in at the synchronization related activity b/w the owner modules
			param_sync	=	new param_value_sync(selected_base_param_list);

		}// End of Constructor


		/**
		 * @fn     top_module::~top_module
		 * @brief  Destructor
		 */
		~top_module()
		{
			
		}
		

	private	:
	
	// Declare a CCI configuration broker interface instance
	cci::cnf::cci_cnf_broker_if* myTopModBrokerIF; //!< CCI Configuration broker instance

	/** std::vector storing the searched owner parameters references to CCI base parameter pointers*/
	std::vector<cci::cnf::cci_base_param*> selected_base_param_list; //!< std::vector storing (selected) owner parameters references

};// End of Class/SC_MODULE

#endif	// End of TOP_MODULE_H

