//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//   Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>
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

#ifndef __GC_PORT_IF_H__
#define __GC_PORT_IF_H__

#include "gc_globals.h"        // global settings
#include "gc_transaction.h"

namespace gs {
namespace ctr {

  
/// Interface to be implemented by APIs and Plugins which use the gc_port.
/**
 * This interface is to be implemented by each API/Plugin which uses the gc_port directly.
 */
class gc_port_if
{
public:
  virtual ~gc_port_if() {};

  virtual void transport(ControlTransactionHandle& tr) = 0;
};


} // end namespace ctr
} // end namespace gs

#endif
