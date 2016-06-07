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
 *  @file    parameter_configurer.h
 *  @brief   This file defines a CONFIGURATOR.
 *           This file declares and defines a sc_module which demonstrates the
 *           means to access the cci-parameters using name-based look up access
 *           approach and set their various attributes
 *  @author  P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *  @date    12th September, 2011 (Monday)
 */
#ifndef EXAMPLES_EX07_PARAMETER_INFORMATION_EX07_PARAMETER_CONFIGURER_H_
#define EXAMPLES_EX07_PARAMETER_INFORMATION_EX07_PARAMETER_CONFIGURER_H_

#include <cci_configuration>
#include <cassert>
#include <string>
#include "xreport.hpp"

/**
 *  @class  ex07_parameter_configurer
 *  @brief  This SCMODULE instantiates a CCI configuration broker to access and modify CCI parameter values.
 */
SC_MODULE(ex07_parameter_configurer) {
 public:
  /**
   *  @fn     ex07_parameter_configurer
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex07_parameter_configurer)
      : check(0) {
    // Get the broker responsible for this module
    // using 'get_current_broker' API
    myBrokerInterface = &cci::cci_broker_manager::get_current_broker(
        cci::cci_originator(*this));

    // Assert if broker handle returned is NULL
    assert(myBrokerInterface != NULL && "Broker Handle Returned is NULL");

    // Check for the broker type (default or private)
    // using 'is_private_broker()' API
    if (myBrokerInterface->is_private_broker())
      // Access broker's name using 'name()'
      XREPORT("[CFGR C_TOR] : Broker Type : " << myBrokerInterface->name());
    else
      XREPORT("[CFGR C_TOR] : Broker Type : " << myBrokerInterface->name()
              << " - is not a private broker.");

    int_param_str = "param_owner.mutable_int_param";
    string_param_str = "param_owner.mutable_string_param";

    intParamExists = false;
    strParamExists = false;

    // Broker interface checks for the existance of a (int type) parameter
    if (myBrokerInterface->param_exists(int_param_str)) {
      int_param_ptr = myBrokerInterface->get_param(int_param_str);

      assert(int_param_ptr != NULL
             && "Integer CCI Parameter Handle returned NULL");

      intParamExists = true;
    } else {
      XREPORT_ERROR("[CFGR C_TOR] : Integer parameter does not exist.");

      intParamExists = false;
    }

    // Broker interface checks for existance of a (std::string type)
    // parameter using 'param_exists' API
    if (myBrokerInterface->param_exists(string_param_str)) {
      // If parameter exists, get handle of the parameter using
      // 'get_param' API
      str_param_ptr = myBrokerInterface->get_param(string_param_str);

      // Report if parameter handle is returned NULL
      assert(str_param_ptr != NULL
             && "String CCI Parameter Handle returned NULL");

      strParamExists = true;
    } else {
      XREPORT_ERROR("[CFGR C_TOR] : String parameter does not exist");

      strParamExists = false;
    }

    XREPORT("Prior to " << sc_core::sc_time_stamp()
            << "\tdemonstrating 'is_initial_value' API");

    // Check if string type cci-parameter has any initial value using
    // 'is_initial_value' API
    if (str_param_ptr->is_initial_value()) {
      XREPORT("[CFGR C_TOR] : " << str_param_ptr->get_name()
              << "\tInitial Value : " << str_param_ptr->get_value().get_string());
    } else {
      XREPORT_ERROR("[CFGR] : No initial value set for "
                    << str_param_ptr->get_name());
    }

    XREPORT("[CFGR C_TOR] : Is initial value ?\t" << std::boolalpha
            << str_param_ptr->is_initial_value());

    // Set documentation to the string type cci-parameter
    XREPORT("[CFGR C_TOR] : Set documentation to the string-type"
            " cci-parameter");
    std::string str_doc = "This is a mutable type string parameter";
    str_param_ptr->set_documentation(str_doc);

    SC_THREAD(run_accessor);
  }

  /**
   *  @fn     void run_accessor()
   *  @brief  Main function to access the parameter's value(s)
   *  @return void
   */
  void run_accessor() {
    while (1) {
      if (strParamExists && intParamExists) {
        XREPORT("@ " << sc_core::sc_time_stamp()
                << "\tdemonstrating 'is_default_value()'");

        // Access parameter's default value status using
        // 'is_default_value()' API
        if (int_param_ptr->is_default_value()) {
          XREPORT("[CFGR] : " << int_param_ptr->get_name()
                  << " default value hasn't been modified.");
          XREPORT("[CFGR] : Is Default Value ? " << "\tReturned status : "
                  << std::boolalpha << int_param_ptr->is_default_value());
        } else {
          XREPORT("[CFGR] : " << int_param_ptr->get_name()
                  << " default value has been modified.");
          XREPORT("[CFGR] : Is Default Value ? " << "\tReturned status : "
                  << std::boolalpha << int_param_ptr->is_default_value());
        }

        wait(2.0, sc_core::SC_NS);

        XREPORT("@ " << sc_core::sc_time_stamp()
                << "\tdemonstrating 'is_default_value()'");

        // Access parameter's default value status using
        // 'is_default_value()' API
        if (!int_param_ptr->is_default_value()) {
          XREPORT("[CFGR] : " << int_param_ptr->get_name()
                  << " value has been modified.");
          XREPORT("[CFGR] : Is Default Value ? " << "\tReturned status : "
                  << std::boolalpha << int_param_ptr->is_default_value());
        }

        if (str_param_ptr->is_default_value()) {
          XREPORT("[CFGR] : " << str_param_ptr->get_name()
                  << " value hasn't been modified");
          XREPORT("[CFGR -> Retrieve] : Parameter name  : "
                  << str_param_ptr->get_name());
          XREPORT("[CFGR -> Retrieve] : Parameter value : "
                  << str_param_ptr->get_value().get_string());
        } else {
          XREPORT("[CFGR] : " << str_param_ptr->get_name() << " New Value : "
                  << str_param_ptr->get_value().get_string());
        }

        wait(2.0, sc_core::SC_NS);

        // Access parameter's name using 'get_name()' API
        XREPORT("[CFGR -> Retrieve] Parameter's name : "
                << int_param_ptr->get_name());

        // Access parameter's value using 'json_serialize' API
        XREPORT("[CFGR -> Retrieve] Parameter's value : "
                << int_param_ptr->json_serialize());

        // Access parameter's documentation using 'get_documentation()'
        XREPORT("[CFGR -> Retreive] Parameter's doc : "
                << int_param_ptr->get_documentation());

        wait(2.0, sc_core::SC_NS);

        XREPORT("@ " << sc_core::sc_time_stamp());
        XREPORT("[CFGR -> Set] :  " << int_param_ptr->get_name()
                << " value to 10");

        // Set parameter's value using 'json_deserialize' API
        int_param_ptr->json_deserialize("10");

        wait(4.0, sc_core::SC_NS);

        XREPORT("@ " << sc_core::sc_time_stamp());

        // Access parameter's name using 'get_name()' API
        XREPORT("[CFGR -> Retrieve] : Parameter name : "
                << int_param_ptr->get_name());

        // Access parameter's value using 'json_serialize' API
        XREPORT("[CFGR -> Retrieve] : Parameter value: "
                << int_param_ptr->json_serialize());

        // Access parameter's documentation using 'get_documentation()'
        XREPORT("[CFGR -> Retrieve] : Parameter doc : "
                << int_param_ptr->get_documentation());

        wait(2.0, sc_core::SC_NS);

        // Access locking status of a parameter using 'is_locked()' API
        //
        // Set password to lock/unlock parameter

        lock_passwd = &check;

        while (check < 3) {
          XREPORT("@ " << sc_core::sc_time_stamp());
          XREPORT("[CFGR] : Checking locked status of parameter");

          // Try accessing and modify value after locking
          lock_status = int_param_ptr->is_locked();

          if (lock_status) {
            XREPORT("[CFGR] : " << int_param_ptr->get_name()
                    << " is in locked state!!");
          } else {
            XREPORT("[CFGR] : " << int_param_ptr->get_name()
                    << " is in unlocked state!!");
          }

          check++;

          if (check == 1) {
            // Demonstration of parameter locking using password
            int_param_ptr->lock(&lock_passwd);
            XREPORT("[CFGR] : Parameter locked using password");
          } else if (check == 2) {
            // Demonstration of parameter unlocking using password
            int_param_ptr->unlock(&lock_passwd);
            XREPORT("[CFGR] : Parameter unlocked using password");
          }

          wait(2.0, sc_core::SC_NS);
        }

        wait(4.0, sc_core::SC_NS);

        XREPORT("@ " << sc_core::sc_time_stamp()
                << "\tdemonstrating 'set_value()' for string-type param using"
                " cci_value");
        XREPORT("[CFGR] : Parameter Documentation : "
                << str_param_ptr->get_documentation());

        cci::cci_value str_value("Hello_New_String");

        // Query basic type of a parameter using 'get_basic_type()' API
        XREPORT("[CFGR -> Set] : Get Basic Type using 'get_basic_type()' : "
                << str_param_ptr->get_basic_type());

        // Set a (std::string) parameter's value using
        // 'set_value' API with cci_value
        str_param_ptr->set_value(str_value);

        wait(2.0, sc_core::SC_NS);

        XREPORT("@ " << sc_core::sc_time_stamp()
                << " demonstrating 'get_latest_write_originator' API");

        // Get the latest write originator for the write value to
        // the string type cci-parameter
        const cci::cci_originator* str_originator = str_param_ptr
            ->get_latest_write_originator();
        XREPORT("[CFGR] : Originator for the latest write on string"
                " type cci-parameter : " << str_originator->name());

        XREPORT("@ " << sc_core::sc_time_stamp()
                << " demonstrating 'get_value' for string-type param"
                " using cci_value");
        XREPORT("[CFGR] : 'Retrieve1' using 'json_serialize' and"
                " 'Retrieve2' using 'get_value()'");
        XREPORT("[CFGR -> Retrieve1] : " << str_param_ptr->get_name()
                << " value is " << str_param_ptr->json_serialize());

        // Query a (std::string) parameter's value by using
        // 'get_value()'API with cci_value
        cci::cci_value rx_value = str_param_ptr->get_value();

        /**
         *  @par Based on the type returned by 'get_basic_type()', query value of cci_value.
         *  @par
         *  If basic parameter type is 'std::string', use 'get_string()' API with cci_value
         *  @par
         *  If basic parameter type is 'int', use 'get_int()' API with cci_value, etc.
         */
        XREPORT("[CFGR -> Retrieve2] : " << rx_value.get_string());

        wait(2.0, sc_core::SC_NS);
      } else {
        XREPORT("Parameter by name " << int_param_str << " not found.");
      }

      wait(20.0, sc_core::SC_NS);
    }
  }

 private:
  cci::cci_broker_if* myBrokerInterface; //!< CCI configuration broker instance

  std::string int_param_str;  //!< For storing hierarchical path of std::string type cci-parameter
  std::string string_param_str; //!< For storing hierarchical path of integer type cci-parameter

  cci::cci_base_param* int_param_ptr;  ///< cci_base parameter
  cci::cci_base_param* str_param_ptr;  ///< cci_base parameter

  // Few Local parameters
  bool intParamExists;  ///< variable for whether parameter exists
  bool strParamExists;  ///< variable for whether a parameter exists
  int check;  ///< flag to check
  bool lock_status; ///< status of any locks on a parameter
  void* lock_passwd;  ///< password for the parameter lock
};
// ex07_parameter_configurer

#endif  // EXAMPLES_EX07_PARAMETER_INFORMATION_EX07_PARAMETER_CONFIGURER_H_
