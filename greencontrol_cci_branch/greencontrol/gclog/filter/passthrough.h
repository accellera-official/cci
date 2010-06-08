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

#ifndef __PASSTHROUGH_H__
#define __PASSTHROUGH_H__

#include "greencontrol/gclog/filter_base.h"

namespace gs {
namespace log {


/// Simple passthrough filter.
/**
 * This filter passes incoming transactions to the next filter and the output if available.
 */
class Passthrough : public FilterBase
{
public:
  /// Contructor.
  Passthrough() {}

  /// Destructor.
  ~Passthrough() {}

  /// Pass the transaction to the filters connected to the filter port.
  void processTransaction(ControlTransactionHandle& th)
  {
/*    if(m_pFilterVec)
      for(unsigned int i=0;i<m_pFilterVec[0].size();i++)
        m_pFilterVec[0][i]->processTransaction(th);
*/
    processFilterPort(0, th);
  }

  /// Return the number of filter ports needed by this filter.
  unsigned int numFilterPorts()
  {
    return 1;
  }
};

  
} // end namespace log
} // end namespace gs

#endif
