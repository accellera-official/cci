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

/**
 * @brief    The configurator class illustrates ways of searching parameters in a model using different patterns 
 * @author   P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date     3rd June, 2011 (Friday)
 */ 
class parameter_configurator : public ::sc_core::sc_module
{
	public:
		
		/// Default constructor
		SC_CTOR(parameter_configurator)
		{
			// Get handle of the broker responsible for the class/module
			myCfgrBrokerIF	=	&cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));

			// Report if handle returned is NULL
			assert(myCfgrBrokerIF != NULL && "Configuration Broker handle is NULL");

			/// Registering SC_THREAD with the SystemC kernel
			SC_THREAD(run_cfgr);

		}// End of Constructor	

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
				std::string cfgr_param_str	=	"top_mod.main_clk_Hz";

				if(myCfgrBrokerIF->exists_param(cfgr_param_str))
				{
					cfgr_param_ptr	=	myCfgrBrokerIF->get_param(cfgr_param_str);

					assert(cfgr_param_str	!= NULL && "Configuration parameter returned is NULL");
				}
				else
					std::cout << "\t[CFGR] : No such parameter exists in top_module";

				wait(5.0, SC_NS);				
				
				if(cfgr_param_ptr != NULL)
				{
					std::cout << "\n@ " << sc_time_stamp() << std::endl;
					std::cout << "\t[CFGR] : Changing the 'main_clk_Hz' of TOP_MODULE to 15000 (Hz)." << std::endl;
					cfgr_param_ptr->json_deserialize("15000");
				}				

				wait(50.0, sc_core::SC_NS);	

			}// End of WHILE

		}// End of SC_THREAD


	private	:
	
	/// Declaring a CCI configuration broker interface instance
	cci::cnf::cci_cnf_broker_if* myCfgrBrokerIF;

	/// CCI base parameter
	cci::cnf::cci_base_param* cfgr_param_ptr;

};// End of Class/SC_MODULE

#endif	// End of PARAMETER_CONFIGURATOR_H

