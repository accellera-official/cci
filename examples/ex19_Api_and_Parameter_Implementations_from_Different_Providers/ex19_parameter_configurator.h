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
 * @file     parameter_configurator.h
 * @brief    This header declares and defines configurator 
 * @author   Girish Verma, CircuitSutra Technologies     <girish@circuitsutra.com>
 *           P V S Phaneendra, CircuitSutra Technologies <pvs@circuitsutra.com>
 * @date     14th December, 2011
 *           16th May, 2011 (Monday)
 */

#ifndef EXAMPLES_EX19_API_AND_PARAMETER_IMPLEMENTATIONS_FROM_DIFFERENT_PROVIDERS_EX19_PARAMETER_CONFIGURATOR_H_
#define EXAMPLES_EX19_API_AND_PARAMETER_IMPLEMENTATIONS_FROM_DIFFERENT_PROVIDERS_EX19_PARAMETER_CONFIGURATOR_H_

#include <cci>
#include <cassert>

#include "xreport.hpp"

/// The configurator class illustrates the different ways in which the value
/// of a cci-parameter of the owner class can be accessed using both a
/// cci_base_parameter object and a cci_param object (by making the
/// configurator a friend class to the owner)
SC_MODULE(ex19_parameter_configurator) {
 public:
  SC_CTOR(ex19_parameter_configurator)
      : cfgr_param_ptr(0),
        cfgr_user_param_ptr(0),
        cfgr_shared_param(0) {
    // Get handle of the broker responsible for the class/module
    myCfgrBrokerIF = &cci::cnf::cci_broker_manager::get_current_broker(*this);

    // Report if handle returned is NULL
    assert(myCfgrBrokerIF != NULL && "Parameter-Setter handle is NULL");

    // Checks the parameter exists using name-based look-up
    if (myCfgrBrokerIF->param_exists("param_owner.mutable_int_param")) {
      XREPORT("[CFGR C_TOR] : Parameter param_owner.mutable_int_param exists");

      // Get handle of the owner parameter
      cfgr_param_ptr = myCfgrBrokerIF->get_param("param_owner.mutable_int_param");

      // Assert if the owner parameter handle returned is NULL
      assert(cfgr_param_ptr != NULL
             && "Parameter Handle for param_owner.mutable_int_param is NULL");
    } else {
      XREPORT("[CFGR C_TOR] : Parameter param_owner.mutable_int_param"
              " doesn't exists.");
    }

    if (myCfgrBrokerIF->param_exists("param_owner.mutable_udtype_param")) {
      XREPORT("[CFGR C_TOR] : Parameter param_owner.mutable_udtype_param"
              " exists");

      // Get handle of the owner parameter
      cfgr_user_param_ptr =
          myCfgrBrokerIF->get_param("param_owner.mutable_udtype_param");

      // Assert if the owner parameter handle returned is NULL
      assert(cfgr_param_ptr != NULL
             && "Parameter param_owner.mutable_udtype_param  Handle is NULL");

      XREPORT("[CFGR C_TOR] : Got param_owner.mutable_udtype_param ");
    } else {
      XREPORT("[CFGR C_TOR] : Parameter param_owner.mutable_udtype_param"
              " doesn't exists.");
      XREPORT("Try Again with mutable_udtype_param ");

      if (myCfgrBrokerIF->param_exists("mutable_udtype_param")) {
        XREPORT("Got Now with mutable_udtype_param");
      } else {
        XREPORT("Try again now directly with param_owner.mutable_udtype_param");
        cfgr_user_param_ptr =
            myCfgrBrokerIF->get_param("param_owner.mutable_udtype_param");

        if (cfgr_user_param_ptr == NULL) {
          XREPORT("Fail to get directly with param_owner.mutable_udtype_param");

          XREPORT("Try again now directly with mutable_udtype_param");
          cfgr_user_param_ptr =
              myCfgrBrokerIF->get_param("mutable_udtype_param");
        }

        if (cfgr_user_param_ptr == NULL) {
          XREPORT("It will always Fail :(");
        }
      }
    }

    // Set parameter value using cci_base_parameter object
    XREPORT("[CFGR C_TOR] : Set parameter value to 10 using"
            " cci_base_parameter");
    cfgr_param_ptr->json_deserialize("10");

    /// Registering SC_THREAD with the SystemC kernel
    SC_THREAD(run_mutable_cfgr);
  }

/// This process illustrates use of the cci_param (shared parameter) that
/// stores the reference of the owner cci-parameter to make changes
/// to the parameter's value
  void run_mutable_cfgr(void) {
    while (1) {
      wait(10.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[CFGR] : Parameter Value   : " << cfgr_shared_param->get());
      if (cfgr_user_param_ptr != NULL) {
        XREPORT("[CFGR] : Get :Parameter Value[User data type]   : "
                << cfgr_user_param_ptr->json_serialize());

        cfgr_user_param_ptr->json_deserialize("EXP");

        XREPORT("[CFGR] : Get :Parameter Value[User data type]   : "
                << cfgr_user_param_ptr->json_serialize());
      }

      wait(5.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[CFGR] : Change value to 20");
      *cfgr_shared_param = 20;

      wait(50.0, sc_core::SC_NS);
    }
  }

/// This parameter gets the reference of the 'int_param', a private-type
/// cci-parameter member of the owner class
  void set_cfgr_parameter(ex19_parameter_owner *owner) {
    // Getting reference of the owner parameter
    cfgr_shared_param = &owner->int_param;
  }

 private:
  /// Declaring a CCI configuration broker interface instance
  cci::cnf::cci_cnf_broker_if* myCfgrBrokerIF;

  /// Declaring a CCI base parameter pointer
  cci::cnf::cci_base_param* cfgr_param_ptr;
  cci::cnf::cci_base_param* cfgr_user_param_ptr;

  /// Declaring a CCI parameter pointer (which will hold the reference of the
  /// owner CCI parameter 'int_param'
  cci::cnf::cci_param<int>* cfgr_shared_param;
};
/// ex19_parameter_configurator

#endif  // EXAMPLES_EX19_API_AND_PARAMETER_IMPLEMENTATIONS_FROM_DIFFERENT_PROVIDERS_EX19_PARAMETER_CONFIGURATOR_H_
