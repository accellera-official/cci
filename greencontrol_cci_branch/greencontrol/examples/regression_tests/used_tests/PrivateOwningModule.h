//   GreenControl framework
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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// GreenConfig
#include "greencontrol/config.h" 

class PrivateOwningModule
: public sc_core::sc_module
{
public:
  PrivateOwningModule(sc_core::sc_module_name name) 
  : sc_core::sc_module(name) 
  , m_privApi(this, "pint_param1_pub", END_OF_PUBLIC_PARAM_LIST)
  , pint_param1_pub("pint_param1_pub") 
  , pint_param2("pint_param2") 
  , pint_param3("pint_param3") 
  {
    std::cout << "** set init OMP.pint_param1_pub" << std::endl;
    m_privApi.setInitValue("OMP.pint_param5_not_existing", "1005");
  }

  ~PrivateOwningModule() {
    std::cout << "** from view of private module" << std::endl;
    gs::cnf::show_database(&m_privApi);

  }
  
  /// Private API
  gs::cnf::GCnf_private_Api  m_privApi;
  
  // Public member parameter
  gs::gs_param<int> pint_param1_pub;
  // Private member parameters
  gs::gs_param<int> pint_param2;
  gs::gs_param<int> pint_param3;

};
