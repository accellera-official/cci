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
 * @file   testbench.cpp
 * @author Christian Schroeder, GreenSocs
 * @author Mark Burton, GreenSocs
 */

#include <systemc>
#include "cci_configuration"

#include "ParameterOwnerModule.h"
#include "ParameterOwnerMutabilityModule.h"
#include "ObserverModule.h"
#include "ParamManipulateModule.h"
#include "ValueModule.h"

class ParamContainer : public sc_core::sc_module
{
public:
	ParamContainer(sc_core::sc_module_name name) : sc_module(name)
	, top_param1("top.param1", "This is a param with top-level-name", "String parameter", cci::CCI_ABSOLUTE_NAME)
	{
        try {
            top_param2 = new cci::cci_param<bool>("param2",
                                                cci::cci_value(
                                                        "This is a param with top-level-name"),
                                                "A bool parameter with absolute name.",
                                                cci::CCI_ABSOLUTE_NAME);
        }
        catch (sc_core::sc_report e) {
            std::cout << std::endl << name
                      << ": Unable to initialize 'ParamContainer.top_param2'" << std::endl;
            switch (cci::cci_report_handler::get_param_failure(e)) {
                case cci::CCI_VALUE_FAILURE:
                    std::cout << e.what() << std::endl;
                    break;
            }
        }

        try {
            top_param3 = new cci::cci_param<int>("top.arbitrary.top_param3",
                                                cci::cci_value(
                                                        "This is a param with top-level-name"),
                                                "A integer parameter with absolute name.",
                                                cci::CCI_ABSOLUTE_NAME);
        }
        catch (sc_core::sc_report e) {
            std::cout << std::endl << name
                      << ": Unable to initialize 'ParamContainer.top_param'" << std::endl;
            switch (cci::cci_report_handler::get_param_failure(e)) {
                case cci::CCI_VALUE_FAILURE:
                    std::cout << e.what() << std::endl;
                    break;
            }
        }
    }
protected:
  cci::cci_param<std::string> top_param1;
  cci::cci_param<bool>*       top_param2;
  cci::cci_param<int>*        top_param3;
};

/// Testbench for the CCI example application which uses the GreenSocs demo implemenation
int sc_main(int argc, char *argv[]) {
  //sc_core::sc_report_handler::set_actions(sc_core::SC_ERROR, sc_core::SC_ABORT);
  //sc_core::sc_report_handler::set_actions(sc_core::SC_WARNING, sc_core::SC_ABORT);

  // Note: this prevents from catching the error!!
  //sc_core::sc_report_handler::set_actions("/ASI/CCI/set_param_failed",  sc_core::SC_DISPLAY);
  sc_core::sc_report_handler::set_actions("/ASI/CCI/cci_value_failure", sc_core::SC_DISPLAY);


  ObserverModule        observer   ("Observer");
  ParamManipulateModule manipulator("Manipulator");
  ParameterOwnerModule  owner      ("Owner");
  ParameterOwnerMutabilityModule  mutability_owner      ("MutabilityOwner");
  ValueModule           valueMod   ("ValueMod");
  ParamContainer		params	   ("top");

  std::cout << std::endl << "------ sc_start() ----------------" << std::endl << std::endl;
  sc_core::sc_start(); 
  std::cout << std::endl << "------ sc_start() returned -------" << std::endl << std::endl;
  
  return EXIT_SUCCESS; 
  
}
