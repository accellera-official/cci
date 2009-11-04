//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007-2009 : GreenSocs Ltd
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

/// doxygen comments

#ifndef __GCNF_DATATYPES_H__
#define __GCNF_DATATYPES_H__


namespace gs {
namespace cnf {


/// CONFIG_SERVICE command for the gs::ctr::ControlTransaction.
/**
 * Config command to specify the command in the gs::ctr::ControlTransaction.
 */
enum GCfgCommand {
  /// No command.
  CMD_NONE=0,
  /// Command: add an explicit parameter. May only be used by parameter owning API (API -> plugin).
  CMD_ADD_PARAM,
  /// Command: set the init value of a parameter (API -> plugin).
  CMD_SET_INIT_VAL,
  /// Command: get the value of a parameter (API -> plugin).
  CMD_GET_VAL,
  /// Command: get the pointer to a parameter (API -> plugin).
  CMD_GET_PARAM,
  /// Command: check if the parameter exists (API -> plugin).
  CMD_EXISTS_PARAM,
  /// Command: DEPRECTATED: get a list of the parameters (API -> plugin), use CMD_GET_PARAM_LIST_VEC instead.
  CMD_GET_PARAM_LIST,
  /// Command: get a vector of the existing parameters (API -> plugin).
  CMD_GET_PARAM_LIST_VEC,
  /// Command: register observer for a parameter (API -> plugin).
  //CMD_REGISTER_PARAM_OBSERVER,
  /// Command: notify an observer and submit new value (Plugin -> API).
  //CMD_NOTIFY_OBSERVER,
  /// Command: register observer for new added or new set parameters (API -> plugin)
  CMD_REGISTER_NEW_PARAM_OBSERVER,
  /// Command: notify an observer about new added (or without add first time set) parameters (Plugin -> API)
  CMD_NOTIFY_NEW_PARAM_OBSERVER,
  /// Command: remove parameter from plugin (API -> plugin)
  CMD_REMOVE_PARAM,
  /// Command: unregister all parameter callbacks for the specified observer module (API -> plugin)
  CMD_UNREGISTER_PARAM_CALLBACKS
};

#ifdef GC_VERBOSE
static inline std::string gcnfCommandToString(unsigned int cmd) {
  switch (cmd) {
    case CMD_NONE:
      return std::string("CMD_NONE");
    case CMD_ADD_PARAM:
      return std::string("CMD_ADD_PARAM");
    case CMD_SET_INIT_VAL:
      return std::string("CMD_SET_INIT_VAL");
    case CMD_GET_PARAM:
      return std::string("CMD_GET_PARAM");
    case CMD_EXISTS_PARAM:
      return std::string("CMD_EXISTS_PARAM");
    case CMD_GET_PARAM_LIST:
      return std::string("CMD_GET_PARAM_LIST");
    //case CMD_REGISTER_PARAM_OBSERVER:
    //  return std::string("CMD_REGISTER_PARAM_OBSERVER");
    case CMD_REGISTER_NEW_PARAM_OBSERVER:
      return std::string("CMD_REGISTER_NEW_PARAM_OBSERVER");
    case CMD_NOTIFY_NEW_PARAM_OBSERVER:
      return std::string("CMD_NOTIFY_NEW_PARAM_OBSERVER");
    case CMD_REMOVE_PARAM:
      return std::string("CMD_REMOVE_PARAM");
    default:
      return std::string("unknown");
  }
}
#endif

  /// Enumeration for natively supported data types, e.g. for cast switch cases, e.g. in StatisticsCalculator
  enum Param_type {
    // default, used when not specified
    PARTYPE_NOT_AVAILABLE = 0,
    // POD
    PARTYPE_INT,
    PARTYPE_UINT,
    PARTYPE_BOOL,
    PARTYPE_DOUBLE,
    PARTYPE_FLOAT,
    PARTYPE_STRING,
    PARTYPE_ULONGLONG,
    PARTYPE_LONGLONG,
    PARTYPE_UCHAR,
    PARTYPE_USHORT,
    PARTYPE_CHAR,
    PARTYPE_SIGNED_CHAR,
    // SystemC
    PARTYPE_SC_BIT,
    PARTYPE_SC_INT_BASE,
    PARTYPE_SC_INT,
    PARTYPE_SC_UINT_BASE,
    PARTYPE_SC_UINT,
    PARTYPE_SC_SIGNED,
    PARTYPE_SC_BIGINT,
    PARTYPE_SC_UNSIGNED,
    PARTYPE_SC_BIGUINT,
    PARTYPE_SC_LOGIC,
    PARTYPE_SC_TIME,
    PARTYPE_SC_ATTRIBUTE,
    PARTYPE_SC_EVENT,
    // Arrays
    PARTYPE_SMPL_ARRAY,
    PARTYPE_EXT_ARRAY,
    // Last
    LAST_PARTYPE
  };
  
  namespace param_attributes {
    /// Enumeration for parameter attributes
    enum param_attribute_enum {
      undefined = 0,
      config,
      runtime_config,
      elab_config,
      read_only,
      analysis,
      temp,
      output,
      internal,
      state,
      gr_attribute,
      gr_register,
      gr_splitio_register,
      gr_sharedio_register,
      gr_bit_range,
      last_attribute
    };
  } // end namespace param_attribute

  static inline std::string param_attributeToString(unsigned int att) {
    switch (att) {
      case param_attributes::undefined:
        return std::string("undefined");
      case param_attributes::config:
        return std::string("config");
      case param_attributes::runtime_config:
        return std::string("runtime_config");
      case param_attributes::elab_config:
        return std::string("elab_config");
      case param_attributes::read_only:
        return std::string("read_only");
      case param_attributes::analysis:
        return std::string("analysis");
      case param_attributes::temp:
        return std::string("temp");
      case param_attributes::output:
        return std::string("output");
      case param_attributes::internal:
        return std::string("internal");
      case param_attributes::gr_attribute:
        return std::string("gr_attribute");
      case param_attributes::gr_register:
        return std::string("gr_register");
      case param_attributes::gr_splitio_register:
        return std::string("gr_splitio_register");
      case param_attributes::gr_sharedio_register:
        return std::string("gr_sharedio_register");
      case param_attributes::gr_bit_range:
        return std::string("gr_bit_range");
      default:
        return std::string("_unknown_");
    }
  }
  
  typedef unsigned int param_attribute;

  /// Parameter callback types
  enum callback_type {
    pre_read,
    post_read,
    pre_write,
    post_write,
    create_param,
    destroy_param,
    post_write_and_destroy,
    no_callback
  };

  /// Callback return types
  /**
   * Override precendence: higher numbers override smaller ones when multiple callback returns are combined to one. <br>
   * Numbers are for internal use, NOT part of the API!
   */
  enum callback_return_type {
    return_nothing = 0, 
    return_value_change_rejected = 1, // may only be used in pre_write callbacks
    return_other_error = 2
  };
  
  /// returns string for callback_type
  static inline std::string callback_type_to_string(callback_type cbt) {
    switch (cbt) {
      case pre_read:
        return std::string("pre_read");
      case post_read:
        return std::string("post_read");
      case pre_write:
        return std::string("pre_write");
      case post_write:
        return std::string("post_write");
      case create_param:
        return std::string("create_param");
      case destroy_param:
        return std::string("destroy_param");
      case post_write_and_destroy:
        return std::string("post_write_and_destroy");
      default:
        return std::string("_unknown_");
    }
  }
  
} // end namespace cnf
} // end namespace gs

#endif
