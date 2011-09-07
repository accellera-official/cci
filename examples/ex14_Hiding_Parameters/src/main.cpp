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
 * @brief     This file instantiates the TOP module which, in turn, instantiates
 *            the PARENT and the CONFIGURATOR modules.  
 * @author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *            Girish Verma, CircuitSutra Technologies   <girish@circuitsutra.com>
 * @date      3rd September, 2011 (Saturday)
 */
#include <cci.h>           /*!Include this header in all cci-based applications*/
#include <systemc.h>

#include "parent.h"
#include "configurator.h"

#include "gs_cci_cnf_private_broker_accessor.h"

/**
 *  @brief   This sc_module constructs both the PARENT module and the CONFIGURATOR
 *           module.  This also creates a (private broker) responsible for the
 *           PARENT module and all the public parameters of the parent and the
 *           child modules have be enlisted and fed to the respective broker.
 */
SC_MODULE(Top)
{
	public	:

		SC_CTOR(Top)
			// cci::cnf::gs_cci_private_broker_accessor(sc_core::sc_module& owner, std::vector<std::string> public_parameters)
		: privBroker (new cci::cnf::gs_cci_private_broker_accessor(*this, 
				            boost::assign::list_of("parent_inst.parent_int_buffer")("parent_inst.child_inst.pub_int_param")))
			// Broker instantiated above is to be passed as an argument to the parent module
		, parent_inst(new parent("parent_inst", privBroker))
		, param_cfgr("param_cfgr")
		{
			// Nothing to implement
		}

		~Top()
		{
			delete parent_inst;
			delete privBroker;
		}

	protected	:
		cci::cnf::cci_cnf_broker_if* privBroker;
		parent*  									   parent_inst;
		configurator                 param_cfgr;

};// End of SC_MODULE


/**
 * @brief      The TOP_MODULE is instantiated within this sc_main function
 */
int sc_main(int sc_argc, char* sc_argv[])
{
	/// Instantiating TOP module
	Top      Top("Top");

	// Start and run the simulation till 50 nanoseconds
	sc_core::sc_start(26.0, sc_core::SC_NS);

	return EXIT_SUCCESS;

}// End of MAIN
