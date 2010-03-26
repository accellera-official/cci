//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007-2010 : GreenSocs Ltd
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
//   This program is free software.
// 
//   If you have no applicable agreement with GreenSocs Ltd, this software
//   is licensed to you, and you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   If you have a applicable agreement with GreenSocs Ltd, the terms of that
//   agreement prevail.
// 
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
//   02110-1301  USA 
// 
// ENDLICENSETEXT

// doxygen comments

#ifndef __INITIALIZE_IF_H__
#define __INITIALIZE_IF_H__

namespace gs {
namespace ctr {

  // forward declarations for typedef
  template <typename gc_port_T> class GC_Core_T;
  class gc_port;
  
  // GC_Core forward declaration
  typedef GC_Core_T<gc_port> GC_Core;
  
/// Interface to be implemented by each API. @see typedef gs::ctr::initialize_if
/**
 * Because the APIs should not be sc_core::sc_modules (they shouldn't be named) they cannot
 * use the callback functions end_of_elaboration etc.
 *
 * This provides some forwarded kernel callbacks.
 *
 * This interface automatically connects to the Core's Callback Dispatcher.
 */
template <typename GC_Core_T>
class initialize_if_T
{

public:

  initialize_if_T() {
    GC_Core_T::get_instance().get_CallbackDispatcher().add(this);
  }
  
  virtual ~initialize_if_T() {
    GC_Core_T::get_instance().get_CallbackDispatcher().remove(this);
  }

  /**
   * Inside this method the initial configuration has to be done.
   * This is called by the CallbackDispatcher during the start_of_simulation callback.
   */
  virtual void start_initial_configuration() = 0;

  /**
   * Informs the APIs about the done start_of_simulation call in the CallbackDispatcher:
   * leave the initialze-mode
   */ 
  virtual void end_initialize_mode() = 0;
  
  /// Can optionally be overloaded by the implementing class to get called on end_of_elaboration
  /**
   * Informs the APIs about the done end_of_elaboration call in the CallbackDispatcher.
   */ 
  virtual void gc_end_of_elaboration() { } 
    
};

/// initialize interface
typedef initialize_if_T<GC_Core> initialize_if;
    
} // end namespace ctr
} // end namespace gs

#endif
