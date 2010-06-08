//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Michael Ruetz <m.ruetz@tu-bs.de>,
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// doxygen comments

#include "TestIP.h"
//#include "Timer.h"

#define EMPH(mes) {cout << endl << endl << mes << endl << endl;}

void TestIP::main_action()
{
  EMPH("----------------------- Start of TestIP::main_action() -------------------------");

//  std::vector<std::string> list = gc_api->getParamList();
//  gs::ctr::show_vector(list);

  timeTest();

  EMPH("------------------------ End of TestIP::main_action() --------------------------");
}

void TestIP::timeTest(void)
{
//  CTimer timer;
  int numTests;
  double time;

  numTests = 10;
  time = 0.0;
//  timer.Init();

  for(int j=0;j<numTests;j++)
  {
//    timer.StartFrame();

    for(int i=0;i<1000;i++)
    {
      *((gs::gs_param<sc_int<32> > *)gc_api->getPar("TestIP.int_param")) = i;
//    cout << "int_param = " << toolApi.getPar("TestIP.int_param")->getString() << endl;
    }

//    timer.EndFrame();
//    time += timer.GetElapsedTime();
//    cout << timer.GetElapsedTime() << endl;
  }
//  cout << "Average time per run: " << time / numTests << endl;
}
