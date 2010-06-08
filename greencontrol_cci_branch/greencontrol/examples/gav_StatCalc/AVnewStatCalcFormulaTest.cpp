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


#include "AVnewStatCalcFormulaTest.h"

using namespace std;
using namespace sc_core;
using namespace sc_dt;

void AVnewStatCalcFormulaTest::main_action() {

  cout << "--------------------------------------------------------" << endl;
  cout << "***************  TEST 1: Formula with POD types  *****************" << endl;
  gs::gs_param<double> inp_dbl_par("inp_dbl_par");
  gs::gs_param<int> inp_int_par("inp_int_par");
  gs::gs_param<int> inp_int_par2("inp_int_par2");
  gs::gs_param<unsigned int> inp_uint_par("inp_uint_par");
  /*inp_int_par = 99;
  inp_int_par2 = 2;
  inp_uint_par = 202;
  inp_dbl_par = 0.5;*/
  // (((99-2) / 2) + (.5*202) = 149.5

  inp_int_par = 99;
  inp_int_par2 = 2;
  inp_uint_par = 202;
  inp_dbl_par = 1.5;
  // (((99-2) / 2) + (1.5*202) = 351.5
  
  // (((int-int2) / int2) + (dbl*uint))

  gs::av::Calculator<double> c1("no1");
  c1.calc("+", c1.calc("/", c1.calc("-", inp_int_par, inp_int_par2), inp_int_par2), c1.calc("*", inp_dbl_par, inp_uint_par));
  gs::av::StatCalc<double> s1("StatCalc1", &c1);
  
  GC_REGISTER_PARAM_CALLBACK(s1.get_result_param(), AVnewStatCalcFormulaTest, result_callback);

  cout <<endl<< "Result StCalc s1 = " << *s1.get_result_param() <<endl<< endl;
  
  inp_uint_par = 202;
  cout <<endl<< "Result StCalc s1 = " << *s1.get_result_param() <<endl<< endl;

  inp_uint_par = 502;
  cout <<endl<< "Result StCalc s1 = " << *s1.get_result_param() <<endl<< endl;

  GAV_REGR_TEST("PASS/FAIL: Passed if results of s1 == 0, 351.5, 801.5");

  
  cout << "--------------------------------------------------------" << endl;
  cout << "***************  TEST 2: Formula with SystemC types  *****************" << endl;
  gs::gs_param<sc_int<10> >    bint("bint", 10);
  gs::gs_param<sc_uint<10> >   buint("buint", 10);
  gs::gs_param<sc_biguint<70> >bbuint("bbuint", 10);
  gs::gs_param<sc_bigint<70> > bbint("bbint", 10);
  bint = 10;
  buint = 100;
  bbuint = 2;
  bbint = 1000000000;
  // ((( 1000000000-100  ) / 2     ) + 10  )) = 499999960
  // ((((bbint     -buint) / bbuint) + bint))
  gs::av::Calculator<unsigned long long> cB("no2");
  cB.calc("+", cB.calc("/", cB.calc("-", bbint, buint), bbuint), bint);
  gs::av::StatCalc<unsigned long long> sB("no2", &cB);

  GC_REGISTER_PARAM_CALLBACK(sB.get_result_param(), AVnewStatCalcFormulaTest, result_callback);
  
  cout <<endl<< "Result StCalc sB = " << *(sB.get_result_param()) <<endl<< endl;

  bint = 10;
  cout <<endl<< "Result StCalc sB = " << *(sB.get_result_param()) <<endl<< endl;

  // = 499999980
  bint = 30;
  cout <<endl<< "Result StCalc sB = " << *(sB.get_result_param()) <<endl<< endl;

  GAV_REGR_TEST("PASS/FAIL: Passed if results of sB == 0, 499999960, 499999980");

  
  cout << "--------------------------------------------------------" << endl;
  cout << "***************  TEST: Test bool operators  *****************" << endl;
  // TODO: test bool operators
  gs::gs_param<bool> bool1("bool1", true);
  gs::gs_param<bool> bool2("bool2", false);
  
  gs::av::Calculator_bit<bool> c3("no3");
  c3.calc("|", bool1, bool2);
  gs::av::StatCalc<bool> s3("no3", &c3);

  bool1 = true;
  cout <<endl<< "Result StCalc s3 = " << *(s3.get_result_param()) << " (should be true)" <<endl<< endl;
  bool1 = false;
  cout <<endl<< "Result StCalc s3 = " << *(s3.get_result_param()) << " (should be false)" <<endl<< endl;

  GAV_REGR_TEST("PASS/FAIL: Passed if results of s3 == 1 (=true), 0 (=false)");
 

  
  cout << "--------------------------------------------------------" << endl;
  cout << "***************  TEST: Test convenient operators  *****************" << endl;
  inp_int_par = 5;
  inp_int_par2 = 3;
  gs::av::Calculator<int> ca("calcA");
  gs::gs_param_base &b1 = inp_int_par;
  gs::gs_param_base &b2 = inp_int_par2;
  //gs::gs_param_base &bu3 = inp_uint_par;
  ca(ca(b1 - ca(b1 + b2)) + ca(ca(b1 * b2) / b2)); // (b1 - (b1+b2)) + ((b1*b2) / b2)
  //                                                   5  -  5 + 3   + (  5*3   / 3 ) = 2
  gs::av::StatCalc<int> sa("StcA", ca);
  inp_int_par = 5;
  cout << "result calcA = " << sa.get_result_param()->getString() << endl;
  GAV_REGR_TEST("PASS/FAIL: Test convenient operators passed if result of calcA == 2");


  cout << "--------------------------------------------------------" << endl;
  cout << "***************  TEST: Test mix calc and convenient operators  *****************" << endl;
  gs::av::Calculator<int> ca2("calcA2");
  ca2(b1 - ca2.calc("+", inp_int_par, inp_int_par2)); // (b1 - (inp_int_par + inp_int_par2))
  gs::av::StatCalc<int> sa2("StcA", ca2);
  inp_int_par = 5;
  cout << "result calcA2 = " << sa2.get_result_param()->getString() << endl;
  GAV_REGR_TEST("PASS/FAIL: Test convenient operators passed if result of calcA2 == -3");
  
  
  cout << "--------------------------------------------------------" << endl;
  cout << "***************  TEST: Test bitwise operations  *****************" << endl;
  gs::av::Calculator_bit<int> cb("calcB");
  cb.calc("|", cb.calc("&", inp_int_par, inp_int_par2), inp_uint_par); // (b1 & b2) | bu3
  gs::av::StatCalc<int> sb("StcB", cb);
  //   56   &   63    |    1664
  // 111000   111111
  //   = 111011       |    1664
  //                     11010000000
  //   = 11010111000 = 1720
  cout << "manually ((56 & 3) | 1664) = (" << (56 & 63)<< ") | 1664 = " << ((56 & 63) | 1664) << endl;
  inp_int_par = 56; inp_int_par2 = 63; inp_uint_par = 1664;
  cout << "result calcB = " << sb.get_result_param()->getString() << endl;
  GAV_REGR_TEST("PASS/FAIL: Test bitwise operations passed if result of calcB == 1720");
  
  
  cout << "--------------------------------------------------------" << endl;
  cout << "***************  TEST: Test double calc and constant parameter  *****************" << endl;
  gs::av::Calculator<double> c("calc");
  gs::gs_param_base &i1 = inp_int_par;
  gs::gs_param_base &i2 = inp_int_par2;
  gs::gs_param_base &d  = inp_dbl_par;
  c.calc("/",c( c(i1 + i2) + c(d * i2) ),2.5);  // ((i1+i2 + d*i2)  / 2)
  //                                              56+63 + 30.5*63 / 2
  //                                               119    1921.5   
  cout << "manually (((56+63) + 30.5*63) / 2.5) = " << (((56+63) + 30.5*63) / 2.5) << endl;
  gs::av::StatCalc<double> s("Stc", c);
  inp_dbl_par = 30.5;
  cout << "result calc = " << s.get_result_param()->getString() << endl;
  GAV_REGR_TEST("PASS/FAIL: Test double calc and constant parameter passed if result of calc == 816.2");

  
  cout << "--------------------------------------------------------" << endl;
  cout << "***************  TEST: Test static parameters  *****************" << endl;
  gs::av::Calculator<int> cc("calcC");
  cc.calc("+", cc.calc("-", b1, 5), 412); // (b1 - 5) + 412
  gs::av::StatCalc<int> sc("StcC", cc);
  inp_int_par = 10;
  cout << "result calcC = " << sc.get_result_param()->getString() << endl;
  GAV_REGR_TEST("PASS/FAIL: Test static parameters passed if result of calcC == 417");


  cout << "--------------------------------------------------------" << endl;
  cout << "***************  TEST: Test empty calculator  *****************" << endl;
  gs::av::Calculator<int> cEmpty("calcEmpty");
  gs::av::Trigger td;
  gs::av::StatCalc<int> sd("StcD", td, cEmpty);
  td.calculate_now();
  GAV_REGR_TEST("PASS/FAIL: Test empty calculator passed if no error when being called");
  
  
  cout << "--------------------------------------------------------" << endl;
  cout << "***************  TEST: Test user added function  *****************" << endl;
  gs::av::Calculator<int> cE("calcE");
  cE.calc("average", inp_int_par, inp_int_par2);
  gs::av::StatCalc<int> se("StcE", cE);
  inp_int_par = 44;
  inp_int_par2 = 60;
  cout << "result calcE = " << se.get_result_param()->getString() << endl;
  GAV_REGR_TEST("PASS/FAIL: Test user added function 'average'; passed if average was called successfully, result == 52");

  gs::av::Calculator_bit<int> cF("calcF");
  cF.calc("average", inp_int_par, inp_int_par2);
  gs::av::StatCalc<int> sF("StcF", cF);
  inp_int_par = 44;
  cout << "result calcF = " << sF.get_result_param()->getString() << endl;
  GAV_REGR_TEST("PASS/FAIL: Test user added function 'average' in Calculator_bit; passed if average was called successfully, result == 52");

  gs::av::Calculator<double> cG("calcG");
  cG.calc("div3", inp_int_par, inp_int_par2);
  gs::av::StatCalc<double> sg("StcG", cG);
  inp_int_par = 38;
  cout << "result calcG = " << sg.get_result_param()->getString() << endl;
  GAV_REGR_TEST("PASS/FAIL: Test user added function 'div3'; passed if average was called successfully, result == (38+60)/3 = 32.66...");
  
}
  
void AVnewStatCalcFormulaTest::result_callback(gs::gs_param_base& par) {
  cout << "Callback" << endl;
  if (!par.is_destructing()) {
    cout << "Parameter " << par.getName() << " = " << par.getString() << endl;
  } else 
    cout << "Parameter " << par.getName() << " is destructing." << endl;
}
