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
    
#ifndef __CCI_CONFIGFILE_TOOL_H__
#define __CCI_CONFIGFILE_TOOL_H__

// if this should use the unix getopt function for the parsing of the command line options
// or (if not) the boost program_options should be used (don't forget to link the lib boost_program_options!)
// default: do NOT define this!
// #define USE_GETOPT

//#define ENABLE_SHORT_COMMAND_LINE_OPTIONS  // enables the short synonyms for the gs_ options

#include <cci.h>

#include <sstream>
#include "greencontrol/gcnf/plugin/config_globals.h" // may define USE_GETOPT

#ifdef USE_GETOPT
# include <getopt.h>
# ifdef ARGC_COPY_SIZE
#   undef ARGC_COPY_SIZE
# endif
# define ARGC_COPY_SIZE 20
#else
# include <boost/program_options.hpp>
#endif

#include <boost/algorithm/string.hpp> // for trim
#include <boost/tokenizer.hpp> // for array parsing


#include "greencontrol/core/gc_globals.h"        // global settings
#include "greencontrol/gcnf/plugin/config_globals.h"

#include "gc_config_parser.h"
#include "greencontrol/gcnf/plugin/configset.h"


#ifndef USE_GETOPT
  namespace po = boost::program_options;
#endif
  
  
/// Tool which reads a configuration file and sets parameters and provides configuration of a string representing one line.
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
 * - or parse the command line options: '--gs_configfile <filename>' or '-c <filename>'
 *
 * Additional functionality: parse one line (or string) using <code>config_line(const char* line)</code>
 */
class cci_configFile_Tool :public sc_module
//class ConfigFile_Tool : public sc_module
{
  
public:

  /// Constructor
  //cci_configFile_Tool(const char* name)
	//SC_CTOR(ConfigFile_Tool)
	SC_CTOR(cci_configFile_Tool)
  { 
    mApi = &cci::cnf::cci_broker_manager::get_current_broker(cci::cnf::cci_originator(*this)); // Use default config API
  }
  
  /// Destructor
  //~ConfigFile_Tool() {  }
  ~cci_configFile_Tool() {  }

  /// Read the file and apply configuration for each line immediately
  /**
   * Configure parameters from a config file and applies the configuration
   * immediately to the model.
   *
   * May be called several times with several configuration files
   *
   * Example usage:
   * \code
   *    int sc_main(int argc, char *argv[]) {
   *      ConfigFile_Api fileApi;
   *      fileApi.config("file.cfg");
   *      fileApi.config("other_file.cfg");
   *    }
   * \endcode
   */
  inline void config(const char *filename) {

    gs::cnf::ConfigSet cnf_set;
    cnf_set = create_config_set_from_file(filename);
    
    GCNF_DUMP_N(m_name.c_str(), "Apply config set read from file '"<<filename<<"'");
     gs::cnf::ConfigSet::cnf_data_map_type::const_iterator it;
    for ( it=cnf_set.get_config_map().begin() ; it != cnf_set.get_config_map().end(); it++ ) {
      GCNF_DUMP_N(m_name.c_str(), "Applying value of param '"<<it->first<<"' to '"<<it->second<<"'");
      std::cout << "ConfigFile_Api: Applying value of param '"<<it->first<<"' to '"<<it->second<<"'"<<std::endl;
      mApi->set_init_value(it->first.c_str(), it->second.c_str());
    }
    
  }
  
  /// Read the file and apply configuration for each line immediately
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
   *      ConfigSet cnfset  = ConfigFile_Api.create_config_set_from_file("file.cfg");
   *      ConfigSet cnfset2 = ConfigFile_Api.create_config_set_from_file("other_file.cfg");
   *      TODO
   *    }
   * \endcode
   */
  gs::cnf::ConfigSet create_config_set_from_file(const char *filename) {
    gs::cnf::ConfigSet config_set;
    gs::cnf::ConfigSet macro_set;
    
    GCNF_DUMP_N(m_name.c_str(), "Read config file '"<<filename<<"'");
    unsigned ln = 0;
    
    std::ifstream is(filename);
    if (!is) {
      std::stringstream ss;
      ss << "Cannot open config file '"<<filename<<"'\n";
      SC_REPORT_WARNING(m_name.c_str(), ss.str().c_str());
      return config_set;
    }
    
    std::pair<std::string, std::string> cnf_line_data;
    while(is) {
      is.getline(line, BUFSIZE, '\n'); // line is a static variable
      ln++;
      
      cnf_line_data = interpret_config_line(line);
      
      // if parsing succeeded
      if (cnf_line_data.first.length() > 0) {
        GCNF_DUMP_N(m_name.c_str(), "ConfigFile_Api: Adding pair param '"<<cnf_line_data.first.c_str()<<"', '"<< cnf_line_data.second.c_str() <<"' to ConfigSet");
        if (cnf_line_data.second[0] == '=') { // if this line is a macro definition
          if (cnf_line_data.first.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ_") == std::string::npos) {
            std::string macro_val = cnf_line_data.second.substr(1);
            boost::algorithm::trim(macro_val);
            macro_set.add(cnf_line_data.first, macro_val); 
          }
          else {
            std::stringstream ss;
            ss << "In config file line "<< ln <<": Parsing failed ["<<line<<"]\n";
            SC_REPORT_WARNING(TRACENAME, ss.str().c_str());
          }
        }  
        else {
          // add the configuration to set
          if (!macro_set.has(cnf_line_data.second)) // if value is no macro
            config_set.add(cnf_line_data.first, cnf_line_data.second);
          else // if value is a macro
            config_set.add(cnf_line_data.first, macro_set.get(cnf_line_data.second));
        }
      }
      // if parsing failed in non-empty line
      else if (cnf_line_data.second.length() > 0) {
        std::stringstream ss;
        ss << "In config file line "<< ln <<": Parsing failed ["<<line<<"]\n";
        SC_REPORT_WARNING(TRACENAME, ss.str().c_str());
      }
      //else if empty line

    } // end while
    return config_set;
  }
  
