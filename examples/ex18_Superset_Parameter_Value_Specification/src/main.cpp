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
 * @file     main.cpp
 * @brief    This implementation assigns initial values to a list of cci-parameters
 *           (without any knowledge of them being present in the model) and then
 *           instantiates the 'parameter_owner' and 'parameter_configurator' modules
 * @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * @date     21st July, 2011 (Thursday)
 */
#include <cci>      /*!This header must be included wherever cci-infrastructure is used*/
#include <systemc.h>
#include <assert.h>
#include <string.h>

#include "cci_configFile_Tool.h"
#include "parameter_owner.h"
#include "parameter_configurator.h"

/**
 * @file     main.cpp
 * @brief    Here, a reference of the global broker is taken with the help of
 *           the originator information and then initial values are assigned to
 *           a list of cci-parameters
 */
int sc_main(int sc_argc, char* sc_argv[])
{
  cci::cnf::cci_configFile_Tool configTool("configFileTool");
  configTool.config("Configuration_File.txt");

#if 0
  // In case, if user doesn't want to use the reading from configuration file approach, here
  // is an alternative that assigns initial values to the cci-parameters

	/// Declare cci-originator instance in order to retrieve a reference of the global broker 
	cci::cnf::cci_originator   sc_main_originator("sc_main_originator");
	
	/// Get reference/handle of the default global broker
	cci::cnf::cci_cnf_broker_if* myMainBrokerIF = &cci::cnf::cci_broker_manager::get_current_broker(sc_main_originator);

	/// Assert if the returned broker handle is NULL
	assert(myMainBrokerIF != NULL && "SC_MAIN_BROKER_IF handle is returned NULL");

	std::cout << "\n\t[MAIN] : Set initial value to 'integer type parameter'" << std::endl;
	myMainBrokerIF->set_init_value("param_owner.int_param", "10");

	std::cout << "\n\t[MAIN] : Set initial value to 'float type parameter'" << std::endl;
	myMainBrokerIF->set_init_value("param_owner.float_param", "11.11");

	std::cout << "\n\t[MAIN] : Set initial value to 'string type parameter'" << std::endl;
	myMainBrokerIF->set_init_value("param_owner.string_param", "Used_parameter");

	std::cout << "\n\t[MAIN] : Set initial value to 'double type parameter'" << std::endl;
	myMainBrokerIF->set_init_value("param_owner.double_param", "100.123456789");
#endif

	/// Instatiation of 'parameter_owner' and 'parameter_configurator' modules
	parameter_owner          param_owner("param_owner");
	parameter_configurator   param_cfgr("param_cfgr");

	/// BEOE, EOE and simulation phases advance from here
	sc_start(10.0, SC_NS);

	return EXIT_SUCCESS;

}//End of MAIN
 
