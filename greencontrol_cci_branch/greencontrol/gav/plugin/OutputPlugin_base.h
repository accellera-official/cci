//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007-2009 : GreenSocs Ltd
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


#ifndef __OutputPlugin_base_h__
#define __OutputPlugin_base_h__

#include <map>
#include "gav_globals.h"
#include "event_listener.h"
#include "greencontrol/config.h"
#include "OutputPlugin_if.h"
#include "gav_datatypes.h"

namespace gs {
namespace av {

  //
  // This is not an sc_object because its name should be (a child of the GAV Plugin) 
  // <plugin_name>.OutputPlugin_<constructor_par>
  // and not a child of the user module <initiator_of_the_creation>.OutputPlugin_<constructor_par>
  //
  
  /// Output Plugin base with a default implementation of the OutputPlugin_if functions
  /**
   * Output Plugins derive from this base class to
   * avoid reimplementation for each OutputPlugin.
   *
   * An Output Plugin needs then only implement its constructor, destructor, the init function
   * and the callback function <code>config_callback</code>. Have a detailled
   * look at the function's comments! @see gs::av::OutputPlugin_base::config_callback .
   *
   * Output Plugins should implement the constructor which gets one std::string parameter
   * to allow adding the Output Plugin to the GAV_Plugin's create command.
   * Do not create any files, streams or other memory consuming things 
   * in the constructor! Use the init() function for that!
   *
   * All functions are virtual to give the derived class the oportunity to catch the
   * calls before calling the base class' function. Performance is not a problem (expect 
   * function <code>config_callback</code>. TODO: performance enhancement capabilities!
   */
  class OutputPlugin_base
  : public sc_object,
    public OutputPlugin_if
  {
    
    GC_HAS_CALLBACKS();
    
    typedef std::map<std::string, boost::shared_ptr<cnf::ParamCallbAdapt_b> > cb_adapter_map_type;
    
  public:
    
    /// Constructor, the sc_object's name is built of the OutputPluginType and the constructor parameter.
    /**
     * Do not create any files, streams or other memory consuming things 
     * in the constructor! Use the init() function for that!
     *
     * @param _constructor_param  Generic constructor string
     * @param ev_listn            Event listener helpclass instance that can be used by this Output Plugin
     * @param type                Output plugin type
     * @param start_running       If this output plugin should start running
     */
    OutputPlugin_base(const char* _constructor_param, event_listener<OutputPlugin_base> *ev_listn, OutputPluginType type, bool start_running = true)
    : sc_object((outputPluginTypeToString(type) + "_" + convert_to_sc_name(_constructor_param)).c_str()),
      constructor_param(_constructor_param),
      is_used(false),
      is_running(start_running),
      m_automatic_start_enabled(true),
      ev_listener(ev_listn),
      pause_event_registered(false),
      m_currently_calling_initial_add_for_observation_callback(false),
      m_constructing(true)
    {   
      mCnfApi = cnf::GCnf_Api::getApiInstance(cnf::get_parent_sc_module(this));
      // register callbacks for new added child parameters
      mCnfApi->REGISTER_NEW_PARAM_CALLBACK(OutputPlugin_base, new_param_callback);
      // look for already existing child params and register them (done by callback function)
      std::vector<std::string> v = mCnfApi->getParamList(std::string(name())+".*");
      for (std::vector<std::string>::iterator it = v.begin(); it != v.end(); it++) {
        GAV_DUMP_N(name(), "Handle already existing (maybe implicit) child parameter '"<<(*it).c_str()<<"' of the output plugin (possibly enable params):");
        new_param_callback(*it, mCnfApi->getValue(*it));
      }
      m_constructing = false;
      // TODO register the e_o_elab cb
    }
    
    
    /// Destructor
    /**
     * The destructor puts out an info if this output plugin was never used
     * (if it is not a default one).
     */
    virtual ~OutputPlugin_base() {
      GC_UNREGISTER_CALLBACKS();
      // delete enabled parameters of this
      for (std::vector<gs_param<bool>*>::iterator it = enabled_params.begin(); it != enabled_params.end(); it++) {
        delete *it;
      }
      // Check if ever run
      if (!is_used && constructor_param != "default") {
        GAV_DUMP_N(name(), "Info: The output plugin "<<get_id()<<" was never running!");
        SC_REPORT_INFO(name(), "The output plugin was never running!");
      }
    }
    
    
    /// Callback function for observed parameters; calls init() if needed and forwards the call to config_callbacks.
    inline void internal_config_callback(gs_param_base &par) {
      if (!is_used) { 
        GAV_DUMP_N(name(), "Init Output Plugin (id="<<get_id()<<")");
        this->init();
        is_used = true; 
      }
      config_callback(par);
    }

    /// Callback function for observed parameters; to be implemented by the OutputPlugins!
    /**
     * This function performs the output action!
     *
     * This function gets called the first time for each parameter at the moment
     * when the parameter gets registered for observation. This can be checked 
     * using the bool member m_currently_calling_initial_add_for_observation_callback.
     *
     * This function should handle deleted parameters that are just being
     * destroyed by calling the remove function to remove it from the 
     * observed-list.
     *
     * It may happen that this function is called during base class construction 
     * when there are child parameters of this output plugin existing. The user can 
     * check/filter using the bool member m_constructing.
     * 
     * This function must only do the output action when
     * <code>is_running == true</code>!!
     *
     * Example code:
     \code
      void config_callback(gs_param_base &par) {
       // check for destructing parameters: remove them from callback list if removed
       if (par.is_destructing()) {
         remove(par.getName());
       }
       // only perform output action when not paused (or stopped)
       else if (is_running) {
         // do any output action!
       }
      }
     \endcode
     */
    virtual void config_callback(gs_param_base &par) =0;
    
    /// Callback function; is called if a new param is added
    /**
     * Checks if this is a parameter for this object.
     * If yes, create a bool param to observe that value.
     */
    void new_param_callback(const std::string parname, const std::string value) {
      // check if the new parameter is a child of this
      if (parname.find(name()) == 0) {
        // 1/ Check if only implicit
        if (mCnfApi->getPar(parname) == NULL) {
          GAV_DUMP_N(name(), "new (enabled) param callback for implicit param '"<<parname.c_str()<<"'");
          // 2/ Create the parameter as bool
          gs_param<bool>* par = new gs_param<bool>(parname, value, true);
          enabled_params.push_back(par);
          // 3/ If param value == true: Add the related parameter (if existing) to the observed list
          if (*par == true) {
            if (mCnfApi->existsParam(parname.substr(std::string(name()).size() + 1))) {
              gs_param_base* related_param = mCnfApi->getPar(parname.substr(std::string(name()).size() + 1));
              observe(*related_param);
            }
            else {
              *par = false;
              std::stringstream ss; 
              ss << "Parameter '"; ss << parname.substr(std::string(name()).size() + 1) << "' cannot be observed because it is not existing. Enabled parameter has been resetted to false.";
              SC_REPORT_INFO(name(), ss.str().c_str());
            }
          }
          // 4/ register callback for changes to this bool param
          GC_REGISTER_PARAM_CALLBACK(par, OutputPlugin_base, enabled_param_changed_callback);
        } 
        else {
          GAV_DUMP_N(name(), "new param callback for explicit param '"<<parname.c_str()<<"'");
          // This propably has just been created by myself thus this function has been called again
          //GAV_DUMP_N(name(), "Notfied a new (enabled?) parameter which was not created by the OutputPlugin!");
          //SC_REPORT_INFO(name(), "Notfied a new (enabled?) parameter which was not created by the OutputPlugin!");
        }
      }
    }

    /// Callback function; called if one of the enabled parameter changed
    void enabled_param_changed_callback(gs_param_base &par) {
      // check if the related parameter exists
      //  if not, warning
      //  if yes, add or remove that param to/from this
      gs_param<bool>* enabled_param = par.get_gs_param<bool>();
      if (!enabled_param) {
        assert(false);
        return;
      }
      if (mCnfApi->existsParam(par.getName().substr(std::string(name()).size() + 1))) {
        gs_param_base* related_param = mCnfApi->getPar(par.getName().substr(std::string(name()).size() + 1));
        if (*enabled_param)
          observe(*related_param);
        else
          remove(*related_param);
      }
      else { // if related does not exist
        if (*enabled_param) {
          *enabled_param = false;
          GAV_DUMP_N(name(), "The related parameter '"<<par.getName().substr(std::string(name()).size() + 1).c_str()<<"' to the enabled parameter '"<<par.getName().c_str()<<"' cannot be handled because it is not existing. Enabled parameter has been resetted to false.");
          SC_REPORT_INFO(name(), "The related parameter to an enabled parameter cannot be handled because it is not existing. Enabled parameter has been resetted to false.");
        } // no else with message, otherwise two messages when disabling it automatically above
      }
    }
    
  protected:
    
    /// Init function is called before the first time the plugin outputs anything using the config_callback function.
    /**
     * This function should create needed data structures and create outputs (file, stream etc.) 
     * (as a replacement for the constructor,
     * because the constructor is called for each output plugin
     * during construction of the GAV Plugin independently if it is ever used).
     */
    virtual void init() {};
    
  public:
    
    // /////////////// BEGIN implementation of OutputPlugin_if ///////////////////// //
    
    /// implements gs::av::OutputPlugin_if::observe(gs::gs_param_base&)
    /**
     * @see gs::av::OutputPlugin_if::observe(gs::gs_param_base&)
     */
    virtual bool observe(gs_param_base& par) {
      GAV_DUMP_N(name(), "observe parameter "<<par.getName().c_str());
      // if already observed, print and return
      if (observed_param_cb_adapters.find(par.getName()) != observed_param_cb_adapters.end()) {
        SC_REPORT_INFO(name(), "Parameter already registered for output observation.");
        return false;
      }
      // register callback
      observed_param_cb_adapters.insert( std::make_pair(par.getName(), 
                                           GC_REGISTER_PARAM_CALLBACK(&par, OutputPlugin_base, internal_config_callback) )
                                           );
      // make first output call NOW
      m_currently_calling_initial_add_for_observation_callback = true;
      if (!m_constructing) internal_config_callback(par); // TODO: dirty hack
      m_currently_calling_initial_add_for_observation_callback = false;
      return true;
    }
    
    /// implements gs::av::OutputPlugin_if::observe_all
    /**
     * @see gs::av::OutputPlugin_if::observe_all
     */
    virtual void observe_all(cnf::cnf_api_if& config_api) {
      GAV_DUMP_N(name(), "observe all parameters");
      std::vector<std::string> plist = config_api.getParamList();
      gs_param_base *p;
      for (unsigned int i = 0; i < plist.size(); i++) {
        p = config_api.getPar(plist.at(i));
        if (p != NULL) 
          this->observe( *p );
      }
    }

    /// implements gs::av::OutputPlugin_if::observe(std::vector<gs::gs_param_base*>)
    /**
     * @see gs::av::OutputPlugin_if::observe(std::vector<gs::gs_param_base*>)
     */
    virtual void observe(std::vector<gs::gs_param_base*> params) {
      GAV_DUMP_N(name(), "observe parameters given by vector");
      std::vector<gs::gs_param_base*>::iterator iter;
      for (iter = params.begin(); iter !=params.end(); iter++) {
        if (*iter != NULL) 
          this->observe( **iter );
      }
    }
    
    /// implements gs::av::OutputPlugin_if::observe(std::string getParamList_string)
    /**
     * @see gs::av::OutputPlugin_if::observe(std::string getParamList_string)
     */
    virtual void observe(std::string getParamList_string) {
      GAV_DUMP_N(name(), "observe parameters of '"<<getParamList_string.c_str()<<"'");
      observe(mCnfApi->getParams(getParamList_string));
    }
  
    /// implements gs::av::OutputPlugin_if::remove
    /**
     * @see gs::av::OutputPlugin_if::remove
     */
    virtual bool remove(gs_param_base& par) {
      GAV_DUMP_N(name(), "remove observation of parameter "<<par.getName().c_str());
      cb_adapter_map_type::iterator iter = observed_param_cb_adapters.find(par.getName());
      // If map element exists
      if (iter != observed_param_cb_adapters.end()) {
        // only call unregister if param is not during destructing!!!!!
        if (!par.is_destructing()) {
          // unregister callback
          GC_UNREGISTER_CALLBACK((*iter).second);
        }
        // remove callback adapter from list
        observed_param_cb_adapters.erase(iter);
        return true;
      }
      return false;
    }
    
    /// end_of_elaboration callback to run the output plugin automatically now
    void gc_end_of_elaboration() {
      if (m_automatic_start_enabled) { 
        if (!is_running) {
          GAV_DUMP_N(name(), "GreenAV output plugin "<<get_id()<<" starting automatically at end_of_elaboration.");
          is_running = true;
        } else {
          GAV_DUMP_N(name(), "GreenAV output plugin "<<get_id()<<" has already been started.");
        }
      } else {
        GAV_DUMP_N(name(), "GreenAV output plugin "<<get_id()<<" NOT starting automatically at end_of_elaboration.");
      }
    }
    
    /// Call this to prevent the output plugin from automatically starting at end_of_elaboration
    virtual void dont_start() {
      GAV_DUMP_N(name(), "Prevent this from starting automatically");
      m_automatic_start_enabled = false;
    }
    
    /// implements gs::av::OutputPlugin_if::resume
    /**
     * @see gs::av::OutputPlugin_if::resume
     */
    virtual bool resume() {
      if (!is_running) {
        GAV_DUMP_N(name(), "start output");
        is_running = true;
      } 
      else 
        return false;
      return true;
    }
    
    /// implements gs::av::OutputPlugin_if::pause
    /**
     * @see gs::av::OutputPlugin_if::pause
     */
    virtual bool pause() {
      if (!is_running)
        return false;
      GAV_DUMP_N(name(), "pause output");
      is_running = false;
      return true;
    }
    
    /// implements gs::av::OutputPlugin_if::pause
    /**
     * @see gs::av::OutputPlugin_if::pause
     */
    virtual bool pause(sc_event& ev) {
      GAV_DUMP_N(name(), "pause output until event notified");
      // use event_listener
      ev_listener->create_event_listener(this, &OutputPlugin_base::pause_end_callback, ev);
      return pause();
    }
    
    /// implements gs::av::OutputPlugin_if::pause
    /**
     * @see gs::av::OutputPlugin_if::pause
     *
     * Use the event listener to create a listener for the internal
     * pause_event (if not yet done).
     */
    virtual bool pause(sc_time& t) {
      GAV_DUMP_N(name(), "pause output for "<<t.to_string().c_str());
      if (!pause_event_registered)
        ev_listener->create_event_listener(this, &OutputPlugin_base::pause_end_callback, pause_event);
      pause_event.cancel();
      pause_event.notify(t);
      return pause();
    }
    /// implements gs::av::OutputPlugin_if::pause
    virtual bool pause(double t, sc_time_unit u) {
      sc_time tu = sc_time(t, u);
      return pause(tu);
    }
    
    /// implements gs::av::OutputPlugin_if::running
    virtual bool running() { return is_running; }

    
    // /////////////// END implementation of OutputPlugin_if ///////////////////// //
    
    /// Callback function to be called by the event listener
    void pause_end_callback() {
      resume();
    }
    
  protected:

    /// String that was used to construct this.
    std::string constructor_param;
    
    /// If this Output Plugin has been used (was running) at least once (needed for calling init() on first usage)
    bool is_used;
    
    /// Vector of observed parameters
    cb_adapter_map_type observed_param_cb_adapters;

    /// is_running = true: callbacks are registered and output is activated;
    bool is_running;
    /// If this output plugin will automatically start at end_of_elaboration
    bool m_automatic_start_enabled;

    /// Pointer to the event listener sc_core::sc_module
    event_listener<OutputPlugin_base> *ev_listener;
    
    /// Event that is notified when pause should be stopped (start again)
    sc_event pause_event;
    /// if event is already registered in the event listener
    bool pause_event_registered;

    /// Config API
    cnf::cnf_api_if* mCnfApi;
    
    /// Vector containing pointers to the enabled (enable/disable) parameters of this
    std::vector<gs_param<bool>*> enabled_params;
    
    /// If this output plugin currently calls the callback function with a param that has been 
    bool m_currently_calling_initial_add_for_observation_callback;
    
    /// If this is currently constructing and must not call the pure virtual functions
    bool m_constructing;
  };
      
      
} // end namespace av
} // end namespace gs

#endif
