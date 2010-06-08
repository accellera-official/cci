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

#include "testip2.h"

using namespace std;

void TestIP2::main_action() {

  std::cout << "."<<std::endl;
  DEMO_TRACE(name(), "This is the main action in TestIP");

  return;

  /* To be tested:
      constructor: ok
      contructor with default
      destructor
      getXxxValue()
      setXxxValue(val)
      getType()
      getValue()
      Operator
        +=
        -=
        /=
        *=
        %=
        ^=
        |=
        &=
        <<=
        >>=
        -- pre
        -- post
        ++ pre
        ++ post
        ()

        == ??
        != ??
  */

  // Test of scml_property<int>
#define SHOW(comment) { std::cout << "scmlIntParam ("<< comment <<") = "<<scmlIntParam<<std::endl; }


  scmlIntParam = 1200;
  SHOW("");

  cout << "getType(): "<<scmlIntParam.getType()<<endl<<flush;

  cout << "getIntValue() = "<<scmlIntParam.getIntValue()<<endl<<flush;

  cout << "getValue() = "<<scmlIntParam.getValue()<<endl<<flush;

  cout << "setIntValue(1300)"<<endl<<flush;
  scmlIntParam.setIntValue(1300);
  SHOW("");

  cout << "scmlIntParam++="<<scmlIntParam++<<endl<<flush;
  SHOW("");

  cout << "++scmlIntParam="<<++scmlIntParam<<endl<<flush;
  SHOW("");
  
  scmlIntParam += 1000;
  SHOW("+=1000");

  scmlIntParam -= 500;
  SHOW("-=500");

  scmlIntParam /= 10;
  SHOW("/=10");

  scmlIntParam *= 20;
  SHOW("*=20");

  int a = 3600;
  a %= 33; cout << a<<" =? ";
  scmlIntParam %= 33;
  SHOW("%=33");

  a ^= 2; cout << a<<" =? ";
  scmlIntParam ^= 2;
  SHOW("^=2");

  a |= 10; cout << a<<" =? ";
  scmlIntParam |= 10;
  SHOW("|=10");

  a &= 55; cout << a<<" =? ";
  scmlIntParam &= 55; 
  SHOW("&=55");

  a <<= 3; cout << a<<" =? ";
  scmlIntParam <<= 3;
  SHOW("<<=3");

  a >>= 1; cout << a<<" =? ";
  scmlIntParam >>= 1;
  SHOW(">>=1");

  cout << "Test scmlIntParam "<< scmlIntParam <<" == a " << a << "   :   "<<flush;
  if (scmlIntParam == a) cout << "true"<<endl<<flush;
  else  cout << "false"<<endl<<flush;

  cout << "Test scmlIntParam "<< scmlIntParam <<" == a+1 " << a+1 << "   :   "<<flush;
  if (scmlIntParam == (a+1)) cout << "true"<<endl<<flush;
  else  cout << "false"<<endl<<flush;

  cout << "Test a "<< a <<" == scmlIntParam " << scmlIntParam << "   :   "<<flush;
  if (a == scmlIntParam) cout << "true"<<endl<<flush;
  else  cout << "false"<<endl<<flush;

  cout << "Test scmlIntParam "<< scmlIntParam <<" != a " << a << "   :   "<<flush;
  if (scmlIntParam != a) cout << "true"<<endl<<flush;
  else  cout << "false"<<endl<<flush;

  cout << "Test scmlIntParam "<< scmlIntParam <<" != a+1 " << a+1 << "   :   "<<flush;
  if (scmlIntParam != (a+1)) cout << "true"<<endl<<flush;
  else  cout << "false"<<endl<<flush;

  cout << "Test a "<< a <<" != scmlIntParam " << scmlIntParam << "   :   "<<flush;
  if (a != scmlIntParam) cout << "true"<<endl<<flush;
  else  cout << "false"<<endl<<flush;

  //scmlIntParam.getUIntValue();


}
