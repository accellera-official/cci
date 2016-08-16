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
 *  @file     parent.h
 *  @brief    This header file declares and defines the 'parent' which instantiates
 *            'child' and hides private parameters
 *  @author   P V S Phaneendra, CircuitSutra Technologies   <pvs@circuitsutra.com>
 *            Girish Verma, CircuitSutra Technologies   <girish@circuitsutra.com>
 *  @date     3rd September, 2011 (Saturday)
 */

#ifndef EXAMPLES_EX14_HIDING_PARAMETERS_EX14_PARENT_H_
#define EXAMPLES_EX14_HIDING_PARAMETERS_EX14_PARENT_H_

#include <cci_configuration>
#include <cassert>
#include <string>
#include <vector>

#include "ex14_child.h"
#include "xreport.hpp"

/**
 *  @class  ex14_parent
 *  @brief  The parent module derives from the 'cci_broker_manager' class and
 *          manages its own private broker stack
 */
SC_MODULE(ex14_parent) {
 public:
  SC_HAS_PROCESS(ex14_parent);

  /**
   *  @fn     ex14_parent(sc_core::sc_module _name, cci::cci_broker_manager priv_broker)
   *  @brief  The class constructor
   *  @param  _name The name of the class
   *  @param  priv_broker An instance of a private broker
   *  @return void
   */
  ex14_parent(sc_core::sc_module_name _name,
              cci::cci_broker_manager priv_broker)
      : sc_core::sc_module(_name),
        parent_BrokerIF(priv_broker),
        child_inst("child_inst"),
        // Registering 'parameters' to the broker
        // Here, <broker_inst> is given the reference of the broker declared
        // immediately above PARENT which specifies the list of publicly
        // visible parameters
        parent_int_param("parent_int_param", 300, *priv_broker),
        parent_buffer("parent_int_buffer", 350, *priv_broker) {
    // Asserts if the returned broker handle is NULL
    assert(parent_BrokerIF != NULL
           && "Returned broker handle for 'parent' is NULL");

    XREPORT("[PARENT C_TOR] : Parameter Name : "
            << parent_int_param.get_name() << "\tParameter Value : "
            << parent_int_param.get_value());

    if (parent_BrokerIF != NULL) {
      std::string child_param_path(name());
      child_param_path.append(".child_inst.priv_int_param");

      if (parent_BrokerIF->param_exists(child_param_path)) {
        child_base_param_ptr = parent_BrokerIF->get_param_handle(child_param_path);

        assert(child_base_param_ptr != NULL
               && "Returned broker handle for 'priv_int_param' of 'child'"
               " is NULL");

        // Register 'POST_WRITE' callback to change child's cci-parameter
        // Configurators writes to 'parent_buffer' cci-parameter (registered
        // to the default global broker). Changes to the 'parent_buffer' will
        // be reflected on to the 'priv_int_param' of child as well
        parent_post_write_cb =
            parent_buffer.register_callback(cci::post_write,
                                            this,
                                            cci::bind(&ex14_parent::write_callback,
                                                      this, _1, _2,
                                                      child_base_param_ptr));
      } else {
        XREPORT("[PARENT C_TOR] : Desired cci-parameter of 'child' module is"
                " not available");
      }
    }

    // Declaration of SC_THREAD
    SC_THREAD(run_parent);

    XREPORT("[PARENT C_TOR] : Parameter Name : "
            << parent_buffer.get_name()
            << "\tParameter Value : "
            << parent_buffer.get_value());
  }

  /**
   *  @fn     cci::callback_return_type write_callback(cci::cci_base_param& _base_param, const cci::callback_type& cb_reason, cci::cci_base_param* _child_base_param_ptr)
   *  @brief  Post write parameter callback implementation
   *  @param  _base_param Parameter with the callback
   *  @param  cb_reason The reason for the callback
   *  @param  _child_base_param_ptr A pointer to a child parameter to be updated
   *  @return The exit status of the callback function
   */
  cci::callback_return_type write_callback(
      cci::cci_param_handle & _base_param,
      const cci::callback_type & cb_reason,
      cci::cci_param_handle * _child_base_param_ptr) {
    _child_base_param_ptr->json_deserialize(_base_param.json_serialize());

    XREPORT("[PARENT - post_write_cb] : Parameter Name : "
            << _base_param.get_name()
            << "\tParameter Value : "
            << _base_param.json_serialize());

    return cci::return_nothing;
  }

  /**
   *  @fn     void run_parent(void)
   *  @brief  The main execution function
   *  @return void
   */
  void run_parent(void) {
    while (1) {
      wait(5.0, sc_core::SC_NS);

      std::vector<std::string> parent_param_list =
          parent_BrokerIF->get_param_list();

      XREPORT("@ " << sc_core::sc_time_stamp()
              << "\tVisible parameters to the 'parent' module");

      for (unsigned int i = 0; i < parent_param_list.size(); i++) {
        XREPORT("[PARENT] : Parameter Name : " << parent_param_list[i]);
      }

      wait(30.0, sc_core::SC_NS);
    }
  }

 private:
  cci::cci_broker_if* parent_BrokerIF; ///< Configuration Broker for TOP_MODULE

  ex14_child child_inst;  ///< Owner Module instantiation

  // Declare cci-parameters (registered with the private broker)
  cci::cci_param<int> parent_int_param;  ///< CCI int parameter
  cci::cci_param<int> parent_buffer; ///< CCI int parameter for a buffer

  /// Declare cci_base_param pointers
  cci::cci_param_handle* child_base_param_ptr; ///< Pointer to the child

  // Callback Adaptor Object
  cci::shared_ptr<cci::callb_adapt> parent_post_write_cb;  ///< callback adapter object
};
/// ex14_parent

#endif  // EXAMPLES_EX14_HIDING_PARAMETERS_EX14_PARENT_H_
