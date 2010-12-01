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

#ifndef __ModuleB_h__
#define __ModuleB_h__

#include "globals.h"

#include "greencontrol/config.h"


/// Module having a private API: params are private expect explicitely excluded
class ModuleB
  : public sc_core::sc_module
  {
  public:
    
    SC_HAS_PROCESS(ModuleB);
    /// Constructor
    ModuleB(sc_core::sc_module_name name)
    : sc_core::sc_module(name),
      m_privApi(this, "mB_int_param", END_OF_PUBLIC_PARAM_LIST),
      int_param("mB_int_param", 451),
      int_param2("int_param2", 452)
    {
      SC_THREAD(main_action);

      mApi = gs::cnf::GCnf_Api::getApiInstance(this);
    }
    
    void main_action();
    
  protected:
    /// Private API
    gs::cnf::GCnf_private_Api  m_privApi;
    
    /// Example parameter.
    gs::gs_param<int>   int_param;
    gs::gs_param<int>   int_param2;
    
    /// API got with static function
    gs::cnf::cnf_api_if *mApi;
  };


void ModuleB::main_action() {
  wait (1, sc_core::SC_NS);
  // parameter that must not be seen (no new param callback) by the ModuleTests observer!
  gs::gs_param<int> priv_runtime_param("priv_runtime_param", 43);

  std::vector<std::string> lst = mApi->getParamList();
  std::cout << std::endl << " *** Param List of "<< name() <<" ***:" <<std::endl; 
  gs::ctr::show_vector(lst);
  std::cout << std::endl;
}

#endif
