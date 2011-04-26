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
 *   ******************************************************************************/

/**
  * @file    main.cpp
  * @brief   This function instantiates both the parameter_containter and
  *          access_container modules
  * @author  P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
  * @date    18th April, 2011 : 18:03 hrs IST
  */
#include <systemc.h>
#include "parameter_owner.h"
#include "parameter_configurer.h"

/// SystemC main function
int sc_main(int sc_argc, char* sc_argv[])
{
	std::cout << "\n[MAIN] : parameter_container module declares two cci type parameters." << endl;
	std::cout << "\n\tOne is of 'Integer type' and the other is of 'String type'" << endl;

	parameter_owner     	param_owner("param_owner");
	parameter_configurer	param_cfgr("param_acc");

	sc_start(24.0, SC_NS);

	return EXIT_SUCCESS;
}
