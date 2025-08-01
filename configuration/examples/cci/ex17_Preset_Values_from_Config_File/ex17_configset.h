/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 ****************************************************************************/

/**
 *  @file     ex17_configset.h
 *  @brief    This file is based on the GreenSocs GreenConfig config file parser.
 *  @author   Christian Schroeder
 *            Technical University of Braunschweig, Dept. E.I.S.
 *            http://www.eis.cs.tu-bs.de
 */
#ifndef EXAMPLES_EX17_SPECIFYING_PARAMETER_VALUES_VIA_A_CONFIGURATION_FILE_EX17_CONFIGSET_H_
#define EXAMPLES_EX17_SPECIFYING_PARAMETER_VALUES_VIA_A_CONFIGURATION_FILE_EX17_CONFIGSET_H_

#include <map>
#include <utility>
#include <string>

namespace gs {
namespace cnf {

/**
 *  @class  ex17_ConfigSet
 *  @brief  Data structure carrying a set of configuration date that can be
 *          applied to the model or a module.
 *
 *  Parsers can create and return such a ConfigSet read from a configuration
 *  file. The GCnf_Api can apply the object to the model (top-level) or a specific
 *  module.
 */
class ex17_ConfigSet {
 public:
  typedef std::map<std::string, std::string> cnf_data_map_type; ///< Type of the internal data. Should be used by user when iterating

  /**
   *  @fn     bool add(const std::string& param_name, const std::string& value)
   *  @brief  Adds a new param - value pair to the config set.
   *
   *  Overwrites existing values
   *
   *  @param  param_name  Name of the param
   *  @param  value Value (string representation) of the param
   *  @return If the the added value was a new one. (false if value has been overwritten)
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

  /**
   *  @fn     std::string get(const std::string& param_name)
   *  @brief  Returns the value of the given parameter stored in this ConfigSet
   *  @param  param_name  The name of the parameter to look up the value for.
   *  @return A string form of the parameters value.
   */
  std::string get(const std::string& param_name) const {
    cnf_data_map_type::const_iterator iter = m_cnf_data.find(param_name);
    if (iter != m_cnf_data.end()) {
      return iter->second;
    }
    SC_REPORT_INFO("ConfigSet", "get_value cannot return value of key not"
                   " existing in ConfigSet!");
    return std::string("");
  }

  /**
   *  @fn     bool has(const std::string& param_name)
   *  @brief  Returns true is the value of the given parameter is stored in this ConfigSet.
   *  @param  param_name  The name of the parameter to lookup
   *  @return True or false depending on whether the parameter value is stored here or not.
   */
  bool has(const std::string& param_name) const {
    cnf_data_map_type::const_iterator iter = m_cnf_data.find(param_name);
    if (iter != m_cnf_data.end()) {
      return true;
    }
    return false;
  }

  /**
   *  @fn     const cnf_data_map_type& get_config_map()
   *  @brief  Returns the ConfigSet map, e.g. to iterate over
   *  @return The ConfigSet map data.
   */
  const cnf_data_map_type& get_config_map() const {
    return m_cnf_data;
  }

 protected:
  cnf_data_map_type m_cnf_data; ///< actual config data map
};

}  // end namespace cnf
}  // end namespace gs
// ex17_ConfigSet

#endif  // EXAMPLES_EX17_SPECIFYING_PARAMETER_VALUES_VIA_A_CONFIGURATION_FILE_EX17_CONFIGSET_H_
