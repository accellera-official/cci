//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//   Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
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

// doxygen comments

#ifndef __GCNF_UTILS_H__
#define __GCNF_UTILS_H__

#include "config_globals.h"

namespace gs {
namespace cnf {

    
  /// Returns the next sc_core::sc_module in object hierarchie.
  /**
   * Tries 10 times get_parent_object() to get the 
   * next parent sc_core::sc_module.
   *
   * @param object  Object where to start
   * @return Pointer to parent sc_core::sc_module; NULL if no module
   */
  static sc_core::sc_module* get_parent_sc_module(sc_object *object) {
    sc_object *obj = object;
    sc_core::sc_module *mod = NULL;
    for(int i = 0; i<10; i++) {
      if (!obj) break;
  #if SYSTEMC_API == 210
      obj = obj->get_parent();
  #else
      obj = obj->get_parent_object();
  #endif
      mod = dynamic_cast<sc_module*>(obj);
      if (mod) break;
    }
    return mod;
  }


  /// Returns if the overgiven string is a hierarchical name.
  /**
   * Help function
   * returns if the overgiven string is a hierarchical name.
   * The SC_NAME_DELIMITER is set in the gc_globals.h file.
   *
   * @param   nam string which is checked
   * @return  If the given string is hierarchical.
   */
  static bool isHierarchicalParameterName(const std::string &nam) {
    string::size_type loc = nam.find( SC_NAME_DELIMITER, 0 );
    if( loc != string::npos ) return true;
    else return false;
  }

  /// Returns if the hierarchical name 1 is a child of the hierarchical name 2
  /**
   * Divider is the SC_NAME_DELIMITER (e.g. '.').
   *
   * @param child_name_to_check  Name 1 that is checked if it is a child of parent_name.
   * @param parent_name          Name 2 that is used for the check.
   * @return                     If child_name_to_check is a child of parent_name.
   */
  static inline bool is_child_of(const std::string& child_name_to_check, 
                                 const std::string& parent_name) {
    if (child_name_to_check.find(parent_name) == 0)
      return true;
    else return false;
  }
  
} // end namespace cnf
} // end namespace gs

#endif
