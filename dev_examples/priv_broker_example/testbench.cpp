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
#include <cci_configuration>
#include <boost/assign/list_of.hpp>
#include "ModuleA.h"
#include "ObserverModule.h"

class SubTop
: public sc_core::sc_module
{
public:
    SC_HAS_PROCESS(SubTop);
    SubTop(sc_core::sc_module_name name, const std::string& module_A_name) {
        m_priv_broker = new cci::gs_cci_private_broker(*this,
                  boost::assign::list_of(module_A_name + ".int_param"));
        cci::cci_broker_manager::register_broker(*m_priv_broker);
        m_module_a = new ModuleA(module_A_name.c_str());
    }

    ~SubTop() {
        delete m_module_a;
        delete m_priv_broker;
    }

protected:
    ModuleA* m_module_a;
    cci::cci_broker_if* m_priv_broker;
};

class Top
: public sc_core::sc_module
{
public:
  SC_HAS_PROCESS(Top);
  Top(sc_core::sc_module_name name):
          m_subtop_a("SubTopA", "ModuleA"),
          m_subtop_a2("SubTopA2", "ModuleA2"),
          m_observer("Observer")
  {

  }
  
protected:
  SubTop m_subtop_a;
  SubTop m_subtop_a2;
  ObserverModule m_observer;
};


/// Testbench for the CCI example application which uses the GreenSocs demo implemenation
int sc_main(int argc, char *argv[]) {
  //sc_core::sc_report_handler::set_actions(sc_core::SC_WARNING, sc_core::SC_ABORT);
  sc_core::sc_report_handler::set_actions(sc_core::SC_ERROR, sc_core::SC_ABORT);
  //sc_core::sc_report_handler::set_actions("/ASI/CCI/set_param_failed",  sc_core::SC_DISPLAY);
  //sc_core::sc_report_handler::set_actions("/ASI/CCI/cci_value_failure", sc_core::SC_DISPLAY);


  Top top("Top");

  std::cout << std::endl << "------ sc_start() ----------------" << std::endl << std::endl;
  sc_core::sc_start(); 
  std::cout << std::endl << "------ sc_start() returned -------" << std::endl << std::endl;
  
  return EXIT_SUCCESS; 
  
}


