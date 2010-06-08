// LICENSETEXT
//
//   Copyright (C) 2007 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
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

#ifndef _time_measure_h_
#define _time_measure_h_

#include "sys/time.h"
#include "systemc"

//------------------------------------------------------------------------------------------------------
// Time measurement helper functions
//------------------------------------------------------------------------------------------------------

class timeMeasure {
public:
  // time measure functions 
  timeval tv_start, tv_stop;
  sc_core::sc_time sct_start, sct_stop;
  unsigned overall;
  
  timeMeasure() 
    : overall(0)
  {}
  
  inline void startMeasure() {
    gettimeofday(&tv_start, NULL);
    sct_start = sc_core::sc_time_stamp();
  }
  
  inline void stopMeasure() {
    gettimeofday(&tv_stop, NULL);

    unsigned long m = (tv_stop.tv_sec-tv_start.tv_sec)*1000000 +
      (((tv_stop.tv_usec-tv_start.tv_usec)<0) ? 
       -1*(tv_start.tv_usec-tv_stop.tv_usec) :
       (tv_stop.tv_usec-tv_start.tv_usec));
    // print us
    std::cout << std::dec << m;

    overall += m;
    std::cout << " ("<<overall<<")";
    
    // print "cycles" (we assume one cycle==10ns)
    sct_stop = sc_core::sc_time_stamp();
    std::cout << ", " << (sct_stop.to_double()-sct_start.to_double())/10000.0; // ps/10ns

    
  }
};

#endif
