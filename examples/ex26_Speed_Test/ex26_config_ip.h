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
 *  @file   config_ip.h
 *  @brief  Configure simple_ip's int_param (simple integer) parameter
 *  @author R. Swaminathan, TI
 */

#ifndef EXAMPLES_EX26_SIMPLE_INT_PARAM_EX26_CONFIG_IP_H_
#define EXAMPLES_EX26_SIMPLE_INT_PARAM_EX26_CONFIG_IP_H_

#include <cci_configuration>
#include <string>
#include "xreport.hpp"
#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>


#define LOTS 100000000

/**
 *  @class  ex26_config_ip
 *  @brief  The config_ip configures simple_ip's parameter.
 */
SC_MODULE(ex26_config_ip) {

  tlm_utils::simple_initiator_socket<ex26_config_ip> socket;


 public:
  /**
   *  @fn     void ex26_config_ip()
   *  @brief  The constructor for the class.
   *  @return void
   */
SC_CTOR(ex26_config_ip):
    // Get CCI configuration handle specific for this module
    socket("config_master_socket"),
    m_broker(cci::cci_get_broker()),
    verbose("verbose", false)
  {
    SC_THREAD(execute);
  }

  /**
   * @fn   long int getms()
   * @return the current time in ms
   */
 long int getms() 
 {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000 + tp.tv_usec / 1000;
 }
 

  /**
   *  @fn     void execute()
   *  @brief  Configure the value of "sim_ip.int_param" parameter
   *  @return void
   */
  void execute() {

    tlm::tlm_generic_payload p;
    sc_core::sc_time time(sc_core::SC_ZERO_TIME);

    long int t1, t2, t3, t4;
    
    t1=getms();
    int j = 1;
    for (unsigned long i = 0; i < LOTS; i++) {
      p.set_data_ptr((unsigned char*)&j);
      socket->b_transport( p, time );
    }
    t2=getms();

    t3=getms();
    j = 0;
    for (unsigned long i = 0; i < LOTS; i++) {
      p.set_data_ptr((unsigned char*)&j);
      socket->b_transport( p, time );
    }
    t4=getms();

    if (verbose) {
      XREPORT("Time for " << LOTS << " long long param writes "<< t2-t1);
      XREPORT("Time for " << LOTS << " long long writes "<< t4-t3);
      XREPORT("Parameters are "<< (float)(t2-t1)/(t4-t3) << "x Slower than normal variables");
    } else {
      if ( (float)(t2-t1)/(t4-t3) < 10 ) {
        XREPORT("Parameters are no more than 10x slower than normal variables!");
      }
    }   

    std::ostringstream ss;
    const std::string int_param_name = "sim_ip.int_param";

    // Check for existence of the param
    if (m_broker.param_exists(int_param_name)) {

      // Get handle to the param
      cci::cci_param_handle int_param_handle = m_broker.get_param_handle(int_param_name);
      sc_assert(int_param_handle.is_valid());
      
      // Display value
      std::string new_value = int_param_handle.get_cci_value().to_json();
      XREPORT("execute: [EXTERNAL] Current value of "
              << int_param_handle.get_name() << " is " << new_value);
    } else {
      XREPORT_ERROR("execute: Param (" << int_param_name<< ") is not found!");
    }
  }

 private:
  cci::cci_broker_handle m_broker; ///< CCI configuration handle
  cci::cci_param<bool> verbose; ///< CCI param to indicate verbosity

};
// ex26_config_ip

#endif  // EXAMPLES_EX26_SIMPLE_INT_PARAM_EX26_CONFIG_IP_H_
