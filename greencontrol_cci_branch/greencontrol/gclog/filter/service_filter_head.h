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

#ifndef __SERVICE_FILTER_HEAD_H__
#define __SERVICE_FILTER_HEAD_H__

#include "greencontrol/gclog/filter_base.h"
#include "greencontrol/gclog/filter/service_filter.h"
#include "greencontrol/gclog/output/CSV_File.h"

namespace gs {
namespace log {


/// This class demonstrates the principle of a so called head filter.
/**
 * Instead of creating the single logger components in the testbench, they are created and arranged in a special head
 * or root filter. This way standard filter configurations can be created in a very simple way.
 */
class ServiceFilterHead : public FilterBase
{
public:
  /// Constructor, where all components are created and connected.
  ServiceFilterHead()
  {
    // create a ServiceFilter and some outputs and connect them
    m_pServiceFilter = new ServiceFilter(3);
    m_pCSV_GCnf = new CSV_File("serviceFilterHead_GCnf.csv");
    m_pCSV_GAV = new CSV_File("serviceFilterHead_GAV.csv");

    m_pCSV_GCnf->setHumanReadable(true);
    m_pCSV_GAV->setHumanReadable(true);

    m_pServiceFilter->addFilter(1, m_pCSV_GCnf);
    m_pServiceFilter->addFilter(2, m_pCSV_GAV);

    this->addFilter(0, m_pServiceFilter);
  }

  /// Destructor, where all components get destroyed.
  ~ServiceFilterHead()
  {
    if(m_pServiceFilter)
      delete m_pServiceFilter;
    if(m_pCSV_GCnf)
      delete m_pCSV_GCnf;
    if(m_pCSV_GAV)
      delete m_pCSV_GAV;
  }

  /// Just hand the transaction to the first filter in the configuration.
  void processTransaction(ControlTransactionHandle& th)
  {
    processFilterPort(0, th);
  }

  /// Return the number of filter ports needed by this filter.
  unsigned int numFilterPorts()
  {
    return 1;
  }

protected:
  /// A ServiceFilter that gets created by this head filter.
  ServiceFilter* m_pServiceFilter;

  /// A CSV output that gets created by this head filter.
  CSV_File* m_pCSV_GCnf;

  /// A CSV output that gets created by this head filter.
  CSV_File* m_pCSV_GAV;
};

  
} // end namespace log
} // end namespace gs

#endif
