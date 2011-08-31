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
 * @file     parent.h
 * @brief    This header file declares and defines the 'parent' which instantiates
 *           'child' and hides its parameters
 * @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *           Girish Verma, CircuitSutra Technologies   <girish@circuitsutra.com>
 * @date     5th June, 2011 (Tuesday)
 */
#ifndef PARENT_H
#define PARENT_H

/// Include the "cci.h" header file in all cci-based applications
#include <cci.h>
#include <assert.h>
#include <string>
#include <vector>

#include "child.h"

#include "gs_cci_cnf_private_broker_accessor.h"

/**
 * @brief    The 'parent' module derives from the 'cci_broker_manager'
 *           class and manages it own private broker stack
 */ 
class parent : public sc_module
{
	public:
		
		SC_HAS_PROCESS(parent);	
	
		/// Default constructor
		parent(sc_core::sc_module_name _name, cci::cnf::cci_broker_manager priv_broker)
		: sc_core::sc_module(_name)
	  ,	child_inst("child_inst")
		, parent_BrokerIF(priv_broker)
		,	parent_int_param("parent_int_param", 300, *priv_broker)
		, parent_buffer("parent_int_buffer", 350, *priv_broker)
		{
			// Asserts if the returned broker handle is NULL
			assert(parent_BrokerIF != NULL && "Returned broker handle for 'parent' is NULL");

			std::cout << "\n\t[PARENT C_TOR] : Parameter Name : " << parent_int_param.get_name()
				<< "\tParameter Value : " << parent_int_param.get() << std::endl;			


			if(parent_BrokerIF != NULL)
			{
				std::string child_param_path(name());
				child_param_path.append(".child_inst.priv_int_param");

				if(parent_BrokerIF->exists_param(child_param_path))
				{
					child_base_param_ptr = parent_BrokerIF->get_param(child_param_path);

					assert(child_base_param_ptr != NULL && "Returned broker handle for 'priv_int_param' of 'child' is NULL");
	
					// Register 'POST_WRITE' callback to change child's cci-parameter 
					// Configurators writes to 'parent_buffer' cci-parameter (registered to the default global broker)
					// Changes to the 'parent_buffer' will be reflected on to the 'priv_int_param' of child as well 
					parent_post_write_cb = parent_buffer.register_callback(cci::cnf::post_write,\
        																		this, cci::bind(&parent::write_callback, this, _1, _2, child_base_param_ptr)) ;
				}
				else
					std::cout << "\n\t[PARENT C_TOR] : Desired cci-parameter of 'child' module is not available" << std::endl;

			}// End of IF


			// Declaration of SC_THREAD
			SC_THREAD(run_parent);

			std::cout << "\n\t[PARENT C_TOR] : Parameter Name : " << parent_buffer.get_name()\
				<< "\tParameter Value : " << parent_buffer.get() << std::endl;			

		}// End of Constructor

		
		/// Callback Implementation
		cci::cnf::callback_return_type
			write_callback(cci::cnf::cci_base_param & _base_param, const cci::cnf::callback_type & cb_reason, cci::cnf::cci_base_param * _child_base_param_ptr)
			{
				_child_base_param_ptr->json_deserialize(_base_param.json_serialize());
				
				std::cout << "\n\t[PARENT - post_write_cb] : Parameter Name : " << _base_param.get_name()\
					<< "\tParameter Value : " << _base_param.json_serialize() << std::endl;
			
				return cci::cnf::return_nothing;
			}


		/// SC_THREAD Implementation
		void run_parent (void)
		{
			while(1)
			{
				wait(5.0, SC_NS);

				std::vector<std::string> parent_param_list = parent_BrokerIF->get_param_list();

				std::cout << "\n@ " << sc_time_stamp() << "\tVisible parameters to the 'parent' module" << std::endl;

				for(unsigned int i = 0; i < parent_param_list.size(); i++)
				{
					std::cout << "\t[PARENT] : Parameter Name : " << parent_param_list[i] << std::endl;
				}

				wait(30.0, SC_NS);

			}// End of WHILE

		}// End of SC_THREAD


	private	:

		/// Owner Module instantiation
		child                        child_inst;

		/// Configuration Broker for TOP_MODULE
		cci::cnf::cci_cnf_broker_if* parent_BrokerIF;
	
		/// Declare cci-parameters (registered with the private broker)
		cci::cnf::cci_param<int>	   parent_int_param;

		/// Globally registered and visible cci-parameters
		cci::cnf::cci_param<int>     parent_buffer;

		/// Declare cci_base_param pointers
    cci::cnf::cci_base_param*    child_base_param_ptr;

		/// Callback Adaptor Object
		cci::shared_ptr<cci::cnf::callb_adapt_b> parent_post_write_cb;

};// End of Class

#endif	// End of PARENT_H

