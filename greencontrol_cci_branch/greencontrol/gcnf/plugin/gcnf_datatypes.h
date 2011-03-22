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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
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
enum ConfigCommand {
  /// No command.
  CMD_NONE=0,
  /// Command: add an explicit parameter. May only be used by parameter owning API (API -> plugin).
  CMD_ADD_PARAM,
  /// Command: set the init value of a parameter (API -> plugin).
  CMD_SET_INIT_VAL,
  /// Command: lock the init value of a parameter (API -> plugin).
  CMD_LOCK_INIT_VAL,
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
  CMD_UNREGISTER_PARAM_CALLBACKS,
  /// Command: check if a parameter has ever been accessed (meaning that there was/is an object or the implicit value has been read from at least once)
  CMD_PARAM_HAS_BEEN_ACCESSED
};

#ifdef GC_VERBOSE
inline std::string gcnfCommandToString(unsigned int cmd) {
  switch (cmd) {
    case CMD_NONE:
      return std::string("CMD_NONE");
    case CMD_ADD_PARAM:
      return std::string("CMD_ADD_PARAM");
    case CMD_SET_INIT_VAL:
      return std::string("CMD_SET_INIT_VAL");
    case CMD_LOCK_INIT_VAL:
      return std::string("CMD_LOCK_INIT_VAL");
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
    case CMD_UNREGISTER_PARAM_CALLBACKS:
      return std::string("CMD_UNREGISTER_PARAM_CALLBACKS");
    case CMD_PARAM_HAS_BEEN_ACCESSED:
      return std::string("CMD_PARAM_HAS_BEEN_ACCESSED");
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
    PARTYPE_INT, // =  LONG (for 32 bit)
    PARTYPE_UINT,// = ULONG (for 32 bit)
    PARTYPE_BOOL,
    PARTYPE_DOUBLE,
    PARTYPE_FLOAT,
    PARTYPE_STRING,
    PARTYPE_ULONGLONG,
    PARTYPE_LONGLONG,
    PARTYPE_UCHAR,
    PARTYPE_USHORT,
    PARTYPE_SHORT,
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
  
  /// Namespace for enum gs::cnf::param_attributes::param_attribute_enum
  namespace param_attributes {
    /// Enumeration for parameter attributes
    enum param_attribute_enum { // When changing, please also add the string to function param_attributeToString below!
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
  } // end namespace param_attributes

  inline std::string param_attributeToString(unsigned int att) {
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
      case param_attributes::state:
        return std::string("state");
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
  
  /// Typedef for storage of param_attributes::param_attribute_enum
  typedef unsigned int param_attribute;

  /// Parameter callback types
  enum callback_type {
    /// Parameter will be read from someone, value can be updated here
    pre_read,
    /// Parameter is currently been read. Value shall not be changed within this callback!
    post_read,
    /// Parameter value shall be changed, can be rejected here with return callback_return_type::return_value_change_rejected
    reject_write,
    /// Parameter is about to be changed 
    pre_write,
    /// Parameter value has been changed 
    post_write,
    /// New (implicit or explicit) parameter created (different callback signature!)
    create_param,
    /// Parameter is in destruction
    destroy_param,
    /// legacy support type
    post_write_and_destroy,
    /// shall not be used
    no_callback
  };

  /// Callback return types
  /**
   * Override precendence: higher numbers override smaller ones when multiple callback returns are combined to one. <br>
   * Numbers are for internal use, NOT part of the API!
   */
  enum callback_return_type {
    /// Default return type, nothing special happened
    return_nothing = 0, 
    /// Rejects write, may only be used within callback_type::reject_write callback!
    return_value_change_rejected = 1, 
    /// Some not specified error (this does not change any behavior, especially does not prevent the action that has been announced by this callback)
    return_other_error = 2
  };
  
  /// returns string for callback_type
  inline std::string callback_type_to_string(callback_type cbt) {
    switch (cbt) {
      case pre_read:
        return std::string("pre_read");
      case post_read:
        return std::string("post_read");
      case reject_write:
        return std::string("reject_write");
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
