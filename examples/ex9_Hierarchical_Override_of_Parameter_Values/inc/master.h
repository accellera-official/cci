
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
 * @file     master.h
 * @brief    This file declares and implements the functionality of the master.
 *           Few of the parameters of the master sc_module are configured by the
 *           router sc_module
 * @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *           Parvinder Pal Singh, CircuitSutra Technologies   <parvinder@circuitsutra.com>
 * @date     29th April, 2011 (Friday) : 11:37 hrs IST
 */
#ifndef MASTER_H
#define MASTER_H

/// This header file must be included in every cci-based application
#include <cci.h>

/// Includes the definitions for TLM2 and TLM2 Communication
#include "tlm.h"                 
#include "tlm_utils/simple_initiator_socket.h" 

/**
 * @brief     This module implements the master functionality and contains
 *            tlm2-based socket for communication with the router
 */
SC_MODULE(master)
{

	public :

		int data;	

		/// Declare instance of tlm2-base simple initiator socket
		tlm_utils::simple_initiator_socket<master,32> Master_socket;

		/// Master's Default Constructor
		SC_CTOR(master)
		: data(0)
		,	Master_socket("Master_socket")
		,	master_ID("master_ID", "master_default")
		{
			std::cout << "\n[" << master_ID.json_serialize() << " C_TOR] --------- [MASTER CONSTRUCTOR BEGINS HERE] ----------" << endl;
			
			/// Master's SC_THREAD declaration
			SC_THREAD(run_master);
		}

		/** 
		 * @brief     :   This sc_thread contains the description for tlm2
		 *                socket and communication with router
		 */ 
		void run_master (void)
		{
			tlm::tlm_generic_payload *trans= new tlm::tlm_generic_payload;

			int i=0;

			while(1)
			{
				tlm::tlm_command cmd=static_cast<tlm::tlm_command>( rand() % 2);
        
				if (cmd==tlm::TLM_WRITE_COMMAND)
					data=0xFF000000 | i;

				trans->set_command(cmd);
				trans->set_address(i);
				trans->set_data_ptr(reinterpret_cast<unsigned char*>(&data));
				trans->set_data_length(4);
				trans->set_streaming_width(4);
				trans->set_byte_enable_ptr(0);
				trans->set_dmi_allowed(false);
				trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
				sc_time delay=sc_time(0,SC_NS);
               	          	     	       	          	     	           
				if (cmd==tlm::TLM_WRITE_COMMAND)
				{
					std::cout << "[Initiators Message]=>At address " << hex<< i << " sending transaction with command = Write"
						<< ", data=" << hex <<data  << " at time " << sc_time_stamp() <<endl;
				}
				else
				{
					std::cout << "[Initiators Message]=>At address "<< hex << i << " sending transaction with command= Read "
						<< " at time " << sc_time_stamp() <<endl;
				}// End of IF	
				
				
				Master_socket->b_transport(*trans,delay);  
        
				if( trans->is_response_error())
					SC_REPORT_ERROR("TLM_2",trans->get_response_string().c_str());
        
				if(delay.to_double()!= 0)
					wait(delay);
				
				if (cmd==tlm::TLM_WRITE_COMMAND)
				{
					std::cout <<"[Initiators Message]=>At address "<< hex<< i <<" received response of Write transaction "
						<< " at time " << sc_time_stamp() <<endl;
				}
				else
				{
        	std::cout <<"[Initiators Message]=>At address "<< hex<< i <<" received response of Read transaction "
        		<<" data "<< data  << " at time " << sc_time_stamp() <<endl;
				}// End of IF	
					
				std::cout << "---------------------------------------------------------------------" << endl;
				
				wait(5.0,SC_NS);	// Wait for some delay

				i=i+4;

			}// End of WHILE             

		}// End of SC_THREAD


	private :
		
		/// Elaboration Time Parameter for assigning master ID (initialized by top_module)
		cci::cnf::cci_param<std::string, cci::cnf::elaboration_time_parameter> master_ID;

};// End of SC_MODULE

#endif	// End of MASTER_H
