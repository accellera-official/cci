//   OSCI CCI WG
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// 
// ENDLICENSETEXT


#ifndef __ObserverModule_H__
#define __ObserverModule_H__

#include <systemc>

#include "ex_globals.h"
#include "cci.h"
#include "../dummy_implementation/gs_cci.h"


/// Module which registers for parameter changes
class ObserverModule
: public sc_core::sc_module
{
public:

  SC_HAS_PROCESS(ObserverModule);
  ObserverModule(sc_core::sc_module_name name);
  
  ~ObserverModule();
  
  /// Main action to make tests with parameters.
  void main_action();

  /// Callback function with default signature.
  void config_callback(cci::cci_param_base& par);

  /// Callback function with default signature.
  void config_new_param_callback(cci::cci_param_base& par);

  // Optional callback function with default signature.
  //void config_callback_deprecated(const std::string parname, const std::string val);

protected:
  /// Pointer the the module's configuration API
  cci::cci_cnf_api* mApi;
  
  /// Vector of callbacks to ensure they can be unregistered when module is destructed
  std::vector< boost::shared_ptr<cci::callb_adapt_b> > mCallbacks;
  
};


#endif
