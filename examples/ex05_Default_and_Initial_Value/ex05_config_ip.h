/*****************************************************************************
  Copyright 2006-2014 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2010-2015 Texas Instruments Inc.
  All rights reserved.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 *****************************************************************************/

/**
 *  @file   config_ip.h
 *  @brief  Configure simple_ip's param_1 (simple integer) parameter's init value;
 *          and this value will override the value specified the default value
 *          specified in the IP 's constructor. Other parameter param_2 is
 *          not changed; and whtever is the default value, will be used in simulation
 *  @author Vishal Goel, Texas Instruments
 */

#ifndef EXAMPLES_EX05_DEFAULT_AND_INITIAL_VALUE_EX05_CONFIG_IP_H_
#define EXAMPLES_EX05_DEFAULT_AND_INITIAL_VALUE_EX05_CONFIG_IP_H_

#include <cci_configuration>
#include <cassert>
#include "xreport.hpp"

/**
 *  @class  ex05_config_ip
 *  @brief  The config_ip configures sim_ip's init-values
 */
SC_MODULE(ex05_config_ip) {
 public:
  /**
   *  @fn     ex05_config_ip
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex05_config_ip) {
    // Get CCI configuration handle specific for this module
    m_cci = &cci::cci_broker_manager::get_current_broker(
        cci::cci_originator(*this));
    assert(m_cci != NULL);
    setup_sim_ip("Attempting to setup config_ip to 10 before IP construction",
                 "10");
  }

  /**
   *  @fn     void setup_sim_ip(const char* msg, const char* val)
   *  @brief  Function to setup the simulation
   *  @param  msg Message to print during setup
   *  @param  val The value to assign to the param
   *  @return void
   */
  void setup_sim_ip(const char *msg, const char *val) {
    XREPORT(msg);

    // Check for existance of sim_ip.param_1 param
    if (m_cci->param_exists("sim_ip.param_1")) {
      XREPORT_ERROR("Instantiate config_ip before simple_ip"
                    " to demonstrate set_initial_cci_value");
    } else {
      XREPORT("Setting up sim_ip.param_1's init-value to " << val);
      m_cci->set_initial_cci_value("sim_ip.param_1",
                                   cci::cci_value::from_json(val));
    }
  }

 private:
  cci::cci_broker_if *m_cci; ///< CCI configuration handle
};
// ex05_config_ip

#endif  // EXAMPLES_EX05_DEFAULT_AND_INITIAL_VALUE_EX05_CONFIG_IP_H_