protected:
  /// Interprets one input line (string) and returns the pair< param_name, value >
  /**
   * The return pair has a special code for success/error submission:<br>
   *  if (param_name == "" AND value == ""): empty line,<br>
   *  if (param_name == "" AND value != ""): parsing failed!
   *
   * @param line  Line to parse
   * @return pair< param_name, value >,<br> if param_name == "" AND value == "": empty line,<br> if param_name == "" AND value != "": parsing failed!
   */
  inline std::pair<std::string, std::string> interpret_config_line(const char *line) {
    std::string::size_type lidx;
    std::string lstr(line);
    std::pair<std::string, std::string> config_data;
    
    // add here each character that should be identified as a separator between param name and value
    std::string separators;
    separators += " "; separators += char(9);
    
    if ((lidx = lstr.find("#")) != std::string::npos)  // remove comment
      lstr = lstr.substr(0, lidx);
    // remove spaces at front 
    boost::algorithm::trim_left(lstr);
    
    if (!lstr.empty()) {
      gs::cnf::parseresult p = gs::cnf::parse(lstr);
      
      switch(p.second) {
        case gs::cnf::parse_result_param: // set param value
        {
          //cout << "vector size="<< p.first.size()<<endl;
          std::string tok(lstr);
          //cout << "token='"<< tok<<"'"<<endl;
          size_t pos = tok.find(p.first[0]);
          // val = ab parametername
          std::string val = tok.substr(pos, tok.length()-pos);
          //cout << "value='"<< val<<"'"<<endl;
          // val = ab nach ' ' nach Parametername
          val = val.substr(val.find_first_of(separators)+1);
          // remove spaces at front and back
          boost::algorithm::trim(val);
          //cout << "value='"<< val<<"'"<<endl;
          // auf '"' beschneiden, falls vorhanden
          //   wenn  quotes                         UND  nicht nur ein quote
          //bool handling_macro = false; // not needed
          //if (val[0] == '=') handling_macro = true; // not needed
          if (val.find_first_of('"') != string::npos && val.find_first_of('"') != val.find_last_of('"')) {
            //cout << "val.find_first_of(quote)"<<val.find_first_of('"')<<endl;
            //cout << "val.find_last_of (quote)"<<val.find_last_of('"')<<endl;
            val = val.substr(
                             val.find_first_of('"')+1,
                             val.find_last_of('"')-val.find_first_of('"')-1);
          }
          //if (handling_macro) val = "=" + val; // This was a bug
          //cout << "value='"<< val<<"'"<<endl;
          // change '\"' to '"'
          string::size_type loc = 0;
          while ((loc = val.find("\\\"", loc)) != string::npos) {
            val.replace(loc, 2, "\"");
            //cout << "value='"<< val<<"'"<<endl;
          }
          std::string parname = p.first[0];
          
          // package the configuration
          config_data.first = parname;
          config_data.second = val;
          return config_data;
        }
          break;
          
        case gs::cnf::parse_result_ignore: 
          config_data.second = "parsing failed";
          break;
          
        default:
          config_data.second = "parsing failed";
          break;
      } // end switch
    } // end if emtpy
    return config_data; // empty config_data = failure (no config data in line)
  }

protected:

  /// Config API which is used by this API
  cci::cnf::cci_cnf_broker_if*  mApi;

  /// Name of this object, given by constructor
  std::string m_name;

	char line[100];
  
};

  
#endif /* __CCI_CONFIGFILE_TOOL_H__ */
