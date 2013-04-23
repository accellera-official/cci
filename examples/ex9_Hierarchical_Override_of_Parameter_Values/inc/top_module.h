/******************************************************************************** 
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
 ********************************************************************************/

/*!
 * \file     top_module.h
 * \brief    Implementation the TOP_MODULE.
 *           This header contains code related to the top module which decides
 *           the model hierarchy for example#9.  
 * \author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *           Girish Verma, CircuitSutra Technologies      <girish@circuitsutra.com>
 *           Parvinder Pal Singh, CircuitSutra Technologies   <parvinder@circuitsutra.com>
 * \date     29th April, 2011 (Friday)
 */
#ifndef TOP_MODULE_H
#define TOP_MODULE_H

#include <cci>      // To be included in all CCI-based applications
#include <assert.h>
#include <vector>
#include <sstream>

#include "master.h"
#include "slave.h"
#include "router.h"

#include "tlm.h"     // Include TLM2 headers

/*!
 * \class     top_module  top_module.h
 * \brief     This SC_MODULE instantiates the router, master and slave modules.
 *            Also, it creates the binding or communication between these modules.
 */
class top_module : public sc_core::sc_module
{

	public :
		
		SC_HAS_PROCESS(top_module);

		/*!
		 * \fn     top_module::top_module
		 * \brief  Top Module's Default Constructor
		 */
		top_module(sc_module_name _name)
		: sc_module(_name)
		,	n_masters("number_of_masters", 0)
		,	n_slaves("number_of_slaves", 0)
		{
			std::stringstream ss;

			std::cout << "\n[TOP_MODULE C_TOR] ----- [TOP MODULE CONSTRUCTOR BEGINS HERE] -----" << endl;


			/// Get handle of the default broker
			myDefaultBroker = &cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));


			/// Assert if broker handle returned is NULL
			assert(myDefaultBroker != NULL && "Default broker accessed by TOP_MODULE is NULL");	

			std::cout << "\n[TOP_MODULE C_TOR] :  Number of masters : " << n_masters.json_serialize() << endl;
			std::cout << "[TOP_MODULE C_TOR] :  Number of slaves : " << n_slaves.json_serialize() << endl;


			/// Set and lock the number of masters in Router Table to value passed from 'sc_main'
			myDefaultBroker->set_init_value("top_module_inst.RouterInstance.r_masters", n_masters.json_serialize());
			myDefaultBroker->lock_init_value("top_module_inst.RouterInstance.r_masters");


			/// Set and lock the number of slaves in Router Table to value passed from 'sc_main'
			myDefaultBroker->set_init_value("top_module_inst.RouterInstance.r_slaves", n_slaves.json_serialize());
			myDefaultBroker->lock_init_value("top_module_inst.RouterInstance.r_slaves");


			/// Declaring and defining router module
			char routerName[15] = "RouterInstance";      /*!<Router_ID>*/
			std::cout << "\n[TOP_MODULE C_TOR] : Creating Router : " << routerName << endl;
			routerInstance = new router(routerName);


			/// Top_Module begins construction of the model hierarchy from here
			// ----------------------------------------------------------------

			if(myDefaultBroker->exists_param("top_module_inst.RouterInstance.addr_limit"))
			{
				cci::cnf::cci_base_param* r_addr_limit_ptr = myDefaultBroker->get_param("top_module_inst.RouterInstance.addr_limit");
				r_addr_max =  atoi((r_addr_limit_ptr->json_serialize()).c_str());

				std::cout << "\n[TOP_MODULE C_TOR] : Router's maximum addressable limit : " << r_addr_max << endl;
			}// End of IF

			/// Creating instances of master(s)
			for(int i = 0; i < n_masters; i++)
			{
				sprintf(masterName, "Master_%d", i);
				std::cout << "\n[TOP_MODULE C_TOR] : Creating Master : " << masterName << endl;
      
				sprintf(stringMisc, "%s.%s.master_ID", name(), masterName);
     
				myDefaultBroker->set_init_value(stringMisc, masterName);
				masterList.push_back(new master(masterName));
      
				///     Binding of Master to Router 
				std::cout<< "\n[TOP MODULE C_TOR] : Binding Router_Initiator to " << masterName <<endl;
				masterList[i]->Master_socket.bind(routerInstance->Router_target); 

			}// End of FOR  


			slaveSize = 128;                                    // Defining slave size

			/// Creating instances of slave(s)
			for(int i = 0; i < n_slaves; i++)      
			{
				sprintf(slaveName, "Slave_%d", i);
				std::cout << "\n[TOP_MODULE C_TOR] : Creating Slave : " << slaveName << endl;
     
				sprintf(stringMisc, "%s.%s.slave_ID", name(), slaveName);
				myDefaultBroker->set_init_value(stringMisc, slaveName);

				// Set initial value for maximum slave size(memory)
				sprintf(stringMisc, "%s.%s.s_size", name(), slaveName);
				ss.clear();
				ss.str("");
				ss << slaveSize;

				myDefaultBroker->set_init_value(stringMisc, ss.str());
				slaveList.push_back(new slave(slaveName));

				///       Binding Router to Slave
				std::cout<< "\n[TOP MODULE C_TOR] : Binding Router_Initiator to " << slaveName <<endl;
				routerInstance->Router_initiator.bind(slaveList[i]->Slave_socket);

			}// End of FOR  


			/// Try re-setting locked values for Router Table contents
			for(int i = 0; i < n_slaves; i++)
			{
				sprintf(slaveName, "%s.RouterInstance.r_index_%d", name(), i);
				ss.clear();
				ss.str("");
				ss << i;				

				try	{
					std::cout << "\n[TOP_MODULE C_TOR] : Re-setting fields of Slave_" << i << endl;
					myDefaultBroker->set_init_value(slaveName, ss.str());
				}	catch (sc_core::sc_report exception)	{
  				std::cout <<  "\n[ROUTER : Caught] : " << exception.what() << endl;}
	
				sprintf(slaveName, "%s.RouterInstance.r_sa_%d", name(), i);
				ss.clear();
				ss.str("");
				ss << (i*slaveSize);				
				
				sprintf(slaveBaseAddr, "%s.Slave_%d.s_base_addr", name(), i);
				myDefaultBroker->set_init_value(slaveBaseAddr, ss.str());

				try	{
					std::cout << "\n[TOP_MODULE C_TOR] : Re-setting start addr of Slave_" << i << endl;
					myDefaultBroker->set_init_value(slaveName, ss.str());
				}	catch (sc_core::sc_report exception)	{
	  				std::cout <<  "\n[ROUTER : Caught] : " << exception.what() << endl;	}
	
				sprintf(slaveName, "%s.RouterInstance.r_ea_%d", name(), i);
				ss.clear();
				ss.str("");
				ss << ((i+1)*slaveSize - 1);				


				try	{
					std::cout << "\n[TOP_MODULE C_TOR] : Re-setting end addr of Slave_" << i << endl;
					myDefaultBroker->set_init_value(slaveName, ss.str());
				}	catch (sc_core::sc_report exception)	{
	  				std::cout <<  "\n[ROUTER : Caught] : " << exception.what() << endl;	}

			}// End of FOR

		}// End of constructor
		

		/*!
		 *  \fn     top_module::~top_module
		 *  \brief  Destructor of the top module
		 */
		~top_module()
		{
		  // @TODO De-allocate all master and slave properly 
			if(!masterList.empty())
				masterList.clear();

			if(!slaveList.empty())
				slaveList.clear();
		}// End of destructor


	private :

		// Immutable type cci-parameters

		cci::cnf::cci_param<int, cci::cnf::immutable_parameter> n_masters; //!< Number of Masters to be instantiated
		cci::cnf::cci_param<int, cci::cnf::immutable_parameter> n_slaves;  //!< Number of Slaves to be instantiated

		cci::cnf::cci_cnf_broker_if* myDefaultBroker;		//!< Configuration broker instance 

		router*	routerInstance;  	//!< Declaration of a router pointer

		// STD::VECTORs for creating instances of master and slave
		std::vector<master*>  masterList;  //!< STD::VECTOR for masters
		std::vector<slave*>   slaveList;   //!< STD::VECTOR for slaves
	
		char masterName[15];               //!< Master_ID
		char slaveName[15] ;               //!< Slave_ID
		char stringMisc[50];               //!< String to be used for misc things
		char slaveBaseAddr[50];			

		int addrValue;                     //!< Address Value
		int slaveSize;                     //!< Maximum Slave Size (initial value)
		int r_addr_max;                    //!< Maximum Router Table's memory range

};// End of SC_MODULE

#endif	// End of TOP_MODULE_H
