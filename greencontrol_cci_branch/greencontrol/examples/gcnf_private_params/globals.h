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

#ifndef __globals_h__
#define __globals_h__

// Must be included BEFORE SystemC because SystemC 2.1
// has a buggy boost implementation included!!!!
#include <boost/tokenizer.hpp> // for parameter array!

#ifndef SC_INCLUDE_DYNAMIC_PROCESSES
  #define SC_INCLUDE_DYNAMIC_PROCESSES
#endif
#include <systemc>


//#define GC_VERBOSE
//#define GCNF_VERBOSE

#define SHOW_REGRESSION_TEST_RESULTS_ON

#ifdef SHOW_REGRESSION_TEST_RESULTS_ON
# define REGR_TEST(string) { std::cout <<"------------------------------------------------------------------------------------------"<<std::endl<<"    *** Regression test result: ***"<< std::endl<<"   @" << sc_core::sc_time_stamp() << " /" << sc_core::sc_delta_count() << " ("<<this->name() << "): " << string <<std::endl<<"------------------------------------------------------------------------------------------"<< std::endl; }
#else
# define REGR_TEST(string)
#endif


#endif
