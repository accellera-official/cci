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
 * \file     slave.h
 * \brief    Slave module implementation.
 *           This file declares and implements the functionality of the slave.
 *           Few of the parameters of the slave sc_module are configured by the 
 *           router sc_module
 * \author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *           Parvinder Pal Singh, CircuitSutra Technologies   <parvinder@circuitsutra.com>
 * \date     5th May, 2011 (Thursday)
 */
#ifndef SLAVE_H
#define SLAVE_H

#include <cci>  // This header must be included in all cci-applications

#include "tlm.h"                            // TLM2 headers
#include "tlm_utils/simple_target_socket.h" // TLM2 simple initiator sockets

/*!
 * \class    slave  slave.h
 * \brief    This module implements the slave functionality and contains tlm 
 *           socket for communication with router
 */
class slave : public sc_core::sc_module
{

	public :
		
		tlm_utils::simple_target_socket<slave,32> Slave_socket; //!< Simple Initiator Socket
		sc_time read_latency,write_latency;                     //!< latency

		SC_HAS_PROCESS(slave);

		/*!
		 * \fn     slave::slave
		 * \brief   Slave's Default Constructor
		 */
		slave(sc_module_name _name)
		: sc_module(_name)
		,	Slave_socket("Slave_socket")
		,	slave_ID("slave_ID", "slave_default")
		,	s_base_addr("s_base_addr", 0)
		,	s_size("s_size", 256)
		{
			std::cout << "\n[" << slave_ID.json_serialize() << " C_TOR] --------- [SLAVE CONSTRUCTOR BEGINS HERE] ----------" << endl;
			std::cout << "[" << slave_ID.json_serialize() << " C_TOR] : Base Address : " << s_base_addr.get() << endl;

			/// Register b_transport 
			Slave_socket.register_b_transport(this,&slave::b_transport);
			
			write_latency=sc_time(3,SC_NS);
			read_latency=sc_time(5,SC_NS);
			
			mem = new int[s_size.get()];			

			for(unsigned int i=0; i < s_size.get(); i++)
				mem[i] = 0xAABBCCDD | i;

			/// Slave's SC_THREAD declaration
			SC_THREAD(run_slave);

		}// End of constructor


		/*!
		 * \fn     slave::~slave()
		 * \brief  Slave's default destructor
		 */
		~slave()
		{
			// Nothing to destruct
		} 


		/*!
		 * \fn     void run_slave void ()
		 * \brief  Slave's SC_THREAD implementation
		 */
		void run_slave (void)
		{
			// Implements no functionality				
		}
		
		/*!
		 * \fn            void b_transport(tlm::tlm_generic_payload&, sc_time&)
 		 * \brief         TLM2.0 b_transport layer callback
 		 * \param         tlm::tlm_generic_payload&  Transaction payload object
 		 * \param         sc_time&                   Annotated time delay
 		 * \return        void
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

		// Elaboration Time Parameter for assigning slave ID (initialized by top_module)
		cci::cnf::cci_param<std::string, cci::cnf::elaboration_time_param> slave_ID; //!< Elaboration Time Parameter for Slave ID
		
		// Mutable time parameter for setting slave's base address (initialized by router)
		cci::cnf::cci_param<int, cci::cnf::elaboration_time_param> s_base_addr; //!< Slave's base address*/

		// Mutable time parameter for setting slave's size (initialized by router);
		cci::cnf::cci_param<unsigned int>	s_size;  //!< Slave's size

		int* mem;

};// End of SC_MODULE

#endif	// End of SLAVE_H
