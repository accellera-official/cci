
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

/**
 * \file     router.h
 * \brief    Definition of the router module.
 *           This file declares and implements the functionality of the router.
 *           Few of the parameters of the slave and master sc_module(s) are 
 *           configured by the router sc_module
 * \authors  P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 * \date     29th April, 2011 (Friday)
 */
#ifndef ROUTER_H
#define ROUTER_H

#include <cci>       // Include this header in all cci-base applications
#include <assert.h>
#include <vector> 

#include "tlm.h"                                          // Include the below headers for tlm2 communication
#include "tlm_utils/multi_passthrough_target_socket.h"    // TLM2 multi-passthrough target sockets
#include "tlm_utils/multi_passthrough_initiator_socket.h" // TLM2 multi-passthrough initiator sockets

/*!
 * \class     router router.h
 * \brief     This module describes the attributes and functionality of the router.
 *            Router creates its Router Table within the 'before_end_of_elaboration'
 *            callback
 */
class router : public sc_core::sc_module
{

	public :

		// Declare tlm multi-passthrough sockets for target and initiator modules	
		tlm_utils::multi_passthrough_target_socket<router,32> Router_target;      //!< multi-passthrough initiator socket
 		tlm_utils::multi_passthrough_initiator_socket<router,32> Router_initiator;//!< multi-passthrough target socket

		SC_HAS_PROCESS(router);

