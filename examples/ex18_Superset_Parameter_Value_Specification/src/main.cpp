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

#include <cci.h>
#include <systemc.h>
#include <assert.h>
#include <string.h>

/**
 * @file      main.cpp
 * @brief     This is the main testbench file.
 * @author    P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date      15th June, 2011 (Wednesday)
 */
#include "parameter_owner.h"
#include "parameter_configurator.h"

/**
 * @brief     The file assigns initial values to a set/list of cci_parameters
 *            (having no previous knowledge of the existence of any of the parameters
 *            within the owner module).  Later instantiates the owner and configurator modules
 * @author    P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date      15th June, 2011 (Wednesday)
 */
int sc_main(int sc_argc, char* sc_argv[])
{
	cci::cnf::cci_originator            sc_main_originator("sc_main_originator");
	
	cci::cnf::cci_cnf_broker_if* myMainBrokerIF = &cci::cnf::cci_broker_manager::get_current_broker(sc_main_originator);

	assert(myMainBrokerIF != NULL && "SC_MAIN_BROKER_IF handle is returned NULL");

	std::cout << "\n\t[MAIN] : Set initial value to 'integer type parameter'" << std::endl;
	myMainBrokerIF->set_init_value("param_owner.int_param", "10");

	std::cout << "\n\t[MAIN] : Set initial value to 'float type parameter'" << std::endl;
	myMainBrokerIF->set_init_value("param_owner.float_param", "11.11");

	std::cout << "\n\t[MAIN] : Set initial value to 'string type parameter'" << std::endl;
	myMainBrokerIF->set_init_value("param_owner.string_param", "sample_string");

	std::cout << "\n\t[MAIN] : Set initial value to 'double type parameter'" << std::endl;
	myMainBrokerIF->set_init_value("param_owner.double_param", "100.123456789");

	parameter_owner          param_owner("param_owner");
	parameter_configurator   param_cfgr("param_cfgr");

	sc_start(15.0, SC_NS);

	return EXIT_SUCCESS;
}
