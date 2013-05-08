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

/*!
 * \file      main.cpp
 * \brief     This file instantiates the TOP module which, in turn, instantiates
 *            the PARENT and the CONFIGURATOR modules.  
 * \author    P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *            Girish Verma, CircuitSutra Technologies   <girish@circuitsutra.com>
 * \date      3rd September, 2011 (Saturday)
 */
#include <cci>             // Include this header in all cci-based applications
#include <systemc.h>       // SystemC Headers

#include "parent.h"
#include "configurator.h"

#include "gs_cci_cnf_private_broker_accessor.h"

/*!
 * \class   Top  Top.h
 * \brief   This sc_module constructs both the PARENT module and the CONFIGURATOR
 *          module.  This also creates a (private broker) responsible for the
 *          PARENT module and all the public parameters of the parent and the
 *          child modules have be enlisted and fed to the respective broker.
 */
class Top : public sc_core::sc_module
{
	public	:

		SC_HAS_PROCESS(Top);
	
		/*!
		 * \fn      Top::Top(sc_module_name)
		 * \brief   Constructor
		 */
		Top(sc_module_name _name)
		: sc_module(_name)
			/// \par Register the cci-parameters of the PARENT & CHILD modules to the PRIVATE BROKER & GLOBAL BROKER
			/// \par
			/// 1. Instantiate a broker immediately above the 'desired' module
			/// cci::cnf::gs_cci_private_broker_accessor(sc_core::sc_module& owner, std::vector<std::string> public_parameters)
			/// \par
			/// 2. Feed this broker's instance to the 'desired' module 
			/// parent_inst(new parent("parent_inst", privBroker))
		, privBroker (new cci::cnf::gs_cci_private_broker_accessor(*this, 
				            boost::assign::list_of("parent_inst.parent_int_buffer")("parent_inst.child_inst.pub_int_param")))
			// Broker instantiated above is to be passed as an argument to the parent module
		, parent_inst(new parent("parent_inst", privBroker))
		, param_cfgr("param_cfgr")
		{
			// Nothing to implement
		}

		/*!
		 * \fn     Top::~Top
		 * \brief  Destructor
		 */	
		~Top()
		{
			delete parent_inst;
			delete privBroker;
		}

	protected	:
		cci::cnf::cci_cnf_broker_if* privBroker;  //!< Broker that hides the parameters not passed to it as argument
		parent*  									   parent_inst; //!< Parent module pointer
		configurator                 param_cfgr;  //!< Configurator module instance

};// End of SC_MODULE


/*!
 * \fn         int sc_main (int, char **)
 * \brief      The TOP_MODULE is instantiated within this sc_main function
 */
int sc_main(int sc_argc, char* sc_argv[])
{
	/// Instantiate TOP module
	Top      Top("Top");

	// Start and run the simulation
	sc_core::sc_start(26.0, sc_core::SC_NS);

	return EXIT_SUCCESS;

}// End of MAIN
