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
#include "memory_block.h"

/**
 * @brief      The 'processor' module register's callbacks on the references of the
 *             cci-parameters of the two register module and the memory stack module,
 *             and does few checkings and comparisons
 * @author     P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date       4th August, 2011 (Thursday)
 */
SC_MODULE(processor)
{
	public:

		/// Default Constructor		
		SC_CTOR(processor)
		: addr_lines_module("addr_lines_mod")
		, memory_block_module("memory_block")
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


			// Get handle of the 'mem_size' cci-parameter of 'memory_block'
			if(processor_BrokerIF != NULL)
			{
				std::string param_path(name());
				param_path.append(".memory_block.mem_size");

				if(processor_BrokerIF->exists_param(param_path))
				{
					mem_size_base_ptr	=	processor_BrokerIF->get_param(param_path);

					assert(mem_size_base_ptr != NULL && "Returned handle of 'memory_block_size' cci-parameter is NULL");
				}
				else
					std::cout << "\n\t[PROCESSOR C_TOR] : Parameter " << param_path << "\tdoesn't not exists" << std::endl;

			}// End of IF


			/// Checks for the condition whether the default total number of the address lines can address the default address location
			total_addr_lines = atoi(addr_lines_base_ptr->json_serialize().c_str());
      mem_block_size  = atoi(mem_size_base_ptr->json_serialize().c_str());
			compareFunction(total_addr_lines, mem_block_size);
			

			/// Registering 'POST_WRITE' callbacks on the cci-parameters of the two register modules
			addr_lines_post_wr_cb	  =	addr_lines_base_ptr->register_callback(cci::cnf::post_write, this,
			                                cci::bind(&processor::addr_lines_post_wr_cb_func, this, _1, _2, mem_size_base_ptr));
			mem_block_post_wr_cb	  =	mem_size_base_ptr->register_callback(cci::cnf::post_write, this,
			                                cci::bind(&processor::mem_block_post_wr_cb_func, this, _1, _2, addr_lines_base_ptr));

		}// End of constructor

		
		// Implementation of the callback function - 'addr_lines_post_wr_cb_func'
		cci::cnf::callback_return_type
			addr_lines_post_wr_cb_func(cci::cnf::cci_base_param & _base_param,
			 const cci::cnf::callback_type & cb_reason, cci::cnf::cci_base_param * _mem_size_base_ptr)
			{
				std::cout << "\n\t[PROCESSOR addr_lines_post_wr_cb] : Parameter Name : " << _base_param.get_name()
					<< "\tParameter Value : " << _base_param.json_serialize() << std::endl;

				std::cout << "\t[PROCESSOR addr_lines_post_wr_cb] : Parameter Name : " << _mem_size_base_ptr->get_name()
					<< "\tParameter Value : " << _mem_size_base_ptr->json_serialize() << std::endl;

				total_addr_lines = atoi(_base_param.json_serialize().c_str());
				mem_block_size  = atoi(_mem_size_base_ptr->json_serialize().c_str());

				compareFunction(total_addr_lines, mem_block_size);

				return cci::cnf::return_nothing;

			}// End of Callback
		

		// Implementation of the callback function - 'mem_block_post_wr_cb_func'
		cci::cnf::callback_return_type
			mem_block_post_wr_cb_func(cci::cnf::cci_base_param & _base_param,
			 const cci::cnf::callback_type & cb_reason, cci::cnf::cci_base_param * _addr_lines_base_ptr)
			{
				std::cout << "\n\t[PROCESSOR mem_block_post_wr_cb] : Parameter Name : " << _base_param.get_name()
					<< "\tParameter Value : " << _base_param.json_serialize() << std::endl;

				std::cout << "\t[PROCESSOR mem_block_post_wr_cb] : Parameter Name : " << _addr_lines_base_ptr->get_name()
					<< "\tParameter Value : " << _addr_lines_base_ptr->json_serialize() << std::endl;

				mem_block_size = atoi(_base_param.json_serialize().c_str());
				total_addr_lines  = atoi(_addr_lines_base_ptr->json_serialize().c_str());

				compareFunction(total_addr_lines, mem_block_size);

				return cci::cnf::return_nothing;

			}// End of Callback


		void compareFunction(int lines, int addr_value)
		{
			static int check = 0;

			if(addr_value < (pow(2, lines)-1) )
			{
				if(check == 0)
					std::cout << "\t[PROCESSOR fn] : User may proceed with the present configuration" << std::endl;
				else
					std::cout << "\t[PROCESSOR fn] : The number of address lines can now address the current memory size" << std::endl;	

				check = 0;
			}
			else
			{
				std::cout << "\t[PROCESSOR fn] : The address lines cannot address the current memory size" << std::endl;
				
				check = 1;
			}
		}// End of Function - 'compareFunction'

	
	private	:
	
		/// Declare parameter_owner instance
		address_lines_register                       addr_lines_module;
		memory_block                                 memory_block_module;
		
		/// Configuration broker instance
		cci::cnf::cci_cnf_broker_if*                 processor_BrokerIF;

		int total_addr_lines;
		int mem_block_size;

		/// CCI Base parameter pointer
		cci::cnf::cci_base_param*                    addr_lines_base_ptr;
		cci::cnf::cci_base_param*                    mem_size_base_ptr;

		/// Callbacks
		cci::shared_ptr<cci::cnf::callb_adapt_b>     addr_lines_post_wr_cb;
		cci::shared_ptr<cci::cnf::callb_adapt_b>     mem_block_post_wr_cb;

};// End of SC_MODULE

#endif // End of PROCESSOR_H
