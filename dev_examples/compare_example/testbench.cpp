/*****************************************************************************
  Copyright 2016 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2011 GreenSocs
  All rights reserved.
  Authors:
      Christian Schroeder <schroeder@eis.cs.tu-bs.de>

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

class test_module : public sc_core::sc_module
{
SC_HAS_PROCESS(test_module);
public:
	test_module(sc_core::sc_module_name name) : sc_core::sc_module(name)
		, int_param_mutable  ("int_param_mutable", cci::cci_value(10),   cci::CCI_TOP_LEVEL_NAME)
		, int_param_mutable2 ("int_param_mutable2",  100,                cci::CCI_TOP_LEVEL_NAME)
		, int_param_immutable("int_param_immutable", cci::cci_value(11), cci::CCI_TOP_LEVEL_NAME)
		, int_param_elab     ("int_param_elab",      11,                 cci::CCI_TOP_LEVEL_NAME)
	{
		SC_THREAD(perform_test);
	}

	void perform_test()
	{

		cci::cci_param<std::string> str_param("str_param", cci::cci_value("This is a \nstring"));

		std::cout << "int_param_mutable2 == int_param_mutable: " << 
		((int_param_mutable2 == int_param_mutable) ? "true" : "false") << std::endl;

		std::cout << "int_param_elab == int_param_mutable: " << 
		((int_param_elab == int_param_mutable) ? "true" : "false") << std::endl;

		std::cout << "int_param_elab == int_param_immutable: " << 
		((int_param_elab == int_param_immutable) ? "true" : "false") << std::endl;

		std::cout << "int_param_elab != int_param_immutable: " << 
		((int_param_elab != int_param_immutable) ? "true" : "false") << std::endl;
	}

protected:
  cci::cci_param<int, cci::mutable_param>          int_param_mutable;
  cci::cci_param<int, cci::mutable_param>          int_param_mutable2;
  cci::cci_param<int, cci::immutable_param>        int_param_immutable;
  cci::cci_param<int, cci::elaboration_time_param> int_param_elab;
};
  
  
/// Testbench for the CCI development example testing the compare functionality of parameters of different mutability
int sc_main(int argc, char *argv[]) {
	test_module tm("test_module");
	sc_core::sc_start();
  return EXIT_SUCCESS; 
}
