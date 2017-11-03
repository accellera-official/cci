/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 ****************************************************************************/

/**
 *  @file   simple_ip.h
 *  @brief  A simple IP that has a mutable integer parameter
 *  @author R. Swaminathan, TI
 */

#ifndef EXAMPLES_EX26_SIMPLE_INT_PARAM_EX26_SIMPLE_IP_H_
#define EXAMPLES_EX26_SIMPLE_INT_PARAM_EX26_SIMPLE_IP_H_

#include <cci_configuration>
#include <tlm.h>
#include <tlm_utils/simple_target_socket.h>
#include "xreport.hpp"
#include <sys/time.h>

/**
 *  @class  ex26_simple_ip
 *  @brief  A simple IP which owns a CCI param.
 */
SC_MODULE(ex26_simple_ip) {
 public:

  tlm_utils::simple_target_socket<ex26_simple_ip> socket;


  /**
   *  @fn     void ex26_simple_ip()
   *  @brief  The constructor for the class.
   *  @return void
   */
  SC_CTOR(ex26_simple_ip)
    : socket("simple_ip_socket")
    // Initialize int_param with 0
    , int_param("int_param", 0)
    , normal_int(0)
  {

    socket.register_b_transport(this, &ex26_simple_ip::b_transport);


    SC_THREAD(execute);
    XREPORT("Ctor for simple IP complete");
  }

  /**
   *  @fn     void ~ex26_simple_ip()
   *  @brief  The destructor for the class.
   *  @return void
   */
  ~ex26_simple_ip() {
    XREPORT_PLAIN("Dtor: Current value of " << int_param.get_name() << " is "
                  << int_param);
  }

  /**
   *  @fn     void execute()
   *  @brief  The main execution block (no real functionality)
   *  @return void
   */
  void execute() {
    // Update int_param value to 1
    XREPORT("execute: Set value of int_param to 1");
    int_param = 1;
    XREPORT("execute: Current value of int_param is "<< int_param);

  }

  void b_transport( tlm::tlm_generic_payload& trans, sc_core::sc_time& delay )
  {
    int i=*(trans.get_data_ptr());
    if (i) {
      int_param = int_param + 1; // do a bit of reading and writing....
    } else {
      normal_int = normal_int + 1;
    }
  }

  
 private:
  cci::cci_param<int> int_param; ///< CCI param to hold buffer size
  int normal_int;
};
// ex26_simple_ip

#endif  // EXAMPLES_EX26_SIMPLE_INT_PARAM_EX26_SIMPLE_IP_H_

