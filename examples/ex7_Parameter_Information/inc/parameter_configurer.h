/*******************************************************************************
 *   The following code is derived, directly or indirectly, from the SystemC
 *   source code Copyright (c) 1996-2010 by all Contributors.
 *   All Rights reserved.
 *
 *   The contents of this file are subject to the restrictions and limitations
 *   set forth in the SystemC Open Source License Version 2.2.0 (the "License");
 *   One may not use this file except in compliance with such restrictions and
 *   limitations.  One may obtain instructions on how to receive a copy of the
 *   License at http://www.systemc.org/.  Software distributed by Contributors
 *   under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 *   ANY KIND, either express or implied. See the License for the specific
 *   language governing rights and limitations under the License.
 *******************************************************************************/

/**
  * @file    parameter_configurer.h
  * @brief   This file defines a sc_module which demonstrates means to access
  *          cci-parameters by name-based look up access approach and set their
  *          various attributes
  * @author  P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
  * @date    18th April, 2011 (Monday) : 17:43 hrs IST
  */
#ifndef PARAMETER_CONFIGURER_H
#define PARAMETER_CONFIGURER_H

/// Include 'cci.h' header in all CCI-based applications
#include <cci.h>
#include <assert.h>

/**
  * @brief   This sc_module instantiates a cci configuration broker which accesses
  *          the references of the cci-parameters and sets their attributes like 
  *          value, documentation, etc. 
  */
