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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
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
 * Because the Service objects (plugins and APIs) shall not be sc_core::sc_modules 
 * (they shall not appear in SystemC-Analysis) they cannot
 * use the SystemC elaboration and simulation callbacks.
 *
 * This interface (with some implementation) provides some these kernel callbacks
 * to GreenControl Services.
 *
 * This interface automatically connects to the Core's Callback Dispatcher.
 *
 * Usage:
 * \code
   class MyServiceAPI : gs::ctr::initialize_if
   {                                          
   public:                                    
     MyServiceAPI() {  do something  }        
     ~MyServiceAPI() {  do something  }       
     void start_initialize_mode() { do something }
     void end_initialize_mode() { do something }  
     void gc_end_of_elaboration() { do something }
     // etc.                                      
   }
   \endcode
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

  /// Gets called on start_of_simulation (starting GreenControl initialize-mode) (Optional)
  /**
   * This starts the initialize mode for the listening object:
   * Inside this method some initial configuration can be done.
   * This is called by the CallbackDispatcher during the start_of_simulation callback.
   */
  virtual void start_initialize_mode() { };

  /// Ending GreenControl initialize-mode (Called during start_of_simulation) (Optional)
  /**
   * This ends the initialize-mode for the listening object:
   * Called during start_of_simulation after all start_initialize_mode calls have been done.
   * This is called by the CallbackDispatcher during the start_of_simulation callback.
   */ 
  virtual void end_initialize_mode() { };
  
  /// Gets called on before_end_of_elaboration (Optional)
  /**
   * Forwards the before_end_of_elaboration call. Called by the CallbackDispatcher.
   */ 
  virtual void gc_before_end_of_elaboration() { } 

  /// Gets called on end_of_elaboration (Optional)
  /**
   * Forwards the end_of_elaboration call. Called by the CallbackDispatcher.
   */ 
  virtual void gc_end_of_elaboration() { } 

  /// Gets called on start_of_simulation (Optional)
  /**
   * Forwards the start_of_simulation call. Called by the CallbackDispatcher.
   */ 
  virtual void gc_start_of_simulation() { } 
  
  /// Gets called on end_of_simulation (Optional)
  /**
   * Forwards the end_of_simulation call. Called by the CallbackDispatcher.
   */ 
  virtual void gc_end_of_simulation() { } 
  
};

/// initialize interface
typedef initialize_if_T<GC_Core> initialize_if;
    
} // end namespace ctr
} // end namespace gs

#endif
