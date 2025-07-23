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

#ifndef EXAMPLES_EX21_PARAM_DESTRUCTION_RESURRECTION_EX21_SIMPLE_IP_H_
#define EXAMPLES_EX21_PARAM_DESTRUCTION_RESURRECTION_EX21_SIMPLE_IP_H_

#include <cci_configuration>
#include "xreport.hpp"

/**
 *  @class  ex21_simple_ip
 *  @brief  A simple IP which owns a CCI param.
 */
SC_MODULE(ex21_simple_ip) {
public:

    /**
     *  @fn     void ex21_simple_ip()
     *  @brief  The constructor for the class.
     *  Create parameter on the heap for destruction.
     *  @return void
     */
    SC_CTOR(ex21_simple_ip)
    // Initialize int_param_ptr with 10
    : int_param_ptr(new cci::cci_param<int>("int_param", 10)) {
        SC_THREAD(execute);
        XREPORT("Ctor: Default value of " << int_param_ptr->name() << " is "
                                          << *int_param_ptr);
    }

    /**
     *  @fn     void ~ex21_simple_ip()
     *  @brief  The destructor for the class.
     *  @return void
     */
    ~ex21_simple_ip() {
        XREPORT_PLAIN(
                "Dtor: Current value of " << int_param_ptr->name() << " is "
                                          << *int_param_ptr);
    }

    /**
     *  @fn     void execute()
     *  @brief  The main execution block (no real functionality)
     *  @return void
     */
    void execute() {
        // Wait for 10ns to allow config_ip to update int_param value
        wait(10, sc_core::SC_NS);

        // Update int_param value to 20
        XREPORT("execute: Set value of int_param to 20");
        *int_param_ptr = 20;
        XREPORT("execute: Current value of int_param is " << *int_param_ptr);

        // Allow config_ip to end first part
        wait(20, sc_core::SC_NS);

        // Destroy parameter (and so invalidate param handles)
        XREPORT("execute: Destroy sim_ip.int_param");
        delete int_param_ptr;

        // Allow config_ip to check param handles validity
        wait(30, sc_core::SC_NS);

        // Resurrect destructed parameter with value -10
        XREPORT("execute: Resurrect sim_ip.int_param");
        int_param_ptr = new cci::cci_param<int>("int_param", -10);

        wait(40, sc_core::SC_NS);
    }

private:
    cci::cci_param<int> *int_param_ptr; ///< CCI param to hold buffer size
};
// ex21_simple_ip

#endif  // EXAMPLES_EX21_PARAM_DESTRUCTION_RESURRECTION_EX21_SIMPLE_IP_H_

