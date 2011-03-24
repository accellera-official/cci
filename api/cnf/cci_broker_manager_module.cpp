// LICENSETEXT
//
//   Copyright (C) 2011 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


#include "cci.h"

__CCI_OPEN_CONFIG_NAMESPACE__


// Implementation cci_broker_manager_module

cci_broker_manager_module::cci_broker_manager_module(sc_core::sc_module_name name, cci_cnf_broker_if* broker)
: sc_core::sc_module(name)
, cci_broker_manager(broker) 
{
}


__CCI_CLOSE_CONFIG_NAMESPACE__
