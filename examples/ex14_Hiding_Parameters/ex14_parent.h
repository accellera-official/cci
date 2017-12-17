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
#include <string>
#include <vector>

#include "ex14_child.h"
#include "xreport.hpp"

/**
 *  @class  ex14_parent
 *  @brief  The parent module
 */
SC_MODULE(ex14_parent) {
 public:
  SC_HAS_PROCESS(ex14_parent);

  /**
   *  @fn     ex14_parent(sc_core::sc_module _name)
   *  @brief  The class constructor
   *  @param  _name The name of the class
   *  @return void
   */
  ex14_parent(sc_core::sc_module_name _name)
      : sc_core::sc_module(_name),
        m_broker(cci::cci_get_broker()),
        child_inst("child_inst"),
        parent_int_param("parent_int_param", 300),
        parent_buffer("parent_int_buffer", 350),
        child_param_handle(cci::cci_originator(*this))
  {

    XREPORT("[PARENT C_TOR] : Parameter Name : "
            << parent_int_param.get_name() << "\tParameter Value : "
            << parent_int_param.get_value());

    std::string child_param_path(name());
    child_param_path.append(".child_inst.priv_int_param");

    child_param_handle = m_broker.get_param_handle(child_param_path); 
    if (child_param_handle.is_valid()) {
      // Register 'POST_WRITE' callback to change child's cci-parameter
      // Configurators writes to 'parent_buffer' cci-parameter (registered
      // to the default global broker). Changes to the 'parent_buffer' will
     // be reflected on to the 'priv_int_param' of child as well
      cci::cci_param_post_write_callback_untyped untyped_post_write_cb(
                      sc_bind(&ex14_parent::untyped_post_write_callback,
                              this, sc_unnamed::_1,child_param_handle));

      parent_post_write_cb =
              parent_buffer.register_post_write_callback(untyped_post_write_cb);

    } else {
      XREPORT("[PARENT C_TOR] : Desired cci-parameter of 'child' module is"
            " not available");
    }

    // Declaration of SC_THREAD
    SC_THREAD(run_parent);

    XREPORT("[PARENT C_TOR] : Parameter Name : "
            << parent_buffer.get_name()
            << "\tParameter Value : "
            << parent_buffer.get_value());
  }

  /**
   *  @fn     void typed_post_write_callback(const cci::cci_param_write_event<int> & ev)
   *  @brief  Post Callback function for writes on parameters
   *  @return void
   */
  void untyped_post_write_callback(const cci::cci_param_write_event<> & ev,
                                 cci::cci_param_handle child_param_handle)
  {
    child_param_handle.set_cci_value(ev.new_value);

    XREPORT("[PARENT - post_write_cb] : Parameter Name : "
            << ev.param_handle.get_name()
            << "\tParameter Value : "
            << ev.new_value);
  }

  /**
   *  @fn     void run_parent(void)
   *  @brief  The main execution function
   *  @return void
   */
  void run_parent(void) {
    while (1) {
      wait(5.0, sc_core::SC_NS);

      std::vector<cci::cci_param_untyped_handle> parent_param_list =
              m_broker.get_param_handles();

      XREPORT("@ " << sc_core::sc_time_stamp()
              << "\tVisible parameters to the 'parent' module");

      for (unsigned int i = 0; i < parent_param_list.size(); i++) {
        XREPORT("[PARENT] : Parameter Name : "
                        << parent_param_list[i].get_name());
      }

      wait(30.0, sc_core::SC_NS);
    }
  }

 private:
  cci::cci_broker_handle m_broker;

  ex14_child child_inst;  ///< Owner Module instantiation

  // Declare cci-parameters (registered with the private broker)
  cci::cci_param<int> parent_int_param;  ///< CCI int parameter
  cci::cci_param<int> parent_buffer; ///< CCI int parameter for a buffer

  /// Declare cci_param pointers
  cci::cci_param_handle child_param_handle; ///< Handle to the child

  /// Callback handle
  cci::cci_callback_untyped_handle parent_post_write_cb;  /// callback handle
};
/// ex14_parent

#endif  // EXAMPLES_EX14_HIDING_PARAMETERS_EX14_PARENT_H_
