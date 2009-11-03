//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2008 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Marcus Bartholomeu <bartho@greensocs.com>
//     GreenSocs
//     http://www.greensocs.com
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

#ifndef __ENVVAR_SUBST_H__
#define __ENVVAR_SUBST_H__

#include "greencontrol/gcnf/plugin/config_globals.h"

namespace gs {
namespace cnf {


  /// Do environment variable substitution
  /**
   * Change the given string to substitute variables in the form
   * $(FOO) from the corresponding value taken from the shell
   * environment.
   * @param str          parameter value string to search and replace variables
   * @param parname      name of the parameter (to show up in error messages)
   */
  static inline std::string envvar_subst(const std::string& str, const std::string& parname)
  {
    std::string val(str);

    // Do environment variable substitution
    size_t start_idx = 0; // position of the start token "$("
    size_t end_idx = 0;   // position of the end token ")"
    // search the start char
    while ((start_idx = val.find("$", end_idx)) != string::npos) {
      // test for ignore sequence
      if (val[start_idx + 1] == '$') {
        GCNF_DUMP_N("Variable Substitution", "param '" << parname << "': escape sequence $$ -> $");
        val.replace(start_idx, 2, "$");
        end_idx = start_idx + 1;
        continue;
      }
      // test for false sequence
      if (val[start_idx + 1] != '(') {
        end_idx = start_idx + 2;
        continue;
      }
      // search for the end sequence
      if ((end_idx = val.find(")", start_idx)) != string::npos ) {
        // extract name
        string varname(val.substr(start_idx + 2, end_idx - (start_idx + 2)));
        // check valid name
        bool valid=true;
        for (std::string::iterator i = varname.begin(); i != varname.end(); i++) {
          if (!isalnum(*i) && *i != '_') {
            std::stringstream ss;
            ss << "invalid char '" << *i << "' in variable name while defining parameter " << parname << ": " << varname << "\n";
            SC_REPORT_WARNING("Variable Substitution", ss.str().c_str());
            valid = false;
          }
        }
        if (!valid) continue; // skip replacement
        // extract value (if var not found, use "")
        string varval;
        if (char* pvarval = getenv(varname.c_str()))
          varval = pvarval;
        // replace
        GCNF_DUMP_N("Variable Substitution", "param '" << parname << "': $(" << varname << ") -> \"" << varval << "\"");
        val.replace(start_idx, end_idx + 1 - start_idx, varval);
        end_idx = start_idx + varval.length();
      } else {
        std::stringstream ss;
        ss << "unterminated variable reference while defining parameter " << parname << "\n";
        SC_REPORT_WARNING("Variable Substitution", ss.str().c_str());
      }
    }
    return val;
  }


} // end namespace cnf
} // end namespace gs

#endif
