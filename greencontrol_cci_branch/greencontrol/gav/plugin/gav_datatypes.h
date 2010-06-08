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

/// doxygen comments

#ifndef __gav_datatypes_h__
#define __gav_datatypes_h__

#include <map>

namespace gs {
namespace av {
      
      
  /// AV_SERVICE command for the gs::ctr::ControlTransaction.
  /**
   * Config command to specify the command in the gs::ctr::ControlTransaction.
   */
  enum GAVCommand {
    /// No command.
    CMD_NONE=0,
    /// Adds a parameter to a OutputPlugin
    CMD_ADD_TO_OUTPUT_PLUGIN,
    /// Creates a OutputPlugin instance of the spceified type
    CMD_CREATE_OUTPUT_PLUGIN,
    /// Return an event listener for Trigger objects
    CMD_GET_EVENT_LISTENER
  };
#ifdef GAV_VERBOSE  
  static std::string gavCommandToString(unsigned int cmd) {
    switch (cmd) {
      case CMD_NONE:
        return std::string("CMD_NONE");
      case CMD_ADD_TO_OUTPUT_PLUGIN:
        return std::string("CMD_ADD_TO_OUTPUT_PLUGIN");
      case CMD_CREATE_OUTPUT_PLUGIN:
        return std::string("CMD_CREATE_OUTPUT_PLUGIN");
      case CMD_GET_EVENT_LISTENER:
        return std::string("CMD_GET_EVENT_LISTENER");
      default:
        return std::string("unknown");
    }
  }
#endif
  
  /// OutputPlugin type for transportation via ControlTransaction
  typedef unsigned OutputPluginType;

  /// Type of map plugin ID with name
  typedef std::map<OutputPluginType, std::string> OutPluginNameMap_type;

  /// Map of Output Plugins Names. Key: OutputPluginType, value: String representation of plugin type
  struct OutPluginName {
    static OutPluginNameMap_type& getMap() {
      static OutPluginNameMap_type nameMap;
      return nameMap;
    }
  };

  // Auxiliary macro to register plugin type and name
  // (use GAV_REGISTER_PLUGIN to register plugin classes)
  #define GAV_REGISTER_PLUGIN_NAME(id, alias)                                  \
    const OutputPluginType alias = id;                                         \
    namespace {                                                                \
      std::pair<OutPluginNameMap_type::iterator,bool> alias##_RegisterName =   \
         OutPluginName::getMap().insert(std::make_pair(alias,#alias));         \
    }

  // Register DEFAULT_OUT
  GAV_REGISTER_PLUGIN_NAME(0, DEFAULT_OUT)

  // Register NULL_OUT
  GAV_REGISTER_PLUGIN_NAME(1, NULL_OUT)


  static inline std::string outputPluginTypeToString(OutputPluginType type) {
    if (OutPluginName::getMap().find(type) != OutPluginName::getMap().end()) {
      return OutPluginName::getMap()[type];
    } else {
      return std::string("unknown");
    }
#ifdef GAV_VERBOSE
    if (false) gavCommandToString(0); // suppresses compiler warning that the function is never been called
#endif
  }  
    
      
} // end namespace av
} // end namespace gs

#endif
