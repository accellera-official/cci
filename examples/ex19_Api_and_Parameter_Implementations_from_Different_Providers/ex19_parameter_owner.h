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
 *  @file      parameter_owner.h
 *  @brief     This sc_module is the owner class of a cci-based parameter
 *  @author    Girish Verma, CircuitSutra Technologies     <girish@circuitsutra.com>
 *             P V S Phaneendra, CircuitSutra Technologies <pvs@circuitsutra.com>
 *  @date      14th December, 2011 (Wednesday);
 *             (Last Modifed On) 16th May, 2011 (Monday)
 */

#ifndef EXAMPLES_EX19_API_AND_PARAMETER_IMPLEMENTATIONS_FROM_DIFFERENT_PROVIDERS_EX19_PARAMETER_OWNER_H_
#define EXAMPLES_EX19_API_AND_PARAMETER_IMPLEMENTATIONS_FROM_DIFFERENT_PROVIDERS_EX19_PARAMETER_OWNER_H_

#include <cci_configuration>

#include "ex19_parameter_impl_by_abc.h"
#include "xreport.hpp"

// Forward declaration of the configurator class
class ex19_parameter_configurator;

/**
 *  @class  ex19_parameter_owner
 *  @brief  This sc_module declares a cci-parameter.  Also, it declares and makes
 *          the configurator class a friend class of this sc_module (class)
 */
SC_MODULE(ex19_parameter_owner) {
 public:
  friend class ex19_parameter_configurator;

  /**
   *  @fn     ex19_parameter_owner()
   *  @brief  The class constructor
   *  @return void
   */
  SC_CTOR(ex19_parameter_owner)
      : int_param("mutable_int_param", 5),
        udtype_param("mutable_udtype_param", user_data_type(0x100, 0x200, 1)) {
    XREPORT("[OWNER C_TOR] : Default Value : " << int_param.get());
    XREPORT("[OWNER C_TOR] : Default Value udtype_param : "
            << udtype_param.get());

    // SC_THREAD declaration
    SC_THREAD(run_owner);
  }

  /**
   *  @fn     void run_owner(void)
   *  @brief  This process sets and reads the value of the cci parameter.
   *  @return void
   */
  void run_owner(void) {
    while (1) {
      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[OWNER] : Parameter Value : " << int_param.get());

      wait(5.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[OWNER] : Set parameter value to 15.");
      int_param = 15;

      wait(15.0, sc_core::SC_NS);

      XREPORT("@ " << sc_core::sc_time_stamp());
      XREPORT("[OWNER] : Parameter Value   : " << int_param.get());

      wait(50.0, sc_core::SC_NS);
    }
  }

 private:
  // Declare an instance of mutable CCI parameter of type 'int'
  cci::cci_param<int> int_param; ///< An int cci parameter
  cci::cci_param<user_data_type> udtype_param; ///< An user defined type cci parameter
};
// ex19_parameter_owner

#endif  // EXAMPLES_EX19_API_AND_PARAMETER_IMPLEMENTATIONS_FROM_DIFFERENT_PROVIDERS_EX19_PARAMETER_OWNER_H_
