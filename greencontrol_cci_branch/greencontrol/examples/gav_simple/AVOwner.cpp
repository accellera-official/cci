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


#include "AVOwner.h"

using namespace std;
using namespace sc_core;

void AVOwner::main_action() {

  cout << endl << endl;
  cout << name() << ": set parameters to different values at different time points" << endl;

  //m_GAV_Api.add_to_default_output(gs::av::CSV_FILE_OUT, &uint_param);
  uint_param = 1;

  wait(1, SC_NS);
  int_param = 100;
  uint_param = 670;
  str_param = "Hello World!";
  int_param = 101;
  uint64_param = 18446744073709551615ULL;
  logic_param = 'z';
  wait(SC_ZERO_TIME);
  
  wait(1, SC_NS);
  int_param = 102;
  int_param = 103;
  int_param = 104;
  wait(SC_ZERO_TIME);
  int_param = 105;
  int_param = 106;
  
  str_param = "Hello Germany!";
  wait(SC_ZERO_TIME);
  str_param = "Hello Arizona!";
  wait(SC_ZERO_TIME);
  str_param = "Hello France!";

  uint64_param = 18446744073709551614ULL;
  logic_param = 1;

  cout << endl << endl;

}
