/*
 *  Created by Christian Schroeder on 13.01.10.
 *  Copyright 2010 TU Braunschweig, E.I.S.. All rights reserved.
 */

//#include "cci.h"
//#include "gs_cci.h"


using namespace std;

class A_if {
public:
  virtual int foo() = 0;
};

class B_if 
: virtual public A_if // dies soll nicht virtuell sein!
{
};

class A_impl
: virtual public A_if // dies soll von A_if ableiten, damit es in B_if nicht virtuell sein muss!
{
public:
  virtual int foo() { return 10; }
};

class B 
: public B_if
, public A_impl 
{
public:
  using A_impl::foo;
};





class C
: public B_if
{
public:
  virtual int foo() { return pMyB->foo(); }
  B* pMyB;
};



//int sc_main(int argc, char *argv[]) {
int main(int argc, char *argv[]) {

  B myb;
  myb.foo();
  
  
  C myc; myc.pMyB = &myb;
  A_if* myc_A_if_ptr = &myc;
  C &mycref = *static_cast<C* >(myc_A_if_ptr);

  return 0; 
}
