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


#ifndef __OutputPlugin_if_h__
#define __OutputPlugin_if_h__


#include "greencontrol/config.h"


namespace gs {
namespace av {
  
  
  /// OutputPlugin interface.
  class OutputPlugin_if
  : public gs::ctr::initialize_if
  {
  public:

    OutputPlugin_if() : m_id(0) { }
    
    virtual ~OutputPlugin_if() { };
    
    /// Adds a parameter which has to be observed by this Output Plugin
    /**
     * This function calls the config_callback immediately!
     *
     * Important: When overloading this manually, remember to set  
     *            m_currently_calling_initial_observe_callback when
     *            calling the config_callback function.
     *
     * @param par               Reference to the parameter which should be observed
     * @return     If the parameter was added (true) or it was already existing (false).
     */
    virtual bool observe(gs::gs_param_base& par) =0;

    /// Lets the Output Plugin observe all (currently!) existing parameters.
    /**
     * Does not register for new param callback! 
     * TODO: extensibility for future; problem: GCnf_Api instance must be valid!
     *
     * This function calls the config_callback immediately for each added param!
     *
     * Note: This might be usefull to be used with private config APIs.
     *
     * @param config_api  Reference to a GreenConfig API where to get the 
     *                    list of all parameters.
     */
    virtual void observe_all(gs::cnf::cnf_api_if& config_api) =0;

    /// Lets the Output Plugin observe all parameters in the vector. E.g. use cnf_api::getParams(parent_name.*) to get such a vector.
    /**
     * Adds all parameters given in the vector to be observed by this 
     * output plugin.
     *
     * This function calls the config_callback immediately for each added param!
     *
     * Use example: Observe all parameters (including parameters of childs etc.)
     * underneath the hierarchy level "anyModule.subModule":
     *
     * <code>outpPlugin.observe(mCnfApi.getParams("anyModule.subModule.*"));</code>
     *
     * @param params  Vector containing the parameters that should be observed.
     */
    virtual void observe(std::vector<gs::gs_param_base*> params) =0;

    /// Lets the Output Plugin observe all parameters of the module specified by the string ("mod.*" is allowed to observe the params of the childs etc.).
    /**
     * Observe all parameters (including parameters of childs etc.)
     * underneath the hierarchy level "anyModule.subModule":
     *
     * <code>outpPlugin.observe("anyModule.subModule.*");</code>
     *
     * This function calls the config_callback immediately for each added param!
     *
     * The given string is given directly to the getParams function of the config API.
     *
     * @param getParamList_string  String to be given to the getParams function of the config API.
     */
    virtual void observe(std::string getParams_string) =0;
    
    /// Unregisters the specified parameter not to be observed any longer.
    /**
     * Note: This function uses the gs_param_base instead of the parameter
     * name because of the function is_destructing() which is needed to
     * decide whether to unregister the callback or not!
     *
     * @param par  Reference to the parameter which should not be observed any longer.
     * @return     If the parameter was observed.
     */
    virtual bool remove(gs::gs_param_base& par) =0;
    
    /// Avoids automatic resume during end_of_elaboration
    virtual void dont_start() =0; // TODO how to register for the e_o_elab cb?
    
    /// Resumes the output after pause.
    virtual bool resume() =0;
    
    /// Pauses the output until <code>resume()</code> is called. (Callbacks should be kept, not unregistered.)
    virtual bool pause() =0;
    /// Pauses the output until the given event occurs. (Callbacks should be kept, not unregistered.)
    virtual bool pause(sc_event&) =0;
    /// Pauses the output for the given time. (Callbacks should be kept, not unregistered.)
    virtual bool pause(sc_time&) =0;
    /// Pauses the output for the given time. (Callbacks should be kept, not unregistered.)
    virtual bool pause(double t, sc_time_unit u) =0;
    // if the output plugin is running (each output plugin can decide if starting paused or not!)
    virtual bool running() =0;
    
    /// initialize_if dummy
    virtual void start_initialize_mode() { }
    /// initialize_if dummy
    virtual void end_initialize_mode() { }
    // initialize_if implementation to get called on end_of_elaboration
    //virtual void gc_end_of_elaboration() { }; // is already implemented empty in interface
    
    // ////////////////////// Unique Output Plugin ID /////////////////////////
    
    /// Returns the unique Ouput Plugin ID
    unsigned int get_id() { if (m_id == 0) m_id = inc_max_id(); return m_id; }
    
  private:
    /// System-wide unique Output Plugin ID 
    unsigned int m_id;
    /// Increment and return the maximum id to identify an Output Plugin
    static unsigned int inc_max_id() {
      static unsigned int max_id = 0;
      return ++max_id;
    }
  };
  

} // end namespace av
} // end namespace gs

#endif
