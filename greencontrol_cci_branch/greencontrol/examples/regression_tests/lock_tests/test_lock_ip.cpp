//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2010 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

#include "test_lock_ip.h"

using namespace std;
using namespace sc_core;
using namespace sc_dt;

#define IP3test(a) cout << "IPLock test: " << a <<endl<<flush
#define SPACE cout << endl<<flush

void Test_Lock_IP::main_action() {

  SPACE;
  GC_DUMP_N(name(), "This is the main action in Test_Lock_IP");
  SPACE;

  // /////////////////////////////////////////////////////////////////////////
  // ///////    gs_param lock tests    ///////////////////////////////////////
  // /////////////////////////////////////////////////////////////////////////
  
  
  // Lock tests:
  //
  // - lock without password
  // - check successful lock with all access functions
  // - override function succeeds
  // - lock with password
  // - override function without (and with wrong) password fail
  // - override function with correct password succeeds
  // - unlock works with correct pwd (not with wrong one)
  
  SPACE; SPACE;
  IP3test("******************* ---------------- **************************");
  IP3test("******************* INT (etc.) PARAM **************************");
  IP3test("******************* ---------------- **************************");
  SPACE; SPACE;
  
  int_param = 1;
  IP3test("'int_param = (unsigned int) 1' int_param="<<int_param);

  if (int_param.lock())
    IP3test("***** int_param locked *****");
  else
    SC_REPORT_ERROR(name(), "param lock failed!");

  IP3test("shall all fail:");
  
  SPACE;

  int_param = 2000;
  IP3test("'int_param = 2000' int_param="<<int_param);
  
  int_param += 1500;
  IP3test("'int_param += 1500' : int_param="<<int_param);

  IP3test("int_param++="<<int_param++);
  IP3test("int_param="<<int_param);

  IP3test("++int_param="<<++int_param);
  IP3test("int_param="<<int_param);

  IP3test("int_param--="<<int_param--);
  IP3test("int_param="<<int_param);

  IP3test("--int_param="<<--int_param);
  IP3test("int_param="<<int_param);

  int_param += 1500;
  IP3test("'int_param += 1500' : int_param="<<int_param);

  int_param -= 200;
  IP3test("'int_param -= 200' : int_param="<<int_param);

  int_param /= 4;
  IP3test("'int_param /= 4' : int_param="<<int_param);

  int_param *= 3;
  IP3test("'int_param *= 3' : int_param="<<int_param);

  int_param %= 6;
  IP3test("'int_param %= 6' : int_param="<<int_param);

  int_param ^= 5;
  IP3test("'int_param ^= 5' : int_param="<<int_param);

  int_param &= 60;
  IP3test("'int_param &= 60' : int_param="<<int_param);

  int_param |= 3;
  IP3test("'int_param |= 3' : int_param="<<int_param);

  int u = int_param; u <<= 2;
  int_param <<= 2;
  IP3test("'int_param <<= 2' : int_param ("<<u<<") ="<<int_param);

  u >>= 2;
  int_param >>= 2;
  IP3test("'int_param >>= 2' : int_param ("<<u<<") ="<<int_param);
  
  SPACE;

  IP3test("Override lock shall work: int_param=2");
  int_param.setValue(2, NULL);
  IP3test("'int_param = 2' : int_param ="<<int_param);

  SPACE;
  
  void* pwd = &u;
  void* pwd2 = &int_param;

  if (int_param.lock(pwd))
    IP3test("***** int_param locked with pwd *****");
  else
    SC_REPORT_ERROR(name(), "param lock failed!");
  
  SPACE;
  int_param = 2000;
  IP3test("'int_param = 2000' shall fail: int_param="<<int_param);

  SPACE;
  IP3test("Override lock without password shall FAIL:");
  int_param.setValue(3, NULL);
  IP3test("'int_param = 3' : int_param ="<<int_param);

  SPACE;
  IP3test("Override lock with WRONG password shall FAIL:");
  int_param.setValue(5, pwd2);
  IP3test("'int_param = 5' : int_param ="<<int_param);
  
  SPACE;
  IP3test("Override lock with correct pwd shall work:");
  int_param.setValue(4, pwd);
  IP3test("'int_param = 4' : int_param ="<<int_param);

  SPACE;
  if (int_param.unlock())
    SC_REPORT_ERROR(name(), "param unlock worked with wrong pwd!");
  else
    IP3test("int_param unlock failed (and should fail) with wrong password");

  SPACE;
  if (int_param.lock(pwd2))
    SC_REPORT_ERROR(name(), "param lock failed!");
  else
    IP3test("int_param lock with new pwd failed (and should have failed!)");
  
  SPACE;

  int_param.unlock(pwd);
  IP3test("***** int_param unlocked *****");

  SPACE;


  // /////////////////////////////////////////////////////////////////////////
  // ///////    std::string:   Full Test of all operators    /////////////////
  // /////////////////////////////////////////////////////////////////////////

  SPACE; SPACE;
  IP3test("******************* ---------------- **************************");
  IP3test("*******************   STRING PARAM   **************************");
  IP3test("******************* ---------------- **************************");
  SPACE; SPACE;

  str_param = "unchanged";
  IP3test("str_param = "<<str_param);

  SPACE;
  IP3test("***** lock str_param *****");
  str_param.lock();
  
  string stra("1000");
  str_param = stra;
  IP3test("'str_param = \"1000\"' str_param="<<str_param);

  SPACE;

  IP3test("'str_param != \"1000\"' ");
  if (str_param != "1000")  
    cout<<"               true" <<endl;
  else
    cout<<"               false"<<endl;

  IP3test("'str_param == \"1000\"' ");
  if (str_param == "1000")  
    cout<<"               true" <<endl;
  else
    cout<<"               false"<<endl;

  IP3test("'str_param != \"test\"' ");
  if (str_param != "test")  
    cout<<"               true" <<endl;
  else
    cout<<"               false"<<endl;

  // operator <
  IP3test("'str_param < \"Hallo\"' ");
  if (str_param < "Hallo")  
    cout<<"               true" <<endl;
  else
    cout<<"               false"<<endl;

  IP3test("str_param < str_param2");
  if (str_param < str_param2)  
    cout<<"               true" <<endl;
  else
    cout<<"               false"<<endl;

  IP3test("\"Hallo\" < str_param ");
  if ("Hallo" < str_param)  
    cout<<"               true" <<endl;
  else
    cout<<"               false"<<endl;

  SPACE;

  IP3test("'str_param > \"Hallo\"' ");
  if (str_param > "Hallo")  
    cout<<"               true" <<endl;
  else
    cout<<"               false"<<endl;

  IP3test("'str_param <= \"Hallo\"' ");
  if (str_param <= "Hallo")  
    cout<<"               true" <<endl;
  else
    cout<<"               false"<<endl;

  IP3test("'str_param >= \"Hallo\"' ");
  if (str_param >= "Hallo")  
    cout<<"               true" <<endl;
  else
    cout<<"               false"<<endl;

  SPACE;

  str_param = "Hello ";
  IP3test("str_param = '"<<str_param<<"'");
  str_param2 = "it's me";
  IP3test("str_param2 = '"<<str_param2<<"'");

  // operator +
  str_param = str_param + str_param2;
  IP3test("'str_param = str_param + str_param2' = '"<< str_param<<"'");

  str_param = str_param + " after";
  IP3test("'str_param = str_param + \" after\"' = '"<< str_param<<"'");

  str_param = "before " + str_param;
  IP3test("'str_param = \"before \" + str_param' = '"<< str_param<<"'");

  SPACE;

  std::stringstream ss;
  ss << str_param;
  IP3test("'ss << str_param: ss = '"<<ss.str()<<"'");

  std::stringstream ss2;
  ss2 << str_param;
  string test_str2;
  ss2 >> test_str2;
  ss >> str_param2;
  IP3test("'ss >> str_param2: str_param2 ('"<< test_str2<<"')= '"<<str_param2<<"'");

  SPACE;
  IP3test("***** unlock str_param *****");
  str_param.lock();

  SPACE;

}

