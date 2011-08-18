/*********************************************************************************
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
  * @file    main.cpp
  * @brief   This function instantiates both the parameter_containter and
  *          access_container modules
  * @author  P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
  * @date    28th April, 2011 : 18:03 hrs IST
  */
#include <cci.h>
#include <systemc.h>
#include <assert.h>
#include "parameter_owner.h"
#include "parameter_configurer.h"
#include "observer.h"

/// SystemC main function
int sc_main(int sc_argc, char* sc_argv[])
{
	// @date 28th April, 2011 (Thursday) 
	// Appended the code for originator, broker and 'set_init_value' API
	
	// Creating an originator to access the global broker
	const std::string myOrgStr = "sc_main_originator";
	cci::cnf::cci_originator myOriginator(myOrgStr);

	// Get handle of the broker using the originator
	cci::cnf::cci_cnf_broker_if* globalBroker = &cci::cnf::cci_broker_manager::get_current_broker(myOriginator);

	// Assert if broker handle returned is NULL
	assert(globalBroker != NULL && "Reference to global broker returned (in sc_main) is NULL.");

	// Set initial value to the 'int_param' of 'parameter_owner' class before
	// their constructor begins
	std::cout << "\n[MAIN] : Setting 'param_owner.mutable_string_param' value to 'Initialized within sc_main()'" << endl;
	
	/// Demonstrating use of 'set_init_value' API to assign initial value before the
	/// construction of the model hierarchy begins.
	globalBroker->set_init_value("param_owner.mutable_string_param", "Initialized within sc_main");

	std::cout << "\n[MAIN] : parameter_container module declares two cci type parameters." << endl;
	std::cout << "\n\tOne is of 'Integer type' and the other is of 'String type'" << endl;
	std::cout << "\n\t'Integer type' has 'post_write' callback registered within the OWNER, while" << std::endl;
	std::cout << "\t'String type' parameter has 'pre_read' callback registered" << endl;

	/// Instantiation of sc_modules
	parameter_owner     	param_owner("param_owner");
	parameter_configurer	param_cfgr("param_cfgr");

	/// Instantiating observer class
	observer              observer_class;

	sc_start(24.0, SC_NS);

	return EXIT_SUCCESS;
}
