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

#ifndef __CCI_BROKER_REGISTRY_H__
#define __CCI_BROKER_REGISTRY_H__


__CCI_OPEN_CONFIG_NAMESPACE__


/// Registry for all brokers, for internal use only
/**
 * This stores pure brokers, no accessors!
 * Note this is NOT the broker stack! @see cci::cnf::cci_broker_stack
 * Used by cci_broker_manager
 * @TODO Take care for memory management!
 */
class cci_broker_registry {
  
  /// Friend class, this shall only be used by the broker manager
  friend class cci_broker_manager;
  
  /// Allow the implementation to add the global broker to the registry
  friend cci_cnf_broker_if& create_global_cnf_broker();

public:

  void insert(cci_cnf_broker_if& broker);
  void remove(cci_cnf_broker_if& broker);
  int size() const;

protected:
  
  explicit cci_broker_registry();

  ~cci_broker_registry();

  /// Instantiates and returns the broker registry
  /**
   * Access only for friend class (broker manager)
   */
  static cci_broker_registry& registry();
  
private:
  std::vector<cci_cnf_broker_if*> m_broker_vec;

};


__CCI_CLOSE_CONFIG_NAMESPACE__

#endif
