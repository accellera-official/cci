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
 * @date     9th June, 2011 (Thursday)
 */
#ifndef TOP_MODULE_H
#define TOP_MODULE_H

/// Include the "cci.h" header file in all cci-based applications
#include <cci.h>
#include <assert.h>
#include <vector>

#include "parameter_owner.h"

/**
 * @brief    The configurator class registers 'post_write' callbacks for the
 *           owner's parameters in order to update an owner cci_parameter directly
 *           when another cci_parameter value is modified 
 * @author   P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date     9th June, 2011 (Thursday)
 */ 
class top_module : public sc_module
{
	public:
		
		/// Pointers to the owner module
		parameter_owner*   param_owner1;
		parameter_owner*   param_owner2;

		/// Default constructor
		SC_CTOR(top_module)
		{
			/// Get handle of the broker responsible for the class/module
			myTopModBrokerIF	=	&cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));
		
			std::string str1,str2;		/*!Strings to store the names of the owner's parameters*/
			str1="clk_freq_Hz";
			str2="clock_speed_Hz";	
			param_owner1	= new parameter_owner("param_owner1", str1);
			param_owner2	= new parameter_owner("param_owner2", str2);

			/// Report if handle returned is NULL
			assert(myTopModBrokerIF != NULL && "Configuration Broker handle is NULL");
			
			std::string param1_str = "top_mod.param_owner1.clk_freq_Hz";
			std::string param2_str = "top_mod.param_owner2.clock_speed_Hz";

			/// Check for existence of the owner cci_parameter using name-based look up access
			/// and then assign their reference to respective cci_base_param
			if(myTopModBrokerIF->exists_param(param1_str))
			{
				cci::cnf::cci_base_param *temp = myTopModBrokerIF->get_param(param1_str);
				returnBaseParamList.push_back(temp);
			
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
				returnBaseParamList.push_back(temp);
			
				std::cout << "\n\t[TOP_MODULE C_TOR] : Parameter Name : " << temp->get_name()	\
					<< "\tParameter Value : " << temp->json_serialize() << std::endl;
			}
			else
				std::cout << "\t[TOP_MODULE C_TOR] : Parameter Name : " << param2_str << "\tnot found." << std::endl;
					
			for(unsigned int i = 1; i < returnBaseParamList.size(); i++)	{
				synchValues(returnBaseParamList[0], returnBaseParamList[i]);
			}
		
	}// End of Constructor

		
	/// Pre-Write and Post-Write Callbacks Implementation
	cci::cnf::callback_return_type
		write_callback(cci::cnf::cci_base_param & _base_param_1,const cci::cnf::callback_type& cb_reason,  cci::cnf::cci_base_param * _base_param_2)
		{
			// Decision on Pre-Write & Post-Write callbacks
			std::cout << "\t[TOP_MODULE - post_write callback] : Parameter Name : "
				<< _base_param_1.get_name() << "\tValue : " << _base_param_1.json_serialize() << std::endl;

			_base_param_2->json_deserialize(_base_param_1.json_serialize());
					
			return cci::cnf::return_nothing;

		}// End of Write Callbacks

		
		/**
 		  * @brief     Function for synchronizing the values of cci_parameter of OWNER modules via the TOP_MODULE
 		  * @param     _input_param Reference of Integer type CCI parameter
 		  * @param     _out_param   Reference of cci_base_param pointers to the selected owner parameters
		  * @return    void 
 		  */	
		void synchValues (cci::cnf::cci_base_param * _base_param_1, cci::cnf::cci_base_param * _base_param_2)
		{
			
			main_clk_post_write_cb_vec.push_back(_base_param_1->register_callback(cci::cnf::post_write,\
				this, cci::bind(&top_module::write_callback, this, _1, _2,_base_param_2)) );

			main_clk_post_write_cb_vec.push_back(_base_param_2->register_callback(cci::cnf::post_write,\
				this, cci::bind(&top_module::write_callback, this, _1, _2, _base_param_1)) );
		}

	private	:
	
	/// Declaring a CCI configuration broker interface instance
	cci::cnf::cci_cnf_broker_if* myTopModBrokerIF;

	/// Callback Adaptor Objects
	std::vector<cci::shared_ptr<cci::cnf::callb_adapt_b> > main_clk_post_write_cb_vec;

	/// std::vector storing the searched owner parameters references to CCI base parameter pointers
	std::vector<cci::cnf::cci_base_param*> returnBaseParamList;

};// End of Class/SC_MODULE

#endif	// End of TOP_MODULE_H

