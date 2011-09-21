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

/*!
 * \file     parent.h
 * \brief    This header file declares and defines the 'parent' which instantiates
 *           'child' and hides private parameters
 * \author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *           Girish Verma, CircuitSutra Technologies   <girish@circuitsutra.com>
 * \date     3rd September, 2011 (Saturday)
 */
#ifndef PARENT_H
#define PARENT_H

#include <cci.h>  // Include the "cci.h" header file in all cci-based applications
#include <cci.h>
#include <assert.h>
#include <string>
#include <vector>

#include "child.h"


/*!
 * \class    parent parent.h
 * \brief    The 'parent' module derives from the 'cci_broker_manager'
 *           class and manages it own private broker stack
 */ 
class parent : public sc_module
{
	public:
		
		SC_HAS_PROCESS(parent);	
	
		/*!
		 * \fn      parent::parent(sc_core::sc_module_name, cci::cnf::cci_broker_manager)
		 * \brief   Default Constructor
		 * \param   sc_core::sc_module_name      Name of the sc_module
		 * \param   cci::cnf::cci_broker_manager Private broker (instantiated) immediately above the parent (within Top module)
		 */
		parent(sc_core::sc_module_name _name, cci::cnf::cci_broker_manager priv_broker)
		: sc_core::sc_module(_name)
		, parent_BrokerIF(priv_broker)
	  ,	child_inst("child_inst")
			/// \par Registering 'parameters' to the broker
			/// \par
			/// <cci-parameter_instance>("<parameter_name>", <default_value>, <broker_inst>)
			/// \par
			///  Here, <broker_inst> is given the reference of the broker declared immediately above PARENT which
			///  specifies the list of publicly visible parameters
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
	
					/// Register 'POST_WRITE' callback to change child's cci-parameter 
					/// Configurators writes to 'parent_buffer' cci-parameter (registered to the default global broker)
					/// Changes to the 'parent_buffer' will be reflected on to the 'priv_int_param' of child as well 
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

		
		/*!
		 * \fn      parent::~parent
		 * \brief   Destructor
		 */
		~parent()
		{

		}


		/*!
		 * \fn      cci::cnf::callback_return_type write_callback(cci::cnf::cci_base_param&, const cci::cnf::callback_type&, cci::cnf::cci_base_param*)
		 * \brief   'POST_WRITE' Callback Implementation
		 * \param   cci::cnf::cci_base_param&   Reference of the selected cci_base_param ('parent_int_buffer' of PARENT)
		 * \param   cci::cnf::callback_type&    Callback type
		 * \param   cci::cnf::cci_base_param*   Reference of the cci_base_param of 'priv_int_param' of the CHILD module
		 * \return  cci::cnf::callback_return_type  Callback Return Type
		 */
		cci::cnf::callback_return_type
			write_callback(cci::cnf::cci_base_param & _base_param, const cci::cnf::callback_type & cb_reason, cci::cnf::cci_base_param * _child_base_param_ptr)
			{
				_child_base_param_ptr->json_deserialize(_base_param.json_serialize());
				
				std::cout << "\n\t[PARENT - post_write_cb] : Parameter Name : " << _base_param.get_name()\
					<< "\tParameter Value : " << _base_param.json_serialize() << std::endl;
			
				return cci::cnf::return_nothing;

			}// End of Callback Implementation


		/*!
		 * \fn     void run_parent (void)
		 * \brief  SC_THREAD Implementation
		 */
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

		// Configuration Broker for TOP_MODULE
		cci::cnf::cci_cnf_broker_if* parent_BrokerIF;  //!< Configuration broker instance

		// Owner Module instantiation
		child                        child_inst;  //!<  Child module instance
	
		// Declare cci-parameters (registered with the private broker)
		cci::cnf::cci_param<int>	   parent_int_param; //!< cci-param to be registered to the PRIVATE BROKER
		cci::cnf::cci_param<int>     parent_buffer;  //!< Globally registered and visible cci-parameters

		/// Declare cci_base_param pointers
    cci::cnf::cci_base_param*    child_base_param_ptr; //!< cci_base_param storing reference of CHILD's private parameter

		// Callback Adaptor Object
		cci::shared_ptr<cci::cnf::callb_adapt> parent_post_write_cb; //!< POST_WRITE callback adaptor object

};// End of Class

#endif	// End of PARENT_H

