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

#ifndef __ModuleAB_h__
#define __ModuleAB_h__

#include "globals.h"

#include "greencontrol/config.h"

#include "ModulePub.h"


/// Module doing nothing with private APIs and containing public Modules
class ModuleAB
  : public sc_core::sc_module
  {
  public:
    
    SC_HAS_PROCESS(ModuleAB);
    /// Constructor
    ModuleAB(sc_core::sc_module_name name)
    : sc_core::sc_module(name),
      submod("ModuleABA"),
      int_param("mAB_int_param", 1111),
      int_param2("int_param2", 2222)
    {
      SC_THREAD(main_action);
      
      mApi = gs::cnf::GCnf_Api::getApiInstance(this);
    }
    
    void main_action() {
      std::vector<std::string> lst = mApi->getParamList();
      std::cout << std::endl << " *** Param List of "<< name() <<" ***:" <<std::endl; 
      gs::ctr::show_vector(lst);
      std::cout << std::endl;
    }
    
  protected:
    /// Sub-Module
    ModulePub submod;
    
    /// Example parameter.
    gs::gs_param<int>   int_param;
    gs::gs_param<int>   int_param2;
    
    /// API got with static function
    gs::cnf::cnf_api_if *mApi;
  };

#endif
