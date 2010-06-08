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

#ifndef __FILTER_BASE_H__
#define __FILTER_BASE_H__

// Limit the number of max. allowed filter ports, to avoid memory problems.
#define MAX_NUM_FILTER_PORTS 100

namespace gs {
namespace log {


/// Abstract base class for filters used by the GC_Logger.
/**
 * This base class has to be implemented by each filter that is used by the GC_Logger.
 */
class FilterBase
{
public:

  /// Constructor.
  FilterBase()
  {
    m_iFilterPorts = 0;
    m_pFilterVec = NULL;
    m_bFirstCall = true;
    m_bFirstActivation = true;
  }

  /// Destructor.
  /**
   * Destroys the filter ports if they have been initialized.
   */
  virtual ~FilterBase()
  {
    if(m_pFilterVec != NULL)
      delete [] m_pFilterVec;
  }

  /// Gets called when the filter is added to the GC_Logger or another filter.
  void wasAdded()
  {
    // Call activate() once.
    if(m_bFirstActivation)
    {
      this->activate();
      m_bFirstActivation = false;
    }
  }

  /// Add a filter to the specified filter port.
  /**
   * By calling this method a filter can be added to a specified filter port. The first time this method is called, the
   * filter ports get created.
   *
   * @param filterPort  Index that specifies the port to which the filter shall be added.
   * @param pFilter     Pointer to the filter that shall be added.
   */
  void addFilter(unsigned int filterPort, FilterBase* pFilter)
  {
    std::stringstream ss;

    // create the filter ports
    if(m_bFirstCall)
    {
      createFilterPorts();
      m_bFirstCall = false;
    }

    if(pFilter == NULL)
    {
      ss << "Trying to add a NULL pointer. (FilterPort: " << filterPort << ")";
      SC_REPORT_WARNING("FilterBase::addFilter()", ss.str().c_str());
    }
    else if(filterPort >= m_iFilterPorts)
    {
      ss << "The specified filter port does not exist. (FilterPort: " << filterPort << ")";
      SC_REPORT_WARNING("FilterBase::addFilter()", ss.str().c_str());
    }
    else
    {
      // add the filter to the filter port (std::vector) and inform the filter that it was added
      m_pFilterVec[filterPort].push_back(pFilter);
      pFilter->wasAdded();
    }
  }

  /// This method is used to filter the transactions and pass them to the next modules.
  virtual void processTransaction(ControlTransactionHandle& th) = 0;

  /// Returns the number of filter ports that are needed by the child filter.
  virtual unsigned int numFilterPorts() = 0;

protected:

  /// This method gets called the first time the filter gets added to the GC_Logger or another filter.
  /**
   * It may be overridden by the child filter. So it can be informed that it has been added to the GC_Logger or another
   * filter. This way the child filter may take some last actions before it gets used.
   */
  virtual void activate() {}

  /// Create the filter ports.
  /**
   * This method creates the filter ports that are needed by the child filter. It uses the numFilterPorts() method of the
   * child filter to create the right number of ports. If the child filter doesn't need any filter ports, they won't get
   * created.
   */
  void createFilterPorts()
  {
    std::stringstream ss;

    // Doing this in the constructor would be cleaner, but this can't be done there, since the parent ctor is called before the child ctor. At this point the
    // child object hasn't been initialized and so there is no way to use its numFilterPorts()-method implementation.
    m_iFilterPorts = this->numFilterPorts();
    if(m_iFilterPorts > MAX_NUM_FILTER_PORTS)
    {
      m_iFilterPorts = MAX_NUM_FILTER_PORTS;
      ss << "Trying to create a filter with " << this->numFilterPorts() << " FilterPorts. Only up to " << MAX_NUM_FILTER_PORTS << " Ports per Filter are allowed.";
      ss << " Defaulting to " << m_iFilterPorts << " FilterPorts.";
      SC_REPORT_WARNING("FilterBase::createFilterPorts()", ss.str().c_str());
    }

    // only create the ports that are needed
    if(m_iFilterPorts > 0)
      m_pFilterVec = new std::vector<FilterBase*>[m_iFilterPorts];
    else
      m_pFilterVec = NULL;
  }

  /// Send the transaction to all filters bound to the specified filter port.
  /**
   * This method calls the processTransaction() method of each filter connected to the specified filter port. This way the
   * transactions get passed to the connected filters.
   *
   * @param port  Index of the filter port that should be processed.
   * @param th    ControlTransaction that gets passed to all filters that are connected to the specified port.
   */
  void processFilterPort(unsigned int port, ControlTransactionHandle& th)
  {
    // call every filter connected to the specified port
    if(port < m_iFilterPorts)
    {
      if(m_pFilterVec)
        for(unsigned int i=0;i<m_pFilterVec[port].size();i++)
          m_pFilterVec[port][i]->processTransaction(th);
    }
    else
    {
      std::stringstream ss;
      ss << "The specified filter port does not exist. (FilterPort: " << port << ")";
      SC_REPORT_WARNING("FilterBase::processFilterPort()", ss.str().c_str());
    }
  }

protected:
  /// Array of filter ports (std::vector).
  std::vector<FilterBase*>* m_pFilterVec;

private:
  /// Number of filter ports to create.
  unsigned int m_iFilterPorts;

  /// Used to create the filter ports only once.
  bool m_bFirstCall;

  /// Used to call activate() only once.
  bool m_bFirstActivation;
};

  
} // end namespace log
} // end namespace gs

#endif
