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
 * @file      main.cpp
 * @brief     This is the main testbench file.  This file shows the up-front
 *            planning required to be done in order to access an owner class
 *            private memmbers accessible within the configurator class by
 *            making the configurator a friend class of the owner
 * @author    Girish Verma, CircuitSutra Technologies     <girish@circuitsutra.com>
 *            P V S Phaneendra, CircuitSutra Technologies <pvs@circuitsutra.com>
 * @date      14th December, 2011 (Wednesday).
 *            (Modified On) 16th May, 2011 (Monday)
 */
#include <systemc.h>

#include "parameter_owner.h"
#include "parameter_configurator.h"

/**
 * @brief      This function instantiates owner and configurator class objects
 *             and shows details of the up-front planning required to make the
 *             owner class private members accesible within the configurator 
 * @author     P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date       14th December, 2011 (Wednesday)
 *             (Modified On) 16th May, 2011 (Monday)
 */
int sc_main(int sc_argc, char* sc_argv[])
{
	/// Instantiation of the owner and configurator sc_modules
	parameter_owner           param_onwer("param_owner");
	parameter_configurator    param_cfgr("param_setter");

	/// The instance of the owner class being passed as an argument to an API of the
	/// configurator class will serve in getting references to the owner class
	/// protected/private members within the configurator later
	param_cfgr.set_cfgr_parameter (&param_onwer);

	// Start and run the simulation till 50 nanoseconds
	sc_core::sc_start(50.0, sc_core::SC_NS);

	return EXIT_SUCCESS;

}// End of MAIN
