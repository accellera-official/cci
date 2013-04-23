/*******************************************************************************
  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2010 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.
******************************************************************************/

/**
 *  @file   simple_ip.h
 *  @brief  A simple IP that has a mutable integer parameter
 *  @author R. Swaminathan, TI
 */

#ifndef INC_SIMPLE_IP_H
#define INC_SIMPLE_IP_H

//------------------------------------------------------------------------------
#include "cci.h"
#include "xreport.hpp"

//------------------------------------------------------------------------------
/// The SimpleIP owns a CCI param
SC_MODULE(simple_ip)
{
 public:
  /// Ctor
 SC_CTOR(simple_ip)
   // Initialize int_param with 0
   : int_param_ip("int_param_ip"                         // Param name - In line with SystemC Hierarchy
                  , 1)                                   // Default value
    
    , int_param_ip_dup("sim_ip.int_param_ip"             // Param name (Duplicate)
                       , 2                               // Default value
                       , cci::cnf::CCI_TOP_LEVEL_NAME    // Force to top-level
                       , cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this))) // Get Global broker

    , int_param_top("int_param_top"                      // Param name - Absolute
                    , 3                                  // Default value
                    , cci::cnf::CCI_TOP_LEVEL_NAME       // Force to top-level
                    , cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this))) // Get Global broker

    , int_param_custom("top.sub.int_param_custom"        // Param name - Custom
                       , 4                               // Default value
                       , cci::cnf::CCI_TOP_LEVEL_NAME    // Force to top-level
                       , cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this))) // Get global broker
    {
      SC_THREAD(execute);
      XREPORT("Ctor: Default value of "<< int_param_ip.get_name()<< " is "<< int_param_ip);
      XREPORT("Ctor: Default value of "<< int_param_ip_dup.get_name()<< " is "<< int_param_ip_dup);
      XREPORT("Ctor: Default value of "<< int_param_top.get_name()<< " is "<< int_param_top); 
      XREPORT("Ctor: Default value of "<< int_param_custom.get_name()<< " is "<< int_param_custom); 
    }

  ~simple_ip() {
      XREPORT_PLAIN("Dtor: Current value of "<< int_param_ip.get_name()<< " is "<< int_param_ip);
      XREPORT_PLAIN("Dtor: Current value of "<< int_param_ip_dup.get_name()<< " is "<< int_param_ip_dup);
      XREPORT_PLAIN("Dtor: Current value of "<< int_param_top.get_name()<< " is "<< int_param_top); 
      XREPORT_PLAIN("Dtor: Current value of "<< int_param_custom.get_name()<< " is "<< int_param_custom); 
  }

  /// Main execution block (no functionality)
  void execute() {
    // Wait for 10ns to allow config_ip to update int_param value
    wait(10, sc_core::SC_NS);

    XREPORT("execute: Current value of "<< int_param_ip.get_name()    << " is "<< int_param_ip);
    XREPORT("execute: Current value of "<< int_param_ip_dup.get_name()<< " is "<< int_param_ip_dup);
    XREPORT("execute: Current value of "<< int_param_top.get_name()   << " is "<< int_param_top);
    XREPORT("execute: Current value of "<< int_param_custom.get_name()<< " is "<< int_param_custom);

    // Update param values
    XREPORT("execute: Set int_param_ip = 10"); 
    XREPORT("execute: Set int_param_ip_dup = 20"); 
    XREPORT("execute: Set int_param_top = 30"); 
    XREPORT("execute: Set int_param_custom = 40"); 
    int_param_ip     = 10;
    int_param_ip_dup = 20;
    int_param_top    = 30;
    int_param_custom = 40;
    XREPORT("execute: Current value of "<< int_param_ip.get_name()    << " is "<< int_param_ip);
    XREPORT("execute: Current value of "<< int_param_ip_dup.get_name()<< " is "<< int_param_ip_dup);
    XREPORT("execute: Current value of "<< int_param_top.get_name()   << " is "<< int_param_top);
    XREPORT("execute: Current value of "<< int_param_custom.get_name()<< " is "<< int_param_custom);
  }

 private:
  /// CCI param to hold buffer size
  cci::cnf::cci_param<int> int_param_ip;
  cci::cnf::cci_param<int> int_param_ip_dup;
  cci::cnf::cci_param<int> int_param_top;
  cci::cnf::cci_param<int> int_param_custom;
}; // simple_ip

#endif // INC_SIMPLE_IP_H

