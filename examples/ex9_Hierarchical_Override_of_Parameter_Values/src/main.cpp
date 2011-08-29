/******************************************************************************** 
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
 ********************************************************************************/

/**
 * @file     main.cpp 
 * @brief    This file declares and implements the functionality of the slave.
 *           Few of the parameters of the slave sc_module are configured by the 
 *           router sc_module
 * @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * @date     29th April, 2011 (Friday) : 12:52 hrs IST
 */
#include <cci.h>             /*!<Always include this header in all CCI applications>*/
#include <systemc.h>         /*!<SystemC header file for 'sc_main'*/
#include <assert.h>          
#include "top_module.h"      /*!<'top_module' instance shall be created within main*/

/**
 * @brief     The sc_main function instantiates the top module which, in turn,
 *            instantiates master, router and slave modules
 */
int sc_main(int sc_argc, char* sc_argv[])
{
	/// Instantiating originator to get access to the global broker
	std::string myString = "sc_main_originator";
	cci::cnf::cci_originator myOriginator(myString);	

	
	/// Get handle to the default broker
	cci::cnf::cci_cnf_broker_if* myGlobalBroker = &cci::cnf::cci_broker_manager::get_current_broker(myOriginator);

	// Assert if broker handle returned is NULL
	assert(myGlobalBroker != NULL && "Handle of the returned global broker is NULL");


	std::cout << "\n[MAIN] : Setting initial value of the number of masters to 2" << endl;		

	/// Set initial value to the number of master(s) (within top_module)
	std::string masterHierarchicalName = "top_module_inst.number_of_masters";
	myGlobalBroker->set_init_value(masterHierarchicalName, "2");


	std::cout << "\n[MAIN] : Setting initial value of the number of masters to 1" << endl;		

	// The program considers only the last set initial value
	myGlobalBroker->set_init_value(masterHierarchicalName, "1"); // The program takes in the last set initial value


	std::cout << "\n[MAIN] : Setting initial value of the number of slaves to 4" << endl;		
	
	/// Set initial value to the number of slave(s) (within top_module)
	std::string slaveHierarchicalName = "top_module_inst.number_of_slaves";
	myGlobalBroker->set_init_value(slaveHierarchicalName, "4");


	/// Set the maximum addressing limit for the router 
	myGlobalBroker->set_init_value("top_module_inst.RouterInstance.addr_max", "1024");
	

	/// Set and lock the Router Table initials values for slave_1 
	//  These values have again been tried to set within the Top_MODULE
	//  @see top_module.h
	std::cout << "\n[MAIN] : Set and lock Router Table Slave_1 contents" << endl;		
	myGlobalBroker->set_init_value("top_module_inst.RouterInstance.r_index_1", "1");	
	myGlobalBroker->lock_init_value("top_module_inst.RouterInstance.r_index_1");	
	
	std::cout << "\n[MAIN] : Set and lock Router Table Start Address for Slave_1 to 128" << endl;		
	myGlobalBroker->set_init_value("top_module_inst.RouterInstance.r_sa_1", "128");	
	myGlobalBroker->lock_init_value("top_module_inst.RouterInstance.r_sa_1");	

	std::cout << "\n[MAIN] : Set and lock Router Table End Address for Slave_1 to 255" << endl;		
	myGlobalBroker->set_init_value("top_module_inst.RouterInstance.r_ea_1", "255");	
	myGlobalBroker->lock_init_value("top_module_inst.RouterInstance.r_ea_1");	


	std::cout << "\n[MAIN] : Instantiate top module after setting initial values to top_module, router and slave parameters" << endl;		

	/// Instantiate TOP_MODULE responsible for creating the model hierarchy
	top_module	top_mod("top_module_inst");


	/// Start the simulation
	sc_start(sc_time(1140,SC_NS));

	return EXIT_SUCCESS;

}// End of 'sc_main' 
