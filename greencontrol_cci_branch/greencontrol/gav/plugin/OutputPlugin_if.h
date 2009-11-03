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


#ifndef __OutputPlugin_if_h__
#define __OutputPlugin_if_h__


#include "greencontrol/config.h"


namespace gs {
namespace av {
  
  
  /// OutputPlugin interface.
  class OutputPlugin_if
  {
  public:

    virtual ~OutputPlugin_if() { };
    
    /// Adds a parameter which has to be observed by this Output Plugin
    /**
     * @param par  Reference to the parameter which should be observed
     * @return     If the parameter was added (true) or it was already existing (false).
     */
    virtual bool observe(gs::gs_param_base& par) =0;

    /// Lets the Output Plugin observe all (currently!) existing parameters.
    /**
     * Does not register for new param callback! 
     * TODO: extensibility for future; problem: GCnf_Api instance must be valid!
     *
     * @param config_api  Reference to a GreenConfig API where to get the 
     *                    list of all parameters.
     */
    virtual void observe_all(gs::cnf::cnf_api& config_api) =0;

    /// Lets the Output Plugin observe all parameters in the vector. E.g. use cnf_api::getParams(parent_name.*) to get such a vector.
    /**
     * Adds all parameters given in the vector to be observed by this 
     * output plugin.
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
    
  };
  

} // end namespace av
} // end namespace gs

#endif
