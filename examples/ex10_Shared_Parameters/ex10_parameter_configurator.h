/*****************************************************************************
  Copyright 2006-2014 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2010-2015 CircuitSutra Technologies Pvt. Ltd.
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
 *  @file     parameter_configurator.h
 *  @brief    This file declares and defines the configurator module
 *  @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date     16th May, 2011 (Monday)
 */

#ifndef EXAMPLES_EX10_SHARED_PARAMETERS_EX10_PARAMETER_CONFIGURATOR_H_
#define EXAMPLES_EX10_SHARED_PARAMETERS_EX10_PARAMETER_CONFIGURATOR_H_

#include <cci_configuration>
#include <cassert>
#include "xreport.hpp"

/**
 *  @class  ex10_parameter_configurator
 *  @brief  This module demonstrates different ways to access a CCI parameter of the owner class.
 */
SC_MODULE(ex10_parameter_configurator) {
 public:
  /**
   *  @fn     ex10_parameter_configurator
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex10_parameter_configurator) {
    // Get handle of the broker responsible for the class/module
    myCfgrBrokerIF =
        &cci::cci_broker_manager::get_current_broker(cci::cci_originator(*this));

    // Report if handle returned is NULL
    assert(myCfgrBrokerIF != NULL && "Parameter-Setter handle is NULL");

    // Checks the parameter exists using name-based look-up
    if (myCfgrBrokerIF->param_exists("param_owner.mutable_int_param")) {
      XREPORT("[CFGR C_TOR] : Parameter exists");

      // Get handle of the owner parameter
      cfgr_param_ptr =
          myCfgrBrokerIF->get_param("param_owner.mutable_int_param");

      // Assert if the owner parameter handle returned is NULL
      assert(cfgr_param_ptr != NULL && "Parameter Handle is NULL");
    } else {
      XREPORT("[CFGR C_TOR] : Parameter doesn't exists.");
    }

    // Set parameter value using cci_base_parameter object
    XREPORT("[CFGR C_TOR] : Set parameter value to 10"
            " using cci_base_parameter");
    cfgr_param_ptr->json_deserialize("10");

    // Registering SC_THREAD with the SystemC kernel
    SC_THREAD(run_mutable_cfgr);
  }

  /**
   *  @fn     void run_mutable_cfgr(void)
   *  @brief  Updates the value of the cci parameter
   *  @reutrn void
   */
  void run_mutable_cfgr(void) {
    while (1) {
      wait(10.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[CFGR] : Parameter Value   : " << cfgr_shared_param->get());

      wait(5.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[CFGR] : Change value to 20");
      *cfgr_shared_param = 20;

      wait(50.0, sc_core::SC_NS);
    }
  }

  /**
   *  @fn     void set_cfgr_parameter(ex10_parameter_owner* owner)
   *  @brief  Set the value of the shared parameter
   *  @param  owner The owner of the parameter
   *  @return void
   */
  void set_cfgr_parameter(ex10_parameter_owner *owner) {
    // Getting reference of the owner parameter
    cfgr_shared_param = &owner->int_param;
  }

 private:
  cci::cci_broker_if* myCfgrBrokerIF;  ///< cci configuration broker interface instance
  cci::cci_base_param* cfgr_param_ptr; ///< CCI base parameter pointer
  cci::cci_param<int>* cfgr_shared_param;  ///< Declaring a CCI parameter pointer (which will hold the reference of the owner CCI parameter 'int_param'
};
// ex10_parameter_configurator

#endif  // EXAMPLES_EX10_SHARED_PARAMETERS_EX10_PARAMETER_CONFIGURATOR_H_
