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
 * @brief     This file reads the inputs (parameter name and parameter value pair)
 *            from the 'Configuration Text File' and assigns the initial values to
 *            the corresponding parameters before the construction of the modules begins
 * @author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *            Girish Verma, CircuitSutra Technologies   <girish@circuitsutra.com>
 * @date      12th July, 2011 (Tuesday)
 */

#include <cci>                  /*!To be included in all CCI based applications*/
#include <systemc.h>
#include <unistd.h>
#include "cci_configFile_Tool.h"
#include "parameter_owner.h"
#include "parameter_configurator.h"

#include <string.h>

/**
 * @brief      This function illustrates the way of reading paramters name-value
 *             pair from an external configuration file and then, assigns the
 *             respective initial values to the parmeters via the GreenSoCs parser
 */
int sc_main(int sc_argc, char* sc_argv[])
{
	/// Declare an instance of the cci_configFile_Tool that reads in the initial values to be
	/// set to a cci-parameter via a configuration file, parses them and sets them to the
	/// appropriate parameters
    cci::cnf::cci_configFile_Tool	configTool("configFileTool");
	configTool.config("./ex17_Specifying_Parameter_Values_via_a_Configuration_File/Configuration_File.txt");

	/// Declare instances of the owner and configurator classes
	parameter_owner           param_owner("param_owner");
	parameter_configurator    param_cfgr("param_setter");

	/// Start and run the simulation till the time specified
	sc_core::sc_start(50.0, sc_core::SC_NS);
	
	return EXIT_SUCCESS;

}// End of MAIN
