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
 * @file   broker_hierarchy.cpp
 * @author Guillaume Delbergue, GreenSocs / Ericsson
 * @brief  Broker hierarchy example
 *
 *                      A (Public Broker)
 *                      |
 *                      B (Public Broker)
 *                      |
 *    ------------------+------------------
 *   /                                     \
 *  C1 (Private Broker 1 by inheritance)   C2 (Private Broker 1)
 *  |                                      |
 *  D1 (Private Broker 2)                  D2 (Private Broker 1 by inheritance)
 *
*/

#include <cci_configuration>

// Private broker
#include "gs_cci_cnf_private_broker.h"

using namespace sc_core;
using namespace cci;

#define BROKER_HIERARCHY_CURRENT_BROKER_PRINT_(broker)                         \
std::cout << "Module name: " << this->name() << std::endl;                     \
std::cout << "Parent module name: " << get_parent_object()->name()             \
          << std::endl;                                                        \
std::cout << "Broker name: " << broker.name() << std::endl;                    \
std::cout << "Broker is private ? " << std::boolalpha                          \
          << broker.is_private_broker() << std::endl << std::endl

/// Module D1
SC_MODULE(D1)
{
    SC_CTOR(D1):
        brokerD1(cci_broker_manager::get_broker())
    {
        BROKER_HIERARCHY_CURRENT_BROKER_PRINT_(brokerD1);
    }

private:
    cci_broker_handle brokerD1;
};

/// Module D2
SC_MODULE(D2)
{
    SC_CTOR(D2):
        brokerD2(cci_broker_manager::get_broker())
    {
        BROKER_HIERARCHY_CURRENT_BROKER_PRINT_(brokerD2);
    }

private:
    cci_broker_handle brokerD2;
};


/// Module C1
SC_MODULE(C1)
{
    SC_CTOR(C1):
        brokerC1(cci_broker_manager::get_broker()),
        privateBrokerD1(new cci::gs_cci_private_broker(
                "A.B.C1.D1.privateBroker",
                *this,
                std::vector<std::string>()))
    {

        BROKER_HIERARCHY_CURRENT_BROKER_PRINT_(brokerC1);

        cci_broker_manager::register_broker(*privateBrokerD1);

        moduleD1 = new D1("D1");
    }

    ~C1() {
        delete moduleD1;
        delete privateBrokerD1;
    }

private:
    cci_broker_handle brokerC1;
    cci_broker_if* privateBrokerD1;

public:
    D1* moduleD1;
};

/// Module C2
SC_MODULE(C2)
{
    SC_CTOR(C2):
        brokerC2(cci_broker_manager::get_broker())
    {
        BROKER_HIERARCHY_CURRENT_BROKER_PRINT_(brokerC2);

        moduleD2 = new D2("D2");
    }

    ~C2() {
        delete moduleD2;
    }

private:
    cci_broker_handle brokerC2;

public:
    D2* moduleD2;
};

/// Module B
SC_MODULE(B)
{
    SC_CTOR(B):
        brokerB(cci_broker_manager::get_broker()),
        privateBrokerC2(new cci::gs_cci_private_broker(
                "A.B.C2.privateBroker",
                *this,
                std::vector<std::string>()))
    {
        BROKER_HIERARCHY_CURRENT_BROKER_PRINT_(brokerB);

        cci_broker_manager::register_broker(*privateBrokerC2);

        moduleC1 = new C1("C1");
        moduleC2 = new C2("C2");
    }

    ~B() {
        delete moduleC1;
        delete moduleC2;
        delete privateBrokerC2;
    }

private:
    cci_broker_handle brokerB;
    cci_broker_if* privateBrokerC2;

public:
    C1* moduleC1;
    C2* moduleC2;
};

/// Module A
SC_MODULE(A)
{
    SC_CTOR(A):
        brokerA(cci_broker_manager::get_broker())
    {
        std::cout << "Module name: " << this->name() << std::endl;
        std::cout << "Broker name: " << brokerA.name() << std::endl
                  << std::endl;

        moduleB = new B("B");
    }

    ~A() {
        delete moduleB;
    }

private:
    B* moduleB;
    cci_broker_handle brokerA;
};

int sc_main(int, char*[])
{
    A moduleA("A");
    sc_start();
    return 0;
}

#undef BROKER_HIERARCHY_CURRENT_BROKER_PRINT_