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

#define GCLOG_RETURN

#include "greencontrol/config.h"
#include "greencontrol/log.h"

#include "TestIP.h"
#include "TestSetup.h"

int sc_main(int argc, char *argv[])
{
  unsigned int testNum = 0;

  gs::ctr::GC_Core  core;
  gs::cnf::ConfigDatabase  cnfdatabase("ConfigDatabase");
  gs::cnf::ConfigPlugin  configPlugin(&cnfdatabase);

  TestSetup ts;

  // create the specified test
  if(argc > 1)
  {
    testNum = atoi(argv[1]);
    std::cout << "Running TestCase " << testNum << std::endl;
    ts.setupTest((TestCase)testNum);
  }
  else {
    ts.choose_warning();
    ts.setupTest(MISC);
  }

  // TestIP
  TestIP ip("TestIP");

  sc_core::sc_start();

  if (testNum == 0)
    ts.choose_warning();

  return EXIT_SUCCESS; 
}
