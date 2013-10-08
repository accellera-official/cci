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
 * \brief   The observer class and the parameter_owner module are instantiated here
 * \author  P V S Phaneendra, CircuitSutra Technologies <pvs@circuitsutra.com>
 * \date    11th October, 2011
 */
#include <systemc.h>
#include "observer.h"
#include "parameter_owner.h"


/*!
 * \fn      int sc_main (int, char **)
 * \brief   sc_main function
 * \param   sc_argc
 * \param   sc_argv[]
 * \return  int
 */
int sc_main(int sc_argc, char* sc_argv[])
{
	/// Instantiate the observer class
	observer                 observer_inst;

	/// Instantiate the parameter_owner module
	parameter_owner          param_owner("param_owner");

	/// Simulation starts
	sc_start(15.0, SC_NS);

	return EXIT_SUCCESS;

}// End of 'sc_main'
