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
 * @brief    This file declares and defines configurator and implements the
 *           locking and unlocking mechnanisms on the owner parameter 
 * @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * @date     10th May, 2011 (Tuesday)
 */
#ifndef PARAMETER_CONFIGURATOR_H
#define PARAMETER_CONFIGURATOR_H

/// Include the "cci.h" header file in all cci-based applications
#include <cci.h>
#include <assert.h>

/**
 * @class    parameter_configurator parameter_configurator.h
 * @brief    This class lists and defines the locking and unlocking mechnanisms
 *           by a parameter-setter(configurator)
 */ 
class parameter_configurator : public sc_module
{
	public	:
		
		/** Default constructor*/
		SC_CTOR(parameter_configurator)
		{
			/// Getting handle of the default broker for the class/sc_module
			myConfigBroker = &cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));
			
			// Assert if broker handle returned is NULL
			assert(myConfigBroker != NULL && "Broker handle returned is NULL");

			if(myConfigBroker->exists_param("param_owner.mutable_int_param"))
			{	
				// Getting handle for the integer parameter of onwer module by the configurator
				int_param_ptr = myConfigBroker->get_param("param_owner.mutable_int_param");
				
				assert(int_param_ptr != NULL && "Base parameter handle returned NULL");
			}
			else
				std::cout << "\n\t[CFGR C_TOR] : int_param not found" << std::endl; 
			
			/// Registering SC_THREAD with the SystemC kernel
			SC_THREAD(run_mutable_cfgr);

		}// End of Constructor	


		/**
		 * @fn     parameter_configurator::~parameter_configurator
		 * @brief  Destructor
		 */
		~parameter_configurator()
		{

		}


		/**
		 * @fn         void run_mutable_cfgr (void)
		 * @brief      This process illustrates various locking and unlocking mechanisms
		 *             of a parameter using 'lock()', 'lock(&password)'. 'unlock()',
		 *             'unlock(&password)' APIs.  'locked()' API is used to query the 
		 *             locking status of the parameter
		 */ 	
		void run_mutable_cfgr (void)
		{
			while(1)
			{
				wait(2.0, SC_NS);			// wait for some time

				/// Before configurator tries to assign a new value to the parameter, it
				/// is always recommended to check the locking status of the parameter	
				std::cout << "\n@ " << sc_time_stamp() << std::endl;
				std::cout << "\t[CFGR] : Parameter locking status : " << int_param_ptr->locked() << "\tValue : " << int_param_ptr->json_serialize() << std::endl;
				
				/// 'locked' API returns TRUE when a parameter is in LOCKED STATE and FALSE when in UNLOCKED STATE
				if(int_param_ptr->locked())
					std::cout << "\t[CFGR] :  Cannot assign new value to the parameter as it is already locked!!" << endl;
				else
					int_param_ptr->json_deserialize("2");
				std::cout << "\t[CFGR] : Parameter locking status : " << int_param_ptr->locked() << "\tValue : " << int_param_ptr->json_serialize() << std::endl;

				wait(5.0, SC_NS);
				
				std::cout << "\n@ " << sc_time_stamp() << std::endl;
				std::cout << "\t[CFGR] : Parameter locking status : " << int_param_ptr->locked() << "\tValue : " << int_param_ptr->json_serialize() << std::endl;
				std::cout << "\t[CFGR] : Parameter is not locked!! Assign a new value '3' to it" << std::endl;	
				int_param_ptr->json_deserialize("3");
				std::cout << "\t[CFGR] : Parameter locking status : " << int_param_ptr->locked() << "\tValue : " << int_param_ptr->json_serialize() << std::endl;


				/// Demonstrating 'lock' API to lock a parameter without a password
				std::cout << "\n\t[CFGR] : Lock parameter without password" << std::endl;	
				int_param_ptr->lock();
				std::cout << "\t[CFGR] : Parameter locking status : " << int_param_ptr->locked() << "\tValue : " << int_param_ptr->json_serialize() << std::endl;
				
				/// Demonstrating 'unlock' API to lock a parameter without a password
				std::cout << "\n\t[CFGR] : Unlock parameter without password" << std::endl;
				int_param_ptr->unlock();				
				std::cout << "\t[CFGR] : Parameter locking status : " << int_param_ptr->locked() << "\tValue : " << int_param_ptr->json_serialize() << std::endl;


				/// Demonstrating 'lock' API to lock a parameter with a password
				// 'Lock' API returns TRUE if parameter is locked successfully and FALSE if it could not be locked
				psswd = NULL; 
				std::cout << "\n\t[CFGR] : Lock with password" << std::endl;
	
				if(int_param_ptr->lock(&psswd))
					std::cout << "\t[CFGR] : Parameter locked with password (password not known to OWNER)" << std::endl;
				else
					std::cout << "\t[CFGR] : Could not lock the parameter." << std::endl;	

				std::cout << "\t[CFGR] : Parameter locking status : " << int_param_ptr->locked() << "\tValue : " << int_param_ptr->json_serialize() << std::endl;


				try	{
					std::cout << "\t[CFGR] : Set parameter value to '4'" << endl;
					int_param_ptr->json_deserialize("4");
				}	catch	(sc_core::sc_report exception)	{
						std::cout << "\t[CFGR] : Caught Exception : " << exception.what() << endl;	}

				std::cout << "\t[CFGR] : Parameter locking status : " << int_param_ptr->locked() << "\tValue : " << int_param_ptr->json_serialize() << std::endl;

				// 'Unlock' API returns TRUE if parameter is unlocked successfully and FALSE if it could not be unlocked
				/// Demonstrating unlocking with the right password
				std::cout << "\n\t[CFGR] : Now, unlock with the correct password" << std::endl;
				int_param_ptr->unlock(&psswd);
				std::cout << "\t[CFGR] : Parameter locking status : " << int_param_ptr->locked() << "\tValue : " << int_param_ptr->json_serialize() << std::endl;

				wait(50.0, SC_NS);	

			}// End of WHILE

		}// End of SC_THREAD


	private	:
	
		// CCI configuration broker interface instance
		cci::cnf::cci_cnf_broker_if* myConfigBroker;  //!< CCI Configuration broker instance

		// CCI base parameter handle to access the actual owner's parameter
		cci::cnf::cci_base_param*     int_param_ptr;  //!< cci_base_param to store reference of 'int' type cci-param

		char* paramName;
	
		/// Some passwords local to the configurator
		void* psswd;
		void* newpsswd;

};// End of class (SC_MODULE)

#endif	// End of PARAMETER_CONFIGURATOR_H

