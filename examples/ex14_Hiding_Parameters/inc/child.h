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
 * @file      child.h
 * @brief     This file declares and implements the 'child' module. The
 *            'child' module is instantiated by the 'parent' module and the
 *            parent module hides the private cci-parameter of the 'child'
 *            details from the external world.
 * @author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * @date      2nd September, 2011 (Saturday)
 */
#ifndef CHILD_H
#define CHILD_H

/// Include the 'cci.h' header in all cci-based applications
#include <cci.h>
#include <assert.h>
#include <string.h>


/**
 * @brief      The 'child' module registers - public & private cci-parameters to
 *             the broker responsible for the module
 */
class child : public sc_module
{
	public:
		
		/// Default Constructor		
		SC_CTOR(child)
		:	priv_int_param("priv_int_param", 100, cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this)))
		,	pub_int_param("pub_int_param", 150, cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this)))
		{
			// This returns the reference to the broker responsible for this module
			child_BrokerIF	=	&cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));
			
			assert(child_BrokerIF != NULL && "Returned broker handle of 'child' module is NULL");

			std::cout << "\n\t[CHILD C_TOR] : Is Private Broker? : " << std::boolalpha
				<< child_BrokerIF->is_private_broker() << std::endl;

			std::cout << "\n\t[CHILD C_TOR] : Parameter Name   : " << priv_int_param.get_name()
				<< "\tParameter Value : " << priv_int_param.get() << std::endl; 

			std::cout << "\n\t[CHILD C_TOR] : Parameter Name   : " << pub_int_param.get_name()
				<< "\tParameter Value : " << pub_int_param.get() << std::endl; 

			// Declare a SC_THREAD
			SC_THREAD(run_child);

		}// End of constructor


		/// Implementation of the SC_THREAD
		void run_child (void)
		{
			// List of cci_parameters directly visible to the outside world
			std::vector<std::string> child_param_list = child_BrokerIF->get_param_list();

			while(1)
			{
				std::cout << "\n@ " << sc_time_stamp() << "\tVisible parameters to 'child' module" << std::endl;

				for(unsigned int i = 0; i < child_param_list.size(); i++)
				{
					std::cout << "\t[CHILD] : Parameter Name : " << child_param_list[i] << std::endl; 
				}

				wait(25.0, SC_NS);

				std::cout << "\n@ " << sc_time_stamp() << std::endl;

				std::cout << "\t[CHILD] : Parameter Name : " << priv_int_param.get_name()
					<< "\tParameter Value : " << priv_int_param.get() << std::endl;
			
				wait(10.0, SC_NS);

			}// End of WHILE

		}// End of SC_THREAD


	private	:
	
		/// Declare a configuration broker
		cci::cnf::cci_cnf_broker_if*	child_BrokerIF;

		/// Declare instances of mutable CCI parameters of type 'int'
		cci::cnf::cci_param<int>	  priv_int_param;   /*!To be registered to the private broker*/
		cci::cnf::cci_param<int>	  pub_int_param;   /*!To be registered to the global broker*/

};// End of SC_MODULE

#endif // End of CHILD_H
