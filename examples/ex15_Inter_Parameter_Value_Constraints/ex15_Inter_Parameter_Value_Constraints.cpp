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
 * @brief     This file defines the 'sc_main' function 
 * @author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * @date      10th June, 2011 (Friday)
 */
#include <systemc.h>
#include "processor.h"
#include "configurator.h"

/**
 * @fn        int sc_main (int, char** )
 * @brief     This function instantiates the two modules - PROCESSOR & CONFIGURATOR modules
 */
int sc_main(int sc_argc, char* sc_argv[])
{
	std::cout << "\n\t[MAIN] : In this example, the following is condition is verified" << std::endl;
	std::cout << "\n\t[MAIN] : \t\tx <= 2^n - 1" << std::endl;
	std::cout << "\n\t[MAIN] : where, 'x' : value of 'mem_block_size' (Memory Block size), and" << std::endl;
	std::cout << "\t[MAIN] :        'n' : total number of address lines - 'curr_addr_lines'" << std::endl;

	/// Instantiation of the owner and configurator sc_modules
	processor                 processor("processor");
	configurator              param_cfgr("param_cfgr");

	// Start and run the simulation till 50 nanoseconds
	sc_start(15.0, SC_NS);

	return EXIT_SUCCESS;

}// End of MAIN
