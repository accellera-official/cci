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
