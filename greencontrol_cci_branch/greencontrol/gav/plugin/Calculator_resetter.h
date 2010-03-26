//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2008 : GreenSocs Ltd
//    http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//   Robert Guenzel <guenzel@eis.cs.tu-bs.de>,
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

#ifndef __Calculator_resetter_h__
#define __Calculator_resetter_h__

#include "gav_globals.h"
#include "../../gcnf/apis/gs_param/gs_param.h"
#include "calc_if.h"

#include <deque>


namespace gs {
namespace av {
      
  /// GreenAV calculator class allowing resetting an input parameter.
  /**
   * Class for the StatCalc calculation part.
   * Implements <code>calc_if</code>.
   *
   * This calculator resets the given input parameter each time
   * the operator() is called to the given default reset value.
   *
   * Before resetting the parameter to the value the calculator 
   * may call another calculator which e.g. may use the resetted
   * parameter as an input.
   *
   * This Calculator class gives through the trigger to the 
   * connected Calculator.
   *
   * Warning: Never use a trigger that triggers on parameter
   * changes of the resetted! That will lead to infinite
   * loops.
   *
   * Use example:
   *  A transaction per second counter. The counter is resetted 
   *  each second by a Calculator_resetter (triggered by a 1 sec. 
   *  sample interval trigger). Before the reset, another Calculator
   *  is called by this one to update the actual transactions per
   *  second value.
   *
   * Code example:
   * <code>
       // DYNAMIC overall hit rate Statistics Calculator
       // formula Calculator
       gs::av::Calculator<double> *trpsec_calc // Create Calculator
         = new gs::av::Calculator<double>("dynamic_tr_p_sec_overall");
       trpsec_calc->enable_sloppy(); // allow division by zero
       trpsec_calc->calc("per_second", tr_cnt, sc_core::sc_time_stamp().to_seconds()); // set formula: (tr_cnt 'per seconds')
       // reset Calculator
       gs::av::Calculator_resetter<unsigned int> *tr_cnt_res_calc // Create Calculator
         = new gs::av::Calculator_resetter<unsigned int>(tr_cnt, 0, trpsec_calc);
       // Trigger
       gs::av::Trigger *tr = new gs::av::Trigger(1, SC_MS); erzeugt endlosschleife!
       // StatCalc
       overall_tr_per_sec = new gs::av::StatCalc<double>("dynamic_overall_trans_per_sec_StC", tr, tr_cnt_res_calc); // create Statistics Calculator
   * </code>
   *
   */
  template<typename T>
  class Calculator_resetter
  : public calc_if {

  public:
    /// Constructor with a user chosen name
    Calculator_resetter(gs_param<T> &par_to_reset, const T default_reset_value, calc_if *connected_calculator = NULL)
    : got_result_param(false),
      got_input_params(false),
      m_connected_calculator(connected_calculator),
      m_reset_param(&par_to_reset),
      m_reset_value(default_reset_value)
    {
    }

    /// Destructor
    ~Calculator_resetter() {
      GAV_DUMP_N(name(), "Destruct Calculator.");      
    }
  
    
    
    /// implements gs::av::calc_if::operator()
    /**
     * Calculation function called by the trigger.
     */
    void operator()() {
      cout << "calculate resetter calc" << endl;
      // perform calculation of connected calculator
      if (m_connected_calculator != NULL)
        (*m_connected_calculator)();
      // reset the input parameter
      *m_reset_param = m_reset_value;
    }
    
    /// implements gs::av::calc_if::get_result_param
    /**
     * Returns the result parameter of the connected
     * calculator (if existing). Otherwise returns 
     * the reset parameter.
     *
     * Take care of the type of this returned gs_param_base
     * which is not necessarily of the data type of this
     * calculator but maybe of the type of the connected
     * calculator.
     *
     * @returns Result parameter of this StCalc.
     */
    gs_param_base* get_result_param() {
      got_result_param = true;
      if (m_connected_calculator == NULL) {
        return m_reset_param;
      }
      return m_connected_calculator->get_result_param();
    }
    
    /// implements gs::av::calc_if::get_used_params
    std::vector<const gs_param_base*>& get_used_params() {
      got_input_params = true;
      // initialize the vector
      if (m_connected_calculator != NULL) 
        input_parameters = m_connected_calculator->get_used_params();
      else
        input_parameters.clear();

      // search for this reset param in the vector
      bool is_already_in_vector = false;
      for (unsigned int i = 0; i < input_parameters.size(); i++) {
        if (input_parameters[i] == m_reset_param) {
          is_already_in_vector = true;
          cout << "reset param is already in vector" << endl;
        }
      }
      
      // if not already in connected calc, add the reset param
      if (!is_already_in_vector) {
        input_parameters.push_back(m_reset_param);
        cout << "reset param is NOT YET in vector" << endl;
      }
      return input_parameters;
    }
    
   
  protected:
    
    // Returns the name of this
    const char* name() {
      return "Calculator_resetter";
    }
    
    /// If the user already has got the result parameter
    bool got_result_param;
    /// If the StatCalc already has got the input parameters
    bool got_input_params;

    /// Vector containing all parameters that this Calculator uses as input
    std::vector<const gs_param_base*> input_parameters;
    
    /// Parameter list 
    std::vector<std::pair<const gs_param_base*,const gs_param_base*> > m_params;

    /// Pointer to a (possibly) connected calculator object whose calculation is called before here the reset is done
    calc_if *m_connected_calculator;
    
    /// Pointer to the parameter that should be resetted when this calculator does calculation
    gs_param<T> *m_reset_param;
    
    /// The value the parameter is resetted to
    T m_reset_value;
    
  }; // end of class Calculator_resetter
  
  
} // end namespace av
} // end namespace gs

#endif // __Calculator_h__
