/*****************************************************************************
  Copyright 2016 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2011 GreenSocs
  All rights reserved.
  Authors:
      Christian Schroeder <schroeder@eis.cs.tu-bs.de>
      Mark Burton <mark@greensocs.com>

  Copyright 2016 Ericsson
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


#include <systemc>
#include "cci_configuration"

class ParamContainer : public sc_core::sc_module
{
public:
	ParamContainer(sc_core::sc_module_name name) : sc_module(name)
    , int_param_with_initial_value     ("int_param_with_initial_value",      10)
    , int_param_without_initial_value  ("int_param_without_initial_value",   11)
    , int_param_with_late_initial_value("int_param_with_late_initial_value", 12)
	{}
protected:

  cci::cci_param<int> int_param_with_initial_value;
  cci::cci_param<int> int_param_without_initial_value;
  cci::cci_param<int> int_param_with_late_initial_value;
};


/// Shows a parameter list with information about value, explicit/implicit status and is_initial_value status
void show_param_list() {
  static cci::cci_broker_if* mBroker = &cci::cci_broker_manager::get_current_broker(cci::cci_originator("PARAM_LIST"));

  std::cout << std::endl << "**** Parameter list: ";
  std::vector<std::string> vec = mBroker->get_param_list();
  std::vector<std::string>::iterator iter;
  std::stringstream ss_show;
  for (iter = vec.begin() ; iter < vec.end(); iter++) {
    ss_show << std::endl;
    ss_show << *iter << " : ";
    cci::cci_param_handle p = mBroker->get_param_handle(*iter);
    if (p.is_valid()) {
      ss_show << "expl.";
      if (p.is_initial_value())       ss_show << ", initial";
      ss_show << ", val=" << p.get_cci_value().to_json();
    } else {
      ss_show << "impl. (is always initial)";
      ss_show << ", val=" << mBroker->get_cci_value(*iter);
    }
  }
  std::cout << "   " << ss_show.str() << std::endl<<std::endl;
}

/// Testbench for the CCI development example testing the is_initial_value functionality
int sc_main(int argc, char *argv[]) {
  
  cci::cci_broker_if* mBroker = &cci::cci_broker_manager::get_current_broker(cci::cci_originator("SCMAIN"));
  
  mBroker->set_initial_cci_value("params.int_param_with_initial_value",
                                 cci::cci_value::from_json("100"));
  mBroker->set_initial_cci_value("params.int_param_only_implicit",
                                 cci::cci_value::from_json("130"));
  
  ParamContainer params("params");
  
  show_param_list();  

  // This does NOT change the status to initial because this write to an explicit parameter is translated 
  // 'just' into a usual write
  mBroker->set_initial_cci_value("params.int_param_with_late_initial_value",
                                 cci::cci_value::from_json("120"));
  
  show_param_list();

  std::cout << std::endl << "------ sc_start() ----------------" << std::endl << std::endl;
  sc_core::sc_start(); 
  std::cout << std::endl << "------ sc_start() returned -------" << std::endl << std::endl;
  
  return EXIT_SUCCESS; 
  
}
