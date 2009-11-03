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

#include "testip.h"

void TestIP::main_action() {

  std::cout << "."<<std::endl;
  /*  cout << "This is the main action in TestIP" << endl<<flush;
      
  // create Transaction an send it to config plugin
  ControlTransactionHandle th = api.init_port.create_transaction();
  th->set_mCmd(CONFIG);
  th->set_mTarget("TestTool");
  th->set_mValue("TestValue2");

  ControlPhase p(ControlPhase::CONTROL);
  //cout << "notify init_port" <<endl<<flush;
  //ControlTransactionContainer ctc = ControlTransactionContainer(th,p);
  //api.init_port.out->notify(ctc, PEQ_IMMEDIATE);
  //cout << "wait for init_port... " <<endl<<flush;
  //api.init_port.wait(ControlTransactionContainer(th,p));
  //cout << "... init_port TestIP notified" <<endl<<flush;
  */

}
