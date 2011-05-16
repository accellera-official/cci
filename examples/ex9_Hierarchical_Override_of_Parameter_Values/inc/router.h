
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
 * @file     router.h
 * @brief    This file declares and implements the functionality of the router.
 *           Few of the parameters of the slave and master sc_module(s) are 
 *           configured by the router sc_module
 * @authors  P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date     29th April, 2011 (Friday) : 11:23 hrs IST
 */
#ifndef ROUTER_H
#define ROUTER_H

/// Include this header in all cci-applications
#include <cci.h>
#include <assert.h>
#include <vector>

/// Headers for tlm2 communication
#include "tlm.h"
#include "tlm_utils/multi_passthrough_target_socket.h"
#include "tlm_utils/multi_passthrough_initiator_socket.h"

/**
 * @brief     This module describes the attributes and functionality of the router.
 *            Router creates its Router Table within the 'before_end_of_elaboration'
 *            callback
 * @author    P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 *            Girish Verma, CircuitSutra Technologies Pvt. Ltd.
 * @date      29th April, 2011 (Friday) : 11:35 hrs IST
 *            (Last Modified on) 4th May, 2011 (Wednesday) 
 */
SC_MODULE(router)
{

	public :
		
		tlm_utils::multi_passthrough_target_socket<router,32> Router_target;
 		tlm_utils::multi_passthrough_initiator_socket<router,32> Router_initiator;

		/// Router Default Constructor
		SC_CTOR(router)
		:	Router_target("Router_target")
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

			// Registering b_transport
			Router_target.register_b_transport(this,&router::b_transport);
		}

		/**
     * @brief     : The Router Table contents are filled within this 'before_end_of_elaboration' callback
     * @param[in]  void (Callbacks registered with the SystemC Kernel just like processes)
     * @param[out] void (Callbacks registered with the SystemC kernel just like processes)
     */ 
		void before_end_of_elaboration (void)
		{
			std::cout << "\n[ROUTER in beoe] : Number of master(s) : " << r_masters.json_serialize() << endl;
			std::cout << "[ROUTER in beoe] : Number of slave(s) : " << r_slaves.get() << endl;
			std::cout << "[ROUTER in beoe] : Maximum Addressable Limit of the router : " << addr_limit.get() << endl;
			
			char slaveName[10];	/*!<Holds router table's fields' names*/
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
				r_slave_index.push_back(new cci::cnf::cci_param<unsigned int, cci::cnf::elaboration_time_parameter>(slaveName, i));
				
				sprintf(slaveName, "r_sa_%d", i);
				r_addr_start.push_back(new cci::cnf::cci_param<unsigned int, cci::cnf::elaboration_time_parameter>(slaveName, (i*addrSize)));

				sprintf(slaveName, "r_ea_%d", i);
				r_addr_end.push_back(new cci::cnf::cci_param<unsigned int, cci::cnf::elaboration_time_parameter>(slaveName, ((i+1)*addrSize -1)));
			}

			for(int i = 0; i < r_slaves; i++)
			{
				sprintf(stringName, "top_module_inst.Slave_%d.s_base_addr", i);				

				if(myBrokerForRouter->exists_param(stringName))
				{	
					base_ptr = myBrokerForRouter->get_param(stringName);
					assert(base_ptr != NULL && "Slave Base Address Handle returned is NULL");
				}
				std::cout << "\t|\t" << r_slave_index[i]->get()
				          << "\t\t|\t" << r_addr_start[i]->get()
				          << "\t\t|\t" << r_addr_end[i]->get() 
				          << "\t\t|\t" << base_ptr->json_serialize()
				          << "\t\t|" << endl;
				std::cout << "\t--------------------------------------------------------------------------------------------------" << endl;
			}// End of FOR*/
		}// End of before_end_of_elaboration callback

		/// TLM2.0 b_transport layer callback 
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

		/// Master ID assigned by the top_module upon instantiation
		cci::cnf::cci_param<int, cci::cnf::elaboration_time_parameter> r_masters;

		/// Slave ID assigned by the top_module upon instantiation
		cci::cnf::cci_param<int, cci::cnf::elaboration_time_parameter> r_slaves;

		/// Router Addressing Range
		cci::cnf::cci_param<unsigned int, cci::cnf::mutable_parameter> addr_limit;

		/// CCI configuration broker
		cci::cnf::cci_cnf_broker_if*	myBrokerForRouter;

		/// Router Table contents holding slaves related information
		/// Field#1 : Slave_Index; Field#2 : Slave#_Start_Addr; Field#3 : Slave#_End_Addr
		std::vector< cci::cnf::cci_param<unsigned int, cci::cnf::elaboration_time_parameter> * > r_slave_index;
		std::vector< cci::cnf::cci_param<unsigned int, cci::cnf::elaboration_time_parameter> * > r_addr_start;
		std::vector< cci::cnf::cci_param<unsigned int, cci::cnf::elaboration_time_parameter> * > r_addr_end;

		cci::cnf::cci_base_param* base_ptr;    /*!<CCI base parameter for slave base address*/

		int addrSize;
		char stringName[50];
};// End of SC_MODULE

#endif	// End of ROUTER_H
