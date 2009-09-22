/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2009 by all Contributors.
  All Rights reserved.

  Developed by GreenSocs : http://www.greensocs.com/
   Christian Schroeder, schroeder@eis.cs.tu-bs.de
   Mark Burton, mark@greensocs.com

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/


#ifndef __CCI_DATATYPES_H__
#define __CCI_DATATYPES_H__


#include <boost/cstdint.hpp>


namespace cci {

  
  /// Enumeration for basic data types, whose rough getting and setting is supported by base class directly
  enum basic_param_type {
    partype_not_available = 0, // default, type unknown
    partype_number,   // used for all param data types that can handle integer numbers
    partype_real,     // used for all param data types that can handle real numbers
    partype_bool,     // used for all param data types that can handle bool
    partype_string,   // used for all param data types that can handle strings
    partype_list,     // used for all param lists 
    partype_other     // used for all param other data types (objects etc.)
  };
  
  /// Error code enumeration for setting methods
  enum set_param_error_type {
    set_param_success,      // successfully set value
    set_param_bad_type,     // provided value is of the wrong type
    set_param_bad_value,    // provided value is not useful (out of range or illegal for other reason)
    set_param_not_settable, // this parameter is not settable TODO: supported?
    set_param_failed        // other error like memory allocations inside the called object failed or something else just went bad
  };
  
  /// Error code enumeration for getting methods
  enum get_param_error_type {
    get_param_success,       // sucessfully got value
    get_param_not_gettable,  // param not gettable TODO: supported?
    get_param_failed         // other error, e.g. failed to allocate return object
  };
  
  /// Error code enumeration for parameter adds
  enum add_param_error_type {
    add_param_success,             // parameter added successfully
    add_param_success_but_renamed, // parameter added successfully but it has been renamed automatically
    add_param_failed               // parameter not added
  };
  
  /// Error code enumeration for parameter removes
  enum remove_param_error_type {
    remove_param_successful,           // success
    remove_param_failed_not_available, // The given pointer was not in the registry (e.g. it was NULL or simply not there)
    remove_param_failed                // other error
  };
  
  // Output of the parameter objects
#ifdef CCI_PARAM_VERBOSE
# define CCI_PARAM_DUMP(msg) { std::cout<<this->name()<<": "<<msg<<std::endl; } 
# define CCI_PARAM_DUMP_WITHNAME(name, msg) { std::cout<<name<<": "<<msg<<std::endl; } 
#else
# define CCI_PARAM_DUMP(msg)
# define CCI_PARAM_DUMP_WITHNAME(name, msg)
#endif

  
} // end namespace cci

#endif
