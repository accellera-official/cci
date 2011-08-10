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
 * @file     configurator.h
 * @brief    This header implements the functionality of the configurator
 * @author   P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date     12th July, 2011 (Tuesday)
 */
#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

/// Include the "cci.h" header file in all cci-based applications
#include <cci.h>
#include <assert.h>

/**
 * @brief    This module gets the references to all the cci-parameters within
 *           the model and writes values to it.
 * @author   P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date     4th August, 2011 (Thursday)
 */ 
class configurator : public sc_module
{
	public:
		
		/// Default constructor
		SC_CTOR(configurator)
		{
			// Get handle of the broker responsible for the class/module
			myCfgrBrokerIF	=	&cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));

			// Report if handle returned is NULL
			assert(myCfgrBrokerIF != NULL && "Configuration Broker handle is NULL");


			/// Hierarchical names for the cci_parameters of the owner modules	
			std::string cfgr_param_str2	=	"processor.addr_lines_mod.curr_addr_lines";
			std::string cfgr_param_str3	=	"processor.addr_holding_mod.curr_addr_pos";


			/// Check for the existence of 'curr_addr_lines' cci_parameter of ADDRESS_LINES_REGISTER
			if(myCfgrBrokerIF->exists_param(cfgr_param_str2))
			{
				addr_lines_base_ptr	=	myCfgrBrokerIF->get_param(cfgr_param_str2);

				assert(addr_lines_base_ptr	!= NULL && "Handle of 'curr_addr_lines' parameter returned is NULL");
			}
			else
				std::cout << "\t[CFGR C_TOR] : Parameter " << cfgr_param_str2 << "\tdoesn't exists" << std::endl;


			/// Check for the existence of 'curr_addr_pos' cci_parameter of ADDRESS_HOLDING_REGISTER
			if(myCfgrBrokerIF->exists_param(cfgr_param_str3))
			{
				addr_value_base_ptr	=	myCfgrBrokerIF->get_param(cfgr_param_str3);

				assert(addr_value_base_ptr	!= NULL && "Handle of 'curr_addr_pos' parameter returned is NULL");
			}
			else
				std::cout << "\t[CFGR C_TOR] : Parameter " << cfgr_param_str3 << "\tdoesn't exists" << std::endl;


			/// Registering SC_THREAD with the SystemC kernel
			SC_THREAD(run_cfgr);

		}// End of Constructor	


		/// SC_THREAD Implementation
		void run_cfgr (void)
		{
			while(1)
			{
				std::cout << "\n@ " << sc_time_stamp() << std::endl;

				std::cout << "\t[CFGR] : Modify the 'curr_addr_lines' to 11" << std::endl;
				addr_lines_base_ptr->json_deserialize("11");

				wait(5.0, SC_NS);	
	
				std::cout << "\n@ " << sc_time_stamp() << std::endl;
			
				std::cout << "\t[CFGR] : Changing the 'curr_addr_pos' to 2196" << std::endl;
				addr_value_base_ptr->json_deserialize("2196");
		
				wait(5.0, SC_NS);

			}// End of WHILE

		}// End of SC_THREAD


	private	:
	
	/// Declaring a CCI configuration broker interface instance
	cci::cnf::cci_cnf_broker_if* myCfgrBrokerIF;

	/// CCI base parameters
	cci::cnf::cci_base_param* proc_mem_base_ptr;
	cci::cnf::cci_base_param* addr_lines_base_ptr;
	cci::cnf::cci_base_param* addr_value_base_ptr;

};// End of Class/SC_MODULE

#endif	// End of CONFIGURATOR_H

