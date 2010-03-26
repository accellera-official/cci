//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//   Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
//   This program is free software.
// 
//   If you have no applicable agreement with GreenSocs Ltd, this software
//   is licensed to you, and you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   If you have a applicable agreement with GreenSocs Ltd, the terms of that
//   agreement prevail.
// 
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
//   02110-1301  USA 
// 
// ENDLICENSETEXT

// doxygen comments

#include "testtool_neg_wrong_api.h"
#include "greencontrol/core/helpfunctions.h"

void TestTool_Wrong_Api::main_action() {
  const char* hier_param_name = "dummyParName";
  const char* value = "dummyVal";

  // /// Routing tests

  // Send wrong target field:
  // Results in routing error!!
  {
    gs::ctr::ControlTransactionHandle th = directUserApi.getTransaction();
    th->set_mService(gs::ctr::CONFIG_SERVICE);
    th->set_mCmd(gs::cnf::CMD_SET_INIT_VAL);
    th->set_mTarget((gs::ctr::gc_port_if*)123123123); // WRONG!!! 
    th->set_mSpecifier(hier_param_name);
    th->set_mValue(value);
    //directUserApi.sendTransaction(th);  // add comment here to avoid error
  }

  // Send wrong target field:
  // Results in routing error!!
  {
    gs::ctr::ControlTransactionHandle th = directUserApi.getTransaction();
    th->set_mService(gs::ctr::NO_SERVICE); // WRONG!!!
    th->set_mCmd(gs::cnf::CMD_SET_INIT_VAL);
    th->set_mSpecifier(hier_param_name);
    th->set_mValue(value);
    //directUserApi.sendTransaction(th);  // add comment here to avoid error
  }


  // Send wrong target field:
  // Results in routing error!!
  {
    gs::ctr::ControlTransactionHandle th = directUserApi.getTransaction();
    th->set_mService(gs::ctr::NO_SERVICE); // WRONG!!!
    th->set_mCmd(gs::cnf::CMD_SET_INIT_VAL);
    th->set_mSpecifier(hier_param_name);
    th->set_mValue(value);
    //directUserApi.sendTransaction(th);  // add comment here to avoid error
  }
    
  // /// Config plugin tests
  
  // Get not existing parameter:
  std::cout << "Get not existing parameter" << std::endl;
  gcnfUserApi->getParam("not.existing.parameter");

  // Register observer for not existing parameter
  std::cout << "Register observer for not existing parameter" << std::endl;
  try {
    gcnfUserApi->getUpdateEvent("not.existing.parameter");
  } catch (gs::cnf::RegisterObserverFailedException &e) {  }

  // Add parameter twice
  std::cout << "Add parameter twice" << std::endl;
  gcnfUserApi->addParam("new.param");
  std::cout << "#1 done" << std::endl;
  //gcnfUserApi->addParam("new.param");  // add comment here to avoid error
  std::cout << "#1 done?" << std::endl;
 
  // Set name with wrong parameter name syntax
  std::cout << "Register observer for not existing parameter" << std::endl;
  gcnfUserApi->setParam("my;parameter.name", "value");

  std::cout << "parameter list:"<<std::endl;
  gs::ctr::show_vector(gcnfUserApi->getParamList());
 
}
