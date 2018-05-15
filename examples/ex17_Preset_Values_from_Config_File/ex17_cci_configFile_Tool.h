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
 *  @file     ex17_cci_configFile_Tool.h
 *  @brief    This config file parser is based on the GreenSocs GreenConfig config
 *            file parser.
 *  @author   Christian Schroeder <schroeder@eis.cs.tu-bs.de>
 *            Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>
 *            Technical University of Braunschweig, Dept. E.I.S.
 *            http://www.eis.cs.tu-bs.de
 */
#ifndef EXAMPLES_EX17_SPECIFYING_PARAMETER_VALUES_VIA_A_CONFIGURATION_FILE_EX17_CCI_CONFIGFILE_TOOL_H_
#define EXAMPLES_EX17_SPECIFYING_PARAMETER_VALUES_VIA_A_CONFIGURATION_FILE_EX17_CCI_CONFIGFILE_TOOL_H_

#include <cci_configuration>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <sstream>
#include <utility>
#include <string>
#include <vector>

#include "ex17_configset.h"

namespace cci {

#define CONFIG_FILE_TOOL_BUFSIZE 1024
#define TRACENAME "/Accellera/CCI_examples/CONFIG_FILE_TOOL"

enum gc_config_parse_result {
  parse_result_ignore = 0,
  parse_result_param
};

typedef std::pair<std::vector<std::string>, gc_config_parse_result> parseresult;  ///< Typedef for use in ConfigFile_Tool

/**
 *  @class  ex17_cci_configFile_Tool
 *  @brief  Tool which reads a configuration file and sets parameters and provides
 *          configuration of a string representing one line.
 *
 *  Config File Tool which reads a configuration file and uses the Tool_GCnf_Api
 *  to set the parameters.
 *
 *  One instance can be used to read and configure several config files.
 *
 *  Supports Simple Parameter Array setting of members and sizes individually
 *  (since there need nothing to be supported explicitely).
 *  Does NOT support the setting of all members at once (e.g. '{10 22 33}')!
 *
 *  The usage of this Tool:
 *  - instantiate one object
 *  - call <code>config(filename)</code>
 *
 *  Additional functionality: parse one line (or string) using
 *  <code>config_line(const char* line)</code>
 */
class ex17_cci_configFile_Tool {
 public:
  /**
   *  @fn     explicit ex17_cci_configFile_Tool(const chat* name)
   *  @brief  The class constructor
   *  @param  name  The name for the tool
   *  @return void
   */
  explicit ex17_cci_configFile_Tool(const char* name)
      : mToolOriginator(name),
        mApi(cci::cci_get_global_broker(mToolOriginator))
  {}

  /**
   *  @fn     ~ex17_cci_configFile_Tool()
   *  @brief  The class destructor
   *  @return void
   */
  ~ex17_cci_configFile_Tool() {}

  /**
   *  @fn     inline void config(const char* filename)
   *  @brief  Read a config file and apply configuration immediately.
   *
   *  Configure parameters from a config file and applies the configuration
   *  immediately to the model.
   *
   *  May be called several times with several configuration files
   *
   *  Example usage:
   *  \code
   *      int sc_main(int argc, char *argv[]) {
   *        cci_configFile_Tool fileTool;
   *        fileTool.config("file.cfg");
   *        fileTool.config("other_file.cfg");
   *      }
   *  \endcode
   *  @param  filename  The name of the file to read from
   *  @return void
   */
  inline void config(const char *filename) {
    gs::cnf::ex17_ConfigSet cnf_set;
    cnf_set = create_config_set_from_file(filename);

    gs::cnf::ex17_ConfigSet::cnf_data_map_type::const_iterator it;
    for (it = cnf_set.get_config_map().begin();
        it != cnf_set.get_config_map().end(); it++) {
      std::cout << "ConfigFile_Api: Applying preset value of param '"
                << it->first << "' to '" << it->second << "'" << std::endl;
      mApi.set_preset_cci_value(it->first.c_str(),
                                  cci::cci_value::from_json(
                                          it->second.c_str()));
    }
  }

