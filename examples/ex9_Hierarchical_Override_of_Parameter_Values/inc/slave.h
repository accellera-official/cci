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

/*
 * @file     slave.h
 * @brief    This file declares and implements the functionality of the slave.
 *           Few of the parameters of the slave sc_module are configured by the 
 *           router sc_module
 * @author   P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date     29th April, 2011 (Friday) : 10:55 hrs IST
 */
#ifndef SLAVE_H
#define SLAVE_H

// This header must be included in all cci-applications
#include <cci.h>

// These headers are included for tlm2 communication
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"

/*
 * @brief    This module implements the slave functionality and contains tlm 
 *           socket for communication with router
 * @author   P V S Phaneendra, CircuitSutra Technologies Pvt. Ltd.
 * @date     29th April, 2011 (Friday) : 11:00 hrs IST
 *           (Last Modified on) 3rd May, 2011 (Tuesday)
 */
SC_MODULE(slave)
{

	public :
		
		tlm_utils::simple_target_socket<slave,32> Slave_socket;
		sc_time read_latency,write_latency;

		/// Slave's Default Constructor
		SC_CTOR(slave)
		:	Slave_socket("Slave_socket")
		,	slave_ID("slave_ID", "slave_default")
		,	s_base_addr("s_base_addr", 0)
		,	s_size("s_size", 256)
		{
			std::cout << "\n[" << slave_ID.json_serialize() << " C_TOR] --------- [SLAVE CONSTRUCTOR BEGINS HERE] ----------" << endl;
			std::cout << "[" << slave_ID.json_serialize() << " C_TOR] : Base Address : " << s_base_addr.get() << endl;

			// Registering b_transport 
			Slave_socket.register_b_transport(this,&slave::b_transport);
			
			write_latency=sc_time(3,SC_NS);
			read_latency=sc_time(5,SC_NS);
			
			mem = new int[s_size.get()];			

			for(unsigned int i=0; i < s_size.get(); i++)
				mem[i] = 0xAABBCCDD | i;

			/// Slave's SC_THREAD declaration
			SC_THREAD(run_slave);
		}

		/// Slave's SC_THREAD implementation
		void run_slave (void)
		{
			// Implements no functionality				
		}
		
		/*
 		 * @brief      :  TLM2.0 b_transport layer callback
 		 * @param[in]     tlm::tlm_generic_payload&
 		 * @param[in]     sc_time&
 		 * @param[out]    void
 		 * @author     :  Parvinder Pal Singh, CircuitSutra Technologies Pvt. Ltd.
 		 * @date       :  5th May, 2011 (Thursday)
 		 */
		void b_transport(tlm::tlm_generic_payload& trans,sc_time& delay)
		{
			tlm::tlm_command cmd = trans.get_command();
	  	sc_dt::uint64    adr =  trans.get_address() - s_base_addr.get();
	  	unsigned char*   ptr = trans.get_data_ptr();
			unsigned int     len = trans.get_data_length();
			unsigned char*   byt = trans.get_byte_enable_ptr();
			unsigned int     wid = trans.get_streaming_width();
			
			std::cout << "[SLAVE] : adr ---- " << adr << endl;
			std::cout << "[SLAVE] : base addr ---- " << s_base_addr.get() << endl;

			// Check for storage address overflow
			if (adr > s_size.get())	{ 
				trans.set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE ); 
				return;}// End of IF 

			// Target unable to support byte enable attribute
			if (byt)	{ 																			 
				trans.set_response_status( tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE ); 
				return;}// End of IF

			// Target unable to support streaming width attribute
 			if (wid < len)	{																 
				trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE ); 
				return;}// End of IF

			if (cmd == tlm::TLM_READ_COMMAND)	{		
				memcpy(ptr, &mem[adr], len);
				delay=delay+read_latency;
			}	else if ( cmd == tlm::TLM_WRITE_COMMAND)	{	
				memcpy(&mem[adr], ptr, len);
				delay=delay+write_latency;
			}// End of IF

			trans.set_response_status(tlm::TLM_OK_RESPONSE);
		}// End of b_transport layer callback

	private :

		/// Elaboration Time Parameter for assigning slave ID (initialized by top_module)
		cci::cnf::cci_param<std::string, cci::cnf::elaboration_time_parameter> slave_ID;
		
		/// Mutable time parameter for setting slave's base address (initialized by router)
		cci::cnf::cci_param<int, cci::cnf::elaboration_time_parameter> s_base_addr;

		/// Mutable time parameter for setting slave's size (initialized by router);
		cci::cnf::cci_param<unsigned int>	s_size;

		int* mem;
};// End of SC_MODULE

#endif	// End of SLAVE_H
