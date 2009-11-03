//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2008 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
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

#ifndef __OutputPluginEnabledParamsMod_h__
#define __OutputPluginEnabledParamsMod_h__

#include "test_globals.h"

#include "greencontrol/config.h"
#include "greencontrol/gav/apis/gav_api/GAV_Api.h"
// include the output plugin(s) that are needed
#include "greencontrol/gav/plugin/Stdout_OutputPlugin.h"


/// Test IP.
/**
 * Owns some parameters that are observed by output plugins and
 * enabled with their related parameters within that output plugin
 *
 * Creates an additional output plugin.
 */
class EnaMod
: public sc_core::sc_module
  {
    
  public:
    
    SC_HAS_PROCESS(EnaMod);
    
    /// Constructor
    EnaMod(sc_core::sc_module_name name)
    : sc_core::sc_module(name),
      explicitParam("ExplicitParam", 344),
      explicitParamB("ExplicitParamB", 344)
    { 
      SC_THREAD(main_action);
      mAVApi = gs::av::GAV_Api::getApiInstance();
      mCnfApi = gs::cnf::GCnf_Api::getApiInstance(this);
    }
    
    /// Destructor
    ~EnaMod() {
    }
    
    void main_action();

    boost::shared_ptr<gs::av::GAV_Api> mAVApi;
    gs::cnf::cnf_api *mCnfApi;
    
    gs::gs_param<int> explicitParam;
    gs::gs_param<int> explicitParamB;
    
  };


#endif
