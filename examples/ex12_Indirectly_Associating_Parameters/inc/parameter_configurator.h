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
 * @brief    This file takes the references of the owner's parameters using 
 *           cci_base_param and sets values to the owner's parameters at various
 *           timing points 
 * @author   P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date     9th June, 2011 (Thursday)
 */
#ifndef PARAMETER_CONFIGURATOR_H
#define PARAMETER_CONFIGURATOR_H

/// Include the "cci.h" header file in all cci-based applications
#include <cci.h>
#include <assert.h>
#include <vector>

/**
 * @brief    The configurator class searches for the owner parameters using the
 *           get_param API.  'get_param_list' API implementation is resulting in warnings,
 *           though the parameters are being searched as per the string pattern provided.
 *           Hence, 'get_param' API has been adopted as of now 
 * @author   P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date     9th June, 2011 (Thursday)
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

			/// Hierarchical names for the cci_parameters of the owner modules
			std::string cfgr_param_str1	=	"top_mod.param_owner1.clk_freq_Hz";
			std::string cfgr_param_str2	=	"top_mod.param_owner2.clock_speed_KHz";

			/// Check for the existence of 'clk_freq_Hz' cci_parameter of owner module 1
			if(myCfgrBrokerIF->exists_param(cfgr_param_str1))
			{
				cfgr_param_ptr1	=	myCfgrBrokerIF->get_param(cfgr_param_str1);

				assert(cfgr_param_str1	!= NULL && "Configuration parameter returned is NULL");

				std::cout << "\n\t[CFGR C_TOR] : Parameter Name : " << cfgr_param_ptr1->get_name()	\
					<< "\tParameter Value : " << cfgr_param_ptr1->json_serialize() << std::endl; 
			}
			else
				std::cout << "\t[CFGR] : Parameter " << cfgr_param_str1 << "\tdoesn't exists in top_module" << std::endl;

			/// Check for the existence of 'clock_speed_Hz' cci_parameter of owner module 2
			if(myCfgrBrokerIF->exists_param(cfgr_param_str2))
			{
				cfgr_param_ptr2	=	myCfgrBrokerIF->get_param(cfgr_param_str2);

				assert(cfgr_param_str2	!= NULL && "Configuration parameter returned is NULL");

				std::cout << "\n\t[CFGR C_TOR] : Parameter Name : " << cfgr_param_ptr2->get_name()	\
					<< "\tParameter Value : " << cfgr_param_ptr2->json_serialize() << std::endl; 
			}
			else
				std::cout << "\t[CFGR] : Parameter " << cfgr_param_str1 << "\tdoesn't exists in top_module" << std::endl;
				
			/// Registering SC_THREAD with the SystemC kernel
			SC_THREAD(run_cfgr);

		}// End of Constructor	

		/**
		  * @brief    Within this 'before_end_of_elaboration' callback phase, the
		  *           value of the cci_parameter of owner(1) is modified and the
		  *           (updated) values of both the owners are read
		  * @param    void
		  * @return   void
		  */
		void before_end_of_elaboration (void)
		{
			/// Change the value of the cci_parameter 'clk_freq_Hz' of OWNER (1) to '5000' (Hz)
			if(cfgr_param_ptr1 != NULL)
			{
				std::cout << "\n[CFGR within beoe] Within the BEOE phase" << std::endl;
				std::cout << "\t[CFGR within beoe] : Changing the 'clk_freq_Hz' of OWNER (1) to 5000 (Hz)." << std::endl;
				cfgr_param_ptr1->json_deserialize("5000");
			}				
				
			std::cout << "\n\t[CFGR within beoe] : Parameter Name : " << cfgr_param_ptr1->get_name()
				<< "\tParameter Value : " << cfgr_param_ptr1->json_serialize() << std::endl;

			std::cout << "\t[CFGR within beoe] : Parameter Name : " << cfgr_param_ptr2->get_name()
				<< "\tParameter Value : " << cfgr_param_ptr2->json_serialize() << std::endl;

		}// End of 'before_end_of_elaboration' callback


		/**
		  * @brief      Within this sc_process, the value of the cci_parameter of owner(2)
		  *             is modified and the (updated) values of both the owners are read
		  *             parameters
		  * @param      void
		  * @return     void
		  */ 	
		void run_cfgr (void)
		{
			while(1)
			{
				/// Change the value of the cci_parameter 'clock_speed_KHz' of OWNER (2) to '12' (KHz)
				if(cfgr_param_ptr2 != NULL)
				{
					std::cout << "\n@ " << sc_time_stamp() << std::endl;
					std::cout << "\t[CFGR] : Changing the 'clock_speed_KHz' of OWNER (2) to 12 (KHz)." << std::endl;
					cfgr_param_ptr2->json_deserialize("12.00");
				}				

				std::cout << "\n\t[CFGR] : Parameter Name : " << cfgr_param_ptr1->get_name()
					<< "\tParameter Value : " << cfgr_param_ptr1->json_serialize() << std::endl;

				std::cout << "\t[CFGR] : Parameter Name : " << cfgr_param_ptr2->get_name()
					<< "\tParameter Value : " << cfgr_param_ptr2->json_serialize() << std::endl;

				wait(50.0, sc_core::SC_NS);	

			}// End of WHILE

		}// End of SC_THREAD


	private	:
	
	/// Declaring a CCI configuration broker interface instance
	cci::cnf::cci_cnf_broker_if* myCfgrBrokerIF;

	/// CCI base parameters
	cci::cnf::cci_base_param* cfgr_param_ptr1;
	cci::cnf::cci_base_param* cfgr_param_ptr2;

};// End of Class/SC_MODULE

#endif	// End of PARAMETER_CONFIGURATOR_H

