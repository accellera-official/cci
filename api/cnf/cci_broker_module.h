// LICENSETEXT
//
//   Copyright (C) 2010 : GreenSocs Ltd
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

/// Class identifying a module which provides a (private) broker
/**
 * To get access, search the SystemC hierarchy using get_parent_object()
 * and cast the sc_objects to this class
 */
class cci_broker_manager {

public:
  
  /// Constructor taking an optional broker instance
  /**
   * @param broker Broker this should use and return (Default=NULL)
   */
  cci_broker_manager(cci_cnf_broker_if* broker = NULL);
  
  /// Destructor
  virtual ~cci_broker_manager();

  /// Returns the broker being responsible for this module
  /**
   * Returned broker is either one being registered manually at this module,
   * or one from any hierarchically upwards module
   * or the top-level one
   *
   * @return Broker (config broker), is never NULL
   */
  virtual cci_cnf_broker_if* get_broker();

  /// Register a private broker to be used and returned by this module
  /**
   * TODO: Problem: all params created before this call did not use this broker to register themselves!
   *       So maybe don't provide this function? In that case, how shall the cci_broker_manager_module class work if it cannot get a private broker?
   */
  virtual void register_private_broker(cci_cnf_broker_if* broker);

public:
  
  /// Recursive search in SystemC hierarchy for a broker
  static cci_cnf_broker_if* search_for_broker(sc_core::sc_object* ob);
  
  cci_cnf_broker_if* m_broker;
  
};

/// Convenience broker manager class which is an sc_module
/**
 * @see cci_broker_manager
 */
class cci_broker_manager_module
: public sc_core::sc_module 
, public cci_broker_manager {
public:
  /// Constructor with sc_module_name
  /**
   * This constructor has _not_ a broker parameter because the owning sc_module 
   * shall be already constructed before the private broker mechanism can be 
   * used, to ensure the mechanism is allowed to use (e.g. cast) the module.
   *
   * @param name sc_module name
   * @see cci_broker_manager
   */
  cci_broker_manager_module(sc_core::sc_module_name name);
};

__CCI_CLOSE_CONFIG_NAMESPACE__

//#include "cci_broker_module.hpp"

#endif /* __CCI_BROKER_MANAGER_MODULE_H__ */
