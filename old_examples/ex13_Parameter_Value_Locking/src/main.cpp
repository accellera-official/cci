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
 * @brief     This file instantiates the owner and configurator modules and
 *            sets the simulation time
 * @author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * @date      10th May, 2011
 */
#include <systemc.h>           // SystemC headers
#include "parameter_owner.h"
#include "parameter_configurator.h"

/**
 * @fn        int sc_main (int sc_argc, char** )
 * @brief     This function instantiates the owner and configurator modules
 */
int sc_main(int sc_argc, char* sc_argv[])
{
	/// OWNER and CONFIGURATOR modules instantiation
	parameter_owner            param_owner("param_owner");
	parameter_configurator     param_cfgr("param_cfgr");

	// Setting simulation time and starting the simulation
	sc_start(25.0, SC_NS);
	
	return EXIT_SUCCESS;

}// End of sc_main
