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
 * @date     3rd June, 2011 (Friday)
 */
#ifndef PARAMETER_CONFIGURATOR_H
#define PARAMETER_CONFIGURATOR_H

/// Include the "cci.h" header file in all cci-based applications
#include <cci.h>
#include <assert.h>
#include <vector>

#include "parameter_owner.h"

/**
 * @brief    The configurator class illustrates ways of searching parameters in a model using different patterns 
 * @author   P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date     3rd June, 2011 (Friday)
 */ 
class parameter_configurator : public sc_module
{
	public:
		
		/// Default constructor
		SC_CTOR(parameter_configurator)
		{
			// Get handle of the broker responsible for the class/module
			myCfgrBrokerIF	=	&cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));

			/// Report if handle returned is NULL
			assert(myCfgrBrokerIF != NULL && "Configuration Broker handle is NULL");

			/// Registering callback for the creation of the pointer to the cci_parameter of the owner module
			cfgr_create_param_cb = myCfgrBrokerIF->register_callback(cci::cnf::create_param,
			                                       "param_owner.int_param", this, cci::bind(&parameter_configurator::create_callback, this, _1, _2));
			cfgr_pre_read_param_cb = myCfgrBrokerIF->register_callback(cci::cnf::pre_read,
			                                       "param_owner.int_param", this, cci::bind(&parameter_configurator::read_callback, this, _1, _2));
			cfgr_pre_write_param_cb = myCfgrBrokerIF->register_callback(cci::cnf::pre_write,
			                                       "param_owner.int_param", this, cci::bind(&parameter_configurator::write_callback, this, _1, _2));
			cfgr_post_write_param_cb = myCfgrBrokerIF->register_callback(cci::cnf::post_write,
			                                       "param_owner.int_param", this, cci::bind(&parameter_configurator::write_callback, this, _1, _2));
			cfgr_destroy_param_cb = myCfgrBrokerIF->register_callback(cci::cnf::destroy_param,
			                                       "param_owner.int_param", this, cci::bind(&parameter_configurator::destroy_callback, this, _1, _2));
			
			/// Create pointer to the owner module
			param_owner	=	new parameter_owner("param_owner");

			/// Registering SC_THREAD with the SystemC kernel
			SC_THREAD(run_cfgr);

		}// End of Constructor	

		cci::cnf::callback_return_type
			create_callback(cci::cnf::cci_base_param & _base_param, const cci::cnf::callback_type & cb_reason)
			{
				switch(cb_reason)
				{
					case cci::cnf::create_param	:	{
						std::cout <<	"\n\t[CFGR - create_param_cb] : Parameter Name : " << _base_param.get_name() << "\thas been created." << std::endl;
						std::cout <<	"\n\t[CFGR - create_param_cb] : Parameter Value : " << _base_param.json_serialize() << std::endl;
						break;	}

					default :
						std::cout << "\t\n[CFGR - create_param_cb] : Invalid callback reason" << std::endl;
				}

				return cci::cnf::return_nothing;
			}

		cci::cnf::callback_return_type
			read_callback(cci::cnf::cci_base_param & _base_param, const cci::cnf::callback_type & cb_reason)
			{
				switch(cb_reason)
				{
					case cci::cnf::pre_read	:	{
						std::cout << "\n\t[CFGR - pre_read_cb] : Parammeter Name : " << _base_param.get_name() << "\tvalue will be read." << std::endl;
						break;	}

					default :
						std::cout << "\n\t[CFGR - read_param_cb] : Invalid callback reason" << std::endl;
				}
				return cci::cnf::return_nothing;
			}

		cci::cnf::callback_return_type
			write_callback(cci::cnf::cci_base_param & _base_param, const cci::cnf::callback_type & cb_reason)
			{
				switch(cb_reason)
				{
					case cci::cnf::pre_write	:	{
						std::cout	<< "\n\t[CFGR - pre_write_cb] : Parameter Name : " << _base_param.get_name()
							<< "\tParameter Value : " << _base_param.json_serialize() << std::endl;
						break;	}
				
					case cci::cnf::post_write	:	{
						std::cout << "\n\t[CFGR - post_write_cb] : Parameter Name : " << _base_param.get_name()
							<< "\tParameter Value : " << _base_param.json_serialize() << std::endl;
						break;	}

					default :
						std::cout << "\n\t[CFGR - write_param_cb] : Invalid callback reason" << std::endl;
				}
				return cci::cnf::return_nothing;
			}

		cci::cnf::callback_return_type
			destroy_callback(cci::cnf::cci_base_param & _base_param, const cci::cnf::callback_type & cb_reason)
			{
				switch(cb_reason)
				{
					case cci::cnf::destroy_param	:	{
						std::cout << "\n\t[CFGR - destroy_param_cb] : Parameter Name : "
						<< _base_param.get_name() << "\thas been destroyed." << std::endl;
					break;	}
			
					default	:
						std::cout << "\n\t[CFGR - destroy_param_cb] : Invalid callback reason" << std::endl;
				}
				return cci::cnf::return_nothing;
			}

		/**
		  * @brief      This process illustrates use of the cci_param (shared parameter)
		  *             that stores the reference of the owner cci-parameter to make changes
		  *             to the parameter's value
		  * @param      void
		  * @return     void
		  */ 	
		void run_cfgr (void)
		{
			while(1)
			{

				wait(5.0, sc_core::SC_NS);	

			}// End of WHILE

		}// End of SC_THREAD


	private	:
	
	/// Declaring a CCI configuration broker interface instance
	cci::cnf::cci_cnf_broker_if* myCfgrBrokerIF;

	/// Pointer to the owner module
	parameter_owner*          param_owner;

	/// Registering all the various callbacks of the owner module cci_parameters
	cci::shared_ptr<cci::cnf::callb_adapt>    cfgr_create_param_cb;
	cci::shared_ptr<cci::cnf::callb_adapt>    cfgr_pre_read_param_cb;
	cci::shared_ptr<cci::cnf::callb_adapt>    cfgr_pre_write_param_cb;
	cci::shared_ptr<cci::cnf::callb_adapt>    cfgr_post_write_param_cb;
	cci::shared_ptr<cci::cnf::callb_adapt>    cfgr_destroy_param_cb;

};// End of Class/SC_MODULE

#endif	// End of PARAMETER_CONFIGURATOR_H