  /**
   *  @fn     gs::cnf::ex17_ConfigSet create_config_set_from_file(const char* filename)
   *  @brief  Read a config file and return configuration without applying it.
   *
   *  Creates a ConfigSet from a config file WITHOUT applying the
   *  configuration to the model!
   *
   *  May be called several times with several configuration files
   *
   *  Example usage:
   *  \code
   *     int sc_main(int argc, char *argv[]) {
   *       ConfigFile_Api fileApi;
   *       ConfigSet cnfset  =
   *           ConfigFile_Api.create_config_set_from_file("file.cfg");
   *       ConfigSet cnfset2 =
   *           ConfigFile_Api.create_config_set_from_file("other_file.cfg");
   *       TODO
   *     }
   *  \endcode
   *  @param  filename  The name of the file to open.
   *  @return The configuration.
   */
  gs::cnf::ex17_ConfigSet create_config_set_from_file(const char *filename) {
    gs::cnf::ex17_ConfigSet config_set;
    gs::cnf::ex17_ConfigSet macro_set;

    unsigned ln = 0;

    std::ifstream is(filename);
    if (!is) {
      std::stringstream ss;
      ss << "Cannot open config file '" << filename << "'\n";
      SC_REPORT_WARNING(m_name.c_str(), ss.str().c_str());
      return config_set;
    }

    std::pair<std::string, std::string> cnf_line_data;
    while (is) {
      // line is a static variable
      is.getline(line, CONFIG_FILE_TOOL_BUFSIZE, '\n');
      ln++;

      cnf_line_data = interpret_config_line(line);

      // if parsing succeeded
      if (cnf_line_data.first.length() > 0) {
        // if this line is a macro definition
        if (cnf_line_data.second[0] == '=') {
          if (cnf_line_data.first.find_first_not_of(
              "ABCDEFGHIJKLMNOPQRSTUVWXYZ_") == std::string::npos) {
            std::string macro_val = cnf_line_data.second.substr(1);
            boost::algorithm::trim(macro_val);
            macro_set.add(cnf_line_data.first, macro_val);
          } else {
            std::stringstream ss;
            ss << "In config file line " << ln << ": Parsing failed [" << line
               << "]\n";
            SC_REPORT_WARNING(TRACENAME, ss.str().c_str());
          }
        } else {
          // add the configuration to set
          // if value is no macro
          if (!macro_set.has(cnf_line_data.second)) {
            config_set.add(cnf_line_data.first, cnf_line_data.second);
          } else {
            // if value is a macro
            config_set.add(cnf_line_data.first,
                           macro_set.get(cnf_line_data.second));
          }
        }
      // if parsing failed in non-empty line
      } else if (cnf_line_data.second.length() > 0) {
        std::stringstream ss;
        ss << "In config file line " << ln << ": Parsing failed [" << line
           << "]\n";
        SC_REPORT_WARNING(TRACENAME, ss.str().c_str());
      }
    }

    return config_set;
  }

 protected:
  /**
   *  @fn     static parseresult parse(const std::string& token_)
   *  @brief  Parse function to be used by ConfigFile_Tool
   *  @param  token_  A token.
   *  @return A classification of the given token.
   */
  static parseresult parse(const std::string &token_) {
    std::string::size_type idx;
    std::vector<std::string> tokenlist;
    std::string token(token_);

    // test for empty token --> ignore
    if (token == "") {
      return parseresult(tokenlist, parse_result_ignore);
    }

    // test for comment --> remove
    idx = token.find("#");
    if (idx != std::string::npos) {
      token = token.substr(0, idx);
    }

    // test for tabs and spaces and create token list
    while ((idx = token.find_first_of(" \t")) != std::string::npos) {
      if (idx > 0) {  // is a word
        tokenlist.push_back(token.substr(0, idx));
      }
      token = token.substr(idx + 1);
    }

    if (token.size() > 0) {
      tokenlist.push_back(token);
    }

    if (tokenlist.empty()) {
      return make_pair(tokenlist, parse_result_ignore);
    }

    if (tokenlist.size() == 1) {
      return make_pair(tokenlist, parse_result_ignore);
    } else {
      return make_pair(tokenlist, parse_result_param);
    }

    return make_pair(tokenlist, parse_result_ignore);
  }

  /**
   *  @fn     inline std::pair<std::string, std::string> interpret_config_line(const char* line)
   *  @brief  Interprest one input line (string) and returns the pair<param_name, value>
   *
   *  The return pair has a special code for success/error submission:<br>
   *   if (param_name == "" AND value == ""): empty line,<br>
   *   if (param_name == "" AND value != ""): parsing failed!
   *
   *  @param  line  Line to parse
   *  @return pair< param_name, value >,<br> if param_name == "" AND value == "":
   *     empty line,<br> if param_name == "" AND value != "": parsing failed!
   */
  inline std::pair<std::string, std::string>
      interpret_config_line(const char *line) {
    std::string::size_type lidx;
    std::string lstr(line);
    std::pair<std::string, std::string> config_data;

    std::string separators;
    separators += " ";
    separators += static_cast<char>(9);

    if ((lidx = lstr.find("#")) != std::string::npos) {
      lstr = lstr.substr(0, lidx);
    }
    boost::algorithm::trim_left(lstr);

    if (!lstr.empty()) {
      parseresult p = parse(lstr);

      switch (p.second) {
        case parse_result_param:  // set param value
        {
          std::string tok(lstr);
          size_t pos = tok.find(p.first[0]);
          std::string val = tok.substr(pos, tok.length() - pos);
          val = val.substr(val.find_first_of(separators) + 1);
          boost::algorithm::trim(val);
          std::string parname = p.first[0];

          // package the configuration
          config_data.first = parname;
          config_data.second = val;
          return config_data;
        }
          break;

        case parse_result_ignore:
          config_data.second = "parsing failed";
          break;

        default:
          config_data.second = "parsing failed";
          break;
      }
    }
    // empty config_data = failure (no config data in line)
    return config_data;
  }

 protected:
  cci::cci_originator mToolOriginator; ///< This tool's originator information

  cci::cci_broker_handle mApi;  ///< Config API which is used by this tool

  std::string m_name; ///< Name of this object, given by constructor

  char line[CONFIG_FILE_TOOL_BUFSIZE];
};

}  // end namespace cci
// ex17_cci_configFile_Tool

#endif  // EXAMPLES_EX17_SPECIFYING_PARAMETER_VALUES_VIA_A_CONFIGURATION_FILE_EX17_CCI_CONFIGFILE_TOOL_H_
