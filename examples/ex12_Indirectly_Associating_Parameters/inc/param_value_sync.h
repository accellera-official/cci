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
 * @file     param_value_sync.h
 * @brief    This file declares and defines the 'param_value_sync' class that
 *           registers callbacks on the cci-parameters of the owner modules.  This
 *           registration of callbacks is in order to establish synchronization between
 *           the respective cci-parameters
 * @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * @date     24th August, 2011 (Wednesday)
 */
#ifndef PARAM_VALUE_SYNC_H
#define PARAM_VALUE_SYNC_H

#include <cci.h>  // Include the "cci.h" header file in all cci-based applications
#include <assert.h>
#include <vector>
#include <sstream>

/**
 * @class    param_value_sync param_value_sync.h
 * @brief    The 'param_vale_sync' class registers 'post_write' callbacks on the
 *           owner's parameters in order to update an owner cci_parameter directly
 *           when another cci_parameter value is modified. 
 */ 
class param_value_sync
{
	public:
		
		/**
		 * @fn     param_value_sync::param_value_sync(std::vector<cci::cnf::cci_base_param * > )
		 * @brief  Overloaded constructor // No default constructor
		 * @param  std::vector<cci::cnf::cci_base_param*>  Selected base parameters list passed from the top_module
		 */
		param_value_sync(std::vector<cci::cnf::cci_base_param * > BaseParamList) 
			/// Define an originator for the class in order to get hold of the default broker
		:	ValueSyncOriginator("ValueSyncOriginator")
		{
			/// Get handle of the broker responsible for the class/module
			ValueSyncBrokerIF	=	&cci::cnf::cci_broker_manager::get_current_broker(ValueSyncOriginator);

			/// Copy the list of selected base parameters to a local std::vector
			returnBaseParamList =  BaseParamList;
	
			
			for(unsigned int i = 1; i < returnBaseParamList.size(); i++)
			{
				float conversion_factor;
				
				/// Determine the 'conversion factor' b/w the two cci_base_params
				conversion_factor = multiplyWithConversionFactor(returnBaseParamList[0]->get_name(), returnBaseParamList[i]->get_name());

				/// Synchonize the second cci_base_param value to the first using the conversion factor determined above

				synchValuesWithCF(returnBaseParamList[0], returnBaseParamList[i], conversion_factor);

			}// End of FOR
		
		}// End of Constructor

		
		/**
		 * @fn     param_value_sync::~param_value_sync
		 * @brief  Destructor
		 */
		~param_value_sync()
		{
			
		}

		
		/**
		 * @fn      cci::cnf::callback_return_type write_callback(cci::nf::cci_base_param, const cci::cnf::callback_type&, cci::cnf::cci_base_param*)
		 * @brief   'PRE_WRITE' and 'POST_WRITE' Callbacks Implementation
		 * @param   cci::cnf::cci_base_param&  Selected base parameter's reference
		 * @param   cci::cnf::callback_type&   Callback Type (pre/post-write type)
		 * @param   cci::cnf::cci_base_param*  Other cci_base_param within the selected cci_base_param vector passed by top_module
		 * @param   conv_fact  Conversion factor b/w the other cci_base_param values (in order to synchronize)
		 * @return  cci::cnf::callback_return_type Callback return type
		 */
		cci::cnf::callback_return_type
			write_callback(cci::cnf::cci_base_param & _base_param_1,\
											const cci::cnf::callback_type& cb_reason,\
											cci::cnf::cci_base_param * _base_param_2, float conv_fact )
			{
				// Decision on Pre-Write & Post-Write callbacks
				std::cout << "\t[PARAM_VALUE_SYNC - post_write callback] : Parameter Name : "
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
		 * @fn        float multiplyWithConversionFactor(std::string, std::string )
	 	 * @brief     Function computing the conversion factor to be multiplied
	 	 *            with the 'main_clk_Hz' parameter of the PARAM_VALUE_SYNC while
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

		}// End of multiplyWithConversionFactor 
			
		/**
		 * @fn        void synchValuesWithCF (cci::cnf::cci_base_param*, cci::cnf::cci_base_param*, float)
 		 * @brief     Function for synchronizing the values of cci_parameter of OWNER modules via the PARAM_VALUE_SYNC
 		 * @param     _input_param Reference of Integer type CCI parameter
 		 * @param     _out_param   Reference of cci_base_param pointers to the selected owner parameters
		 * @return    void 
 		 */	
		void synchValuesWithCF (cci::cnf::cci_base_param * _base_param_1, cci::cnf::cci_base_param * _base_param_2, float conv_fact)
		{
			/// In order to synchronize even the default values of the owner modules,
			/// use cci_base_param of one parameter as reference, write the same value
			/// (using CF) to the other pararmeter's cci_base_param using
			/// JSON serialize/deserialize APIs manually
			float CF = multiplyWithConversionFactor(_base_param_1->get_name(), _base_param_2->get_name());
			float freq = atof((_base_param_1->json_serialize()).c_str());      
      float operand1 = freq * CF;
			std::stringstream ss;
			ss.clear();
			ss.str("");
			ss << operand1;
			 _base_param_2->json_deserialize(ss.str());
			
			post_write_cb_vec.push_back(_base_param_1->register_callback(cci::cnf::post_write,\
				this, cci::bind(&param_value_sync::write_callback, this, _1, _2,_base_param_2,conv_fact)) );

			post_write_cb_vec.push_back(_base_param_2->register_callback(cci::cnf::post_write,\
				this, cci::bind(&param_value_sync::write_callback, this, _1, _2, _base_param_1,(1.0/conv_fact))) );

		}// End of synchValuesWithCF


	private	:
	
	// Declaring a CCI configuration broker interface instance
	cci::cnf::cci_originator     ValueSyncOriginator; //!< CCI Originator object

	// Declaring a CCI configuration broker interface instance
	cci::cnf::cci_cnf_broker_if* ValueSyncBrokerIF;   //!< CCI configuration broker instance

	// Callback Adaptor Objects
	std::vector<cci::shared_ptr<cci::cnf::callb_adapt> > post_write_cb_vec; //!< 'post_write' callback adaptor object

	/** std::vector storing the searched owner parameters references to CCI base parameter pointers*/
	std::vector<cci::cnf::cci_base_param*> returnBaseParamList; //!< Searched and selected base parameters list passed by the top_module

};// End of Class

#endif	// End of PARAM_VALUE_SYNC_H

