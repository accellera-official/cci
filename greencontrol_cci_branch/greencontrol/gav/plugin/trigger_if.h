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
//   Robert Guenzel <guenzel@eis.cs.tu-bs.de>
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

#ifndef __trigger_if_h__
#define __trigger_if_h__

#include "calc_if.h"
#include "Calculator.h"

namespace gs {
namespace av {
    
  /// Interface for the trigger class
  /**
   * Class for the StatisticsCalculator trigger part.
   *
   * This interface has be be implemented by each trigger object
   * that should be used within a StatCalc.
   */
  class trigger_if {
  public:
    
    virtual ~trigger_if() { }
    
    /// Initialization function: Gives the parameter to the trigger that represents its activation status
    /**
     * The trigger has to keep this parameter up to date
     * according the activation status (update the param if
     * the status changes) and it has to register a callback
     * and change the activation status according the value.
     *
     * The caller has to call this initialization function before
     * set_calc_object and set_used_params.
     */
    virtual void set_activated_param(gs::gs_param<bool> &activated) = 0;

    /// Initialization function: Sets the calculation object which to be called when triggered
    /**
     * Sets a calculation object which implements the general calc_if
     * to be called by this trigger.
     *
     * Prefer using <code>set_native_calc_object</code> if possible because
     * then the trigger call will not be virtual.
     */
    virtual void set_calc_object(calc_if*) = 0;
    
    /// Initialization function: Gives the parameters to the trigger that are used by the calculation.
    /**
     * Used during initialization:
     *
     * Called by the StCalc to register all parameters that
     * are used by the calculation.
     *
     * This vector is got from the <code>get_used_params()</code>
     * (gs::av::calc_if::get_used_params)
     * of the calculation object (implementing the calc_if).
     */
    virtual void set_used_params(std::vector<const gs_param_base*>) = 0;

    /// Sets the GreenAV native calculation object which to be called when triggered
    /**
     * Prefer using this instead of <code>set_calc_object</code> if possible because
     * this trigger call will not be virtual.
     */
    //void set_native_calc_object(Calculator*);
    
    /// Returns if this StCalc is activated
    virtual bool is_activated() = 0;
    
    /// Activates this trigger (if it is deactivated)
    /**
     * Activates this Trigger (if it is deactivated).
     */
    virtual void activate() = 0;
    
    /// Deactivates this trigger (if it is activated)
    /**
     * Deactivates this Trigger (if it is activated).
     *
     * Should unregister all input parameter callbacks 
     * (change and destruct callbacks) due to performance
     * reason: A deactivated trigger should produce no
     * overhead!
     */
    virtual void deactivate() = 0;
    
    // To be called by an event listener when watched event notified
    virtual void event_callback() = 0;
    
    // To be called by an event listener when interval ends
    virtual void interval_callback() = 0;
    
  };
      
} // end namespace av
} // end namespace gs

#endif // __trigger_if_h__
