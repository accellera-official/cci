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
 * @brief    This header declares and defines top module which instantiates the owner modules 
 * @author   P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date     3rd June, 2011 (Friday)
 */
#ifndef TOP_MODULE_H
#define TOP_MODULE_H

/// Include the "cci.h" header file in all cci-based applications
#include <cci.h>
#include <assert.h>
#include <string.h>
#include <vector>
#include <sstream>

#include "parameter_owner_1.h"
#include "parameter_owner_2.h"

/**
 * @brief    The configurator class illustrates ways of searching parameters in a model using different patterns 
 * @author   P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date     3rd June, 2011 (Friday)
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

		/**
 			* @brief    This end_of_elaboration callback function creates the list of
 			*           the cci_base_param pointers for the selected OWNER cci_parameters
 			*           based on the pattern set
 			* @param    void
 			* @return   void
 			*/ 
		void end_of_elaboration (void)
		{
			/// Selected parameters list
			tempList	=	myTopModBrokerIF->get_param_list("top_mod.param_owner_2.*"); 
			returnParameterList	=	myTopModBrokerIF->get_param_list("top_mod.param_owner_1.*"); 
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

			/// Registering 'pre_write' and 'post_write' callbacks using 'synchValues' function
			for(unsigned int i = 0; i < returnBaseParamList.size(); i++)	{
				
				/// Value translation
				conversion_factor = multiplyWithConversionFactor(main_clk_Hz.get_name(), returnParameterList[i]);

				/// Registering callbacks along with conversion factors
				//synchValues(main_clk_Hz,returnBaseParamList[i]);
				synchValues(main_clk_Hz,returnBaseParamList[i], conversion_factor);
			}
		
		}// End of end_of_elaboration Callback

		
		/// Pre-Write and Post-Write Callbacks Implementation
		cci::cnf::callback_return_type
			//write_callback(cci::cnf::cci_base_param& base_param, const cci::cnf::callback_type& cb_reason, cci::cnf::cci_base_param * _out_param)	{
			write_callback(cci::cnf::cci_base_param& base_param, const cci::cnf::callback_type& cb_reason, cci::cnf::cci_base_param * _out_param, float _conv_fac)	{

				std::stringstream ss;

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

						/// Assigning changed value of the TOP_MODULE cci_param to the cci_base_param
						//  of the selected owner parameters
						//_out_param->json_deserialize(base_param.json_serialize());
						int freq = atoi((base_param.json_serialize()).c_str());
						float operand1 = freq * _conv_fac;
						ss.clear();
						ss.str("");
						ss << operand1; 
						_out_param->json_deserialize(ss.str());

						break;	}

					default	:
						std::cout << "\t[TOP_MODULE callback] : Callback type unknown" << std::endl;
					
				}// End of SWITCH-CASE Statements
				
				return cci::cnf::return_nothing;

			}// End of Write Callbacks

		/**
 		  * @brief     Function computing the conversion factor to be multiplied
 		  *            with the 'main_clk_Hz' parameter of the TOP_MODULE while
 		  *            assigning the same value to owner parameters consistent with
 		  *            their units
 		  * @param     parent_str parameter name of the top_module that has registered pre_write and post_write callbacks
 		  * @param     child_str  parameter name of the owner modules which have been selected based on string patterns
 		  * @return    float      Conversion factor with which the owner parameter value will be multiplied with
 		  */ 
		float multiplyWithConversionFactor(std::string parent_str, std::string child_str)
		{
			float returnValue = 0.0;			
			
			char* str1 = &parent_str[0];
			char* str2 = &child_str[0];
			char* ans1 = strrchr(str1, '_');
			char* ans2 = strrchr(str2, '_');
			std::string s1(ans1);
			std::string s2(ans2);

			if((s1 == "_Hz") && (s2 == "_Hz")) 
				returnValue = 1.0;
			else if((s1 == "_Hz") && (s2 == "_KHz")) 
				returnValue = 0.001;
			else if((s1 == "_Hz") && (s2 == "_MHz")) 
				returnValue = 0.000001;
			else if((s1 == "_KHz") && (s2 == "_Hz")) 
				returnValue = 1000.0;
			else if((s1 == "_KHz") && (s2 == "_KHz")) 
				returnValue = 1.0;
			else if((s1 == "_KHz") && (s2 == "_MHz")) 
				returnValue = 0.001;
			else if((s1 == "_MHz") && (s2 == "_Hz")) 
				returnValue = 1000000.0;
			else if((s1 == "_MHz") && (s2 == "_KHz")) 
				returnValue = 1000.0;
			else if((s1 == "_MHz") && (s2 == "_MHz")) 
				returnValue = 1.0;
					
			std::cout << "\tParent_str: " << parent_str << "\tChild_str : " << child_str << "\tCF : " << returnValue << std::endl;

			return returnValue;
		} 
		
		/**
 		  * @brief      Function for synchronizing the values of cci_parameter of OWNER modules via the TOP_MODULE
 		  * @param      _input_param Reference of Integer type CCI parameter
 		  * @param      _base_param  cci_base_param pointers of selected owner parameters
 		  */	
		//void synchValues (cci::cnf::cci_param<int> & _input_param, cci::cnf::cci_base_param * _base_param)
		void synchValues (cci::cnf::cci_param<int> & _input_param, cci::cnf::cci_base_param * _base_param, float _CF)
		{
			/// Registering 'pre_write' callback for the top module cci parameter
			main_clk_pre_write_cb	=	main_clk_Hz.register_callback(cci::cnf::pre_write,
				this, cci::bind(&top_module::write_callback, this, _1, _2, _base_param, _CF));

			/// Registering 'post_write' callbacks for all the listeners (selected base parameters list)
			main_clk_post_write_cb_vec.push_back(main_clk_Hz.register_callback(cci::cnf::post_write,\
				this, cci::bind(&top_module::write_callback, this, _1, _2, _base_param, _CF)) );
		}


	private	:
	
	/// Declaring a CCI configuration broker interface instance
	cci::cnf::cci_cnf_broker_if* myTopModBrokerIF;

	/// A CCI Instance of the configurator
	cci::cnf::cci_param<int>  main_clk_Hz;

	/// Callback Adaptor Objects
	cci::shared_ptr<cci::cnf::callb_adapt_b>  main_clk_pre_write_cb;

	/// std::vector for holding the 'post_write' callback shared pointers
	std::vector<cci::shared_ptr<cci::cnf::callb_adapt_b> > main_clk_post_write_cb_vec;

	/// std::vector storing the desired owner parameters list searched using a pattern
	std::vector<std::string> tempList;
	std::vector<std::string> returnParameterList;
	
	/// std::vector storing the searched owner parameters references to CCI base parameter pointers
	std::vector<cci::cnf::cci_base_param*> returnBaseParamList;

	float conversion_factor;

};// End of Class/SC_MODULE

#endif	// End of TOP_MODULE_H

