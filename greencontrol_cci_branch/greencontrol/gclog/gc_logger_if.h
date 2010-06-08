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

#ifndef __GC_LOGGER_IF_H__
#define __GC_LOGGER_IF_H__

namespace gs {
namespace log {

using namespace gs::ctr;


/// Interface to be implemented by the GC_Logger.
/**
 * This interface is to implemented by the GC_Logger. The GC_Core uses this interface to communicate with
 * the GC_Logger instead of using the it directly, thereby avoiding a cyclic inclusion.
 */
class GC_Logger_if
{
public:
  virtual ~GC_Logger_if() {}

  virtual void log(ControlTransactionHandle& th) = 0;
  virtual void logReturn(ControlTransactionHandle& th) = 0;
};


} // end namespace log
} // end namespace gs

#endif
