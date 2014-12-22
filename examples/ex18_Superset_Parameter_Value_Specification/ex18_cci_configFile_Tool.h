/*******************************************************************************
 * The following code is derived, directly or indirectly, from the SystemC
 * source code Copyright (c) 1996-2010 by all Contributors.
 * All Rights reserved.
 * 
 * The contents of this file are subject to the restrictions and limitations
 * set forth in the SystemC Open Source License Version 2.2.0 (the "License");
 * One may not use this file except in compliance with such restrictions and
 * limitations.  One may obtain instructions on how to receive a copy of the
 * License at http://www.systemc.org/.  Software distributed by Contributors
 * under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
 * ANY KIND, either express or implied. See the License for the specific
 * language governing rights and limitations under the License.
 * *******************************************************************************/

/**
 * This config file parser is based on the GreenSocs GreenConfig config file parser.
 *
 *   GreenSocs Ltd
 *          http://www.greensocs.com/ , email: info@greensocs.com
 *
 *   Developed by :
 *
 *   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
 *   Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>
 *   Technical University of Braunschweig, Dept. E.I.S.
 *   http://www.eis.cs.tu-bs.de
**/

#ifndef EXAMPLES_EX18_SUPERSET_PARAMETER_VALUE_SPECIFICATION_EX18_CCI_CONFIGFILE_TOOL_H_
#define EXAMPLES_EX18_SUPERSET_PARAMETER_VALUE_SPECIFICATION_EX18_CCI_CONFIGFILE_TOOL_H_

#include <cci>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <sstream>
#include <utility>
#include <string>
#include <vector>

#include "ex18_configset.h"

namespace cci {
namespace cnf {

#define CONFIG_FILE_TOOL_BUFSIZE 1024
#define TRACENAME "/ASI/CCI/CONFIG_FILE_TOOL"

enum gc_config_parse_result {
  parse_result_ignore = 0,
  parse_result_param
};
/// Typedef for use in ConfigFile_Tool
typedef std::pair<std::vector<std::string>, gc_config_parse_result> parseresult;

/// Tool which reads a configuration file and sets parameters and provides
/// configuration of a string representing one line.
/**
 * Config File Tool which reads a configuration file and uses the Tool_GCnf_Api
 * to set the parameters.
 *
 * One instance can be used to read and configure several config files.
 *
 * Supports Simple Parameter Array setting of members and sizes individually
 * (since there need nothing to be supported explicitely).
 * Does NOT support the setting of all members at once (e.g. '{10 22 33}')!
 *
 * The usage of this Tool:
 * - instantiate one object
 * - call <code>config(filename)</code>
 *
 * Additional functionality: parse one line (or string) using
 * <code>config_line(const char* line)</code>
 */
class ex18_cci_configFile_Tool {
 public:
  explicit ex18_cci_configFile_Tool(const char* name)
      : mToolOriginator(name) {
    // get responsible broker
    mApi = &cci::cnf::cci_broker_manager::get_current_broker(mToolOriginator);
  }

  ~ex18_cci_configFile_Tool() {}

  /// Read a config file and apply configuration immediately
  /**
   * Configure parameters from a config file and applies the configuration
   * immediately to the model.
   *
   * May be called several times with several configuration files
   *
   * Example usage:
   * \code
   *    int sc_main(int argc, char *argv[]) {
   *      cci_configFile_Tool fileTool;
   *      fileTool.config("file.cfg");
   *      fileTool.config("other_file.cfg");
   *    }
   * \endcode
   */
  inline void config(const char *filename) {
    gs::cnf::ex18_ConfigSet cnf_set;
    cnf_set = create_config_set_from_file(filename);

    gs::cnf::ex18_ConfigSet::cnf_data_map_type::const_iterator it;
    for (it = cnf_set.get_config_map().begin();
         it != cnf_set.get_config_map().end(); it++) {
      std::cout << "ConfigFile_Api: Applying initial value of param '"
                << it->first << "' to '" << it->second << "'" << std::endl;
      mApi->json_deserialize_initial_value(it->first.c_str(),
                                           it->second.c_str());
    }
  }

  /// Read a config file and return configuration without applying it
  /**
   * Creates a ConfigSet from a config file WITHOUT applying the 
   * configuration to the model!
   *
   * May be called several times with several configuration files
   *
   * Example usage:
   * \code
   *    int sc_main(int argc, char *argv[]) {
   *      ConfigFile_Api fileApi;
   *      ConfigSet cnfset  =
   *          ConfigFile_Api.create_config_set_from_file("file.cfg");
   *      ConfigSet cnfset2 =
   *          ConfigFile_Api.create_config_set_from_file("other_file.cfg");
   *      TODO
   *    }
   * \endcode
   */
  gs::cnf::ex18_ConfigSet create_config_set_from_file(const char *filename) {
    gs::cnf::ex18_ConfigSet config_set;
    gs::cnf::ex18_ConfigSet macro_set;

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
          if (!macro_set.has(cnf_line_data.second))  // if value is no macro
            config_set.add(cnf_line_data.first, cnf_line_data.second);
          else
            // if value is a macro
            config_set.add(cnf_line_data.first,
                           macro_set.get(cnf_line_data.second));
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
  /// parse function to be used by ConfigFile_Tool
  /**
   * Classify a token.
   * @param token_ A token.
   * @return A classification of the given token.
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

  /// Interprets one input line (string) and returns the pair <param_name,value>
  /**
   * The return pair has a special code for success/error submission:<br>
   *  if (param_name == "" AND value == ""): empty line,<br>
   *  if (param_name == "" AND value != ""): parsing failed!
   *
   * @param line  Line to parse
   * @return pair<param_name, value>,<br> if param_name == "" AND value == "":
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
          if (val.find_first_of('"') != std::string::npos
              && val.find_first_of('"') != val.find_last_of('"')) {
            val = val.substr(val.find_first_of('"') + 1,
                             val.find_last_of('"')
                             - val.find_first_of('"') - 1);
          }
          std::string::size_type loc = 0;
          while ((loc = val.find("\\\"", loc)) != std::string::npos) {
            val.replace(loc, 2, "\"");
          }
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
  /// This tool's originator information
  cci::cnf::cci_originator mToolOriginator;

  /// Config API which is used by this tool
  cci::cnf::cci_cnf_broker_if* mApi;

  /// Name of this object, given by constructor
  std::string m_name;

  char line[CONFIG_FILE_TOOL_BUFSIZE];
};
}  // end namespace cnf
}  // end namespace cci
/// ex18_cci_configFile_Tool

#endif  // EXAMPLES_EX18_SUPERSET_PARAMETER_VALUE_SPECIFICATION_EX18_CCI_CONFIGFILE_TOOL_H_