SC_MODULE(parameter_configurer)
{
	public	:

		/// Default constructor
		SC_CTOR(parameter_configurer)
		:	check(0)
		{
			/// Get the broker responsible for this module using 'get_current_broker' API
			myBrokerInterface = &cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));

			// Assert if broker handle returned is NULL
			assert(myBrokerInterface != NULL && "Broker Handle Returned is NULL");


			/// Check for the broker type (default or private) using 'is_private_broker()' API
			if(myBrokerInterface->is_private_broker())
				/// Access broker's name using 'name()' 
				std::cout << "\n\t[CFGR] : Broker Type : " << myBrokerInterface->name() << endl;
			else
				std::cout << "\n\t[CFGR] : " << myBrokerInterface->name() << " is not a private broker." << endl; 


			int_param_str = "param_owner.mutable_int_param";
			string_param_str = "param_owner.mutable_string_param";
			
			intParamExists = false;
			strParamExists = false;	
		
	
			/// Broker interface checks for existance of a (std::string type) parameter using 'exists_param' API
			if(myBrokerInterface->exists_param(string_param_str))
			{
				/// If parameter exists, get handle of the parameter using 'get_param' API
				str_param_ptr = myBrokerInterface->get_param(string_param_str);		
					
				// Report if parameter handle is returned NULL
				assert(str_param_ptr != NULL && "String CCI Parameter Handle returned NULL");
									
				strParamExists = true;
			}
			else
			{
				std::cout << "\n\t[CFGR] : String parameter does not exist" << endl;
					
				strParamExists = false;
			} // End of IF


			// Broker interface checks for the existance of a (int type) parameter
			if(myBrokerInterface->exists_param(int_param_str))
			{
				int_param_ptr =	myBrokerInterface->get_param(int_param_str);
		
				assert(int_param_ptr != NULL && "Integer CCI Parameter Handle returned NULL");					
				
				intParamExists = true;
			}
			else
			{
				std::cout << "\n\t[CFGR] : Integer parameter does not exist." << endl;
				
				intParamExists = false;
			} // End of IF


 			/// Registering SC_THREAD process 
			SC_THREAD(run_accessor);
		}

		/// Destructor
		~parameter_configurer()
		{
			// Destructor does nothing
		}

		/// Within SC_THREAD process definition
		void run_accessor(void)
		{
			while(1) 
			{
				if(strParamExists && intParamExists)
				{
					std::cout << "\n@ " << sc_time_stamp() << "\tdemonstrating 'is_default_value()'" << endl; 
					
					/// Access parameter's default value status using 'is_default_value()' API
					if(int_param_ptr->is_default_value())
						std::cout << "\n\t[CFGR] : " << int_param_ptr->get_name() << " default value hasn't been modified." << endl; 
					else
						std::cout << "\n\t[CFGR] : " << int_param_ptr->get_name() << " default value has been modified." << endl; 
					
					wait(2.0, SC_NS);				
	
					std::cout << "\n@ " << sc_time_stamp() << "\tdemonstrating 'is_default_value()'" << endl;
				
	
					// Access parameter's default value status using 'is_default_value()' API
					if(!int_param_ptr->is_default_value())
						std::cout << "\n\t[CFGR] : " << int_param_ptr->get_name() << " value has been modified." << endl; 


					if(str_param_ptr->is_default_value())
					{
						std::cout << "\n\t[CFGR] : " << str_param_ptr->get_name() << " value hasn't been modified" << endl;			
						std::cout << "\n\t[CFGR -> Retrieve] : Parameter name  : " << str_param_ptr->get_name() << endl;
						std::cout << "\n\t[CFGR -> Retrieve] : Parameter value : " << str_param_ptr->json_serialize() << endl;			
					}
					else
						std::cout << "\n\t[CFGR] : " << str_param_ptr->get_name() << " New Value : "
							<< str_param_ptr->json_serialize()<< endl; 


					wait(2.0, SC_NS);
	
					std::cout << "\n@ " << sc_time_stamp() << "\tdemonstrating 'is_invalid_value()'" << endl;
					

					/// Check parameter's value validity using 'is_invalid_value()' API
					if(int_param_ptr->is_invalid_value())
						std::cout << "\n\t[CFGR] : Invalid parameter value." << endl;
					else
					{
						std::cout << "\n\t[CFGR] : Valid parameter value." << endl;

						// Access parameter's name using 'get_name()' API
						std::cout << "\n\t[CFGR -> Retrieve] Parameter's name : " << int_param_ptr->get_name() << endl;
					}// End of IF


					// Access parameter's value using 'json_serialize' API
					std::cout << "\n\t[CFGR -> Retrieve] Parameter's value : " << int_param_ptr->json_serialize() << endl;
					

					// Access parameter's documentationu using 'get_documentation()' API
					std::cout << "\n\t[CFGR -> Retreive] Parameter's doc : " << int_param_ptr->get_documentation() << endl;
				

					wait(2.0, SC_NS);				
	
					std::cout << "\n@ " << sc_time_stamp() << endl;
					std::cout << "\n\t[CFGR -> Set] :  " << int_param_ptr->get_name() << " value to 10" << endl;
					

					/// Set parameter's value using 'json_deserialize' API
					int_param_ptr->json_deserialize("10"); 


					wait(2.0, SC_NS);

					std::cout << "\n@ " << sc_time_stamp() << endl;
					std::cout << "\n\t[CFGR-> Set] : Parameter doc - 'Modified with Configuration tool'" << endl;
					
					const std::string documentation_string = "Modified with Configuration tool";
					

					/// Set parameter's documentation using 'set_documentation' API
					int_param_ptr->set_documentation(documentation_string);
	
					wait(2.0, SC_NS);
				

					std::cout << "\n@ " << sc_time_stamp() << endl;
					/// Access parameter's name using 'get_name()' API
					std::cout << "\n\t[CFGR -> Retrieve] : Parameter name : " << int_param_ptr->get_name() << endl;
					/// Access parameter's value using 'json_serialize' API
					std::cout << "\n\t[CFGR -> Retrieve] : Parameter value: " << int_param_ptr->json_serialize() << endl;
					/// Access parameter's documentation using 'get_documentation()' API
					std::cout << "\n\t[CFGR -> Retrieve] : Parameter doc : " << int_param_ptr->get_documentation() << endl;
			
					wait(2.0, SC_NS);
			

					/// Access locking status of a parameter using 'locked()' API
					//
					// Set password to lock/unlock parameter
					
					lock_passwd = &check;	
					
					while(check < 3)
					{	
						std::cout << "\n@ " << sc_time_stamp() << endl;
						std::cout << "\n\t[CFGR] : Checking locked status of parameter" << endl;
						
						// Try accessing and modify value after locking
						lock_status = int_param_ptr->locked();
						
						if(lock_status)
							std::cout << "\n\t[CFGR] : " << int_param_ptr->get_name() << " is in locked state!!" << endl;
						else
							std::cout << "\n\t[CFGR] : " << int_param_ptr->get_name() << " is in unlocked state!!" << endl;
						
						check++;

						if(check == 1)
						{
							// Demonstration of parameter locking using password
							int_param_ptr->lock(&lock_passwd);
							std::cout << "\n\t[CFGR] : Parameter locked using password" << endl;
						}
						else if(check == 2)
						{
							// Demonstration of parameter unlocking using password
							int_param_ptr->unlock(&lock_passwd);
							std::cout << "\n\t[CFGR] : Parameter unlocked using password" << endl;
						}// End of IF

						wait(2.0, SC_NS);
					}// End of while (check)
					
					wait(2.0, SC_NS);
			
					std::cout << "\n@ " << sc_time_stamp() << "\tdemonstrating 'is_invalid_value()'" << endl;
					
					/// Access invalid state of a cci parameter using 'is_invalid_value()' API
					if(int_param_ptr->is_invalid_value())
						std::cout << "\n\t[CFGR] : " << int_param_ptr->get_name() << " value is set to invalid." << endl;
					else
						std::cout << "\n\t[CFGR] : " << int_param_ptr->get_name() << " value not set to invalid." << endl;
					

					wait(2.0, SC_NS);
					
					std::cout << "\n@ " << sc_time_stamp() << "\tdemonstrating 'set_value()' using cci_value" << endl;
				
						
					cci::cnf::cci_value str_value("Hello_New_String");
					
					/// Query basic type of a parameter using 'get_basic_type()' API
					std::cout << "\n\t[CFGR -> Set] : Get Basic Type using 'get_basic_type()' : " << str_param_ptr->get_basic_type() << endl;

					/// Set a (std::string) parameter's value using 'set_value' API with cci_value
					str_param_ptr->set_value(str_value);
					
					std::cout << "\n\t[CFGR] : 'Retrieve1' using 'json_serialize' and 'Retrieve2' using 'get_value()'" << endl;
					std::cout << "\n\t[CFGR -> Retrieve1] : " << str_param_ptr->get_name() << " value is " << str_param_ptr->json_serialize() << endl;  					
					
					/// Query a (std::string) parameter's value by using 'get_value()'API with cci_value
					cci::cnf::cci_value rx_value = str_param_ptr->get_value();

					/**	
					  * Based on the type returned by 'get_basic_type()', query value of cci_value.
					  * For example, if returned type is 'std::string', use 'get_string()' API with cci_value
					  */
					std::cout << "\n\t[CFGR -> Retrieve2] : " << rx_value.get_string() << endl;

					wait(2.0, SC_NS);
				}
				else
					std::cout << "\nParameter by name " << int_param_str << " not found." << endl; 

			}// End of outer while
		}// End of SC_THREAD

	private	:

		/// CCI configuration broker instance
		cci::cnf::cci_cnf_broker_if* myBrokerInterface;

		/// std::string types for storing parameters hierarchical paths
		std::string int_param_str;
		std::string string_param_str;
			
		/// Declaring cci_base_parameters
		cci::cnf::cci_base_param* int_param_ptr;
		cci::cnf::cci_base_param* str_param_ptr;			

		// Few Local parameters
		bool  intParamExists; /*!Stores the status whether integer type parameter exists*/
		bool  strParamExists; /*!Stores the status whether string type parameter exists*/	
		int 	check;          /*!Local Variable*/
		bool	lock_status;    /*!Holds lock status of a parameter*/
		void*	lock_passwd; 	  /*!Holds the key(password) for lock/unlock*/
};

#endif	// End of PARAMETER_CONFIGURER_H
