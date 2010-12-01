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

#ifndef __ModuleTop_h__
#define __ModuleTop_h__

#include "globals.h"

#include "greencontrol/config.h"

#include "ModuleA.h"

/// Module doing nothing with private APIs: params are public
class ModuleTop
  : public sc_core::sc_module
  {
  public:
    
    SC_HAS_PROCESS(ModuleTop);
    /// Constructor
    ModuleTop(sc_core::sc_module_name name)
    : sc_core::sc_module(name),
      submod("ModuleA"),
      my_top_int_param("my_top_int_param", 11)
    {
      SC_THREAD(main_action);
      
      mApi = gs::cnf::GCnf_Api::getApiInstance(this);
    }
    
    void main_action() {
      wait (1, sc_core::SC_NS);
      // parameter that should be seen (new param callback) by the ModuleTests observer!
      gs::gs_param<int> normal_pub_runtime_param("normal_pub_runtime_param", 42);
      
      std::vector<std::string> lst = mApi->getParamList();
      std::cout << std::endl << " *** Param List of "<< name() <<" ***:" <<std::endl; 
      gs::ctr::show_vector(lst);
      std::cout << std::endl;
    }
    
  protected:
    /// Sub-Module
    ModuleA submod;

    /// Example parameter.
    gs::gs_param<int>             my_top_int_param;

    /// API got with static function
    gs::cnf::cnf_api_if *mApi;
};

#endif
