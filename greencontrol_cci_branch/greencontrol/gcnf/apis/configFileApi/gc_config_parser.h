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

#ifndef __GC_CONFIG_PARSER_h__
#define __GC_CONFIG_PARSER_h__

#undef TRACENAME
#define TRACENAME "GC_CONFIG_PARSER"

/**
 * \file gc_config_parser.h A simple config file parser for GreenBus models.
 * (C) 2007 Wolfgang Klingauf, TU Braunschweig, E.I.S.
 *     modified for GreenControl Config by CS
 */

namespace gs {
namespace cnf {


  enum gc_config_parse_result {
    parse_result_ignore = 0,
    parse_result_param
  };

#define BUFSIZE 1024
  static char line[BUFSIZE];

  /// Typedef for use in ConfigFile_Tool
  typedef std::pair<std::vector<std::string>, gc_config_parse_result> parseresult;


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
    if (token == "")
      return parseresult(tokenlist, parse_result_ignore);

    // test for comment --> remove 
    idx = token.find("#"); 
    if (idx != std::string::npos) {
      token = token.substr(0, idx);
    }
     
    //cout << "full token: '"<< token.substr(0,idx) << "'"<< endl<< flush;

    // test for tabs and spaces and create token list
    while((idx = token.find_first_of(" \t")) != std::string::npos) {
      if (idx > 0) { // is a word
        //cout << "token: '"<< token.substr(0,idx) <<"'"<< endl<< flush;
        tokenlist.push_back(token.substr(0,idx));
      }    
      token = token.substr(idx+1);
    }
    
    if (token.size() > 0) {
      tokenlist.push_back(token);
    }  

    //cout << "tokenlist.size()=" << tokenlist.size() <<endl<<flush;

    if (tokenlist.empty())
      return make_pair(tokenlist, parse_result_ignore);

    if (tokenlist.size() == 1) {
      //sc_assert(false);
      return make_pair(tokenlist, parse_result_ignore);
    } else {
      return make_pair(tokenlist, parse_result_param);
    }

    return make_pair(tokenlist, parse_result_ignore);
  }


} // end namespace cnf
} // end namespace gs

#endif
