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


#ifndef __Trigger_h__
#define __Trigger_h__

#include <set>
#include <stdarg.h>

#include "gav_globals.h"
#include "event_listener.h"
#include "greencontrol/config.h"
#include "greencontrol/gav/apis/gav_api/GAV_Api.h"


namespace gs {
namespace av {
  
  using cnf::ParamCallbAdapt_b;
  
  
  /// GreenAV default Trigger is responsible for activation events when to use the calculator functor
  /**
   * The constructors specify the way the trigger waits for 
   * calculation events.
   *
   * If there is no constructor for the combination the user
   * needs, use one constructor and one of the functions 
   * afterwards: 
   * - <code>set_event(sc_event& ev)</code>,
   * - <code>set_sample_interval(sc_time& sample)</code>
   * - <code>enable_on_change_activation()</code>
   *
   * The constructor event order is: 
   * sc_event sc_time bool_on_changes gs_param<bool>_condition.
   *
   * Event kinds are:
   * - sc_event for events
   * - sc_time for sample intervals
   * - bool for setting if to register for changes on all input parameters
   * - gs_param<bool> for an additional condition to be checked before calculation
   * - manually chosen parameter changes are a special case (constructor)
   */
  class Trigger 
    : public trigger_if {
      
    public:
      
      GC_HAS_CALLBACKS();

      /// Default constructor: triggered by input parameter changes
      explicit Trigger() { init(NULL, NULL, true, NULL); }

      // ** Single contructors **
      /// Constructor: triggered by event
      explicit Trigger(sc_event& event) { init(&event, NULL, false, NULL); }
      /// Constructor: triggered by event
      explicit Trigger(sc_event* event) { init(event, NULL, false, NULL); }
      /// Constructor: triggered by time interval
      explicit Trigger(sc_time ti) { init(NULL, &ti, false, NULL); }
      /// Constructor: triggered by time interval
      explicit Trigger(double t, sc_time_unit u) { sc_time ti(t,u); init(NULL, &ti, false, NULL); }
      /// Constructor: triggered by any change of one of the input parameters (callbacks)
      explicit Trigger(bool on_changes) { init(NULL, NULL, on_changes, NULL); }
      
      // ** Single contructors with condition **
      /// Constructor: triggered by event, boolean condition
      explicit Trigger(sc_event& event, gs_param<bool> &cond) { init(&event, NULL, false, &cond); }
      /// Constructor: triggered by time interval, boolean condition
      explicit Trigger(sc_time &ti, gs_param<bool> &cond) { init(NULL, &ti, false, &cond); }
      /// Constructor: triggered by time interval, boolean condition
      explicit Trigger(double t, sc_time_unit u, gs_param<bool> &cond) { sc_time ti(t,u); init(NULL, &ti, false, &cond); }
      /// Constructor: triggered by any change of one of the input parameters (callbacks), boolean condition
      explicit Trigger(bool on_changes, gs_param<bool> &cond) { init(NULL, NULL, on_changes, &cond); }
      /// Constructor: boolean condition, default triggered by any change of one of the input parameters (callbacks)
      explicit Trigger(gs_param<bool> &cond) { init(NULL, NULL, true, &cond); }
      /// Constructor: boolean condition, default triggered by any change of one of the input parameters (callbacks)
      explicit Trigger(gs_param<bool> *cond) { init(NULL, NULL, true, cond); }
      
      /// Constructor: vector of param base pointers: triggered by manually chosen parameter changes (callbacks)
      /**
       * These parameters need not to (but may) be the input parameters of the calculator.
       *
       * If all input parameters of the calculation should be used
       * simply use the constructor <code>Trigger(bool)</code>
       * or <code>Trigger()</code>!
       */
      explicit Trigger(std::vector<gs_param_base*> sensitivity_params) { 
        init(NULL, NULL, false, NULL); 
        for(unsigned int i = 0; i < sensitivity_params.size(); i++) {
          sensitivity_parameters.insert(sensitivity_params[i]->getName());
        }
        enabled_sensitivity_param_change_activation = true;
      }
      
      // ** Combined contructors **
      //event time bool gs_param
      /// Constructor: triggered by event and interval and callbacks and boolean condition
      explicit Trigger(sc_event &ev, sc_time &ti, bool on_changes, gs_param<bool> cond) { init(&ev, &ti, on_changes, &cond); }
      //event time gs_param
      /// Constructor: triggered by event and interval and boolean condition
      explicit Trigger(sc_event &ev, sc_time &ti, gs_param<bool> &cond) { init(&ev, &ti, false, &cond); }
      //event time
      /// Constructor: triggered by event and interval
      explicit Trigger(sc_event &ev, sc_time &ti) { init(&ev, &ti, false, NULL); }
      //event bool
      /// Constructor: triggered by event and callbacks
      explicit Trigger(sc_event &ev, bool on_changes) { init(&ev, NULL, on_changes, NULL); }
      //time bool gs_param
      /// Constructor: triggered by interval and callbacks and boolean condition
      explicit Trigger(sc_time &ti, bool on_changes, gs_param<bool> &cond) { init(NULL, &ti, on_changes, &cond); }
      //time bool
      /// Constructor: triggered by interval and callbacks
      explicit Trigger(sc_time &ti, bool on_changes) { init(NULL, &ti, on_changes, NULL); }

    protected:
      
      void init(sc_event* ev, sc_time* ti, bool oc, gs_param<bool>* co) {
        m_is_activated = false;
        deactivated_forewer = false;
        enabled_on_change_activation = false;
        enabled_sensitivity_param_change_activation = false;
        got_result_param =false;
        condition = NULL;
        sample_event_id = -1;
        m_calculator = NULL;
        
        m_name = sc_gen_unique_name("Trigger");

        GAV_DUMP_N(name(), "constructor, Init trigger:");
        m_GCnf_Api = cnf::GCnf_Api::getApiInstance(NULL); // TODO unsauber
        m_GAV_Api = GAV_Api::getApiInstance(NULL);
        m_event_listener = m_GAV_Api->get_event_listener();
        if (!m_event_listener) {
          SC_REPORT_WARNING(name(), "Trigger Event Listener (GreenAV Plugin) is not available! Trigger won't work as expected!");
        }
        
        // if on event
        if (ev != NULL) {
          GAV_DUMP_N(name(), "    set sc_event.");
          set_event(*ev);
        }
        // if sample interval
        if (ti != NULL) {
          GAV_DUMP_N(name(), "    set sample interval "<<ti->to_string().c_str()<<".");
          set_sample_interval(*ti);
        }
        // if on changes
        if (oc) {
          GAV_DUMP_N(name(), "    enable on-change-callbacks.");
          enabled_on_change_activation = true;
        }
        // if condition
        if (co != NULL) {
          GAV_DUMP_N(name(), "    set condition.");
          set_condition(co);
        }
      }
      
    public:
      
      virtual ~Trigger() {
        GC_UNREGISTER_CALLBACKS();
      }
      
      // ///////////// (de)activation //////////////////////////////// //
      
      /// implements gs::av::trigger_if::is_activated
      bool is_activated() {
        return m_is_activated;
      }
      
      /// implements gs::av::trigger_if::activate
      /**
       * Activates this Trigger (if it is deactivated).
       */
      void activate() {
        if (!m_is_activated) {
          GAV_DUMP_N(name(), "activate");
          if (deactivated_forewer) {
            GAV_DUMP_N(name(), "Error activating StatCalc which is deactivated forever because of deleted input parameter.");
            return;
          }
          if (enabled_on_change_activation) {
            // register all input param change callbacks
            if (register_change_callbacks()) {
              m_is_activated = true;
            }
            else SC_REPORT_WARNING(name(), "Error activating StatCalc, registering change callbacks failed.");
          } else {
            // register all input param destruct callbacks
            if (register_destruct_callbacks()) {
              m_is_activated = true;
            }
            else SC_REPORT_WARNING(name(), "Error activating StatCalc, registering destruct callbacks failed.");
          }
          if (enabled_sensitivity_param_change_activation) {
            // register sensitivity parameter callbacks
            if (register_sensitivity_change_callbacks()) {
              m_is_activated = true;
            }
            else SC_REPORT_WARNING(name(), "Error activating StatCalc, registering sensitivity change callbacks failed.");
          }
        }
        else GAV_DUMP_N(name(), "was already activated");
        // update activated gs_param
        assert(activated != NULL); // call set_activated_param before calling this function!
        if (*activated != m_is_activated)
          *activated = m_is_activated;
      }

      /// implements gs::av::trigger_if::deactivate
      /**
       * Deactivates this Trigger (if it is activated).
       *
       * Unregisters all input parameter callbacks 
       * (change and destruct callbacks)
       */
      void deactivate() {
        GAV_DUMP_N(name(), "deactivate");
        if (m_is_activated) {
          m_is_activated = false;
          // unregister all input parameter callbacks
          //  do not only call unreg change cb if enabled_on_change_activation
          //  because deactivate() may be called during registering change callbacks!
          // unregister all input parameter change callbacks
          remove_callbacks(input_parameter_change_callbAdapts);
          // unregister all input parameter destruct callbacks
          remove_callbacks(input_parameter_destruct_callbAdapts);
          // unregister all sensitivity parameter change callbacks
          remove_callbacks(sensitivity_parameter_change_callbAdapts);
        }
        else {
          GAV_DUMP_N(name(), "No activation change");
        }
        // update activated gs_param
        if (*activated != m_is_activated)
          *activated = m_is_activated;
      }

      /// Sets the activation status (calls <code>(de)activate()</code>)
      void set_activation_status(bool _activate) {
        GAV_DUMP_N(name(), "set_activation_status("<<_activate<<")");
        if (m_is_activated && !_activate)
          deactivate();
        else if (!m_is_activated && _activate)
          activate();
        else std::cout << "no activation change" << std::endl;
      }
      
      /// implements gs::av::trigger_if::set_calc_object
      void set_calc_object(calc_if* calc) {
        if (!calc) {
          SC_REPORT_ERROR(name(), "set_calc_object(calc) failed: calc is NULL!");
          return;
        }
        m_calculator = calc;
      }

      /// implements gs::av::trigger_if::set_used_params
      void set_used_params(std::vector<const gs_param_base*> used_params) {
        if (!m_calculator) {
          SC_REPORT_ERROR(name(), "You must set set_calc_object(...) before calling set_used_params(...)!");
          return;
        }
        std::vector<const gs_param_base*>::iterator iter;
        // copy name of each input parameter
        for( iter = used_params.begin(); iter != used_params.end(); iter++ ) {
          input_parameters.insert((*iter)->getName());
        }
        activate();
      }
     
      /// implements gs::av::trigger_if::set_activated_param
      void set_activated_param(gs::gs_param<bool> &activated) {
        this->activated = &activated;
        GC_REGISTER_PARAM_CALLBACK(this->activated, Trigger, activation_param_callback);
      }

      // /////////// calculation activation controlling /////////////// //
      
      /// Enable the parameter-change-trigger
      /**
       * - removes the destruct callbacks
       * - registers change callbacks (with check if still existing)
       *
       * @return If successfully enabled. If false not all parameters are existing any longer.
       */
      bool enable_on_change_activation() {
        GAV_DUMP_N(name(), "enable_on_change_activation() Register the input parameter callbacks.");
        // unregister all input parameter destruct callbacks
        remove_callbacks(input_parameter_destruct_callbAdapts);
        // register change callbacks for all input parameters
        if (register_change_callbacks())
          enabled_on_change_activation = true; // set this if not yet done
        else
          SC_REPORT_ERROR(name(), "enable_on_change_activation(): Registering change callbacks failed!");
        return true;
      }
      
      /// Disable the parameter-change-trigger
      /**
       * - removes the change callbacks
       * - registers destruct watch callbacks (with check if still existing)
       */
      void disable_on_change_activation() {
        GAV_DUMP_N(name(), "disable_on_change_activation() unregister the input parameter change callbacks.");
        // unregister all input parameter change callbacks
        remove_callbacks(input_parameter_change_callbAdapts);
        // register destruct watch callbacks for all input parameters
        register_destruct_callbacks();
        enabled_on_change_activation = false;
      }
     
      /// Sets the boolean condition (guard) which is checked before calculation
      /**
       * When being called repeatedly the old guard will be overwritten.
       *
       * A callback to the function <code>condition_callback</code>
       * is registered for the condition parameter to unregister the
       * callback when being destroyed. The StCalc will be disabled 
       * when this happens.
       *
       * @param cond  Condition (bool parameter) that will be checked each time
       *              NULL: disable condition, calculate each time without cond check.
       */
      void set_condition(gs_param<bool>* cond) {
        if (condition != NULL) {
          GAV_DUMP_N(name(), "change condition (first delete old callback)");
          GC_UNREGISTER_CALLBACK(cond_callbadapt);
          condition = NULL;
        }
        if (cond != NULL) {
          cond_callbadapt = GC_REGISTER_PARAM_CALLBACK(cond, Trigger, condition_callback);
          condition = cond;
        }
      }
      /// see set_condition(gs_param<bool>*)
      void set_condition(gs_param<bool>& cond) {
        set_condition(&cond);
      }
      /// Callback function used only for the condition parameter to check destructing
      /**
       * @param cond_par  Parameter that called the callback.
       */
      void condition_callback(gs_param_base& cond_par) {
        if (cond_par.is_destructing()) {
          GAV_DUMP_N(name(), "removing condition and deactivating StCalc because condition parameter is destructing.");
          condition = NULL;
          deactivate();
        }
      }

      /// Callback function used for the activated parameter to update activation status
      /**
       * Only performs activate/deactivate if needed (otherwise we would
       * run in infinite loops).
       *
       * @param cond_par  Parameter that called the callback.
       */
      void activation_param_callback(gs_param_base& cond_par) {
        GAV_DUMP_N(name(), "Handling activation param callback (="<<cond_par.getString().c_str()<<").");
        if (*activated && !m_is_activated) {
          activate();
        } else if (!*activated && m_is_activated) {
          deactivate();
        }
      }
      
      /// Set an event which results in recalculation when notified.
      /**
       * When called repeatedly all events are valid.
       * Removing events is unfortunately not possible because SystemC
       * does not allow removing spawned methods.
       *
       * @param ev  Event that should result in calculation when being notified
       */
      void set_event(sc_event& ev) {
        //if (m_event_id >= 0)
        //  remove_event();
        //m_event_id = 
        if (m_event_listener) 
          m_event_listener->create_event_listener(this, &trigger_if::event_callback, ev);
      }
      /*void remove_event() {
       if (m_event_listener) {
       if (!m_event_listener->remove_event_listener(m_event_id))
       SC_REPORT_WARNING(name(), "remove_event(): removing event failed!");
       }
       m_event_id = -1;
       }*/
      /// Callback function for events
      /**
       * Callback function is called by the event listener (plugin) waiting for a notified event
       *
       * Called by the event listener which was created by this StCalc
       * inside the GAV_Plugin.
       */
      void event_callback() {
        GAV_DUMP_N(name(), "Event notified!");
        calculate();
      }
      
      /// Set a sample for the recalculation interval.
      /**
       * When called repeatedly the last interval is the valid one.
       *
       * - creates an event_listener if not already existing
       * - cancels the current notifies for the internal event
       * - notifies the event with the sample time
       */
      void set_sample_interval(sc_time& sample) {
        sample_time = sample;
        sample_event.cancel();
        if (sample == sc_time(0, SC_NS)) {
          GAV_DUMP_N(name(), "Remove sample interval!");
          return;
        }
        GAV_DUMP_N(name(), "Set sample interval "<<sample.to_string().c_str()<<"!");
        sample_event.notify(sample_time);
        if (sample_event_id == -1)
          if (m_event_listener) sample_event_id = m_event_listener->create_event_listener(this, &trigger_if::interval_callback, sample_event);
      }
      void set_sample_interval(double t, sc_time_unit u) {
        sc_time tu = sc_time(t, u);
        set_sample_interval(tu);
      }
      /// Callback function for sample intervals
      /**
       * Callback function is called by the event listener (plugin) waiting for a notified event
       *
       * Called by the event listener which was created by this StCalc
       * inside the GAV_Plugin.
       */
      void interval_callback() {
        GAV_DUMP_N(name(), "Event for interval notified!");
        calculate();
        sample_event.notify(sample_time);
      }
      
      /// Manually condition to recalculate.
      void calculate_now() {
        GAV_DUMP_N(name(), "manual calculation!");
        calculate();
      }
      
     
      /// Callback function for activation observed parameters
      void param_change_callback(gs_param_base &p) {
        if (!p.is_destructing()) {
          if (enabled_on_change_activation || enabled_sensitivity_param_change_activation) 
            calculate();
        } else {
          SC_REPORT_INFO(name(), "Deactivating StCalc because of destructed input/sensitivity parameter!");
          deactivate();
          deactivated_forewer = true;
        }
      }
      
      /// Callback function for observing destruction of input parameters
      void param_destruct_watch_callback(gs_param_base &p) {
        if (p.is_destructing()) {
          SC_REPORT_INFO(name(), "Deactivating StCalc because of destructed input parameter!");
          deactivate();
          deactivated_forewer = true;
        }
      }
      
    protected:

      /// Help function: Unregister all callbacks of the given vector
      void remove_callbacks(std::vector<boost::shared_ptr<ParamCallbAdapt_b> >& cbAdaptVec) {
        // unregister all input parameter callbacks
        std::vector<boost::shared_ptr<ParamCallbAdapt_b> >::iterator iter;
        for( iter = cbAdaptVec.begin(); iter != cbAdaptVec.end(); iter++ ) {
          GC_UNREGISTER_CALLBACK(*iter);
        }
        cbAdaptVec.clear();
      }
      /// Help function: Register change callbacks for all input parameters
      /**
       * Checks if parameters are still existing before 
       * registering callbacks.
       */
      bool register_change_callbacks() {
        if (input_parameter_change_callbAdapts.size() != 0) {
          SC_REPORT_WARNING(name(), "Cannot register change callbacks for trigger!");
          return false;
        }
        gs_param_base *par;
        std::set<std::string>::iterator iter;
        // register change callbacks for each input parameter
        for( iter = input_parameters.begin(); iter != input_parameters.end(); iter++ ) {
          // check if parameter is still existing
          par = m_GCnf_Api->getPar(*iter);
          if (par == NULL) {
            SC_REPORT_INFO(name(), "Cannot register callbacks because not all input parameters are still existing! (1)");
            deactivate();
            deactivated_forewer = true;
            return false;
          } else {
            input_parameter_change_callbAdapts.push_back( 
                  GC_REGISTER_PARAM_CALLBACK(par, Trigger, param_change_callback)
            );
          }
        }
        return true;
      }
      /// Help function: Register destruct watch callbacks for all input parameters
      /**
       * Checks if parameters are still existing before 
       * registering callbacks.
       */
      bool register_destruct_callbacks() {
        if (input_parameter_destruct_callbAdapts.size() != 0) {
          SC_REPORT_WARNING(name(), "Cannot register destruct callbacks for trigger!");
          return false;
        }
        gs_param_base *par;
        std::set<std::string>::iterator iter;
        // register change callbacks for each input parameter
        for( iter = input_parameters.begin(); iter != input_parameters.end(); iter++ ) {
          // check if parameter is still existing
          par = m_GCnf_Api->getPar(*iter);
          if (par == NULL) {
            SC_REPORT_INFO(name(), "Cannot register callbacks because not all input parameters are still existing! (2)");
            deactivate();
            deactivated_forewer = true;
            return false;
          } else {
            input_parameter_destruct_callbAdapts.push_back( 
                  GC_REGISTER_PARAM_CALLBACK(par, Trigger, param_destruct_watch_callback)
            );
          }
        }
        return true;
      }
      /// Help function: Register change callbacks for all sensitivity parameters
      /**
       * Checks if parameters are still existing before 
       * registering callbacks.
       */
      bool register_sensitivity_change_callbacks() {
        if (sensitivity_parameter_change_callbAdapts.size() != 0) {
          SC_REPORT_WARNING(name(), "Cannot register sensitivity change callbacks for trigger!");
          return false;
        }
        gs_param_base *par;
        std::set<std::string>::iterator iter;
        // register change callbacks for each sensitivity parameter
        for( iter = sensitivity_parameters.begin(); iter != sensitivity_parameters.end(); iter++ ) {
          // check if parameter is still existing
          par = m_GCnf_Api->getPar(*iter);
          if (par == NULL) {
            SC_REPORT_INFO(name(), "Cannot register callbacks because not all sensitivity parameters are still existing! (3)");
            deactivate();
            deactivated_forewer = true;
            return false;
          } else {
            sensitivity_parameter_change_callbAdapts.push_back( 
                                                         GC_REGISTER_PARAM_CALLBACK(par, Trigger, param_change_callback)
                                                         );
          }
        }
        return true;
      }
      
      /// Internal calculation function: Calculates the formula and updates the result.
      /**
       * Checks if activated
       */
      inline void calculate() {
        // //////// tests before calculation //////////////// //
        if (!m_is_activated) {
          GAV_DUMP_N(name(), "Not calculate because StCalc is deactivated!");
          return;
        } else {
          if (condition != NULL && condition->getValue() == false) {
            GAV_DUMP_N(name(), "Not calculate because condition is false!");
            return;
          }
        }
        GAV_DUMP_N(name(), "Calculate formula.");
        (*m_calculator)();
      }
      
      // Returns the name of this
      const char* name() {
        return m_name.c_str();
      }
      
    protected:
      
      /// Unique name of this Calculator.
      std::string m_name;
      
      /// If the StCalc is activated. If deactivated the callbacks are unregistered.
      bool m_is_activated;
      
      /// If the StCalc is deactivated forever (because of deleted input param)
      bool deactivated_forewer;
      
      /// If the standard reacting on input change callbacks is enabled or not
      bool enabled_on_change_activation;
      
      /// If the user already has got the result parameter
      bool got_result_param;
      
      /// Pointer to a GCnf_Api (for e.g. check param existence)
      cnf::cnf_api_if* m_GCnf_Api;
      
      /// Pointer to a GAV_Api (for e.g. getting event listener)
      shared_ptr<GAV_Api> m_GAV_Api;
      
      /// list of all input parameter names (independent from having (de)activated callbacks)
      std::set<std::string> input_parameters;
      /// list of all callbackAdapters registered for param change callbacks
      std::vector<boost::shared_ptr<ParamCallbAdapt_b> > input_parameter_change_callbAdapts;
      /// list of all callbackAdapters registered for param destruction callbacks
      std::vector<boost::shared_ptr<ParamCallbAdapt_b> > input_parameter_destruct_callbAdapts;

      /// list of all sensitivity parameter names
      std::set<std::string> sensitivity_parameters;
      /// list of all callbackAdapters registered for sensivity param change callbacks
      std::vector<boost::shared_ptr<ParamCallbAdapt_b> > sensitivity_parameter_change_callbAdapts;
      /// If reacting on sensitivity parameter callbacks is enabled or not
      bool enabled_sensitivity_param_change_activation;
      
      /// Condition pointer
      gs_param<bool> *condition;
      /// Callback Adapter for the condition callback
      boost::shared_ptr<ParamCallbAdapt_b > cond_callbadapt;
      
      /// Pointer to the plugin which can handle events to create callbacks to this; can be NULL if plugin not existing
      event_listener<trigger_if>* m_event_listener;
      
      /* /// id of the event which is registered at the event listener for this event, =-1 if no event
       int m_event_id;*/
      /// id of the sample event which is registered at the event listener for this event
      int sample_event_id;
      /// Sample event which is notified repeatedly each interval, =-1 if no event
      sc_event sample_event;
      /// Sample interval time
      sc_time sample_time;
      
      calc_if* m_calculator;
      
      gs::gs_param<bool> *activated;
      
    };
  
      
} // end namespace av
} // end namespace gs


#endif
