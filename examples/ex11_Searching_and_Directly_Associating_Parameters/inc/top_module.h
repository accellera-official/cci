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
#include <sstream>

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
			str2="clock_speed_KHz";	
			param_owner1	= new parameter_owner("param_owner1", str1);
			param_owner2	= new parameter_owner("param_owner2", str2);

			/// Report if handle returned is NULL
			assert(myTopModBrokerIF != NULL && "Configuration Broker handle is NULL");
			
			std::string param1_str = "top_mod.param_owner1.clk_freq_Hz";
			std::string param2_str = "top_mod.param_owner2.clock_speed_KHz";

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
				float conversion_factor;
				conversion_factor = multiplyWithConversionFactor(returnBaseParamList[0]->get_name(), returnBaseParamList[i]->get_name());
				synchValues(returnBaseParamList[0], returnBaseParamList[i], conversion_factor);
			}
		
	}// End of Constructor

		
	/// Pre-Write and Post-Write Callbacks Implementation
	cci::cnf::callback_return_type
		write_callback(cci::cnf::cci_base_param & _base_param_1,\
										const cci::cnf::callback_type& cb_reason,\
										cci::cnf::cci_base_param * _base_param_2, float conv_fact )
		{
			// Decision on Pre-Write & Post-Write callbacks
			std::cout << "\t[TOP_MODULE - post_write callback] : Parameter Name : "
				<< _base_param_1.get_name() << "\tValue : " << _base_param_1.json_serialize() << std::endl;


            float freq = atof((_base_param_1.json_serialize()).c_str());
            float operand1 = freq * conv_fact;
						std::stringstream ss;
            ss.clear();
            ss.str("");
            ss << operand1;
            _base_param_2->json_deserialize(ss.str());


					
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
					
			std::cout << "\n\tParameter1_str: " << parent_str << std::endl;
			std::cout << "\tParameter2_str : " << child_str << std::endl;
			std::cout << "\tConversionFactor : " << returnValue << std::endl;

			return returnValue;
		} 
	
		
		/**
 		  * @brief     Function for synchronizing the values of cci_parameter of OWNER modules via the TOP_MODULE
 		  * @param     _input_param Reference of Integer type CCI parameter
 		  * @param     _out_param   Reference of cci_base_param pointers to the selected owner parameters
		  * @return    void 
 		  */	
		void synchValues (cci::cnf::cci_base_param * _base_param_1, cci::cnf::cci_base_param * _base_param_2,float con_fact)
		{
			
			main_clk_post_write_cb_vec.push_back(_base_param_1->register_callback(cci::cnf::post_write,\
				this, cci::bind(&top_module::write_callback, this, _1, _2,_base_param_2,con_fact)) );

			main_clk_post_write_cb_vec.push_back(_base_param_2->register_callback(cci::cnf::post_write,\
				this, cci::bind(&top_module::write_callback, this, _1, _2, _base_param_1,(1.0/con_fact))) );
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

