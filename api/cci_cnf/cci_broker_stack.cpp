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


#include "cci_debug.h"
#include "cci_datatypes.h"
#include "cci_broker_stack.h"

__CCI_OPEN_CONFIG_NAMESPACE__

#ifdef CCI_CNF_VERBOSE
  static void cci_debug_show_broker_stack(cci_broker_stack* stack, const char* action) {
    std::cout << " | cci_broker_stack result (after action "<< action << "):" << std::endl;
    if (stack->empty()) std::cout << " |   <empty>" << std::endl;
    std::vector<cci_cnf_broker_if*>::reverse_iterator rit;
    for ( rit=stack->rbegin() ; rit < stack->rend(); ++rit ) {
      std::cout << " |   " << (*rit)->name();
      if ((*rit)->is_private_broker()) std::cout << " (priv)";
      std::cout << std::endl;
    }
  }
  #define CCI_SHOW_BROKER_STACK(stack, action) cci::cnf::cci_debug_show_broker_stack(stack, action);
#else
  #define CCI_SHOW_BROKER_STACK(stack, action)
#endif

cci_cnf_broker_if* cci_broker_stack::top() {
  return base::back();
}

const cci_cnf_broker_if* cci_broker_stack::top() const  {
  return base::back();
}

cci_cnf_broker_if* cci_broker_stack::second_top() {
  return base::operator[](base::size()-2);
}

const cci_cnf_broker_if* cci_broker_stack::second_top() const {
  return base::operator[](base::size()-2);
}

void cci_broker_stack::push(cci_cnf_broker_if* broker) {
  base::push_back(broker);
  CCI_SHOW_BROKER_STACK(this, "push")
}

void cci_broker_stack::pop() {
  assert(!base::empty() && "Stack is empty, pop not possible");
  base::pop_back();
  CCI_SHOW_BROKER_STACK(this, "pop")
}

cci_broker_stack::cci_broker_stack(const cci_broker_stack&) {
  assert(false && "Not allowed to copy stack!");
}

cci_broker_stack& cci_broker_stack::operator=(const cci_broker_stack &originator )
{
	 assert(false && "Not allowed to copy stack!");
	 return (*this);
}

__CCI_CLOSE_CONFIG_NAMESPACE__

