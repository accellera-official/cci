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

#ifndef __StatCalc_h__
#define __StatCalc_h__

#include "greencontrol/config.h"
#include "trigger_if.h"
#include "calc_if.h"
#include "Calculator.h"
#include "Calculator_bit.h"
//#include "Calculator_resetter.h"
#include "Trigger.h"

namespace gs {
namespace av {
  
  /// Statistics calculator combining a trigger and a calculator.
  /**
   * The Statistics Calculator manages the analysis of several 
   * input parameters that are calculated in a formula on several
   * activation events.
   * 
   * The StatCalc gets a trigger object and a calculator object.
   */
  template<typename result_type>
  class StatCalc 
  : public sc_object {
  public:

    /// Constructor with any calculator object
    StatCalc(const char* stcalc_name, trigger_if* any_trigger, calc_if* any_calculator)
    : sc_object(stcalc_name),
      m_newed_trigger(NULL),
      m_trigger(any_trigger),
      m_calc(any_calculator),
      m_activated(get_activated_param_name(stcalc_name), false)
    {
      GAV_DUMP_N(name(), "Constructor StatCalc '"<<name()<<"'.");
      if (any_trigger == NULL)
        SC_REPORT_ERROR(name(), "StatCalc - please specify Trigger not NULL!");
      if (any_calculator == NULL)
        SC_REPORT_ERROR(name(), "StatCalc - please specify Calculator not NULL!");
      // Connect trigger and calculator
      any_trigger->set_activated_param(m_activated);
      any_trigger->set_calc_object(any_calculator);
      any_trigger->set_used_params(any_calculator->get_used_params());
    }
    
    /// Constructor with any calculator object
    StatCalc(const char* stcalc_name, trigger_if& any_trigger, calc_if& any_calculator)
    : sc_object(stcalc_name),
      m_newed_trigger(NULL),
      m_trigger(&any_trigger),
      m_calc(&any_calculator),
      m_activated(get_activated_param_name(stcalc_name), false)
    {
      GAV_DUMP_N(name(), "Constructor StatCalc '"<<name()<<"'.");
      // Connect trigger and calculator
      any_trigger.set_activated_param(m_activated);
      any_trigger.set_calc_object(&any_calculator);
      any_trigger.set_used_params(any_calculator.get_used_params());
    }
    
    /// Constructor without trigger: will enable default: trigger on param changes
    /**
     * Creates a trigger object using parameter change callbacks.
     */
    StatCalc(const char* stcalc_name, calc_if* any_calculator)
    : sc_object(stcalc_name),
      m_trigger(NULL),
      m_calc(any_calculator),
      m_activated(get_activated_param_name(stcalc_name), false)
    {
      GAV_DUMP_N(name(), "Constructor StatCalc '"<<name()<<"'.");
      if (any_calculator == NULL)
        SC_REPORT_ERROR(name(), "StatCalc - please specify Calculator not NULL!");
      // create a callback trigger
      m_newed_trigger = new Trigger(true);
      m_trigger = m_newed_trigger;
      // Connect trigger and calculator
      m_newed_trigger->set_activated_param(m_activated);
      m_newed_trigger->set_calc_object(any_calculator);
      m_newed_trigger->set_used_params(any_calculator->get_used_params());
    }

    /// Constructor without trigger: will enable default: trigger on param changes
    /**
     * Creates a trigger object using parameter change callbacks.
     */
    StatCalc(const char* stcalc_name, calc_if& any_calculator)
    : sc_object(stcalc_name),
      m_trigger(NULL),
      m_calc(&any_calculator),
      m_activated(get_activated_param_name(stcalc_name), false)
    {
      GAV_DUMP_N(name(), "Constructor StatCalc '"<<name()<<"'.");
      // create a callback trigger
      m_newed_trigger = new Trigger(true);
      m_trigger = m_newed_trigger;
      // Connect trigger and calculator
      m_newed_trigger->set_activated_param(m_activated);
      m_newed_trigger->set_calc_object(&any_calculator);
      m_newed_trigger->set_used_params(any_calculator.get_used_params());
    }
    
    /// Destructor
    ~StatCalc() {
      m_trigger->deactivate();
      if (m_newed_trigger != NULL)
        delete m_newed_trigger;
    }

    /// implements gs::av::calc_if::get_result_param
    gs_param<result_type>* get_result_param() {
#ifdef GAV_VERBOSE
      assert(m_calc != NULL);
#endif
      return static_cast<gs_param<result_type>*>(m_calc->get_result_param());
    }
    
    /// Returns the calculator object of this StatCalc
    calc_if* get_calculator() {
      return m_calc;
    }
    
    /// Returns the trigger object of this StatCalc
    trigger_if* get_trigger() {
      return m_trigger;
    }
    
    // /////////// Functions of trigger_if called at the trigger /////////// //

    /// Returns if this StCalc is activated
    bool is_activated() {
      if (m_trigger != NULL)
        return m_trigger->is_activated();
    }
    
    /// Activates this trigger (if it is deactivated)
    void activate() {
      if (m_trigger != NULL)
        m_trigger->activate();
    }
    
    /// Deactivates this trigger (if it is activated)
    void deactivate() {
      if (m_trigger != NULL)
        m_trigger->deactivate();
    }
    
    // //////////////////////////////////////////////////////////// //
    
    /// Returns the parameter that represents and manipulates the activated status
    gs_param<bool>* get_activated_param() {
      return &m_activated;
    }

  protected:
    
    /// Builds the name for the activated param name.
    /**
     * Builds the name so that it results in the same
     * prefix as this StatCalc sc_object has.
     *
     * @param  stcalc_name  Local name of this StatCalc
     * @return  (Local) name of the parameter (including limiters to have the same name as this object)
     */
    std::string get_activated_param_name(const char* stcalc_name) {
      // 1/ get parent module name (the name which the gs_param will put as prefix
      // 2/ get name of this sc_object
      // 3/ return the remaining substring of the sc_object name which is not the parent sc_core::sc_module name
      sc_core::sc_module *mod = gs::cnf::get_parent_sc_module(this);
      std::string mod_name;
      if (mod) mod_name = mod->name();
      std::string this_name = name();
      std::ostringstream ret;
      // error case
      if (this_name.find(mod_name) != 0) {
        GAV_DUMP_N(name(), "get_activated_param_name: Failure: parent module name is not part of this object name.");
        ret << std::string(stcalc_name) << SC_NAME_DELIMITER << "activated";
      }
      // normal case
      else {
        std::string subs = this_name.substr(mod_name.size());
        if (subs.find(SC_NAME_DELIMITER) == 0) // remove . at beginning if there is one
          subs = subs.substr(1);
        ret << subs;
        if (ret.str().size() > 0)
          ret << SC_NAME_DELIMITER << "activated";
        else
          ret << std::string(stcalc_name) << SC_NAME_DELIMITER << "activated";
      }
      return ret.str();
    }
    
  protected:
    
    /// Pointer to the trigger object if the trigger was newed by this object 
    trigger_if *m_newed_trigger;

    /// Pointer to the trigger of this StatCalc
    trigger_if *m_trigger;
    /// Pointer to the calculator of this StatCalc
    calc_if *m_calc;

    /// Parameter representing the activated status (of the trigger)
    gs_param<bool> m_activated;
    
  };
      
} // end namespace av
} // end namespace gs

#endif // __StatCalc_h__
