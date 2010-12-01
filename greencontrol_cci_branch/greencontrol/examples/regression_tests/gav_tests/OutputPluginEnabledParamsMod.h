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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

#ifndef __OutputPluginEnabledParamsMod_h__
#define __OutputPluginEnabledParamsMod_h__

#include "test_globals.h"

#include "greencontrol/config.h"
#include "greencontrol/gav/apis/gav_api/GAV_Api.h"
// include the output plugin(s) that are needed
#include "greencontrol/gav/plugin/Stdout_OutputPlugin.h"
#include "greencontrol/gav/plugin/File_OutputPlugin.h"


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
    gs::cnf::cnf_api_if *mCnfApi;
    
    gs::gs_param<int> explicitParam;
    gs::gs_param<int> explicitParamB;
    
  };


#endif
