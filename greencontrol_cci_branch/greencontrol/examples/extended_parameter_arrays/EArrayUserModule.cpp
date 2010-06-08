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

#include "EArrayUserModule.h"

using namespace std;
using namespace sc_core;

void EArrayUserModule::main_action() {

  wait(1, SC_NS);
  
  cout << endl << "EArrayUserModule action" << endl;
  
  gs::gs_param_base *tmp = m_Api->getPar("Owner.myTopArr");
  gs::gs_param_array &topArr = *(dynamic_cast<gs::gs_param_array*>(tmp));
  
  cout << "Array '"<< topArr.getName() << "': " << topArr.getString() << endl;
  cout << endl << endl;
  
  cout << name() << " ********** Register this as an observer of topArr." << endl;
  GC_REGISTER_PARAM_CALLBACK(&topArr, EArrayUserModule, config_callback);
  cout << endl << endl;
  
  cout << "myTopArr: ********** Set callback behavior to CALLBACK_MEMBER_CHANGES" << endl;
  topArr.SET_CALLBACK_BEHAVIOR(gs::cnf::CallbackBehavior::CALLBACK_MEMBER_CHANGES);
  REGR_TEST("PASS/FAIL: Passed if behavior is stored in callback behavior map");
  cout << endl << endl;
  
  cout << "myTopArr: ********** Change value of member my2ndArray0.myIntPar0 which should be called back!" << endl;
  cout << "                     Checks if callback of changed members is forwarded by the array." << endl;
  gs::gs_param<int> *p1 = dynamic_cast<gs::gs_param<int>*>(topArr.getMemberArray("my2ndArray0")->getMember("myIntPar0"));
  *p1 = 200;
  REGR_TEST("PASS/FAIL: Passed if value change callback is forwarded and submitted to this module '"<<name()<<"'");
  cout << endl << endl;

  gs::gs_param_array *m1bArr = topArr.getMemberArray("my2ndArray0");
  gs::gs_param<int> *p1b = ((*m1bArr)[std::string("myIntPar0")].get_gs_param<int>());
  REGR_TEST("PASS/FAIL: Passed if value change is still the same (200) '"<<name()<<"'="<<*p1b);
  cout << endl << endl;
  
  cout << "myTopArr: ********** Set callback behavior to CALLBACK_MEMBER_CHANGES" << endl;
  topArr.SET_CALLBACK_BEHAVIOR(gs::cnf::CallbackBehavior::CALLBACK_MEMBER_CHANGES);
  REGR_TEST("PASS/FAIL: Passed if callback behavior is NOT changed");
  cout << endl << endl;
  
  cout << "myTopArr: ********** Set callback behavior to NOT_CALLBACK_MEMBER_CHANGES" << endl;
  topArr.SET_CALLBACK_BEHAVIOR(gs::cnf::CallbackBehavior::NOT_CALLBACK_MEMBER_CHANGES);
  REGR_TEST("PASS/FAIL: Passed if callback behavior is changed to NOT_CALLBACK_CHANGES");
  cout << endl << endl;
  
  cout << "myTopArr: ********** Change value of member my2ndArray0.myIntPar0 which should NOT be called back!" << endl;
  *p1 = 100;
  REGR_TEST("PASS/FAIL: Passed if this change is NOT submitted to this module '"<<name()<<"'");
  cout << endl << endl;
  
  cout << "myTopArr: ********** Set callback behavior to CALLBACK_MEMBER_CHANGES" << endl;
  topArr.SET_CALLBACK_BEHAVIOR(gs::cnf::CallbackBehavior::CALLBACK_MEMBER_CHANGES);
  REGR_TEST("PASS/FAIL: Passed if callback behavior is changed to CALLBACK_CHANGES");
  cout << endl << endl;

  wait(3, SC_NS);
  REGR_TEST("PASS/FAIL: Passed if value changes callbacks were forwarded and submitted to this module '"<<name()<<"'");
  cout << endl << endl;

  cout << "Array '"<< topArr.getName() << "': " << topArr.getString() << endl;
  cout << endl << endl;

  cout << "myTopArr: ********** Test iterator" << endl;
  gs::gs_param_array::iterator it;
  for (it = topArr.begin(); it != topArr.end(); ++it) {
    cout << (*it)->getName() << "="<< (*it)->getString()<< endl;
  }
  REGR_TEST("PASS/FAIL: Passed if iterator went through the two members of myTopArr");
  cout << endl << endl;

  cout << "my2ndArray0: ********** 2nd Test iterator" << endl;
  gs::gs_param_array::iterator it2;
  for (it2 = topArr.getMemberArray("my2ndArray0")->begin(); it2 != topArr.getMemberArray("my2ndArray0")->end(); ++it2) {
    cout << (*it2)->getName() << "="<< (*it2)->getString()<< endl;
  }
  REGR_TEST("PASS/FAIL: Passed if iterator went through the members of my2ndArray0");
  cout << endl << endl;
  
  cout << "my2ndArray0: ********** Test find(name) function to get iterator" << endl;
  gs::gs_param_array::iterator it3;
  it3 = topArr.getMemberArray("my2ndArray0")->find("myIntPar1");
  cout << "find(myIntPar1) -> iterator points to member '"<< (*it3)->getName() << "'"<<endl;
  REGR_TEST("PASS/FAIL: Passed if iterator points to the member 'Owner.myTopArr.my2ndArray0.myIntPar1'");
  cout << endl << endl;
  
}

void EArrayUserModule::config_callback(gs::gs_param_base& par) {
  cout << name() << ": Callback method called:" << endl;
  cout << "  Parameter '" << par.getName() << "'"<< endl
  << "    of type '" << par.getTypeString() << "'" << endl;
  
  if (par.is_destructing()) {
    cout << "    is just being destroyed!" << endl;
  } 
  
  else if (par.is_extended_array()) {
    cout << "    is a changed Extended Parameter Array (e.g. with changed member number)" << endl;
    cout << par.getString() << endl; 
  }
  
  else {
    cout << "    changed to value '" << par.getString() << "'." << endl;
  }
  cout << endl;
 
}
