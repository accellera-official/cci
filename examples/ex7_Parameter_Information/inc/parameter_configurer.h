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

/*!
 * \file    parameter_configurer.h
 * \brief   This file defines a CONFIGURATOR.
 *          This file declares and defines a sc_module which demonstrates the 
 *          means to access the cci-parameters using name-based look up access
 *          approach and set their various attributes
 * \author  P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * \date    12th September, 2011 (Monday)
 */
#ifndef PARAMETER_CONFIGURER_H
#define PARAMETER_CONFIGURER_H

#include <cci>     // Include 'cci' header in all CCI-based applications
#include <assert.h>

/*!
 * \class   parameter_configurer parameter_configurer.h
 * \brief   This sc_module instantiates a cci configuration broker which accesses
 *          the references of the cci-parameters and sets their attributes like 
 *          value, documentation, etc. 
 */
class parameter_configurer : public sc_core::sc_module
{
	public	:
		
		SC_HAS_PROCESS(parameter_configurer);

		/*!
		 *  \fn     parameter_configurer::parameter_configurer
		 *  \brief  Default constructor
		 */
		parameter_configurer(sc_module_name _name)
		: sc_module(_name)
		,	check(0)
		{
			/// Get the broker responsible for this module using 'get_current_broker' API
			myBrokerInterface = &cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));

			// Assert if broker handle returned is NULL
			assert(myBrokerInterface != NULL && "Broker Handle Returned is NULL");


			/// Check for the broker type (default or private) using 'is_private_broker()' API
			if(myBrokerInterface->is_private_broker())
				/// Access broker's name using 'name()' 
				std::cout << "\n\t[CFGR C_TOR] : Broker Type : " << myBrokerInterface->name() << endl;
			else
				std::cout << "\n\t[CFGR C_TOR] : Broker Type : " << myBrokerInterface->name() << " - is not a private broker." << endl; 


			int_param_str = "param_owner.mutable_int_param";
			string_param_str = "param_owner.mutable_string_param";
			
			intParamExists = false;
			strParamExists = false;	
	
	
			// Broker interface checks for the existance of a (int type) parameter
			if(myBrokerInterface->param_exists(int_param_str))
			{
				int_param_ptr =	myBrokerInterface->get_param(int_param_str);
		
				assert(int_param_ptr != NULL && "Integer CCI Parameter Handle returned NULL");					
				
				intParamExists = true;
			}
			else
			{
				std::cout << "\n\t[CFGR C_TOR] : Integer parameter does not exist." << endl;
				
				intParamExists = false;
			} // End of IF


			/// Broker interface checks for existance of a (std::string type) parameter using 'param_exists' API
			if(myBrokerInterface->param_exists(string_param_str))
			{
				/// If parameter exists, get handle of the parameter using 'get_param' API
				str_param_ptr = myBrokerInterface->get_param(string_param_str);		
					
				// Report if parameter handle is returned NULL
				assert(str_param_ptr != NULL && "String CCI Parameter Handle returned NULL");
									
				strParamExists = true;
			}
			else
			{
				std::cout << "\n\t[CFGR C_TOR] : String parameter does not exist" << endl;
					
				strParamExists = false;
			} // End of IF

			
			std::cout << "\nPrior to " << sc_time_stamp() << "\tdemonstrating 'is_initial_value' API" << std::endl;

			/// Check if string type cci-parameter has any initial value using 'is_initial_value' API
			if(str_param_ptr->is_initial_value())
				std::cout << "\n\t[CFGR C_TOR] : " << str_param_ptr->get_name()\
					<< "\tInitial Value : " << str_param_ptr->json_serialize() << std::endl;
			else
				std::cout << "\t[CFGR] : No initial value set for " << str_param_ptr->get_name() << std::endl;

			std::cout << "\t[CFGR C_TOR] : Is initial value ?\t"\
				<< std::boolalpha << str_param_ptr->is_initial_value() << std::endl;


			/// Set documentation to the string type cci-parameter
			std::cout << "\n\t[CFGR C_TOR] : Set documentation to the string-type cci-parameter" << std::endl;
			std::string str_doc = "This is a mutable type string parameter";
			str_param_ptr->set_documentation(str_doc);

 			/// Register SC_THREAD process 
			SC_THREAD(run_accessor);
		}

		/*!
		 *  \fn       parameter_configurer::~parameter_configurer
		 *  \brief    Default Destructor
		 */
		~parameter_configurer()
		{
			// Destructor does nothing
		}


		/*!
		 *  \fn     void run_accessor (void)
		 *  \brief  It uses various APIs like 'is_default_value', 'is_invalid_value',
		 *          'is_initial_value', etc. to check the status of parameters.  It
		 *          also illustrates various APIs required to set, get, lock, unlock
		 *          the parameter values and their documentation
		 */
		void run_accessor(void)
		{
			while(1) 
			{
				if(strParamExists && intParamExists)
				{
					std::cout << "\n@ " << sc_time_stamp() << "\tdemonstrating 'is_default_value()'" << endl; 
					
					/// Access parameter's default value status using 'is_default_value()' API
					if(int_param_ptr->is_default_value())
					{	
						std::cout << "\n\t[CFGR] : " << int_param_ptr->get_name() << " default value hasn't been modified." << endl; 
						std::cout << "\t[CFGR] : Is Default Value ? " << "\tReturned status : "\
							<< std::boolalpha << int_param_ptr->is_default_value() << endl; 
					}
					else
					{
						std::cout << "\n\t[CFGR] : " << int_param_ptr->get_name() << " default value has been modified." << endl; 
						std::cout << "\t[CFGR] : Is Default Value ? " << "\tReturned status : "\
							<< std::boolalpha << int_param_ptr->is_default_value() << endl; 
					}

					wait(2.0, SC_NS);				
	
					std::cout << "\n@ " << sc_time_stamp() << "\tdemonstrating 'is_default_value()'" << endl;
	
					// Access parameter's default value status using 'is_default_value()' API
					if(!int_param_ptr->is_default_value())
					{
						std::cout << "\n\t[CFGR] : " << int_param_ptr->get_name() << " value has been modified." << endl; 
						std::cout << "\t[CFGR] : Is Default Value ? " << "\tReturned status : " \
							<< std::boolalpha << int_param_ptr->is_default_value() << endl; 
					}//End of IF


					if(str_param_ptr->is_default_value())
					{
						std::cout << "\n\t[CFGR] : " << str_param_ptr->get_name() << " value hasn't been modified" << endl;			
						std::cout << "\n\t[CFGR -> Retrieve] : Parameter name  : " << str_param_ptr->get_name() << endl;
						std::cout << "\n\t[CFGR -> Retrieve] : Parameter value : " << str_param_ptr->json_serialize() << endl;			
					}
					else
						std::cout << "\n\t[CFGR] : " << str_param_ptr->get_name() << " New Value : "\
							<< str_param_ptr->json_serialize()<< endl; 


					wait(2.0, SC_NS);
	
					std::cout << "\n@ " << sc_time_stamp() << "\tdemonstrating 'is_invalid_value()'" << endl;
					

					/// Check parameter's value validity using 'is_invalid_value()' API
					if(int_param_ptr->is_invalid_value())
					{
						std::cout << "\n\t[CFGR] : Is Invalid Value ? " << "\tReturned status : "\
							<< std::boolalpha << int_param_ptr->is_invalid_value() << endl; 
						std::cout << "\t[CFGR] : Parameter value is invalid" << endl;
					}
					else
					{
						std::cout << "\n\t[CFGR] : Is Invalid Value ? " << "\tReturned status : "\
							<< std::boolalpha << int_param_ptr->is_invalid_value() << endl; 
						std::cout << "\t[CFGR] : Valid parameter value" << endl;
					}//End of IF-ELSE


					// Access parameter's name using 'get_name()' API
					std::cout << "\n\t[CFGR -> Retrieve] Parameter's name : " << int_param_ptr->get_name() << endl;

					// Access parameter's value using 'json_serialize' API
					std::cout << "\n\t[CFGR -> Retrieve] Parameter's value : " << int_param_ptr->json_serialize() << endl;

					// Access parameter's documentationu using 'get_documentation()' API
					std::cout << "\n\t[CFGR -> Retreive] Parameter's doc : " << int_param_ptr->get_documentation() << endl;
				

					wait(2.0, SC_NS);				
	
					std::cout << "\n@ " << sc_time_stamp() << endl;
					std::cout << "\n\t[CFGR -> Set] :  " << int_param_ptr->get_name() << " value to 10" << endl;
					

					/// Set parameter's value using 'json_deserialize' API
					int_param_ptr->json_deserialize("10"); 


					//wait(2.0, SC_NS);
					wait(4.0, SC_NS);

					std::cout << "\n@ " << sc_time_stamp() << endl;

					/// Access parameter's name using 'get_name()' API
					std::cout << "\n\t[CFGR -> Retrieve] : Parameter name : " << int_param_ptr->get_name() << endl;

					/// Access parameter's value using 'json_serialize' API
					std::cout << "\n\t[CFGR -> Retrieve] : Parameter value: " << int_param_ptr->json_serialize() << endl;

					/// Access parameter's documentation using 'get_documentation()' API
					std::cout << "\n\t[CFGR -> Retrieve] : Parameter doc : " << int_param_ptr->get_documentation() << endl;
			
					wait(2.0, SC_NS);
			

					/// Access locking status of a parameter using 'is_locked()' API
					//
					// Set password to lock/unlock parameter
					
					lock_passwd = &check;	
					
					while(check < 3)
					{	
						std::cout << "\n@ " << sc_time_stamp() << endl;
						std::cout << "\n\t[CFGR] : Checking locked status of parameter" << endl;
						
						// Try accessing and modify value after locking
						lock_status = int_param_ptr->is_locked();
						
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
					{
						std::cout << "\n\t[CFGR] : " << int_param_ptr->get_name() << " value is set to invalid." << endl;
						std::cout << "\t[CFGR] : Is Invalid Value ? " << std::boolalpha << int_param_ptr->is_invalid_value() << endl; 
					}
					else
					{
						std::cout << "\n\t[CFGR] : " << int_param_ptr->get_name() << " value not set to invalid." << endl;
						std::cout << "\t[CFGR] : Is Invalid Value ? " << "\tReturned status : "
							<< std::boolalpha << int_param_ptr->is_invalid_value() << endl; 
					}//End of IF-ELSE


					wait(2.0, SC_NS);
					
					std::cout << "\n@ " << sc_time_stamp() << "\tdemonstrating 'set_value()' for string-type param using cci_value" << endl;
					std::cout << "\n\t[CFGR] : Parameter Documentation : " << str_param_ptr->get_documentation() << endl;
						
					cci::cnf::cci_value str_value("Hello_New_String");
					
					/// Query basic type of a parameter using 'get_basic_type()' API
					std::cout << "\n\t[CFGR -> Set] : Get Basic Type using 'get_basic_type()' : " << str_param_ptr->get_basic_type() << endl;

					/// Set a (std::string) parameter's value using 'set_value' API with cci_value
					str_param_ptr->set_value(str_value);

					wait(2.0, SC_NS);

					std::cout << "\n@ " << sc_time_stamp() << " demonstrating 'get_latest_write_originator' API" <<  std::endl;

					/// Get the latest write originator for the write value to the string type cci-parameter
					const cci::cnf::cci_originator* str_originator = str_param_ptr->get_latest_write_originator();
					std::cout << "\n\t[CFGR] : Originator for the latest write on string type cci-parameter : "\
						<< str_originator->name() << std::endl;

					
					std::cout << "\n@ " << sc_time_stamp() << " demonstrating 'get_value' for string-type param using cci_value" <<  std::endl;
					std::cout << "\n\t[CFGR] : 'Retrieve1' using 'json_serialize' and 'Retrieve2' using 'get_value()'" << endl;
					std::cout << "\n\t[CFGR -> Retrieve1] : " << str_param_ptr->get_name() << " value is " << str_param_ptr->json_serialize() << endl;  					
					
					/// Query a (std::string) parameter's value by using 'get_value()'API with cci_value
					cci::cnf::cci_value rx_value = str_param_ptr->get_value();

					/*!	
					 *  \par Based on the type returned by 'get_basic_type()', query value of cci_value.
					 *  \par    
					 *  If basic parameter type is 'std::string', use 'get_string()' API with cci_value
					 *  \par
					 *  If basic parameter type is 'int', use 'get_int()' API with cci_value, etc.
					 */
					std::cout << "\n\t[CFGR -> Retrieve2] : " << rx_value.get_string() << endl;

					wait(2.0, SC_NS);
				}
				else
					std::cout << "\nParameter by name " << int_param_str << " not found." << endl; 

				wait(20.0, SC_NS);

			}// End of outer while

		}// End of SC_THREAD

	private	:

		cci::cnf::cci_cnf_broker_if* myBrokerInterface;  //!< CCI configuration broker instance

		std::string int_param_str;    //!< For storing hierarchical path of std::string type cci-parameter
		std::string string_param_str; //!< For storing hierarchical path of integer type cci-parameter
			
		// Declaring cci_base_parameters
		cci::cnf::cci_base_param* int_param_ptr; //!< cci_base_param for int type cci-param
		cci::cnf::cci_base_param* str_param_ptr; //!< cci_base_param for string type cci-param

		// Few Local parameters
		bool  intParamExists; //!< Stores the status whether integer type parameter exists
		bool  strParamExists; //!< Stores the status whether string type parameter exists	
		int 	check;          //!< Local Variable
		bool	lock_status;    //!< Holds lock status of a parameter
		void*	lock_passwd; 	  //!< Holds the key(password) for lock/unlock

};// End of (SC_MODULE) class

#endif	// End of PARAMETER_CONFIGURER_H
