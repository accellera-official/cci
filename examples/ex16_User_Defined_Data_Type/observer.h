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
  * @file    observer.h
  * @brief   This file defines an observer class demonstrates
  * @author  P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
  *          Girish Verma, CircuitSutra Technologies   <girish@circuitsutra.com>
  * @date    12th September, 2011 (Monday)
  */
#ifndef OBSERVER_H
#define OBSERVER_H

/// Include 'cci' header in all CCI-based applications
#include <cci>
#include <assert.h>
#include <vector>

/**
  * @brief   This observer class registers all types of callbacks on the 
  *          cci-parameter values of interest in order to find the originator
  *          for the appropriate actions on the respective cci-parameters
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
				std::cout << "\n\t[OBSERVER C_TOR] : Broker Type : " << observerBrokerIF->name() << endl;
			else
				std::cout << "\n\t[OBSERVER C_TOR] : Broker Type : " << observerBrokerIF->name() << " - is not a private broker." << endl; 


			/// Gets the reference to the 'udt' type cci-parameter of OWNER module
			obsv_udt_base_ptr = observerBrokerIF->get_param("param_owner.User_data_type_param");

			assert(obsv_udt_base_ptr != NULL && "Returned Handle of 'integer type' cci-parameter is NULL");

			/// Observer registering 'PRE_READ', 'PRE_WRITE' & 'POST_WRITE' callbacks
			/// on the UDT parameter to monitor all actions on it
			udt_pre_read_cb	=	obsv_udt_base_ptr->register_callback(cci::cnf::pre_read, this,
			                                           cci::bind(&observer::read_callback, this, _1, _2));
			udt_pre_write_cb	=	obsv_udt_base_ptr->register_callback(cci::cnf::pre_write, this,
			                                           cci::bind(&observer::write_callbacks, this, _1, _2));
			udt_post_write_cb	=	obsv_udt_base_ptr->register_callback(cci::cnf::post_write, this,
			                                           cci::bind(&observer::write_callbacks, this, _1, _2));
	
		}// End of Constructor


		/// Destructor
		~observer()
		{
			// Destructor does nothing
		}

		
		/// 'PRE_READ' Callbacks Implementations
		cci::cnf::callback_return_type
			read_callback (cci::cnf::cci_base_param & _selected_base_param, const cci::cnf::callback_type & cb_reason)
			{
				switch(cb_reason)
				{
					case cci::cnf::pre_read	:	{
						const cci::cnf::cci_originator* myOriginator = cci::cnf::cci_originator::get_global_originator();

		        std::cout << "\n\t[OBSERVER pre_read_cb] :  Parameter Name : " << _selected_base_param.get_name()	\
    		      <<  "\tOriginator info : " << myOriginator->name() << std::endl;
				
						break;	}
			
					default	:
						std::cout << "\n\t[OBSERVER pre_read_cb] - Unknown Callback Type" << std::endl;

				}// End of SWITCH-CASE

       	return cci::cnf::return_nothing;

			}// End of read_callback


		/// 'PRE_WRITE' & 'POST_WRITE' Callbacks Implementations
		cci::cnf::callback_return_type
			write_callbacks	(cci::cnf::cci_base_param & _selected_base_param, const cci::cnf::callback_type & cb_reason)
			{			
				const cci::cnf::cci_originator* myOriginator = cci::cnf::cci_originator::get_global_originator();
				
				switch(cb_reason)
				{
					case	cci::cnf::pre_write	:	{
						std::cout << "\n\t[OBSERVER pre_write_cb] :  Parameter Name : " << _selected_base_param.get_name()	\
          		<<  "\tOriginator info : " << myOriginator->name() << std::endl;
						break;}

					case	cci::cnf::post_write	:	{
						std::cout << "\n\t[OBSERVER post_write_cb] :  Parameter Name : " << _selected_base_param.get_name()	\
          		<<  "\tOriginator info : " << myOriginator->name() << std::endl;
						break;}
									
					default	:
						std::cout << "\n\t[OBSERVER write_cb] - Unknown Callback Type" << std::endl;

				}// End of SWITCH-CASE

        return cci::cnf::return_nothing;

			}// End of write_callbacks


	private	:

		/// CCI configuration broker instance
		cci::cnf::cci_cnf_broker_if*            observerBrokerIF;

		/// Declare cci_base_param for int type cci-parameter
		cci::cnf::cci_base_param*               obsv_udt_base_ptr;

		/// Callback Adaptor Objects for 'int' type parameter
		cci::shared_ptr<cci::cnf::callb_adapt>    udt_pre_read_cb;
		cci::shared_ptr<cci::cnf::callb_adapt>    udt_pre_write_cb;
		cci::shared_ptr<cci::cnf::callb_adapt>    udt_post_write_cb;

};// End of Class

#endif	// End of OBSERVER_H
