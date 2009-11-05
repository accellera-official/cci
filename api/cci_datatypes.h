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
#include <string>
#include <iostream>


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
