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

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "greencontrol/gclog/filter_base.h"

namespace gs {
namespace log {


/// Simple output to the console.
/**
 * This output module writes the transactions to the console.
 */
class Console : public FilterBase
{
public:
  /// Contructor.
  Console() {}
  /// Destructor.
  ~Console() {}

  /// Write the transaction to the standard output.
  void processTransaction(ControlTransactionHandle& th)
  {
    std::cout << th->toString() << std::endl;
  }

  /// Return the number of filter ports needed by this filter.
  unsigned int numFilterPorts()
  {
    return 0;
  }
};

  
} // end namespace log
} // end namespace gs

#endif
