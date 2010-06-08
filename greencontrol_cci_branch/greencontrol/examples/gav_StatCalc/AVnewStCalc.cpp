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


#include "AVnewStCalc.h"

using namespace std;
using namespace sc_core;

void AVnewStatCalc::main_action() {

  wait(100, SC_NS);
  
  /*gs::gs_param_base *int_par_b  = m_configAPI->getPar("Owner.int_param");
  gs::gs_param<int> &int_par = *dynamic_cast<gs::gs_param<int>*>(int_par_b);
  gs::gs_param_base *uint_par_b = m_configAPI->getPar("Owner.uint_param");
  gs::gs_param<unsigned int> &uint_par = *dynamic_cast<gs::gs_param<unsigned int>*>(uint_par_b);
  m_analysisAPI.add_to_default_output(STDOUT_OUT, &int_par);
  m_analysisAPI.add_to_default_output(STDOUT_OUT, &uint_par);
  */  
  gs::gs_param<int> int_par("int_par");
  gs::gs_param<unsigned int> uint_par("uint_par");
  m_analysisAPI.add_to_default_output(gs::av::STDOUT_OUT, &int_par);
  m_analysisAPI.add_to_default_output(gs::av::STDOUT_OUT, &uint_par);

  int_par = 0;
  uint_par = 0;

  // create StatCalc 'StatCalcTom'
  gs::av::Trigger *stc_tom_t = new gs::av::Trigger(true);
  gs::av::Calculator<int> *stc_tom_c = new gs::av::Calculator<int>("Tom");
  stc_tom_c->calc("+", int_par, uint_par);
  gs::av::StatCalc<int> *stc_tom = new gs::av::StatCalc<int>("StatCalcTom", stc_tom_t, stc_tom_c);

  gs::av::StatCalc<int> *stc_tom_name_check = new gs::av::StatCalc<int>("StatCalcTom", stc_tom_c);
  GAV_REGR_TEST("PASS/FAIL: Passed if second StatCalc instance 'StatCalcTom'will we renamed.");
  delete stc_tom_name_check;
  
  // add StCalc results to output
  m_analysisAPI.add_to_default_output(gs::av::STDOUT_OUT, stc_tom->get_result_param() );
  
 
  // test default callback mode
  // test (de)activation
  int_par = 1;
  uint_par = 2;
  int_par = 10;
  uint_par = 20;
  GAV_REGR_TEST("PASS/FAIL: Passed if each input parameter change does result in a recalculation, results = 1, 3, 12, 30");
  stc_tom->deactivate();
  int_par = 11;
  uint_par = 21;
  GAV_REGR_TEST("PASS/FAIL: Passed if no recalculation because of deactivated");
  stc_tom->activate();
  int_par = 40;
  uint_par = 50;
  GAV_REGR_TEST("PASS/FAIL: Passed re-activated.");
  
  // test disable callbacks
  stc_tom_t->disable_on_change_activation();
  int_par = 100;
  uint_par = 200;
  GAV_REGR_TEST("PASS/FAIL: Passed if no automatic recalculation on input parameter changes because callbacks are unregistered.");
  stc_tom_t->enable_on_change_activation();

  // delete StCalc
  delete stc_tom; stc_tom = NULL;
  delete stc_tom_t; stc_tom_t = NULL;
  delete stc_tom_c; stc_tom_c = NULL;
  int_par = 1;
  uint_par = 2;
  GAV_REGR_TEST("PASS/FAIL: Passed if nothing happens at change of int_par and uint_par because stc was deleted.");
  
  
  // ***** Test different calculation events:
  
  
  // test condition
  gs::gs_param<bool> condition("condition");
  gs::av::Trigger stc_regina_t(condition);
  gs::av::Calculator<int> stc_regina_c("Regina");
  stc_regina_c.calc("+", int_par, uint_par);
  gs::av::StatCalc<int> *stc_regina = new gs::av::StatCalc<int>("StatCalcRegina", &stc_regina_t, &stc_regina_c);
  m_analysisAPI.add_to_default_output(gs::av::STDOUT_OUT, stc_regina->get_result_param() );

  condition = true;
  int_par = 1000;
  GAV_REGR_TEST("PASS/FAIL: Passed if calculation done because condition is true!");
  condition = false;
  int_par = 2000;
  GAV_REGR_TEST("PASS/FAIL: Passed if no calculation because condition is false!");
  condition = true;
  int_par = 100;
  GAV_REGR_TEST("PASS/FAIL: Passed if calculation done because condition is now true again!");
  
  stc_regina_t.disable_on_change_activation();
  
  // test manual call of calculate_now()
  // test (de)activation
  int_par = 5;
  uint_par = 10;
  stc_regina_t.calculate_now();
  GAV_REGR_TEST("PASS/FAIL: Passed if manually called calculation, result = 15");
  stc_regina->deactivate();
  int_par = 10;
  stc_regina_t.calculate_now();
  GAV_REGR_TEST("PASS/FAIL: Passed if deactivated, manually called calculation, BUT NO CALCULATION DONE");
  stc_regina->activate();
  stc_regina_t.calculate_now();
  GAV_REGR_TEST("PASS/FAIL: Passed if manually called calculation after re-activation, result = 20");
  
  
  // set sc_event activation
  // test (de)activation
  sc_event *event0;
  event0 = new sc_event;
  printf("@%s /%d: ", sc_core::sc_time_stamp().to_string().c_str(), (unsigned)sc_core::sc_delta_count());
  cout << "set event" << endl;

  // create StatCalc 'StatCalcCordula'
  gs::av::Trigger *stc_cordula_t = new gs::av::Trigger(*event0);
  gs::av::Calculator<int> *stc_cordula_c = new gs::av::Calculator<int>("Cordula");
  stc_cordula_c->calc("+", int_par, uint_par);
  gs::av::StatCalc<int> *stc_cordula = new gs::av::StatCalc<int>("StatCalcCordula", stc_cordula_t, stc_cordula_c);
  m_analysisAPI.add_to_default_output(gs::av::STDOUT_OUT, stc_cordula->get_result_param() );

  int_par = 99;
  uint_par = 1000;
  wait(1,SC_NS);
  event0->notify();
  event0->notify(1, SC_NS);
  wait(5, SC_NS);
  GAV_REGR_TEST("PASS/FAIL: Passed if calculation of 'StatCalcCordula' done after event notified 1 and 2 ns after setting it. result = 1099!");
  delete event0;
  // TODO: remove event: not yet supported because of SystemC limitations
  // stc_c->remove_event();
  //event0->notify();
  wait(5, SC_NS);
  
  cout <<endl<< "***** sample interval BEGIN ********" << endl;
  // create StatCalc 'StatCalcBob'
  gs::av::Trigger *stc_bob_t = new gs::av::Trigger(10, SC_NS);
  gs::av::Calculator<int> *stc_bob_c = new gs::av::Calculator<int>("Bob");
  stc_bob_c->calc("+", int_par, uint_par);
  gs::av::StatCalc<int> *stc_bob = new gs::av::StatCalc<int>("StatCalcBob", stc_bob_t, stc_bob_c);
  m_analysisAPI.add_to_default_output(gs::av::STDOUT_OUT, stc_bob->get_result_param() );
  
  wait(5, SC_NS);
  int_par = 1;
  wait(14, SC_NS);
  int_par = 2;
  wait(36, SC_NS);
  // remove interval
  stc_bob_t->set_sample_interval(0, SC_NS);
  GAV_REGR_TEST("PASS/FAIL: Passed if calculation of 'StatCalcBob' done after each 10 ns for 40 ns!");
  cout <<endl<< "***** sample interval END ********" << endl<<endl<<endl;
  
  
  // test deleting an input parameter while activated
  gs::gs_param<int> *t1 = new gs::gs_param<int>("t1");
  gs::gs_param<unsigned int> *t2 = new gs::gs_param<unsigned int>("t2");
  gs::av::Trigger *stc_bruce_t = new gs::av::Trigger();
  gs::av::Calculator<int> *stc_bruce_c = new gs::av::Calculator<int>("Bruce");
  stc_bruce_c->calc("+", *t1, *t2);
  gs::av::StatCalc<int> *stc_bruce = new gs::av::StatCalc<int>("StatCalcBruce", stc_bruce_t, stc_bruce_c);
  m_analysisAPI.add_to_default_output(gs::av::STDOUT_OUT, stc_bruce->get_result_param() );
  *t1 = 10;
  *t2 = 20;
  delete t1; t1 = NULL;
  GAV_REGR_TEST("PASS/FAIL: Passed if 'StatCalcBruce' deactivated because of deleted input parameter!");
  stc_bruce->activate();
  GAV_REGR_TEST("PASS/FAIL: Passed if 'StatCalcBruce' activation failed because of deleted input parameter!");
  *t2 = 30;
  delete t2; t2 = NULL;
  
  
  // test statistics: Sliding window.
  wait(1, SC_NS);
  gs::gs_param<int> *r1 = new gs::gs_param<int>("r1");
  gs::gs_param<unsigned int> *r2 = new gs::gs_param<unsigned int>("r2");
  *r1 = 1;
  *r2 = 10;

  // create StatCalc 'StatCalcDanny'
  gs::av::Trigger *stc_danny_t = new gs::av::Trigger(10, SC_NS);
  gs::av::Calculator<int> *stc_danny_c = new gs::av::Calculator<int>("Danny");
  stc_danny_c->calc("+", *r1, *r2);
  stc_danny_c->enable_sliding_window(5); // sliding window
  gs::av::StatCalc<int> *stc_danny = new gs::av::StatCalc<int>("StatCalcDanny", stc_danny_t, stc_danny_c);
  m_analysisAPI.add_to_default_output(gs::av::STDOUT_OUT, stc_danny->get_result_param() );

  cout <<endl<< "***** sliding window sample interval BEGIN ********" << endl;
  wait(5, SC_NS);
  *r1 = 22;
  wait(14, SC_NS);
  // OutputPlugin pause test:
  //op->pause(40, SC_NS);
  //sc_event ev;
  //op->pause(ev);
  *r1 = 33;
  wait(36, SC_NS);
  *r1 = -144;
  wait(5, SC_NS);
  //ev.notify();
  //op->resume();
  *r1 = 155;
  wait(10, SC_NS);
  stc_danny_t->set_sample_interval(0, SC_NS);
  cout <<endl<< "***** sliding window sample interval END ********" << endl;
  GAV_REGR_TEST("PASS/FAIL: Passed if 'StatCalcDanny's result is a sliding window over the input calculations (results=32,37,39,40,40,7,32)!");
  delete r1; r1 = NULL;
  delete r2; r2 = NULL;
  
  wait(10, SC_US);
  
  
  // Test activated gs_param
  cout << endl << "*** Test activated gs_param ***" << endl;
  // create StatCalc 'StatCalcRob'
  gs::av::Calculator<int> *stc_rob_c = new gs::av::Calculator<int>("Rob");
  stc_rob_c->calc("+", int_par, uint_par);
  gs::av::StatCalc<int> *stc_rob = new gs::av::StatCalc<int>("StatCalcRob", stc_rob_c);

  gs::gs_param<bool> *stc_rob_activated = m_configAPI->get_gs_param<bool>(std::string(stc_rob->name())+".activated");
  if (!stc_rob_activated) {
    GAV_REGR_TEST("FAILED: 'StatCalcRob' activated param not existing!");
  } else {
    uint_par = 1;
    cout << "result = "<<stc_rob->get_result_param()->getString() << endl;
    *stc_rob_activated = true;
    int_par = 12;
    cout << "result = "<<stc_rob->get_result_param()->getString() << endl;
    *stc_rob_activated = false;
    int_par = 13;
    cout << "result = "<<stc_rob->get_result_param()->getString() << endl;
    *stc_rob_activated = false;
    int_par = 14;
    cout << "result = "<<stc_rob->get_result_param()->getString() << endl;
    *stc_rob_activated = true;
    int_par = 15;
    cout << "result = "<<stc_rob->get_result_param()->getString() << endl;
    GAV_REGR_TEST("PASS/FAIL: Passed if 'StatCalcRob' activated param disabled the StatCalc successfully!");
  }
  

  cout << "AVnewStCalc end" << endl;
  
  


}
