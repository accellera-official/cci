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
 * @author   Girish Verma, CircuitSutra Technologies     <girish@circuitsutra.com>
 *           P V S Phaneendra, CircuitSutra Technologies <pvs@circuitsutra.com>
 * @date     14th December, 2011
 *           16th May, 2011 (Monday)
 */
#ifndef PARAMETER_CONFIGURATOR_H
#define PARAMETER_CONFIGURATOR_H

/// Include the "cci.h" header file in all cci-based applications
#include <cci.h>

#include <assert.h>

/**
 * @brief    The configurator class illustrates the different ways in which the
 *           value of a cci-parameter of the owner class can be accessed using 
 *           both a cci_base_parameter object and a cci_param object (by making the
 *           configurator a friend class to the owner) 
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
			assert(myCfgrBrokerIF != NULL && "Parameter-Setter handle is NULL");

			// Checks the parameter exists using name-based look-up 
			if(myCfgrBrokerIF->exists_param("param_owner.mutable_int_param"))
			{
				std::cout << "[CFGR C_TOR] : Parameter param_owner.mutable_int_param exists" << std::endl; 
				
				// Get handle of the owner parameter
				cfgr_param_ptr	=	myCfgrBrokerIF->get_param("param_owner.mutable_int_param");
			
				// Assert if the owner parameter handle returned is NULL
				assert(cfgr_param_ptr != NULL && "Parameter Handle for param_owner.mutable_int_param is NULL");
			}		
			else
				std::cout << "[CFGR C_TOR] : Parameter param_owner.mutable_int_param doesn't exists." << std::endl;

			if(myCfgrBrokerIF->exists_param("param_owner.mutable_udtype_param"))
			{
				std::cout << "[CFGR C_TOR] : Parameter param_owner.mutable_udtype_param  exists" << std::endl; 
				
				// Get handle of the owner parameter
				cfgr_user_param_ptr	=	myCfgrBrokerIF->get_param("param_owner.mutable_udtype_param");
			
				// Assert if the owner parameter handle returned is NULL
				assert(cfgr_param_ptr != NULL && "Parameter param_owner.mutable_udtype_param  Handle is NULL");
				
				std::cout<<"[CFGR C_TOR] : Got param_owner.mutable_udtype_param "<< std::endl;

			}		
			else {
				std::cout << "[CFGR C_TOR] : Parameter param_owner.mutable_udtype_param doesn't exists." << std::endl;
				std::cout << "Try Again with mutable_udtype_param "<< std::endl;

				if(myCfgrBrokerIF->exists_param("mutable_udtype_param")) {
					std::cout <<"Got Now with mutable_udtype_param "<<std::endl;
				}
				else {

					std::cout <<"Try again now directly with param_owner.mutable_udtype_param " << std::endl;
					cfgr_user_param_ptr     =       myCfgrBrokerIF->get_param("param_owner.mutable_udtype_param");

					if ( cfgr_user_param_ptr == NULL ) {

						std::cout << "\tFail to get directly with param_owner.mutable_udtype_param " << std::endl;

					  std::cout << "\tTry again now directly with mutable_udtype_param " << std::endl;
						cfgr_user_param_ptr = myCfgrBrokerIF->get_param("mutable_udtype_param");
					}

					if ( cfgr_user_param_ptr == NULL )
						std::cout << "\tIt will always Fail :(" << std::endl;
				}
			}

			// Set parameter value using cci_base_parameter object
			std::cout << "\n\t[CFGR C_TOR] : Set parameter value to 10 using cci_base_parameter" << std::endl;
			cfgr_param_ptr->json_deserialize("10");
	

			/// Registering SC_THREAD with the SystemC kernel
			SC_THREAD(run_mutable_cfgr);

		}// End of Constructor	

		/**
		  * @brief      This process illustrates use of the cci_param (shared parameter)
		  *             that stores the reference of the owner cci-parameter to make changes
		  *             to the parameter's value
		  * @param[in]  void
		  * @param[out] void
		  */ 	
		void run_mutable_cfgr (void)
		{
			while(1)
			{
				wait(10.0, sc_core::SC_NS);
			
				std::cout << "\n@ " << sc_core::sc_time_stamp() << std::endl;
				std::cout << "\t[CFGR] : Parameter Value   : " << cfgr_shared_param->get()	<< std::endl; 
				if(cfgr_user_param_ptr != NULL ) {
				std::cout << "\t[CFGR] : Get :Parameter Value[User data type]   : " << cfgr_user_param_ptr->json_serialize()	<< std::endl; 

				cfgr_user_param_ptr->json_deserialize("EXP");
				//std::cout << "\t[CFGR] : Set :Parameter Value[User data type] 10,20,2  : " << cfgr_user_param_ptr->json_deserialize("EXP")	<< std::endl; 
				std::cout << "\t[CFGR] : Get :Parameter Value[User data type]   : " << cfgr_user_param_ptr->json_serialize()	<< std::endl; 

				}
				
				wait(5.0, sc_core::SC_NS);

				std::cout << "\n@ " << sc_core::sc_time_stamp() << std::endl;
				std::cout << "\n\t[CFGR] : Change value to 20" << std::endl;
				*cfgr_shared_param = 20;
				
				wait(50.0, sc_core::SC_NS);	
			}// End of WHILE

		}// End of SC_THREAD


		/**
 		  *  @brief     This parameter gets the reference of the 'int_param', a
 		  *             private-type cci-parameter member of the owner class
 		  *  @see       ../src/main.cpp
 		  *  @param[in] pointer of the owner 'parameter_owner' class
 		  *  @return    void
 		  */
  	void set_cfgr_parameter(parameter_owner *owner)
		{
			// Getting reference of the owner parameter
			cfgr_shared_param = &owner->int_param; 
		}

	private	:
	
	/// Declaring a CCI configuration broker interface instance
	cci::cnf::cci_cnf_broker_if* myCfgrBrokerIF;

	/// Declaring a CCI base parameter pointer
	cci::cnf::cci_base_param* cfgr_param_ptr;
	cci::cnf::cci_base_param* cfgr_user_param_ptr;

  /// Declaring a CCI parameter pointer (which will hold the reference of the 
  /// owner CCI parameter 'int_param'
	cci::cnf::cci_param<int>* cfgr_shared_param;

};// End of Class/SC_MODULE

#endif	// End of PARAMETER_CONFIGURATOR_H

