//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//   Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
//   This program is free software.
// 
//   If you have no applicable agreement with GreenSocs Ltd, this software
//   is licensed to you, and you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   If you have a applicable agreement with GreenSocs Ltd, the terms of that
//   agreement prevail.
// 
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
//   02110-1301  USA 
// 
// ENDLICENSETEXT

#ifndef __TG_H__
#define __TG_H__


#include <iostream>
#include "boost/shared_ptr.hpp"

#include "demo_globals.h"
#include "time_measure.h"
#include "greencontrol/core/helpfunctions.h"

// SimpleBus API
#include "greenbus/api/simplebusAPI.h"

// GreenControl and GreenConfig
#include "greencontrol/gcnf/apis/scmlApi/scml_property.h"
#include "greencontrol/config.h"

#ifdef ENABLE_SCML_NAMESPACE
using gs::cnf::scml_property;
#endif

/// Traffic Generator for demonstration platform
/**
 * Based on pseudo_master_simplebus.h .cpp (by Wolfgang Klingauf)
 * 
 * Writes and reads alternately to and from the addresses of the 
 * write_addr[]- and read_addr[]-arrays.
 */
class TG
: public sc_core::sc_module
{
  
public:
  tlm::simplebusMasterPort master_port;

private:
  scml_property<gs_uint32> init_size;    // data vector size (1MB)
  scml_property<gs_uint32> write_size;   // write bytes (1024)
  scml_property<gs_uint32> read_size;    // read bytes (1024)
  scml_property<gs_uint32> clk_period;   // clk period in ns (10)
  scml_property<gs_uint32> write_inc;    // increment write byte size per loop (0)
  scml_property<gs_uint32> read_inc;     // increment read byte size per loop (0)
  scml_property<gs_uint32> loops;        // # benchmark loops (10)
  scml_property<gs_uint32> read_count;   // # read transactions per measure (10)
  scml_property<gs_uint32> write_count;  // # write transactions per measure (10)
  scml_property<gs_uint32> write_count_inc; // increment number of write transaction per measure (0)
  scml_property<gs_uint32> read_count_inc;  // increment number of write transaction per measure (0)
  scml_property<gs_uint32> write_count_rnd; // # write transactions is in given random range (0)
  scml_property<gs_uint32> read_count_rnd;  // # read transactions is in given random range (0)
  scml_property<gs_uint32> delay_max;    // max. random clk cycle delay between transactions (100)

  std::vector<gs_uint64> write_addr;      // addresses were to write to
  std::vector<gs_uint64> read_addr;       // addresses were to read from
  std::vector<gs_uint64> write_addr_size; // size of the address space write_addr[i]
  std::vector<gs_uint64> read_addr_size;  // size of the address space read_addr[i]
  std::vector<std::string> target_names;  // names of the targets

  unsigned int target;      // target which is addressed

public:
  SC_HAS_PROCESS(TG);
	
  /// Constructor
  TG(sc_core::sc_module_name name)
    : sc_core::sc_module(name),
      master_port("iport"),

      init_size("init_size", 1048576), // 1MB
      write_size("write_size", 1024),
      read_size("read_size", 1024),
      clk_period("clk_period", 10),
      write_inc("write_inc", 0),
      read_inc("read_inc", 0),
      loops("loops", 2),
      read_count("read_count", 100000),
      write_count("write_count", 100000),
      write_count_inc("write_count_inc", 0),
      read_count_inc("read_count_inc", 0),
      write_count_rnd("write_count_rnd", 0),
      read_count_rnd("read_count_rnd", 0),
      delay_max("delay_max", 10000),
      target(0)
  { 
    m_GCnf_Api = gs::cnf::GCnf_Api::getApiInstance(this);
    
    srand(rand()); // random seed to make multiple master behaviour differ
    //tcount = 0;

    // Pointer to a gs_uint64 parameter
    gs::gs_param<gs_uint64> *tmp_param64;
    // Pointer to a gs_uint32 parameter
    gs::gs_param<gs_uint32> *tmp_param32;

    target_names.resize(3);
    write_addr.resize(3);
    read_addr.resize(3);
    write_addr_size.resize(3);
    read_addr_size.resize(3);
    
    // Addresses for PCIeDevice1
    target_names.at(0) = "PCIeDevice1";
    tmp_param64 = dynamic_cast<gs::gs_param<gs_uint64>* >( m_GCnf_Api->getPar("PCIeDevice1.base_addr") );
    write_addr[0] = read_addr[0] = *tmp_param64;
    cout << "("<<this->name()<<"):" << "write and read address for PCIeDevice1 set to 0x" << hex<<*tmp_param64<<dec<<"="<<write_addr[0] << endl;
    // size of device at address write_addr[i]    
    tmp_param32 = dynamic_cast<gs::gs_param<gs_uint32>* >( m_GCnf_Api->getPar("PCIeDevice1.mem_size") );
    write_addr_size[0] = read_addr_size[0] = *tmp_param32;
    cout << "("<<this->name()<<"):" << "write and read size for PCIeDevice1 set to " << write_addr_size[0] << endl;
    
    // Addresses for PCIeDevice2
    target_names.at(1) = "PCIeDevice2";
    tmp_param64 = dynamic_cast<gs::gs_param<gs_uint64>* >( m_GCnf_Api->getPar("PCIeDevice2.base_addr") );
    write_addr[1] = read_addr[1] = *tmp_param64;
    cout << "("<<this->name()<<"):" << "write and read address for PCIeDevice2 set to 0x" << hex<<*tmp_param64<<dec<<"="<<write_addr[1] << endl;
    // size of device at address write_addr[i]    
    tmp_param32 = dynamic_cast<gs::gs_param<gs_uint32>* >( m_GCnf_Api->getPar("PCIeDevice2.mem_size") );
    write_addr_size[1] = read_addr_size[1] = *tmp_param32;
    cout << "("<<this->name()<<"):" << "write and read size for PCIeDevice2 set to " << write_addr_size[1] << endl;
    
    // Addresses for Mem
    target_names.at(2) = "Mem";
    tmp_param64 = dynamic_cast<gs::gs_param<gs_uint64>* >( m_GCnf_Api->getPar("Mem.base_addr") );
    write_addr[2] = read_addr[2] = *tmp_param64;
    cout << "("<<this->name()<<"):" << "write and read address for Mem set to 0x"<<hex<<*tmp_param64<<dec<<"="<<write_addr[1] << endl;
    // size of device at address write_addr[i]    
    tmp_param64 = dynamic_cast<gs::gs_param<gs_uint64>* >( m_GCnf_Api->getPar("Mem.high_addr") );
    write_addr_size[2] = read_addr_size[2] = *tmp_param64 - read_addr[2];
    cout << "("<<this->name()<<"):" << "write and read size for Mem set to " << *tmp_param64 <<"-"<< read_addr[2] << "="<< write_addr_size[2] << endl;

    SC_THREAD(action);
  }
  
  /// SC_THREAD: Main action creates the test transactions.
  void action();

private:
  void read(gs_uint32 read_size, gs_uint64 read_addr);

  void write(gs_uint32 write_size, gs_uint64 write_addr);

  void read();
  void write();
  bool doWrite();
  bool doRead();
  void updateWrite();
  void updateRead();
  gs_uint32 getRandom(gs_uint32);
  gs_uint64 getRandomAddress(gs_uint64, gs_uint64);
  void end_of_simulation();
  void show_parameters();  

  /// Count the target address
  void updateTarget();
  
private:
  std::vector<gs_uint8> mData;
  timeMeasure tm;	

  /// Counter to fill data
  gs::ctr::letter_digit_iterator letter_digit;
  
  /// GCnf API
  gs::cnf::cnf_api *m_GCnf_Api;

};


#endif
