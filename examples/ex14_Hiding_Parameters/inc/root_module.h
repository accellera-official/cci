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
 * @file     root_module.h
 * @brief    This header declares and defines the 'root_module' which instantiates
 *           parent modules, namely, 'parent' and 'parent_2'
 * @author   P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date     5th June, 2011 (Tuesday)
 */
#ifndef ROOT_MODULE_H
#define ROOT_MODULE_H

/// Include the "cci.h" header file in all cci-based applications
#include <cci.h>
#include <assert.h>
#include <vector>

#include "parent.h"

/**
 * @brief    The 'root_module' module instantiates the parent module
 *           and a cci-parameter which is exposed to the outside world.  It
 *           is the modifications occurring on this parameter which will be
 *           reflected on the 'parent.parent_buffer' cci-parameter.  
 * @author   P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date     5th July, 2011 (Tuesday)
 */ 
class root_module : public sc_module
{
	public:
		
		/// Default constructor
		SC_CTOR(root_module)
		:	parent_inst("parent_inst")
		,	root_module_int_param("root_int_param", 500)
		{
			/// Gets the reference of a DEFAULT GLOBAL BROKER
			root_module_BrokerIF	=	&cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));

			// Asserts if the returned broker handle is NULL
			assert(root_module_BrokerIF != NULL && "Returned broker handle for 'root_module' is NULL");

			std::cout << "\n\t[ROOT_MODULE C_TOR] : Parameter Name : " << root_module_int_param.get_name()
				<< "\tParameter Value : " << root_module_int_param.get() << std::endl;


			if(root_module_BrokerIF != NULL)
			{
				if(root_module_BrokerIF->exists_param("root_module.parent_inst.parent_int_buffer"))
				{
					parent_base_param_ptr = root_module_BrokerIF->get_param("root_module.parent_inst.parent_int_buffer");

					assert(root_module_BrokerIF != NULL && "Returned handle of 'parent_int_buffer' is NULL");
				}
				else
					std::cout << "\n\t[ROOT_MODULE C_TOR] : Parameter 'root_module.parent_inst.parent_int_buffer' not available" << std::endl;

			}//End of IF

			
			/// Registering Callback
			root_post_write_cb = root_module_int_param.register_callback(cci::cnf::post_write,
				                     this, cci::bind(&root_module::write_callback, this, _1, _2, parent_base_param_ptr));


			/// SC_THREAD Declaration
			SC_THREAD(run_root_module);

		}// End of Constructor

		
		/// Implementation of callback
		cci::cnf::callback_return_type
			write_callback(cci::cnf::cci_base_param & _base_param, const cci::cnf::callback_type & cb_reason, cci::cnf::cci_base_param * _parent_base_param)
			{
				std::cout << "\n\t[ROOT_MODULE - post_write_cb] : Parameter Name : " << _base_param.get_name()
					<< "\tParameter Value : " << _base_param.json_serialize() << std::endl;

				_parent_base_param->json_deserialize(_base_param.json_serialize());

				std::cout << "\n\t[ROOT_MODULE - post_write_cb] : Parameter Name : " << _parent_base_param->get_name()
					<< "\tParameter Value : " << _parent_base_param->json_serialize() << std::endl;

				return cci::cnf::return_nothing;
			}


		/// SC_THREAD Implementation
		void run_root_module (void)
		{
			while(1)
			{
				std::vector<std::string> root_module_param_list = root_module_BrokerIF->get_param_list();

				wait(10.0, SC_NS);

				std::cout << "\n@ " << sc_time_stamp() << "\tVisible parameters to 'root_module'" << std::endl;

				for(unsigned int i = 0; i < root_module_param_list.size(); i++)
				{
					std::cout << "\t[ROOT_MODULE] : Parameter Name : " << root_module_param_list[i] << std::endl;
				}

				wait(20.0, SC_NS);
			}
		}


	private	:

		/// Parent Module instantiation
		parent                      parent_inst;

		/// Configuration Broker for ROOT_MODULE
		cci::cnf::cci_cnf_broker_if* root_module_BrokerIF;
	
		/// Declare cci-parameters
		cci::cnf::cci_param<int>	   root_module_int_param;

		/// Create pointers of cci_base_param
		cci::cnf::cci_base_param*	   parent_base_param_ptr;

		/// Callback Adaptor Object
		cci::shared_ptr<cci::cnf::callb_adapt_b>  root_post_write_cb;

};// End of Class

#endif	// End of ROOT_MODULE_H

