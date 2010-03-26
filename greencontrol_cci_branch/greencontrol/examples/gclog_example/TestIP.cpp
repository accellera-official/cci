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
//   This program is free software.
// 
//   If you have no applicable agreement with GreenSocs Ltd, this software
//   is licensed to you, and you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   If you have a applicable agreement with GreenSocs Ltd, the terms of that
//   agreement prevail.
// 
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
//   02110-1301  USA 
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
