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
 * @date     15th June, 2011 (Wednesday)
 */
#ifndef PARAMETER_CONFIGURATOR_H
#define PARAMETER_CONFIGURATOR_H

/// Include the "cci.h" header file in all cci-based applications
#include <cci.h>
#include <assert.h>
#include <vector>

/**
 * @brief    The configurator class illustrates retrieving the list of
 *           unconsumed cci-parameters with the broker API - 'is_used' 
 * @author   P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date     15th June, 2011 (Wednesday)
 */ 
class parameter_configurator : public ::sc_core::sc_module
{
	public:
		
		/// Default constructor
		SC_CTOR(parameter_configurator)
		: param_used_status(0)
		{
			// Get handle of the broker responsible for the class/module
			myCfgrBrokerIF	=	&cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));

			// Report if handle returned is NULL
			assert(myCfgrBrokerIF != NULL && "Configuration Broker handle is NULL");

			// Define hierarchical paths for the list of cci_parameters that have been
			// assumed to be instantiated within owner module by the 'testbench file'
			int_param_str = "param_owner.int_param";
			float_param_str = "param_owner.float_param";
			string_param_str = "param_owner.string_param";
			double_param_str = "param_owner.double_param";

			/// Registering SC_THREAD with the SystemC kernel
			SC_THREAD(run_cfgr);

		}// End of Constructor	


		/**
		  * @brief      This callback function demonstrates using broker API 'is_used'
		  *             to retrieve the list of unconsumed parameters in a model.
		  * @param      void
		  * @return     void
		  */ 	
		void end_of_elaboration (void)
		{
			std::cout << "\n\t[CFGR within EOE] : Check 'used' status of the int type cci-parameter" << std::endl;
			if(!myCfgrBrokerIF->is_used(int_param_str))
				unconsumed_parameter_list.push_back(int_param_str);
			std::cout << "\t[CFGR within EOE] : Parameter Name : " <<  int_param_str
				<< "\tUsed Status : " << myCfgrBrokerIF->is_used(int_param_str) << std::endl;

			std::cout << "\n\t[CFGR within EOE] : Check 'used' status of the float type cci-parameter" << std::endl;
			if(!myCfgrBrokerIF->is_used(float_param_str))
				unconsumed_parameter_list.push_back(float_param_str);
			std::cout << "\t[CFGR within EOE] : Parameter Name : " <<  float_param_str
				<< "\tUsed Status : " << myCfgrBrokerIF->is_used(float_param_str) << std::endl;

			std::cout << "\n\t[CFGR within EOE] : Check 'used' status of the string type cci-parameter" << std::endl;
			if(!myCfgrBrokerIF->is_used(string_param_str))
				unconsumed_parameter_list.push_back(string_param_str);
			std::cout << "\t[CFGR within EOE] : Parameter Name : " <<  string_param_str
				<< "\tUsed Status : " << myCfgrBrokerIF->is_used(string_param_str) << std::endl;

			std::cout << "\n\t[CFGR within EOE] : Check 'used' status of the double type cci-parameter" << std::endl;
			if(!myCfgrBrokerIF->is_used(double_param_str))
				unconsumed_parameter_list.push_back(double_param_str);
			std::cout << "\t[CFGR within EOE] : Parameter Name : " <<  double_param_str
				<< "\tUsed Status : " << myCfgrBrokerIF->is_used(double_param_str) << std::endl;
		}


		/**
		  * @brief      This process retrieves and displays list of unconsumed
		  *             parameters in a model to the user
		  * @param      void
		  * @return     void
		  */ 	
		void run_cfgr (void)
		{
			while(1)
			{
				std::cout << "\n@ " << sc_time_stamp() << std::endl;
				std::cout << "\t[CFGR] : List of all unconsumed parameters in the model" << std::endl;

				for(unsigned int i = 0; i < unconsumed_parameter_list.size(); i++)
					std::cout << "\n\t[CFGR] : Unconsumed Parameter Name : " << unconsumed_parameter_list[i] << std::endl;

				wait(50.0, sc_core::SC_NS);	

			}// End of WHILE

		}// End of SC_THREAD


	private	:
	
	/// Declaring a CCI configuration broker interface instance
	cci::cnf::cci_cnf_broker_if* myCfgrBrokerIF;

	/// CCI base parameter
	cci::cnf::cci_base_param*    cfgr_param_ptr;
 
	/// std::vector to store the list of the unconsumed parameters
	std::vector<std::string>     unconsumed_parameter_list;

	std::string                  int_param_str;
	std::string                  float_param_str;
	std::string                  string_param_str;
	std::string                  double_param_str;

	bool param_used_status;      /*!Variable stores the 'used' status of a cci-parameter*/

};// End of Class/SC_MODULE

#endif	// End of PARAMETER_CONFIGURATOR_H

