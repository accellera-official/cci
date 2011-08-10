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
 * @file      processor.h
 * @brief     This header defines limited 'processor' module functionality
 * @author    P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date      4th August, 2011 (Thursday)
 */
#ifndef PROCESSOR_H
#define PROCESSOR_H

/// Include the 'cci.h' header in all cci-based applications
#include <cci.h>

#include <assert.h>
#include <math.h>
#include <string.h>

#include "address_lines_register.h"
#include "address_holding_register.h"

/**
 * @brief      The 'processor' module register's callbacks on the references of the
 *             cci-parameters of the two register modules and does few checking and
 *             comparisons
 * @author     P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date       4th August, 2011 (Thursday)
 */
SC_MODULE(processor)
{
	public:

		/// Default Constructor		
		SC_CTOR(processor)
		: addr_lines_module("addr_lines_mod")
		, addr_holding_module("addr_holding_mod")
		{
			processor_BrokerIF	=	&cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));

			assert(processor_BrokerIF != NULL && "Broker's Handle for the 'processor' module is returned NULL");

			// Get handle of the 'no_of_addr_lines' cci-parameter of 'address_lines_register'
			if(processor_BrokerIF != NULL)
			{
				std::string param_path(name());
				param_path.append(".addr_lines_mod.curr_addr_lines");

				if(processor_BrokerIF->exists_param(param_path))
				{
					addr_lines_base_ptr	=	processor_BrokerIF->get_param(param_path);

					assert(addr_lines_base_ptr != NULL && "Returned handle of 'no_of_addr_lines' cci-parameter is NULL");
				}
				else
					std::cout << "\n\t[PROCESSOR C_TOR] : Parameter " << param_path << "\tdoesn't not exists" << std::endl;

			}// End of IF


			// Get handle of the 'addr_value' cci-parameter of 'address_holding_register'
			if(processor_BrokerIF != NULL)
			{
				std::string param_path(name());
				param_path.append(".addr_holding_mod.curr_addr_pos");

				if(processor_BrokerIF->exists_param(param_path))
				{
					addr_value_base_ptr	=	processor_BrokerIF->get_param(param_path);

					assert(addr_value_base_ptr != NULL && "Returned handle of 'no_of_addr_lines' cci-parameter is NULL");
				}
				else
					std::cout << "\n\t[PROCESSOR C_TOR] : Parameter " << param_path << "\tdoesn't not exists" << std::endl;

			}// End of IF


			/// Checks for the condition whether the default total number of the address lines can address the default address location
			total_addr_lines = atoi(addr_lines_base_ptr->json_serialize().c_str());
      curr_addr_pos  = atoi(addr_value_base_ptr->json_serialize().c_str());
			compareFunction(total_addr_lines, curr_addr_pos);
			

			/// Registering 'POST_WRITE' callbacks on the cci-parameters of the two register modules
			addr_lines_post_wr_cb	  =	addr_lines_base_ptr->register_callback(cci::cnf::post_write, this,
			                                cci::bind(&processor::addr_lines_post_wr_cb_func, this, _1, _2, addr_value_base_ptr));
			addr_value_post_wr_cb	  =	addr_value_base_ptr->register_callback(cci::cnf::post_write, this,
			                                cci::bind(&processor::addr_value_post_wr_cb_func, this, _1, _2, addr_lines_base_ptr));

		}// End of constructor

		
		// Implementation of the callback function - 'addr_lines_post_wr_cb_func'
		cci::cnf::callback_return_type
			addr_lines_post_wr_cb_func(cci::cnf::cci_base_param & _base_param,
			 const cci::cnf::callback_type & cb_reason, cci::cnf::cci_base_param * _addr_value_base_ptr)
			{
				std::cout << "\n\t[PROCESSOR addr_lines_post_wr_cb] : Parameter Name : " << _base_param.get_name()
					<< "\tParameter Value : " << _base_param.json_serialize() << std::endl;

				std::cout << "\t[PROCESSOR addr_lines_post_wr_cb] : Parameter Name : " << _addr_value_base_ptr->get_name()
					<< "\tParameter Value : " << _addr_value_base_ptr->json_serialize() << std::endl;

				total_addr_lines = atoi(_base_param.json_serialize().c_str());
				curr_addr_pos  = atoi(_addr_value_base_ptr->json_serialize().c_str());

				compareFunction(total_addr_lines, curr_addr_pos);

				return cci::cnf::return_nothing;

			}// End of Callback
		

		// Implementation of the callback function - 'addr_value_post_wr_cb_func'
		cci::cnf::callback_return_type
			addr_value_post_wr_cb_func(cci::cnf::cci_base_param & _base_param,
			 const cci::cnf::callback_type & cb_reason, cci::cnf::cci_base_param * _addr_lines_base_ptr)
			{
				std::cout << "\n\t[PROCESSOR addr_value_post_wr_cb] : Parameter Name : " << _base_param.get_name()
					<< "\tParameter Value : " << _base_param.json_serialize() << std::endl;

				std::cout << "\t[PROCESSOR addr_value_post_wr_cb] : Parameter Name : " << _addr_lines_base_ptr->get_name()
					<< "\tParameter Value : " << _addr_lines_base_ptr->json_serialize() << std::endl;

				curr_addr_pos = atoi(_base_param.json_serialize().c_str());
				total_addr_lines  = atoi(_addr_lines_base_ptr->json_serialize().c_str());

				compareFunction(total_addr_lines, curr_addr_pos);

				return cci::cnf::return_nothing;

			}// End of Callback


		void compareFunction(int lines, int addr_value)
		{
			if(addr_value < (pow(2, lines)-1) )
				std::cout << "\t[PROCESSOR fn] : The number of address lines can address the current address position" << std::endl;
			else
				std::cout << "\t[PROCESSOR fn] : The address lines cannot address the current address position" << std::endl;

		}// End of Function - 'compareFunction'

	
	private	:
	
		/// Declare parameter_owner instance
		address_lines_register                       addr_lines_module;
		address_holding_register                     addr_holding_module;
		
		/// Configuration broker instance
		cci::cnf::cci_cnf_broker_if*                 processor_BrokerIF;

		int total_addr_lines;
		int curr_addr_pos;

		/// CCI Base parameter pointer
		cci::cnf::cci_base_param*                    addr_lines_base_ptr;
		cci::cnf::cci_base_param*                    addr_value_base_ptr;

		/// Callbacks
		cci::shared_ptr<cci::cnf::callb_adapt_b>     addr_lines_post_wr_cb;
		cci::shared_ptr<cci::cnf::callb_adapt_b>     addr_value_post_wr_cb;

};// End of SC_MODULE

#endif // End of PROCESSOR_H
