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

#ifndef __SERVICE_FILTER_H__
#define __SERVICE_FILTER_H__

#include "greencontrol/gclog/filter_base.h"

namespace gs {
namespace log {


/// This filter sorts the transactions based on their service.
/**
 * The number of filter ports is specified by the user. The transactions are passed to the filter ports that have the same
 * index number as the service number specified in the transaction.
 */
class ServiceFilter : public FilterBase
{
public:
  /// Contructor that gets the number of the filter ports that have to be created.
  ServiceFilter(unsigned int maxServices)
  {
    m_maxServices = maxServices;
  }

  /// Destructor.
  ~ServiceFilter() {}

  /// Pass the transaction to the filter port responsible for the corresponding service.
  void processTransaction(ControlTransactionHandle& th)
  {
    if((unsigned int)th->get_mService() < m_maxServices)
      processFilterPort(th->get_mService(), th);
  }

  /// Return the number of filter ports needed by this filter.
  unsigned int numFilterPorts()
  {
    return m_maxServices;
  }

protected:
  /// Number of filter ports to create.
  unsigned int m_maxServices;
};

  
} // end namespace log
} // end namespace gs

#endif
