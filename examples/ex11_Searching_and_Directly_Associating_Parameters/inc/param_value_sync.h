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

#include <cci.h>   //!< Include the "cci.h" header file in all cci-based applications
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
		
		/// Overloaded constructor // No default constructor
		param_value_sync(std::vector<cci::cnf::cci_base_param * > BaseParamList) 

			/// Define an originator for the class in order to get hold of the default broker
		:	ValueSyncOriginator("ValueSyncOriginator")
		{
			/// Get handle of the broker responsible for the class/module
			ValueSyncBrokerIF	=	&cci::cnf::cci_broker_manager::get_current_broker(ValueSyncOriginator);

			/// Copy the list of selected base parameters to a local std::vector
			returnBaseParamList =  BaseParamList;
	
			
			for(unsigned int i = 1; i < returnBaseParamList.size(); i++)	{
				synchValues(returnBaseParamList[0], returnBaseParamList[i]);
			}// End of FOR
		
	}// End of Constructor

		
	/**
	 * @fn     cci::cnf::callback_return_type write_callback(cci::cnf::cci_base_param &, const cci::cnf::callback_type, cci::cnf::cci_base_param)
	 * @brief  'PRE_WRITE' and 'POST-WRITE' Callbacks Implementation
	 * @param  cci::cnf::cci_base_param&  Reference of the cci_base_param of the selected cci-parameter
	 * @param  cci::cnf::callback_type&   Callback Type (pre/post-write type)
	 * @param  cci::cnf::cci_base_param*  Other selected cci_base_param's (to be synchronized) with \a cci::cnf::cci_base_param&
	 * @return cci::cnf::callback_return_type  Callback return type
	 */
	cci::cnf::callback_return_type       
		write_callback(cci::cnf::cci_base_param & _base_param_1,\
										const cci::cnf::callback_type& cb_reason,\
										cci::cnf::cci_base_param * _base_param_2)
		{
			// Post-Write callbacks
			std::cout << "\t[PARAM_VALUE_SYNC - post_write callback] : Parameter Name : "
				<< _base_param_1.get_name() << "\tValue : " << _base_param_1.json_serialize() << std::endl;

			 _base_param_2->json_deserialize(_base_param_1.json_serialize());
					
			return cci::cnf::return_nothing;

		}// End of Write Callbacks

		
		/** 
		  * @fn        void synchValues (cci::cnf::cci_base_param*, cci::cnf::cci_base_param*)
 		  * @brief     Function for synchronizing the values of cci_parameter of OWNER modules via the PARAM_VALUE_SYNC
 		  * @param     _input_param Reference of Integer type CCI parameter
 		  * @param     _out_param   Reference of cci_base_param pointers to the selected owner parameters
		  * @return    void 
 		  */	
		void synchValues (cci::cnf::cci_base_param * _base_param_1, cci::cnf::cci_base_param * _base_param_2)
		{
			/// In order to synchronize even the default values of the owner modules,
			/// use cci_base_param of one parameter as reference, write the same value
			/// to the other pararmeter's cci_base_param using JSON serialize/deserialize APIs
			 _base_param_2->json_deserialize(_base_param_1->json_serialize());
			
			post_write_cb_vec.push_back(_base_param_1->register_callback(cci::cnf::post_write,\
				this, cci::bind(&param_value_sync::write_callback, this, _1, _2,_base_param_2)) );

			post_write_cb_vec.push_back(_base_param_2->register_callback(cci::cnf::post_write,\
				this, cci::bind(&param_value_sync::write_callback, this, _1, _2, _base_param_1)) );
		}// End of synchValues function


	private	:
	
	// Declaring a CCI originator
	cci::cnf::cci_originator     ValueSyncOriginator; //!< cci-originator instance

	// Declaring a CCI configuration broker interface instance
	cci::cnf::cci_cnf_broker_if* ValueSyncBrokerIF;   //!< CCI configuration broker interface instance

	// Callback Adaptor Objects
	std::vector<cci::shared_ptr<cci::cnf::callb_adapt_b> > post_write_cb_vec;  //!< post_write callback adaptor object

	// std::vector storing the searched owner parameters references to CCI base parameter pointers
	std::vector<cci::cnf::cci_base_param*> returnBaseParamList; //!< STD::VECTOR for cci_base_params

};// End of Class

#endif	// End of PARAM_VALUE_SYNC_H