		/*!
		 *  \fn    router::router
		 *  \brief Router Default Constructor
		 */
		router(sc_module_name _name)
		: sc_module(_name)
		,	Router_target("Router_target")
		,	Router_initiator("Router_initiator")
		,	r_masters("r_masters", 0)
		,	r_slaves("r_slaves", 0)
		,	addr_limit("addr_max", 64) 
		, addrSize(0)
		{
			std::cout << "\n[ROUTER C_TOR] --------- [ROUTER CONSTRUCTOR BEGINS HERE] ----------" << endl;
		
			//Get handle of the broker
			myBrokerForRouter = &cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this));
			
			// Assert if NULL
			assert(myBrokerForRouter != NULL && "Router broker handle returned is NULL");

			// Register b_transport
			Router_target.register_b_transport(this,&router::b_transport);

		}// End of Constructor

		/*!
		 * \fn        void before_end_of_elaboration (void)
     * \brief     The Router Table contents are filled within this 'before_end_of_elaboration' callback
     * \param     void (Callbacks registered with the SystemC Kernel just like processes)
     * \return    void (Callbacks registered with the SystemC kernel just like processes)
     */ 
		void before_end_of_elaboration (void)
		{
			std::cout << "\n[ROUTER in beoe] : Number of master(s) : " << r_masters.json_serialize() << endl;
			std::cout << "[ROUTER in beoe] : Number of slave(s) : " << r_slaves.get() << endl;
			std::cout << "[ROUTER in beoe] : Maximum Addressable Limit of the router : " << addr_limit.get() << endl;
			
			char slaveName[10];	                                  //!< Holds router table's fields' names
			addrSize = (unsigned int)(addr_limit.get()/r_slaves);

			/// Printing the Router Table contents
			std::cout << "\n\t====================== ROUTER TABLE INFORMATION ========================" << endl;
			std::cout << "\t--------------------------------------------------------------------------------------------------" << endl;
			std::cout << "\t|\tSlave_ID\t|\tStart Address\t|\tEnd Address\t|\tBase Address\t|" << endl;
			std::cout << "\t--------------------------------------------------------------------------------------------------" << endl;

			/// Sets the contents of the routing table with (default) values calculated within 'beoe' phase
			for(int i = 0; i < r_slaves; i++)
			{
				sprintf(slaveName, "r_index_%d", i);
				r_slave_index.push_back(new cci::cnf::cci_param<unsigned int, cci::cnf::elaboration_time_param>(slaveName, i));
				
				sprintf(slaveName, "r_sa_%d", i);
				r_addr_start.push_back(new cci::cnf::cci_param<unsigned int, cci::cnf::elaboration_time_param>(slaveName, (i*addrSize)));

				sprintf(slaveName, "r_ea_%d", i);
				r_addr_end.push_back(new cci::cnf::cci_param<unsigned int, cci::cnf::elaboration_time_param>(slaveName, ((i+1)*addrSize -1)));

			}// End of FOR

			for(int i = 0; i < r_slaves; i++)
			{
				sprintf(stringName, "top_module_inst.Slave_%d.s_base_addr", i);				

				if(myBrokerForRouter->param_exists(stringName))
				{	
					base_ptr = myBrokerForRouter->get_param(stringName);
					assert(base_ptr != NULL && "Slave Base Address Handle returned is NULL");

				}// End of IF

				std::cout << "\t|\t" << r_slave_index[i]->get()
				          << "\t\t|\t" << r_addr_start[i]->get()
				          << "\t\t|\t" << r_addr_end[i]->get() 
				          << "\t\t|\t" << base_ptr->json_serialize()
				          << "\t\t|" << endl;
				std::cout << "\t--------------------------------------------------------------------------------------------------" << endl;

			}// End of FOR

		}// End of before_end_of_elaboration callback


		/*!
		 * \fn        void b_transport (int, tlm::tlm_generic_payload, sc_time &)
 		 * \brief 		TLM2.0 b_transport layer callback
 		 * \param     i    
 		 * \param     tlm::tlm_generic_payload &  transaction object
 		 * \param     sc_time &      annotated time delay
 		 * \return    void
 		 */
		void b_transport(int i,tlm::tlm_generic_payload& trans,sc_time& delay)
		{
			wait(delay);

			delay=SC_ZERO_TIME;

			sc_dt::uint64 addr	=	trans.get_address();

			if (addr >= sc_dt::uint64(addr_limit.get()))
			{ 
				trans.set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE );
				return ;
			}// End of IF

			for(unsigned int i=0; i < r_slave_index.size(); i++) {
				if ( (addr >= (r_addr_start[i]->get() )) &&  (addr <= (r_addr_end[i]->get() )) ) {
					std::cout << "\n[Router in 'b_transport' layer]" << endl;
					std::cout << "Address       = " << addr << endl;
					std::cout << "Index         = " << (r_slave_index[i])->get() << endl;
					std::cout << "Start addres  = " << (r_addr_start[i]->get()) << endl;
					std::cout << "End   Address = " << (r_addr_end[i]->get()) << endl;
					Router_initiator[(r_slave_index[i])->get()]->b_transport(trans,delay);
					break;
				}// End of IF 
			}// End of FOR

		}// End of b_transport callback


	private :


		/// Demonstrates Model-to-Model Configuration (UC12)
		/// Elaboration Time Parameters for setting up the model hierarcy;

		// Master ID assigned by the top_module upon instantiation
		cci::cnf::cci_param<int, cci::cnf::elaboration_time_param> r_masters; //!< Master ID assigned by the top_module

		// Slave ID assigned by the top_module upon instantiation
		cci::cnf::cci_param<int, cci::cnf::elaboration_time_param> r_slaves;  //!< Slave ID assigned by the top_module

		cci::cnf::cci_param<unsigned int, cci::cnf::mutable_param> addr_limit;//!< Router Addressing Range

		cci::cnf::cci_cnf_broker_if*	myBrokerForRouter;  //!< CCI configuration broker

		/// \par Router Table contents holding slaves related information
		/// \par
		/// Field#1 : Slave_Index
		/// par
		/// Field#2 : Slave#_Start_Addr
		/// \par
		/// Field#3 : Slave#_End_Addr
		std::vector< cci::cnf::cci_param<unsigned int, cci::cnf::elaboration_time_param> * > r_slave_index;
		std::vector< cci::cnf::cci_param<unsigned int, cci::cnf::elaboration_time_param> * > r_addr_start;
		std::vector< cci::cnf::cci_param<unsigned int, cci::cnf::elaboration_time_param> * > r_addr_end;

		cci::cnf::cci_base_param* base_ptr;    /*!<CCI base parameter for slave base address*/

		int addrSize;
		char stringName[50];

};// End of SC_MODULE

#endif	// End of ROUTER_H
