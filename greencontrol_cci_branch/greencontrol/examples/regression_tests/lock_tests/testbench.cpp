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

// SystemC library
#include <systemc>

#include "../demo_globals.h"


// GreenConfig
#include "greencontrol/config.h" 

#include "test_lock_ip.h"                

/// Testbench checking gs_param lock feature
int sc_main(int argc, char *argv[]) {
  
  Test_Lock_IP      ipL("Lock_IP");
  
  sc_core::sc_start();
  
  return EXIT_SUCCESS; 
  
}
