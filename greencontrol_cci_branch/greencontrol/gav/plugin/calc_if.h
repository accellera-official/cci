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

#ifndef __calc_if_h__
#define __calc_if_h__

namespace gs {
namespace av {


  /// Interface for the calculatior class.
  /**
   * Class for the StatisticsCalculator calculation part.
   *
   * This interface has be be implemented by each calculator object
   * that should be used within a StatCalc.
   */
  class calc_if {
  public:
    
    virtual ~calc_if() { }
    
    /// Do the calculation
    /**
     * Before this may called the function get_used_params() must be 
     * called once! (This should be done by the StatCalc when
     * being instantiated with this calculator object).
     */     
    virtual void operator()() = 0;
    
    /// Returns the result parameter of this calculator.
    /**
     * This call has to be used to access the result of this calculator.
     * Call this function after having set the formula completely
     * and after having enabled a sliding window (if needed) since
     * the activation of a sliding window will change the internal
     * result parameter.
     *
     * @returns Result parameter of this StCalc.
     */
    virtual gs_param_base* get_result_param() = 0;
   
    /// Returns all parameters that are used by the calculation.
    /**
     * Called by the StCalc to give the parameters that
     * are used by the calculation to the trigger object
     * (gs::av::trigger_if::set_used_params).
     */
    virtual std::vector<const gs_param_base*>& get_used_params() = 0;
    
  };    
  
} // end namespace av
} // end namespace gs

#endif // __calc_if_h__
