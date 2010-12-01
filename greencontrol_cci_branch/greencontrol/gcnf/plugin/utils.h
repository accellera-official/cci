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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
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
  
  /// Returns the local parameter name
  /**
   * The local parameter name is the last part of the given hier_name 
   * from the last delimiter (usualy '.') on.
   *
   * @param hier_name Full hierarchical parameter name
   * @return Local parameter name
   */
  static inline std::string get_local_param_name(const std::string& hier_name) {
    std::string local_n;
    local_n = hier_name.substr( hier_name.find_last_of(SC_NAME_DELIMITER)+1, hier_name.size()-hier_name.find_last_of(SC_NAME_DELIMITER)+1 );
    return local_n;
  }

  /// Returns the parameter's hierarchy name
  /**
   * The parameter's hierarchy name is the first part of the given hier_name 
   * from the beginning to the last delimiter (usualy '.').
   *
   * @param hier_name Full hierarchical parameter name
   * @return Parameter's hierarchy name
   */
  static inline std::string get_hierarchy_param_name(const std::string& hier_name) {
    std::string local_n;
    local_n = hier_name.substr( 0, hier_name.find_last_of(SC_NAME_DELIMITER));
    return local_n;
  }
  
} // end namespace cnf
} // end namespace gs

#endif
