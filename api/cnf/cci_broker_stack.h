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

#ifndef __CCI_BROKER_STACK_H__
#define __CCI_BROKER_STACK_H__

#include <stack>

__CCI_OPEN_CONFIG_NAMESPACE__


/// Broker stack, for internal use only
/**
 * Used by cci_broker_manager.
 * Stack for system instantiation, similar to sc_module_stack
 * Note there is a broker registry as well. @see cci::cnf::cci_broker_registry
 */
class cci_broker_stack 
: public std::vector<cci_cnf_broker_if*> {
  
protected:
  typedef std::vector<cci_cnf_broker_if*> base;
  
  /// Friend class, this shall only be used by the broker manager
  friend class cci_broker_manager;

private:
  /// Private copy constructor to prevent stack from being copied
  cci_broker_stack(const cci_broker_stack&);
  
public:

  cci_cnf_broker_if* top();
  const cci_cnf_broker_if* top() const;

  cci_cnf_broker_if* second_top();
  const cci_cnf_broker_if* second_top() const;

protected:
  
  /// Push a broker, protected to be called only by broker manager
  void push(cci_cnf_broker_if* broker);
  /// Pop the top broker, protected to be called only by broker manager
  void pop();
  
  explicit cci_broker_stack() {}

  ~cci_broker_stack() {}

  /// Internal function; returns the broker stack used during construction
  /**
   * Contains a static variable for the broker stack.
   * Used instead of a global variable to ease dynamic linking.
   * @return Reference to the broker stack
   */
  static cci_broker_stack& stack() {
    static cci_broker_stack stack;
    return stack;
  }

};


__CCI_CLOSE_CONFIG_NAMESPACE__

#endif
