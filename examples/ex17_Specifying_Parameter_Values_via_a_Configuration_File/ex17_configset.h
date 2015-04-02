/*****************************************************************************
  Copyright 2006-2014 Accellera Systems Initiative Inc.
  All rights reserved.

  Copyright 2010-2015 CircuitSutra Technologies Pvt. Ltd.
  All rights reserved.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 *****************************************************************************/

// This file is based on the GreenSocs GreenConfig config file parser.
//
//   GreenSocs Ltd
//       http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//
//   Christian Schroeder
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//

#ifndef EXAMPLES_EX17_SPECIFYING_PARAMETER_VALUES_VIA_A_CONFIGURATION_FILE_EX17_CONFIGSET_H_
#define EXAMPLES_EX17_SPECIFYING_PARAMETER_VALUES_VIA_A_CONFIGURATION_FILE_EX17_CONFIGSET_H_

#include <map>
#include <utility>
#include <string>

namespace gs {
namespace cnf {

/// Data structure carrying a set of configuration data that can be applied
/// to the model or a module
/**
 * Parsers can create and return such a ConfigSet read from a configuration
 * file.
 * The GCnf_Api can apply the object to the model (top-level) or a specific
 * module.
 */
class ex17_ConfigSet {
 public:
  /// Type of the internal data. Should be used by user when iterating
  typedef std::map<std::string, std::string> cnf_data_map_type;

  /// Adds a new param - value pair to the config set
  /**
   * Overwrites existing values
   *
   * @param param_name  Name of the param
   * @param value       Value (string representation) of the param
   * @return        If the the added value was a new one. (false if value has been overwritten)
   */
  bool add(const std::string& param_name, const std::string& value) {
    std::pair<cnf_data_map_type::iterator, bool> ret;
    ret = m_cnf_data.insert(std::pair<std::string,
                            std::string>(param_name, value));
    if (!ret.second) {
      // erase previous value
      m_cnf_data.erase(ret.first);
      // add new value
      m_cnf_data.insert(std::pair<std::string, std::string>(param_name, value));
    }
    return ret.second;
  }

  /// Returns the value of the given parameter stored in this ConfigSet
  std::string get(const std::string& param_name) const {
    cnf_data_map_type::const_iterator iter = m_cnf_data.find(param_name);
    if (iter != m_cnf_data.end()) {
      return iter->second;
    }
    SC_REPORT_INFO("ConfigSet", "get_value cannot return value of key not"
                   " existing in ConfigSet!");
    return std::string("");
  }

  /// Returns IF the value of the given parameter stored in this ConfigSet
  bool has(const std::string& param_name) const {
    cnf_data_map_type::const_iterator iter = m_cnf_data.find(param_name);
    if (iter != m_cnf_data.end()) {
      return true;
    }
    return false;
  }

  /// Returns the ConfigSet map, e.g. to iterate over
  const cnf_data_map_type& get_config_map() const {
    return m_cnf_data;
  }

 protected:
  /// actual config data map
  cnf_data_map_type m_cnf_data;
};

}  // end namespace cnf
}  // end namespace gs

#endif  // EXAMPLES_EX17_SPECIFYING_PARAMETER_VALUES_VIA_A_CONFIGURATION_FILE_EX17_CONFIGSET_H_
