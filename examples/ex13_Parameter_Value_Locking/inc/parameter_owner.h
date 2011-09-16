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
 * @file      parameter_owner.h
 * @brief     This file defines the sc_module which is the owner class of a
 *            cci- parameter
 * @author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * @date      10th May, 2011 (Tuesday)
 */
#ifndef PARAMETER_OWNER_H
#define PARAMETER_OWNER_H

/// Include the 'cci.h' header in all cci-based applications
#include <cci.h>


/**
 * @class      parameter_owner parameter_owner.h
 * @brief      This sc_module declares an integer parameter and also illustrates
 *             usage of CCI infrastructure in locking and unlocking of the parameter
 */
SC_MODULE(parameter_owner)
{
	public	:
		
		/** Default Constructor*/
		SC_CTOR(parameter_owner)
		:	int_param("mutable_int_param", 0)
		{
			std::cout << "\n\t[OWNER C_TOR] : Parameter locking status : " << int_param.locked() << "\tValue : " << int_param.get() << endl;

			/// SC_THREAD declaration
			SC_THREAD(run_owner);
	
		}// End of constructor

		
		/**
		 * @fn     	parameter_owner::~parameter_owner
		 * @brief   Destructor
		 */
		~parameter_owner()
		{

		}


		/**
		 * @fn            void run_owner (void)
		 * @brief         This process demonstrates usages of various locking and unlocking APIs
		 */
		void run_owner (void)
		{
			while(1)
			{
				std::cout << "\n@ " << sc_time_stamp() << endl;

				std::cout << "\t[OWNER] : Locking the parameter without password" << endl;
				int_param.lock();
				std::cout << "\t[OWNER] : Parameter locking status : " << int_param.locked() << "\tValue : " << int_param.get() << endl;

				/// Illustrating usage of try and catch mechanism in order to catch the exception
				/// raised when trying to set a value to the parameter if it is already locked
				try	{
					std::cout << "\t[OWNER] : Try to set parameter value to 1 from 0" << endl;
					int_param = 1;
				}	catch	(sc_core::sc_report e)	{
						std::cout << "\t[OWNER] : " << name() << "\tFailed to set parameter value to 1 from 0." << endl;
						std::cout << "\t[OWNER] : Caught Exception : " << e.what() << endl;
				}// End of TRY-CATCH
					
				std::cout << "\t[OWNER] : Parameter locking status : " << int_param.locked() << "\tValue : " << int_param.get() << endl;
				
				wait(5.0, SC_NS);	// wait till the configurator executes 'wait'
		
				std::cout << "\n@ " << sc_time_stamp() << endl;
				std::cout << "\t[OWNER] : Unlocking parameter (locked earlier) without password" << endl;
				int_param.unlock();
				std::cout << "\t[OWNER] : Parameter locking status : " << int_param.locked() << "\tValue : " << int_param.get() << endl;
				
				wait(5.0, SC_NS);

				std::cout << "\n@ " << sc_time_stamp() << endl;
				std::cout << "\t[OWNER] : Parameter locking status : " << int_param.locked() << "\tValue : " << int_param.get() << endl;
				key = NULL;
				std::cout << "\n\t[OWNER] : Directly lock the unlocked parameter with password" << endl;
				int_param.lock(&key); 				
	
				std::cout << "\t[OWNER] : Parameter locking status : " << int_param.locked() << "\tValue : " << int_param.get() << endl;

				/// This 'set' API llustrating assigning new value to an already locked parameter without unlocking it
				std::cout << "\n\t[OWNER] : Overwrite value of the (locked!) parameter to 4 without unlocking" << endl;
				int_param.set(4, &key); 				
				std::cout << "\t[OWNER] : Parameter locking status : " << int_param.locked() << "\tValue : " << int_param.get() << endl;
				
				wait(30.0, SC_NS);

			}// End of WHILE	

		}// End of SC_THREAD


	private	:
	
		// CCI parameter
		cci::cnf::cci_param<int>	int_param;  //!< Integer type cci-parameter
		
		void* key;		//!< Arbitrary password to lock the parameter with

};// End of SC_MODULE

#endif // End of PARAMETER_OWNER_H
