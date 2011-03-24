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

#ifndef __CCI_BROKER_MANAGER_H__
#define __CCI_BROKER_MANAGER_H__


__CCI_OPEN_CONFIG_NAMESPACE__

/// Class identifying a module which provides a (private) broker
/**
 * To get access to a (possibly private) broker, use static 
 * cci_broker_manager::get_current_broker function.
 *
 * If an sc_module shall be owner of a private broker, it must derive from this class.
 * It is highly recommended to use this as a base class for only sc_modules 
 * (and modules derived from sc_modules but not as a base for non-modules). 
 * @TODO: Check if this works as a base of a non-sc_module as well.
 */
class cci_broker_manager {

public:
  
  /// Returns an accessor to the broker currently on top of broker stack
  /**
   * Returns an accessor to a private or the global broker.
   * Returns an accessor to the global broker if no broker on the stack.
   *
   * @param originator Originator the accessor shall point to
   * @return Broker (private or global) accessor
   */
  // old name: "search_for_broker"
  static cci_cnf_broker_if& get_current_broker(const cci_originator& originator);

  /// Returns an accessor to the broker currently on second top of broker stack
  /**
   * This can be used by a private broker to identify its parent's broker -
   * which is the broker it needs to forward the public actions to.
   *
   * Returns an accessor to a private or the global broker.
   * Returns an accessor to the global broker if not two brokers on the stack.
   *
   * @param originator Originator the accessor shall point to
   * @return Broker (private or global) accessor
   */
  static cci_cnf_broker_if& get_current_parent_broker(const cci_originator& originator);
  
public:
  
  /// Constructor taking an optional (private) broker instance (recommended to provide an accessor)
  /**
   * @TODO: Memory management for the private broker given to the broker manager: Is this left for the user to be solved? The broker must not be deleted as long as parameters exist using it.
   *
   * @param broker Broker this should use and return (Default=NULL)
   */
  cci_broker_manager(cci_cnf_broker_if* broker = NULL);
  
  /// Destructor
  virtual ~cci_broker_manager();

  /// Register a private broker to be used and returned by this module
  /**
   * TODO: Problem: all params created before this call did not use this broker to register themselves!
   *       So maybe don't provide this function? In that case, how shall the cci_broker_manager_module class work if it cannot get a private broker?
   */
  //virtual void register_private_broker(cci_cnf_broker_if* broker);

// Protected to protect from being called by someone casting through the SystemC hierarchy
protected:
  
  /// Returns the broker accessor being responsible for this module
  /**
   * Protected to protect from being called by someone casting through the SystemC hierarchy
   *
   * Returned broker is either an accessor to the own broker being registered manually at this manager,
   * or one from an hierarchically upwards module
   * or the top-level one.
   *
   * @return Config broker accessor (class member m_broker)
   */
  virtual cci_cnf_broker_if& get_broker() const;

// Private to prevent from modification
private:

  /// Private broker accessor pointer this manager returns (own or upwards one)
  /**
   * This broker is always valid. 
   * This is either an accessor to the own private broker
   * or an accessor to another (private or not private) broker from upwards the hierarchy.
   */
  cci_cnf_broker_if* m_broker;

  /// Private broker (possibly accessor) pointer this manager owns
  /**
   * This is the private broker specified by the user given to this manager.
   * Might be NULL if there is no own private broker!
   */
  cci_cnf_broker_if* m_own_private_broker;

};


__CCI_CLOSE_CONFIG_NAMESPACE__

#endif
