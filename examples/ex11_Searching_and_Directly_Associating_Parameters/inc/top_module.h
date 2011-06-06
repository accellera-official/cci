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
 * @brief    This header declares and defines the top module which instantiates the two owner modules 
 * @author   P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date     2nd June, 2011 (Thursday)
 */
#ifndef TOP_MODULE_H
#define TOP_MODULE_H

/// Include the "cci.h" header file in all cci-based applications
#include <cci.h>
#include <assert.h>
#include <vector>

#include "parameter_owner_1.h"
#include "parameter_owner_2.h"

/**
 * @brief    The configurator class illustrates ways of searching parameters in a model using different patterns 
 * @author   P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date     2nd June, 2011 (Thursday)
 */ 
class top_module : public ::sc_core::sc_module
{
	public:
		
		parameter_owner_1   param_owner_1;
		parameter_owner_2   param_owner_2;

		/// Default constructor
		SC_CTOR(top_module)
		: param_owner_1("param_owner_1")
		, param_owner_2("param_owner_2")
		, main_clk_Hz("main_clk_Hz", 5000)
		{
			/// Get handle of the broker responsible for the class/module
			myTopModBrokerIF	=	&cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));

			/// Report if handle returned is NULL
			assert(myTopModBrokerIF != NULL && "Configuration Broker handle is NULL");
			
		}// End of Constructor	


		void end_of_elaboration (void)
		{
			/// Selected parameters list of OWNER module #2
			tempList	=	myTopModBrokerIF->get_param_list("*._speed_Hz"); 

			/// Selected parameters list of OWNER module #1
			returnParameterList	=	myTopModBrokerIF->get_param_list("*._freq_Hz"); 

			/// Single parameter vector containing the list of all selected parameters
			returnParameterList.insert(returnParameterList.end(), tempList.begin(), tempList.end());

			for(unsigned int i = 0; i < returnParameterList.size(); i++)
			{
				if(myTopModBrokerIF->exists_param(returnParameterList[i]))
				{
					cci::cnf::cci_base_param *temp;
					temp = myTopModBrokerIF->get_param(returnParameterList[i]);
					returnBaseParamList.push_back(temp);
				
					std::cout << "\n\t[TOP_MODULE within beoe] : Parameter Name : " << returnBaseParamList[i]->get_name()	\
						<< "\tParameter Value : " << returnBaseParamList[i]->json_serialize() << std::endl;
				}
					else
						std::cout << "\t[TOP_MODULE within beoe] : Parameter Name : " << returnBaseParamList[i]->get_name() << "\tnot found." << std::endl;
					
			}// End of FOR

			/// Registering callbacks
			for(unsigned int i = 0; i < returnBaseParamList.size(); i++)	{
				synchValues(main_clk_Hz,returnBaseParamList[i]);
			}
		
		}// End of end_of_elaboration Callback

		
		/// Pre-Write and Post-Write Callbacks Implementation
		cci::cnf::callback_return_type
			write_callback(cci::cnf::cci_base_param& base_param, const cci::cnf::callback_type& cb_reason, cci::cnf::cci_base_param * _out_param)	{
				// Decision on Pre-Write & Post-Write callbacks
				switch(cb_reason)
				{
					case cci::cnf::pre_write	:	{

						std::cout << "\n\t[TOP_MODULE - pre_write callback] : Parameter Name : "
							 << base_param.get_name() << "\tValue : " << base_param.json_serialize() << std::endl;

						break;	}

					case cci::cnf::post_write	:	{
						std::cout << "\t[TOP_MODULE - post_write callback] : Parameter Name : "
							<< base_param.get_name() << "\tValue : " << base_param.json_serialize() << std::endl;

						_out_param->json_deserialize(base_param.json_serialize());

						break;	}

					default	:
						std::cout << "\t[TOP_MODULE callback] : Callback type unknown" << std::endl;
					
				}// End of SWITCH-CASE Statements
				
				return cci::cnf::return_nothing;

			}// End of Write Callbacks

		
		/**
 		  * @brief     Function for synchronizing the values of cci_parameter of OWNER modules via the TOP_MODULE
 		  * @param     _input_param Reference of Integer type CCI parameter
 		  * @param     _out_param   Reference of cci_base_param pointers to the selected owner parameters
		  * @return    void 
 		  */	
		void synchValues (cci::cnf::cci_param<int> & _input_param, cci::cnf::cci_base_param * _out_param)
		{
			/// Registering Callbacks for the top module cci parameter
			main_clk_pre_write_cb	=	main_clk_Hz.register_callback(cci::cnf::pre_write,
				this, cci::bind(&top_module::write_callback, this, _1, _2, _out_param));

			main_clk_post_write_cb_vec.push_back(main_clk_Hz.register_callback(cci::cnf::post_write,\
				this, cci::bind(&top_module::write_callback, this, _1, _2, _out_param)) );
		}


	private	:
	
	/// Declaring a CCI configuration broker interface instance
	cci::cnf::cci_cnf_broker_if* myTopModBrokerIF;

	/// A CCI Instance of the configurator
	cci::cnf::cci_param<int>  main_clk_Hz;

	/// Callback Adaptor Objects
	cci::shared_ptr<cci::cnf::callb_adapt_b>  main_clk_pre_write_cb;
	std::vector<cci::shared_ptr<cci::cnf::callb_adapt_b> > main_clk_post_write_cb_vec;

	/// std::vector storing the desired owner parameters list searched using a pattern
	std::vector<std::string> tempList;
	std::vector<std::string> returnParameterList;
	
	/// std::vector storing the searched owner parameters references to CCI base parameter pointers
	std::vector<cci::cnf::cci_base_param*> returnBaseParamList;

};// End of Class/SC_MODULE

#endif	// End of TOP_MODULE_H

