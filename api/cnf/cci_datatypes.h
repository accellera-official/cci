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
//   This program is free software.
// 
//   If you have no applicable agreement with GreenSocs Ltd, this software
//   is licensed to you, and you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   If you have a applicable agreement with GreenSocs Ltd, the terms of that
//   agreement prevail.
// 
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
//   02110-1301  USA 
// 
// ENDLICENSETEXT



#ifndef __CCI_DATATYPES_H__
#define __CCI_DATATYPES_H__


#include <string>
#include <iostream>


__CCI_OPEN_CONFIG_NAMESPACE__

  
  /// Enumeration for basic data types, whose rough getting and setting is supported by base class directly
  enum basic_param_type {
    /// default, type unknown
    partype_not_available = 0, 
    /// used for all param data types that can handle integer numbers
    partype_number,   
    /// used for all param data types that can handle real numbers
    partype_real,     
    /// used for all param data types that can handle bool
    partype_bool,     
    /// used for all param data types that can handle strings
    partype_string,   
    /// used for all param lists 
    partype_list,     
    /// used for all param other data types (objects etc.)
    partype_other     
  };
  
  
  /// Enumeration for cci_param template specifying the parameter type according the lock behavior
  enum param_mutable_type {
    /// Mutable Parameter
    mutable_parameter = 0,
    /// Immutable Parameter
    immutable_parameter,
    /// Elaboration Time Parameter (Mutable up to end_of_elaboration)
    elaboration_time_parameter,
    /// Vendor specific/other Parameter type
    other_parameter
  };
  
  
// Debug output of the parameter objects
#ifdef CCI_PARAM_VERBOSE
# define CCI_PARAM_DUMP(msg) { std::cout<<this->name()<<": "<<msg<<std::endl; } 
# define CCI_PARAM_DUMP_WITHNAME(name, msg) { std::cout<<name<<": "<<msg<<std::endl; } 
#else
# define CCI_PARAM_DUMP(msg)
# define CCI_PARAM_DUMP_WITHNAME(name, msg)
#endif

  
__CCI_CLOSE_CONFIG_NAMESPACE__

#endif
