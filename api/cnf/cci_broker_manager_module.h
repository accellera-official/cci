// LICENSETEXT
//
//   Copyright (C) 2010-2011 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//    Mark Burton, mark@greensocs.com
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

#ifndef __CCI_BROKER_MANAGER_MODULE_H__
#define __CCI_BROKER_MANAGER_MODULE_H__

__CCI_OPEN_CONFIG_NAMESPACE__


/// Convenience broker manager class which is an sc_module
/**
 * @see cci_broker_manager
 */
class cci_broker_manager_module
: public sc_core::sc_module 
, public cci_broker_manager {
public:
  /// Constructor with sc_module_name and optional private broker pointer
  /**
   * @param name sc_module name
   * @param broker Broker this should use and return (Default=NULL)
   * @see cci_broker_manager
   */
  cci_broker_manager_module(sc_core::sc_module_name name, cci_cnf_broker_if* broker = NULL);
};

__CCI_CLOSE_CONFIG_NAMESPACE__

#endif
