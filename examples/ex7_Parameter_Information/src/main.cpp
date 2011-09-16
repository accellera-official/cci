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

/*!
 * \file    main.cpp
 * \brief   The main function starts here.
 *          The sc_main function instantiates 'PARAMETER_OWNER', 'PARAMETER_CONFIGURATOR'
 *          modules and 'OBSERVER' class
 * \author  P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * \date    12th September, 2011
 */
#include <cci.h>          ///< Contains all the definitions for CCI-infrastructure
#include <systemc.h>      ///< Contains all the definitions for SystemC
#include <assert.h>

#include "parameter_owner.h"
#include "parameter_configurer.h"
#include "observer.h"

/*!
 * \fn     int sc_main (int, char**)
 * \brief  The main functions begins the hierarchy construction and sets simulation
 *         related details
 */
int sc_main(int sc_argc, char* sc_argv[])
{
	/// Creating an originator to access the global broker
	const std::string myOrgStr = "sc_main_originator";
	cci::cnf::cci_originator myOriginator(myOrgStr);

	/// Get handle of the broker using the originator
	cci::cnf::cci_cnf_broker_if* globalBroker = &cci::cnf::cci_broker_manager::get_current_broker(myOriginator);

	// Assert if broker handle returned is NULL
	assert(globalBroker != NULL && "Reference to global broker returned (in sc_main) is NULL.");

	// Set initial value to the 'int_param' of 'parameter_owner' class before
	// their constructor begins
	std::cout << "\n[MAIN] : Setting 'param_owner.mutable_string_param' value to 'Initialized within sc_main()'" << endl;
	
	/// Demonstrating use of 'set_init_value' API to assign initial value before the
	/// construction of the model hierarchy begins.
	globalBroker->set_init_value("param_owner.mutable_string_param", "Initialized within sc_main");

	/// Infrastructure created within the example for example illustration
	std::cout << "\n[MAIN] : parameter_container module declares two cci type parameters." << endl;
	std::cout << "\n\tOne is of 'Integer type' and the other is of 'String type'" << endl;
	std::cout << "\n\t'Integer type' has 'pre_read', 'pre/post_write' callback registered within the OBSERVER" << std::endl;

	/// Instantiation of sc_modules
	parameter_owner     	param_owner("param_owner");
	parameter_configurer	param_cfgr("param_cfgr");

	/// Instantiating observer class
	observer              observer_class;

	/// Simulation time
	sc_start(24.0, SC_NS);

	return EXIT_SUCCESS;

}// End of sc_main
