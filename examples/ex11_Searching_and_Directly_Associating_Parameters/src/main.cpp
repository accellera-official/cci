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
 * @brief     This file instantiates the TOP_MODULE and the PARAMETER_CONFIGURATOR 
 * @author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * @date      9th June, 2011 (Thursday)
 */
#include <systemc.h>
#include "top_module.h"
#include "parameter_configurator.h"

/**
 * @brief      The TOP_MODULE instantiated within this function, in turn,
 *             instantiates the PARAMETER_OWNER module
 */
int sc_main(int sc_argc, char* sc_argv[])
{
	/// Instantiation of the owner and configurator sc_modules
	top_module                top_mod("top_mod");
	parameter_configurator    param_cfgr("param_cfgr");

	// Start and run the simulation till 50 nanoseconds
	sc_core::sc_start(30.0, sc_core::SC_NS);

	return EXIT_SUCCESS;

}// End of MAIN
