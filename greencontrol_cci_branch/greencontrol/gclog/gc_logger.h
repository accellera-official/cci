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

// If GCLOG_RETURN is used for the project it has to be set before GC_Logger.h or gc_transaction.h are included the first time. If it isn't defined while
// the header is included the first time, the flag GCLOG_RETURN_NOT_DEFINED is set. So if GCLOG_RETURN is set after the first inclusion of the header but not
// before, a warning is issued, because the GCLOG_RETURN flag has no effect in this case.
#ifdef GCLOG_RETURN
  #ifdef GCLOG_RETURN_NOT_DEFINED
    #ifdef _MSC_VER
      #pragma message("Warning: GCLOG_RETURN is not defined before including GC_Logger.h or gc_transaction.h. Transactions returning from their receivers won't be logged!")
    #else
      #warning GCLOG_RETURN is not defined before including GC_Logger.h or gc_transaction.h. Transactions returning from their receivers won't be logged!
    #endif
  #endif
#endif

#ifndef __GC_LOGGER_H__
#define __GC_LOGGER_H__

#ifndef GCLOG_RETURN
  #define GCLOG_RETURN_NOT_DEFINED
  #ifdef _MSC_VER
    #pragma message("Warning: GCLOG_RETURN is not defined. Transactions returning from their receivers won't be logged!")
  #else
    #warning GCLOG_RETURN is not defined. Transactions returning from their receivers won't be logged!
  #endif
#endif

#include "greencontrol/core/gc_port_if.h"
#include "gc_logger_if.h"
#include "filter_base.h"

namespace gs {
namespace log {

using namespace gs::ctr;


/// Logger to filter and log the transactions routed by the core.
/**
 * This is the GreenControl Logger. It can be used with various filters and output modules, which are
 * capable of writing filtered information from the transactions to specified outputs.
 */
class GC_Logger
  : public GC_Logger_if
  , public gc_port_if
{
public:

  /// Constructor, which initializes the gc_port.
  GC_Logger()
    : m_gc_port(LOG_SERVICE, "GC_Logger", true, this)
  {
    m_pFilter = NULL;
    m_pReturnFilter = NULL;
  }

  /// Destructor
  virtual ~GC_Logger() {}

  /// Set the filters used for normal and returning transactions.
  /**
   * The filter which is set here is used for all transactions. This means normal transactions and transactions on their return path.
   * However a special filter for returning transactions can be set by calling setReturnFilter(). It is allowed to pass a NULL pointer
   * to turn off the logging temporarily.
   */
  void setFilter(FilterBase* pFilter)
  {
    // pFilter == NULL is allowed to turn off logging temporarily
    m_pFilter = pFilter;
    m_pReturnFilter = pFilter;
    // notify the filter
    if(pFilter)
      pFilter->wasAdded();
  }

  /// Set the filter used for returning transactions.
  /**
   * The filter which is set here is used for all transactions, that have been changed by their receiver and are on their way back now.
   * It is allowed to pass a NULL pointer to turn this feature temporarily off. However it is recommended to delete the #define GCLOG_RETURN
   * flag if this feature isn't needed at all, instead of deactivating it by passing a NULL pointer.
   */
  void setReturnFilter(FilterBase* pFilter)
  {
    m_pReturnFilter = pFilter;
    if(pFilter)
      pFilter->wasAdded();
  }

  /// Log the transaction.
  /**
   * This method logs all transactions before they are passed to their receivers.
   */
  void log(ControlTransactionHandle& th)
  {
    // Should be called before processing the transaction, so the filters know that this transaction isn't on the return path.
#ifdef GCLOG_RETURN
    th->resetChangeFlag();
#endif

    if(m_pFilter)
      m_pFilter->processTransaction(th);

#ifdef GCLOG_RETURN
    // Set the LogConfig elements to false, so only those elements that change during processing will be logged in logReturn().
    th->get_mLogConfig()->reset(false);
#endif
  }

#ifdef GCLOG_RETURN
  /// Log a returning transaction.
  /**
   * This method logs transactions that have been changed by their receiver and are on their way back to the sender. A special filter pointer
   * is used, which is set to the normal filter by default. However this filter can be set to a special filter by calling setReturnFilter().
   */
  void logReturn(ControlTransactionHandle& th)
  {
    // log the transaction if it was changed by its receiver
    if(th->hasChanged())
    {
      if(m_pReturnFilter)
        m_pReturnFilter->processTransaction(th);
    }
  }
#else
  void logReturn(ControlTransactionHandle& th)
  {
  }
#endif

  /// Unused transport method provided for gc_port_if.
  void transport(ControlTransactionHandle& th)
  {
  }

protected:
  /// gc_port to the Core.
  gc_port m_gc_port;

  /// Pointer to a filter used for transaction processing.
  FilterBase* m_pFilter;

  /// Pointer to a filter used for processing returning transactions.
  FilterBase* m_pReturnFilter;
};


} // end namespace log
} // end namespace gs

#endif
