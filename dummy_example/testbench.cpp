/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2009 by all Contributors.
  All Rights reserved.

  Developed by GreenSocs : http://www.greensocs.com/
   Christian Schroeder, schroeder@eis.cs.tu-bs.de
   Mark Burton, mark@greensocs.com

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/


#include <systemc>
#include "ParameterOwnerModule.h"
#include "ObserverModule.h"
#include "ParamManipulateModule.h"
#include <cci.h>

/// Testbench for the dummy example
int sc_main(int argc, char *argv[]) {
  
  ParamManipulateModule manipulator("Manipulator");
  ParameterOwnerModule  owner      ("Owner");
  ObserverModule        observer   ("Observer");

  std::cout << "------ sc_start() ----------------" << std::endl;
  sc_core::sc_start();
  std::cout << "------ sc_start() returned -------" << std::endl;
  
  return EXIT_SUCCESS; 
  
}
