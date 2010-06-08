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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
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
