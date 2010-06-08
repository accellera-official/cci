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
//   Marius Montón <marius.monton@uab.cat>,
//     Hardware-Software Prototypes and Solutions Lab (CEPHIS). 
//     Microelectronics and Electronics SystemS Dpt. UAB.
//     http://cephis.uab.cat
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

#include "testip3.h"

using namespace std;
using namespace sc_core;
using namespace sc_dt;

#define IP3test(a) cout << "IP3 test: " << a <<endl<<flush
#define SPACE cout << endl<<flush

void TestIP3::main_action() {

  GC_PRINT_SPACE;
  GC_DUMP_N(name(), "This is the main action in TestIP3");

  // /////////////////////////////////////////////////////////////////////////
  // ///////    Constructor tests gs_param    ////////////////////////////////
  // /////////////////////////////////////////////////////////////////////////
  
  std::string stname = "TestParam";
  const char* ctname = "TestParam";
  gs::gs_param_base *tp;
  gs::gs_param_array *pararr = new gs::gs_param_array("TestArray");
  // macro constructors
  /*delete tp;*/ tp = new gs::gs_param<int>();
  delete tp; tp = new gs::gs_param<int>(stname);
  delete tp; tp = new gs::gs_param<int>(ctname);
  delete tp; tp = new gs::gs_param<int>(stname, std::string("42"));
  delete tp; tp = new gs::gs_param<int>(stname, std::string("42"), true);
  delete tp; tp = new gs::gs_param<int>(ctname, "42");
  delete tp; tp = new gs::gs_param<int>(stname, "42");
  delete tp; tp = new gs::gs_param<int>(ctname, std::string("42"));
  delete tp; tp = new gs::gs_param<int>(stname, 42);
  delete tp; tp = new gs::gs_param<int>(ctname, 42);
  
  delete tp; tp = new gs::gs_param<int>(pararr);
  delete tp; tp = new gs::gs_param<int>(stname, pararr);
  delete tp; tp = new gs::gs_param<int>(ctname, pararr);
  delete tp; tp = new gs::gs_param<int>(stname, std::string("42"), pararr, false, true);
  delete tp; tp = new gs::gs_param<int>(ctname, "42", pararr);
  delete tp; tp = new gs::gs_param<int>(stname, "42", pararr);
  delete tp; tp = new gs::gs_param<int>(ctname, std::string("42"), pararr);
  delete tp; tp = new gs::gs_param<int>(stname, 42, pararr);
  delete tp; tp = new gs::gs_param<int>(ctname, 42, pararr);
  
  delete tp; tp = new gs::gs_param<int>(*pararr);
  delete tp; tp = new gs::gs_param<int>(stname, *pararr);
  delete tp; tp = new gs::gs_param<int>(ctname, *pararr);
  delete tp; tp = new gs::gs_param<int>(stname, std::string("42"), *pararr, false, true);
  delete tp; tp = new gs::gs_param<int>(ctname, "42", *pararr);
  delete tp; tp = new gs::gs_param<int>(stname, "42", *pararr);
  delete tp; tp = new gs::gs_param<int>(ctname,std::string("42"), *pararr);
  delete tp; tp = new gs::gs_param<int>(stname, 42, *pararr);
  delete tp; tp = new gs::gs_param<int>(ctname, 42, *pararr);
  delete tp; tp = NULL;
  
  // gs_param<string> contructors
  delete tp; tp = new gs::gs_param<std::string>();
  delete tp; tp = new gs::gs_param<std::string>(stname);
  delete tp; tp = new gs::gs_param<std::string>(ctname);
  delete tp; tp = new gs::gs_param<std::string>(stname, std::string("value"));
  delete tp; tp = new gs::gs_param<std::string>(stname, std::string("value"), false);
  delete tp; tp = new gs::gs_param<std::string>(ctname, "value");
  delete tp; tp = new gs::gs_param<std::string>(stname, "value");
  delete tp; tp = new gs::gs_param<std::string>(ctname, std::string("value"));
  delete tp; tp = new gs::gs_param<std::string>(stname, "myString");
  delete tp; tp = new gs::gs_param<std::string>(ctname, "myString");
  
  delete tp; tp = new gs::gs_param<std::string>(pararr);
  delete tp; tp = new gs::gs_param<std::string>(stname, pararr);
  delete tp; tp = new gs::gs_param<std::string>(ctname, pararr);
  delete tp; tp = new gs::gs_param<std::string>(stname, std::string("value"), pararr);
  delete tp; tp = new gs::gs_param<std::string>(ctname, "value", pararr);
  delete tp; tp = new gs::gs_param<std::string>(stname, "value", pararr);
  delete tp; tp = new gs::gs_param<std::string>(ctname, std::string("value"), pararr);
  
  delete tp; tp = new gs::gs_param<std::string>(*pararr);
  delete tp; tp = new gs::gs_param<std::string>(stname, *pararr);
  delete tp; tp = new gs::gs_param<std::string>(ctname, *pararr);
  delete tp; tp = new gs::gs_param<std::string>(stname, std::string("value"), *pararr);
  delete tp; tp = new gs::gs_param<std::string>(ctname, "value", *pararr);
  delete tp; tp = new gs::gs_param<std::string>(stname, "value", *pararr);
  delete tp; tp = new gs::gs_param<std::string>(ctname,std::string("value"), *pararr);
  delete tp; tp = NULL;
  
  // /////////////////////////////////////////////////////////////////////////
  // ///////    Constructor tests gs_param<T*>    ////////////////////////////
  // /////////////////////////////////////////////////////////////////////////
  
  // Test all constructors
  gs::gs_param_array *tpaParent = new gs::gs_param_array("TestParentArray");
  gs::gs_param_base *tpa;
  std::vector<int> initVec;
  initVec.push_back(3); initVec.push_back(98); initVec.push_back(67); initVec.push_back(65);
  /*delete tpa;*/ tpa = new gs::gs_param<int*>() ;
  delete tpa; tpa = new gs::gs_param<int*>(tpaParent);
  delete tpa; tpa = new gs::gs_param<int*>(*tpaParent); 
  delete tpa; tpa = new gs::gs_param<int*>(stname);
  delete tpa; tpa = new gs::gs_param<int*>(stname, tpaParent);
  delete tpa; tpa = new gs::gs_param<int*>(stname, *tpaParent);
  delete tpa; tpa = new gs::gs_param<int*>(stname, 5);
  delete tpa; tpa = new gs::gs_param<int*>(stname, 5, tpaParent);
  delete tpa; tpa = new gs::gs_param<int*>(stname, 5, *tpaParent);
  delete tpa; tpa = new gs::gs_param<int*>(ctname);
  delete tpa; tpa = new gs::gs_param<int*>(ctname, tpaParent);
  delete tpa; tpa = new gs::gs_param<int*>(ctname, *tpaParent);
  delete tpa; tpa = new gs::gs_param<int*>(ctname, 5);
  delete tpa; tpa = new gs::gs_param<int*>(ctname, 5, tpaParent);
  delete tpa; tpa = new gs::gs_param<int*>(ctname, 5, *tpaParent);
  delete tpa; tpa = new gs::gs_param<int*>(stname, initVec);
  delete tpa; tpa = new gs::gs_param<int*>(stname, initVec, tpaParent);
  delete tpa; tpa = new gs::gs_param<int*>(stname, initVec, *tpaParent);
  delete tpa; tpa = new gs::gs_param<int*>(ctname, initVec);
  delete tpa; tpa = new gs::gs_param<int*>(ctname, initVec, tpaParent);
  delete tpa; tpa = new gs::gs_param<int*>(ctname, initVec, *tpaParent);
  delete tpa; tpa = NULL;
  
  // /////////////////////////////////////////////////////////////////////////
  // ///////    Constructor tests gs_param_array    //////////////////////////
  // /////////////////////////////////////////////////////////////////////////
  
  // Test all constructors
  gs::gs_param_array *tparr;
  /*delete tparr;*/ tparr = new gs::gs_param_array(tpaParent);
  delete tparr; tparr = new gs::gs_param_array();
  delete tparr; tparr = new gs::gs_param_array(*tpaParent);
  delete tparr; tparr = new gs::gs_param_array(stname);
  delete tparr; tparr = new gs::gs_param_array(stname, tpaParent);
  delete tparr; tparr = new gs::gs_param_array(stname, *tpaParent);
  delete tparr; tparr = new gs::gs_param_array(ctname);
  delete tparr; tparr = new gs::gs_param_array(ctname, tpaParent);
  delete tparr; tparr = new gs::gs_param_array(ctname, *tpaParent);
  delete tparr; tparr = NULL;

  
  
  // /////////////////////////////////////////////////////////////////////////
  // ///////    Test of local parameter    ///////////////////////////////////
  // /////////////////////////////////////////////////////////////////////////

  // register callback which will be called at construction of local parameter
  IP3test("register new param event and callback");
  const sc_event* new_param_event;
  new_param_event = &gs::cnf::GCnf_Api::getApiInstance(this)->getNewParamEvent();

  gs::cnf::GCnf_Api::getApiInstance(this)->REGISTER_NEW_PARAM_CALLBACK(TestIP3, new_param_callback);
  gs::gs_param<string> newpar("Test.newParam", "TestParam should result in new param callback");
  // or deprecated: gs::cnf::GCnf_Api::getApiInstance(this)->addParam("Test.newParam", "TestParam should result in new param callback");
  

  gs::gs_param<int> local_int("local_int", "50");
  IP3test("local_int = "<<local_int);
  local_int += 100;
  IP3test("local_int += 100: "<<local_int);


  // /////////////////////////////////////////////////////////////////////////
  // ///////    Test of initial values    ////////////////////////////////////
  // /////////////////////////////////////////////////////////////////////////

  IP3test(" int_param =" << int_param);
  IP3test(" str_param =" << str_param);
  IP3test(" str_param2=" << str_param2);
  IP3test("uint_param =" << uint_param);
  IP3test("uint_param2=" << uint_param2);
  IP3test("bool_param =" << bool_param);
  IP3test(" dbl_param =" << dbl_param);

  SPACE;

  // /////////////////////////////////////////////////////////////////////////
  // ///////    int:   Full Test of all operators    /////////////////////////
  // /////////////////////////////////////////////////////////////////////////

  unsigned int a = 1000;
  uint_param = a;
  IP3test("'uint_param = (unsigned int) 1000' uint_param="<<uint_param);

  int_param = 500 - uint_param;
  IP3test("'int_param = 500 - uint_param' : int_param="<<int_param);

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

  SPACE;

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

  IP3test("'int_param != 6' ");
  if (int_param != 6)  
    cout<<"               true" <<endl;
  else
    cout<<"               false"<<endl;

  IP3test("'int_param != 4' ");
  if (int_param != 4)  
    cout<<"               true" <<endl;
  else
    cout<<"               false"<<endl;

  // /////////////////////////////////////////////////////////////////////////
  // ///////    unsigned int:   Full Test of all operators    ////////////////
  // /////////////////////////////////////////////////////////////////////////

  SPACE; SPACE;

  unsigned int ai = 1000;
  uint_param = ai;
  IP3test("'uint_param = (unsigned int) 1000' uint_param="<<uint_param);

  uint_param = 3200 - uint_param;
  IP3test("'uint_param = 3200 - uint_param' : uint_param="<<uint_param);

  uint_param += 1500;
  IP3test("'uint_param += 1500' : uint_param="<<uint_param);

  IP3test("uint_param++="<<uint_param++);
  IP3test("uint_param="<<uint_param);

  IP3test("++uint_param="<<++uint_param);
  IP3test("uint_param="<<uint_param);

  IP3test("uint_param--="<<uint_param--);
  IP3test("uint_param="<<uint_param);

  IP3test("--uint_param="<<--uint_param);
  IP3test("uint_param="<<uint_param);

  SPACE;

  uint_param += 1500;
  IP3test("'uint_param += 1500' : uint_param="<<uint_param);

  uint_param -= 200;
  IP3test("'uint_param -= 200' : uint_param="<<uint_param);

  uint_param /= 4;
  IP3test("'uint_param /= 4' : uint_param="<<uint_param);

  uint_param *= 3;
  IP3test("'uint_param *= 3' : uint_param="<<uint_param);

  uint_param %= 6;
  IP3test("'uint_param %= 6' : uint_param="<<uint_param);

  uint_param ^= 5;
  IP3test("'uint_param ^= 5' : uint_param="<<uint_param);

  uint_param &= 60;
  IP3test("'uint_param &= 60' : uint_param="<<uint_param);

  uint_param |= 3;
  IP3test("'uint_param |= 3' : uint_param="<<uint_param);

  unsigned int ui = uint_param; ui <<= 2;
  uint_param <<= 2;
  IP3test("'uint_param <<= 2' : uint_param ("<<ui<<") ="<<uint_param);

  ui >>= 2;
  uint_param >>= 2;
  IP3test("'uint_param >>= 2' : uint_param ("<<ui<<") ="<<uint_param);

  SPACE;

  IP3test("'uint_param != 7' ");
  if (uint_param != 7)  
    cout<<"               true" <<endl;
  else
    cout<<"               false"<<endl;

  IP3test("'uint_param != 4' ");
  if (uint_param != 4)  
    cout<<"               true" <<endl;
  else
    cout<<"               false"<<endl;

  // /////////////////////////////////////////////////////////////////////////
  // ///////    bool:   Full Test of all operators    ////////////////////////
  // /////////////////////////////////////////////////////////////////////////

  SPACE; SPACE;

  IP3test("bool_param="<<bool_param);
  bool ab = true;
  bool_param = ab;
  IP3test("'bool_param = true' bool_param="<<bool_param);

  bool_param = 500 - bool_param;
  IP3test("'bool_param = 500 - ubool_param' : bool_param="<<bool_param);
 
  IP3test("bool_param++="<<bool_param++);
  IP3test("bool_param="<<bool_param);
  
  IP3test("++bool_param="<<++bool_param);
  IP3test("bool_param="<<bool_param);

  /* unsafe for bool: (according to MSVS)
   
  bool_param += 1500;
  IP3test("'bool_param += 1500' : bool_param="<<bool_param);

  // no decrement operator

  SPACE;

  bool_param += 1500;
  IP3test("'bool_param += 1500' : bool_param="<<bool_param);

  bool_param -= 200;
  IP3test("'bool_param -= 200' : bool_param="<<bool_param);

  bool_param /= 4;
  IP3test("'bool_param /= 4' : bool_param="<<bool_param);

  bool_param *= 3;
  IP3test("'bool_param *= 3' : bool_param="<<bool_param);

  bool_param %= 6;
  IP3test("'bool_param %= 6' : bool_param="<<bool_param);

  bool_param ^= 5;
  IP3test("'bool_param ^= 5' : bool_param="<<bool_param);

  bool_param &= 60;
  IP3test("'bool_param &= 60' : bool_param="<<bool_param);

  bool_param |= 3;
  IP3test("'bool_param |= 3' : bool_param="<<bool_param);

  bool boo = bool_param; boo <<= 2;
  bool_param <<= 2;
  IP3test("'bool_param <<= 2' : bool_param ("<<boo<<") ="<<bool_param);

  u >>= 2;
  bool_param >>= 2;
  IP3test("'bool_param >>= 2' : bool_param ("<<boo<<") ="<<bool_param);
*/
  SPACE;

  bool_param = true;
  IP3test("bool_param="<<bool_param);

  IP3test("'bool_param != true' ");
  if (bool_param != true)  
    cout<<"               true" <<endl;
  else
    cout<<"               false"<<endl;

  IP3test("'bool_param != false' ");
  if (bool_param != false)  
    cout<<"               true" <<endl;
  else
    cout<<"               false"<<endl;

  // /////////////////////////////////////////////////////////////////////////
  // ///////    double:   Full Test of all operators    //////////////////////
  // /////////////////////////////////////////////////////////////////////////

  SPACE; SPACE;

  double doua = 10.1;
  dbl_param = doua;
  IP3test("'udbl_param = 10.1' dbl_param="<<dbl_param);

  dbl_param = 500.2 - dbl_param;
  IP3test("'dbl_param = 500.2 - dbl_param' : dbl_param="<<dbl_param);

  dbl_param += 1.05;
  IP3test("'dbl_param += 1.05' : dbl_param="<<dbl_param);

  IP3test("dbl_param++="<<dbl_param++);
  IP3test("dbl_param="<<dbl_param);

  IP3test("++dbl_param="<<++dbl_param);
  IP3test("dbl_param="<<dbl_param);

  IP3test("dbl_param--="<<dbl_param--);
  IP3test("dbl_param="<<dbl_param);

  IP3test("--dbl_param="<<--dbl_param);
  IP3test("dbl_param="<<dbl_param);

  SPACE;

  dbl_param += 9.2;
  IP3test("'dbl_param += 9.2' : dbl_param="<<dbl_param);

  dbl_param -= 100.8;
  IP3test("'dbl_param -= 100.8' : dbl_param="<<dbl_param);

  dbl_param /= 4.1;
  IP3test("'dbl_param /= 4.1' : dbl_param="<<dbl_param);

  dbl_param *= 3.2;
  IP3test("'dbl_param *= 3.2' : dbl_param="<<dbl_param);

  // no binary operators

  SPACE;

  IP3test("'dbl_param != 311.844' ");
  if (dbl_param != 311.844)  
    cout<<"               true" <<endl;
  else
    cout<<"               false"<<endl;

  IP3test("'dbl_param != 4.5' ");
  if (dbl_param != 4.5)  
    cout<<"               true" <<endl;
  else
    cout<<"               false"<<endl;

  // /////////////////////////////////////////////////////////////////////////
  // ///////    float:   Full Test of all operators    //////////////////////
  // /////////////////////////////////////////////////////////////////////////

  SPACE; SPACE;

  float fla = 10.1;
  float_param = fla;
  IP3test("'float_param = 10.1' float_param="<<float_param);

  float_param = 500.2 - float_param;
  IP3test("'float_param = 500.2 - float_param' : float_param="<<float_param);

  float_param += 1.05;
  IP3test("'float_param += 1.05' : float_param="<<float_param);

  IP3test("float_param++="<<float_param++);
  IP3test("float_param="<<float_param);

  IP3test("++float_param="<<++float_param);
  IP3test("float_param="<<float_param);

  IP3test("float_param--="<<float_param--);
  IP3test("float_param="<<float_param);

  IP3test("--float_param="<<--float_param);
  IP3test("float_param="<<float_param);

  SPACE;

  float_param += 9.2;
  IP3test("'float_param += 9.2' : float_param="<<float_param);

  float_param -= 100.8;
  IP3test("'float_param -= 100.8' : float_param="<<float_param);

  float_param /= 4.1;
  IP3test("'float_param /= 4.1' : float_param="<<float_param);

  float_param *= 3.2;
  IP3test("'float_param *= 3.2' : float_param="<<float_param);

  // no binary operators

  SPACE;

  // to compare with original behaviour
  float float_b = 44.4;
  IP3test("float_b = "<< float_b);
  IP3test("'float_b != 44.4' ");
  if (float_b != 44.4)  
    cout<<"               true" <<endl;
  else
    cout<<"               false"<<endl;
  IP3test("'float_b != (float) 44.4' ");
  if (float_b != (float) 44.4)  
    cout<<"               true" <<endl;
  else
    cout<<"               false"<<endl;

  SPACE;

  IP3test("'float_param != 311.844' ");
  if (float_param != 311.844)  
    cout<<"               true" <<endl;
  else
    cout<<"               false"<<endl;
  IP3test("'float_param != (float) 311.844' ");
  if (float_param != (float)311.844)  
    cout<<"               true" <<endl;
  else
    cout<<"               false"<<endl;

  IP3test("'float_param != 4.5' ");
  if (float_param != 4.5)  
    cout<<"               true" <<endl;
  else
    cout<<"               false"<<endl;


  // /////////////////////////////////////////////////////////////////////////
  // ///////    double and float    //////////////////////////////////////////
  // /////////////////////////////////////////////////////////////////////////

  double dbla = 19.3;
  float_param = dbla;
  IP3test("'float_param = 19.3' : float_param="<<float_param);

  float_param = dbl_param + 4.1;
  IP3test("'float_param = dbl_param ("<<dbl_param <<") + 4.1' : float_param="<<float_param);


  // /////////////////////////////////////////////////////////////////////////
  // ///////    std::string:   Full Test of all operators    /////////////////
  // /////////////////////////////////////////////////////////////////////////

  SPACE; SPACE;

  str_param2 = str_param;
  IP3test("'str_param2 = str_param' str_param2="<<str_param2);

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


  // /////////////////////////////////////////////////////////////////////////
  // ///////    Test of get operator =                   /////////////////////
  // /////////////////////////////////////////////////////////////////////////

  SPACE; SPACE;

  int i = int_param;
  std::string str = str_param;
  std::string str2(str_param2);
  unsigned int unsi = uint_param;
  bool bo = bool_param;
  double db = dbl_param;
  float fl = float_param;

  cout << "i    = "<<i << ",  "   <<endl
       << "str  = '"<<str << "',  " <<endl
       << "str2 = '"<<str2 << "',  "<<endl
       << "unsi = "<<unsi << ",  "<<endl
       << "bo   = "<<bo << ",  "  <<endl
       << "db   = "<<db           <<endl
       << "fl   = "<<fl           <<endl<<flush;


  // /////////////////////////////////////////////////////////////////////////
  // ///////    Observer events and callback functions    ////////////////////
  // /////////////////////////////////////////////////////////////////////////

  const sc_event* change_event;
  change_event = &int_param.getUpdateEvent();

  GC_REGISTER_PARAM_CALLBACK(&int_param, TestIP3, config_callback);

  int_param = 44;
  
  // event and callback tests see TestTool.h .cpp


  // /////////////////////////////////////////////////////////////////////////
  // ///////    SystemC    ///////////////////////////////////////////////////
  // /////////////////////////////////////////////////////////////////////////

  {
    SPACE; SPACE;
    
    // sc_int<8>
    IP3test("sc_int_param = " << sc_int_param);
    sc_int<8> i = 100;
    sc_int_param = i;
    IP3test("sc_int_param = (sc_int<8>) 100 = " << sc_int_param);
    sc_int_param = -66;
    i = sc_int_param;
    IP3test("i = (sc_int_param = -66) = " << i);

    SPACE;

    // sc_uint<16>
    IP3test("sc_uint_param = " << sc_uint_param);
    sc_uint<16> ui = 1001;
    sc_uint_param = ui;
    IP3test("sc_uint_param = 1001 = " << sc_uint_param);
    sc_uint_param = 66;
    ui = sc_uint_param;
    IP3test("ui = (sc_uint_param = 66) = " << ui);

    SPACE;

    // sc_bigint<70>
    IP3test("sc_bigint_param = " << sc_bigint_param);
    sc_bigint<70> ib = 12457878787812LL;
    sc_bigint_param = ib;
    IP3test("sc_bigint_param = (sc_bigint<70>) 12457878787812LL = " << sc_bigint_param);
    sc_bigint_param = -660000000000LL;
    ib = sc_bigint_param;
    IP3test("ib = (sc_bigint_param = -660000000000LL) = " << ib);

    SPACE;

    // sc_biguint<70>
    IP3test("sc_biguint_param = " << sc_biguint_param);
    sc_biguint<70> uib = 324578787878ULL;
    sc_biguint_param = uib;
    IP3test("sc_biguint_param = (sc_biguint<70>) 324578787878ULL = " << sc_biguint_param);
    sc_biguint_param = 440000000000000LL;
    uib = sc_biguint_param;
    IP3test("uib = (sc_biguint_param = 440000000000000LL) = " << uib);

    SPACE;

    // sc_bit
    sc_bit b3(1);
    IP3test("sc_bit_param2 = " << sc_bit_param2);
    IP3test("b3 = " << b3);

    //IP3test("sc_bit_param = " << sc_bit_param); // TODO: << >> operators
    sc_bit b(SC_LOGIC_1);
    sc_bit b1 = sc_bit_param;
    IP3test("b = " << b);
    IP3test("b1 = " << b1);
    IP3test("sc_bit_param = " << sc_bit_param);

    sc_bit_param = SC_LOGIC_0;
    IP3test("sc_bit_param = SC_LOGIC_0 = " << sc_bit_param);
    if (sc_bit_param == SC_LOGIC_0)  
      cout<<"               SC_LOGIC_0" <<endl;
    if (sc_bit_param == SC_LOGIC_1)
      cout<<"               SC_LOGIC_1" <<endl;

    sc_bit_param = SC_LOGIC_1;
    IP3test("sc_bit_param = SC_LOGIC_1 = " << sc_bit_param);

    // sc_bv<3>
    //IP3test("sc_bv_param = " << sc_bv_param); // TODO: << >> operators
    sc_bv<3> bv("100");
    sc_bv<3> bv1 = sc_bv_param;
    IP3test("bv = " << bv);
    IP3test("bv1 = " << bv1);
    // TODO sc_bv_param = "111";
    IP3test("sc_bv_param = " << bv);

    gs::gs_param<sc_fixed<32, 16> > sc_fixed_param("sc_fixed_param", "33.4");
    gs::gs_param<sc_ufixed<32, 16> > sc_ufixed_param(string("sc_ufixed_param"), "33.4");
    gs::gs_param<sc_fix> sc_fix_param("sc_fix_param", string("33.4"));
    gs::gs_param<sc_ufix> sc_ufix_param(string("sc_ufix_param"), string("33.4"));
    
    SPACE;
    
    
    // sc_time

    IP3test("time_param1 = 52 ms  = " << time_param1 << ". Value changed from configtest.cfg" << endl);
    IP3test("time_param1 = 500 ns = "  << time_param1 << "?. Value set in constructor" << endl);

    time_param2 = sc_time(2, SC_SEC);
    IP3test("time_param2 = 2 s = " << time_param2);

    std::string setStr = "53.98 us";
    IP3test("deserialize -> serialize -> deserialize test: '" << setStr <<"'");
    time_param2.setString(setStr);
    time_param3.setString(time_param2.getString());
    IP3test("result: " << time_param3.getString());
    
    time_param1.setString(time_param3.getString());
    IP3test("result: " << time_param1.getString());
    
    //huge value (uint64):
    sc_time huge_time = sc_time(0xF7592E67BD3A366AULL, false);
    time_param1 = huge_time;
    IP3test("sc_time large: " << huge_time << endl);
    IP3test("time_param1 huge number: " << time_param1<<endl);
    time_param2.setString(time_param1.getString());
    IP3test("time_param2 huge number: " << time_param2<<endl);
  }

  // /////////////////////////////////////////////////////////////////////////
  // ///////    unsigned long long:  deserialization test   //////////////////
  // /////////////////////////////////////////////////////////////////////////

  SPACE; SPACE;

  unsigned long long ull;
  gs::gs_param<unsigned long long> ull_par("ull_par", 10ull);
  
  ull = 0xFFFFFFFFFFFFFFFFull;
  ull_par.setString("0xFFFFFFFFFFFFFFFF");
  IP3test("unsigned long long param (orig="<<ull<<") = "<<ull_par <<  " = " << ull_par.getString());
  
  ull = 0xFFFFFFFFFFFF0F0Full;
  ull_par.setString("0xFFFFFFFFFFFF0F0F");
  IP3test("unsigned long long param (orig="<<ull<<") = "<<ull_par <<  " = " << ull_par.getString());
  
  ull = 0xF0FFFFFFFFFF0F0Full;
  ull_par = 0xF0FFFFFFFFFF0F0Full;
  IP3test("unsigned long long param (orig="<<ull<<") = "<<ull_par <<  " = " << ull_par.getString());
  
  // /////////////////////////////////////////////////////////////////////////
  // ///////    long long:  deserialization test   ///////////////////////////
  // /////////////////////////////////////////////////////////////////////////
  
  SPACE; SPACE;

  long long ll;
  gs::gs_param<long long> ll_par("ll_par", 100ll);
  
  ll = 0xFFFFFFFFFFFFFFFFll;
  ll_par.setString("0xFFFFFFFFFFFFFFFF");
  IP3test("long long param (orig="<<ll<<") = "<<ll_par <<  " = " << ll_par.getString());
  
  ll = 0xFFFFFFFFFFFF0F0Fll;
  ll_par.setString("0xFFFFFFFFFFFF0F0F");
  IP3test("long long param (orig="<<ll<<") = "<<ll_par <<  " = " << ll_par.getString());
  
  ll = 0xF0FFFFFFFFFF0F0Fll;
  ll_par = 0xF0FFFFFFFFFF0F0Fll;
  IP3test("long long param (orig="<<ll<<") = "<<ll_par <<  " = " << ll_par.getString());
  
  SPACE; SPACE;
  
  // /////////////////////////////////////////////////////////////////////////
  // ///////    unsigned char tests    ///////////////////////////////////////
  // /////////////////////////////////////////////////////////////////////////

  unsigned char uch('G');
  uchar_param = uch;

  char ch('G');
  uchar_param = ch;
  IP3test("'uchar_param = 'G'' : uchar_param = "<<uchar_param);
  uchar_param = 78;
  IP3test("'uchar_param = 78' (='N') : uchar_param = "<<uchar_param);
  
  // /////////////////////////////////////////////////////////////////////////
  // ///////    char tests    ////////////////////////////////////////////////
  // /////////////////////////////////////////////////////////////////////////

  char_param1.setString("88");
  IP3test("'char_param1.setString(\"88\") : char_param1 = "<<char_param1 << "  -  warning ok!");

  char_param1 = 'R';
  IP3test("'char_param1 = 'R'' : char_param1 = "<<char_param1);

  char_param2 = 77;
  IP3test("'char_param2 = 77' (='M'): char_param2 = "<<char_param2);
  
  SPACE; SPACE;

  // /////////////////////////////////////////////////////////////////////////
  // ///////    signed char tests    /////////////////////////////////////////
  // /////////////////////////////////////////////////////////////////////////
  
  signed char sch('G');
  schar_param = sch;
  
  schar_param = ch;
  IP3test("'schar_param = 'G'' : schar_param = "<<schar_param);
  schar_param = 78;
  IP3test("'schar_param = 78' (='N') : schar_param = "<<schar_param);
    
  SPACE; SPACE;

  
  // /////////////////////////////////////////////////////////////////////////
  // ///////    == operator test    //////////////////////////////////////////
  // /////////////////////////////////////////////////////////////////////////
  
  uint_param = 10;
  uint_param2 = 20;
  if (uint_param == uint_param2) {
    IP3test("'uint_param == uint_param2' (10==20) = true (ERROR!)");
    assert(false);
  }
  else {
    IP3test("'uint_param == uint_param2' (10==20) = false");
  }

  uint_param2 = 10;
  if (uint_param == uint_param2) {
    IP3test("'uint_param == uint_param2' (10==10) = true");
  }
  else {
    IP3test("'uint_param == uint_param2' (10==10) = false (ERROR!)");
    assert(false);
  }

  if ((unsigned int)10 == uint_param) {
    IP3test("'10 == uint_param' (10==10) = true");
  }
  else {
    IP3test("'10 == uint_param' (10==10) = false");
    assert(false);
  }

  if (uint_param == (unsigned int)10) {
    IP3test("'uint_param == 10' (10==10) = true");
  }
  else {
    IP3test("'uint_param == uint_param2' (10==10) = false (ERROR!)");
    assert(false);
  }
  
  bool_param = false;
  gs::gs_param<bool> tmp_bool_param("tmp_bool_param", true);
  if (bool_param == tmp_bool_param) {
    IP3test("'bool_param == tmp_bool_param' (false==true) = true (ERROR!)");
    assert(false);
  }
  else {
    IP3test("'bool_param == tmp_bool_param' (false==true) = false");
  }

  bool_param = true;
  if (bool_param == tmp_bool_param) {
    IP3test("'bool_param == tmp_bool_param' (true==true) = true");
  }
  else {
    IP3test("'bool_param == tmp_bool_param' (true==true) = false (ERROR!)");
    assert(false);
  }
  
  SPACE; SPACE;

  // /////////////////////////////////////////////////////////////////////////
  // ///////    API Tests   //////////////////////////////////////////////////
  // /////////////////////////////////////////////////////////////////////////

  gs::cnf::cnf_api *mApi = gs::cnf::GCnf_Api::getApiInstance(this);
  
  // Test val getValue<T>(name)
  int_param = 1234;
  std::string pn = name(); pn += ".int_param";
  int val = mApi->getValue<int>(pn);
  IP3test("Api call getValue('"<<pn<<"') = " << val << "  (should be "<< mApi->getPar(pn)->getString()<<")");

  // Test getValue<T>(name, val)
  uint_param = 5678;
  unsigned int uval;
  pn = name(); pn += ".uint_param";
  bool success = mApi->getValue<unsigned int>(pn, uval);
  assert(success == true);
  IP3test("Api call getValue('"<<pn<<"') = " << uval << "  (should be "<< mApi->getPar(pn)->getString()<<")");

  success = mApi->getValue<unsigned int>("not.existing", uval);
  if (success) {
    IP3test("Api call getValue('not.existing') returned false as expected");
  }
  else assert(false);

  // Test gs_param default implementation with a not specialized data type
  gs::gs_param<short> short_param("short_param");
  short_param = 12;
  short chval;
  success = mApi->getValue<short>(short_param.getName(), chval);
  assert(success == true);
  IP3test("Api call getValue('"<<short_param.getName()<<"') = " << chval << "  (should be "<< short_param <<")");

  SPACE;
  
  // Test get_gs_param function
  pn = name(); pn += ".int_param";
  gs::gs_param<int> *dyIntPar = mApi->get_gs_param<int>(pn);
  if (dyIntPar) IP3test("got parameter '"<< pn << "' object with call get_gs_param<int>(name), val = "<<*dyIntPar);
  else IP3test("FAILURE using get_gs_param<int>!");
  SPACE;
  
  gs::gs_param<unsigned int> *dyUIntPar = mApi->get_gs_param<unsigned int>(pn);
  if (dyUIntPar) IP3test("FAILURE: got parameter '"<< pn << "' with wrong type!");
  else IP3test("passed test get_gs_param<unsigned int> with wrong type!");
  SPACE;
  
  pn = "not.existing";
  dyIntPar = mApi->get_gs_param<int>(pn);
  if (dyIntPar) IP3test("FAILURE getting not existing param with get_gs_param");
  else IP3test("passed test get_gs_param<int> with not existing param!");

  
  SPACE; SPACE;
  
  
  // TODO: == operators for each type

}

void TestIP3::config_callback(gs::gs_param_base& par) {
  if (!par.is_destructing()) {
    printf("TestIP3::config_callback: Callback for changed parameter %s. New value: %s\n", par.getName().c_str(), par.getString().c_str());
  }
  else {
    printf("TestIP3::config_callback: Parameter %s is destructing.\n", par.getName().c_str());
  }
}

void TestIP3::new_param_callback(const std::string parname, const std::string value) {
  printf("TestIP3::new_param_callback: Callback for NEW PARAMETERS: Parameter %s, value: %s\n", parname.c_str(), value.c_str());
}
