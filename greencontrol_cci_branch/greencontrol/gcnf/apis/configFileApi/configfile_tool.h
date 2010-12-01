//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007-2009 : GreenSocs Ltd
//       http://www.greensocs.com/ , email: info@greensocs.com
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

// doxygen comments

#ifndef __CONFIGFILE_TOOL_H__
#define __CONFIGFILE_TOOL_H__

// if this should use the unix getopt function for the parsing of the command line options
// or (if not) the boost program_options should be used (don't forget to link the lib boost_program_options!)
// default: do NOT define this!
// #define USE_GETOPT

//#define ENABLE_SHORT_COMMAND_LINE_OPTIONS  // enables the short synonyms for the gs_ options

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


namespace gs {
namespace cnf {

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
class ConfigFile_Tool
{
  
public:

  /// Constructor
  ConfigFile_Tool(const char* name)
  : m_name(name)
  { 
    mApi = gs::cnf::GCnf_Api::getApiInstance(NULL); // Use default config API
  }
  
  /// Destructor
  ~ConfigFile_Tool() {  }

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

    ConfigSet cnf_set;
    cnf_set = create_config_set_from_file(filename);
    
    GCNF_DUMP_N(m_name.c_str(), "Apply config set read from file '"<<filename<<"'");
    ConfigSet::cnf_data_map_type::const_iterator it;
    for ( it=cnf_set.get_config_map().begin() ; it != cnf_set.get_config_map().end(); it++ ) {
      GCNF_DUMP_N(m_name.c_str(), "Applying value of param '"<<it->first<<"' to '"<<it->second<<"'");
      std::cout << "ConfigFile_Api: Applying value of param '"<<it->first<<"' to '"<<it->second<<"'"<<std::endl;
      mApi->setInitValue(it->first.c_str(), it->second.c_str());
    }
    
  }
  

  /// Perform configuration for one input line (string) (e.g. line of a config file) immediately
  /**
   * @param line  Line to parse
   * @return Success of parsing (true for successfully parsed config lines and empty lines)
   */
  inline bool config_line(const char *line) {

    std::pair<std::string, std::string> config_data;
    config_data = interpret_config_line(line);
    
    // if parsing succeeded
    if (config_data.first.length() > 0) {
      GCNF_DUMP_N(m_name.c_str(), "Applying value of param '"<<config_data.first<<"' to '"<<config_data.second<<"'");
      std::cout << "ConfigFile_Api: Applying value of param '"<<config_data.first<<"' to '"<<config_data.second<<"'"<<std::endl;
      // apply the configuration
      mApi->setInitValue(config_data.first.c_str(), config_data.second.c_str());
      return true;
    }
    // if parsing failed in non-empty line
    else if (config_data.second.length() > 0) {
      return false;
    }
    else // if empty line
      return true;
  }
  

  /// Parses the command line and extracts the configfile option.
  /**
   * Throws a CommandLineException.
   *
   * The call getopt changes the used array argv so use a copy of the
   * command line arguments!
   *
   * Command line: '--gs_configfile <filename>' or '-c <filename>'
   *
   * @param argc The argc of main(...).
   * @param argv The argv of main(...).
   */
  void parseCommandLine(const int argc, const char* const* argv) throw(CommandLineException) {
#ifdef USE_GETOPT
    parseCommandLineWithGetOpt(argc, argv);
#else
    parseCommandLineWithBoost(argc, argv);
#endif
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
  ConfigSet create_config_set_from_file(const char *filename) {
    ConfigSet config_set;
    ConfigSet macro_set;
    
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

#ifndef USE_GETOPT
  /// Parses the command line using boost::program_options and extracts the configfile option.
  /**
   * Throws a CommandLineException.
   *
   * @param argc The argc of main(...).
   * @param argv The argv of main(...).
   */
  void parseCommandLineWithBoost(const int argc, const char* const* argv) throw(CommandLineException) {
    GCNF_DUMP_N(m_name.c_str(), "Parse command line ("<<argc<<" arguments) with boost program_options");

    for (int i = 0; i<argc; i++) {
      std::cout << "#"<< i << ": "<<argv[i] << std::endl;
    }
    
    po::options_description desc("Allowed options");
#ifdef ENABLE_SHORT_COMMAND_LINE_OPTIONS
    desc.add_options()
      ("help,h", "  Command line usage for command line Config parser")
      ("gs_configfile,c", po::value< std::vector<std::string> >(), "gs_configfile     multiple args possible")
      ;
#else
    desc.add_options()
      ("help", "  Command line usage for command line Config parser")
      ("gs_configfile", po::value< std::vector<std::string> >(), "gs_configfile     multiple args possible")
      ;
#endif
    
    po::variables_map vm;
    //po::store(po::parse_command_line(argc, argv, desc), vm); // without allowing unknown options
    po::store(po::command_line_parser(argc, const_cast<char**>(argv)).options(desc).allow_unregistered().run(), vm); // allows unknown options
    
    if (vm.count("help")) {
      std::cout << "Config file command line parser: parse option --help " << std::endl;
      std::cout << "Usage: options_description [options]" << std::endl;
      std::cout << desc;
      return;
    }
    
    if (vm.count("gs_configfile"))
    {
      const std::vector<std::string> *vec = &vm["gs_configfile"].as< std::vector<std::string> >();
      for (unsigned int i = 0; i < vec->size(); i++) {
        GCNF_DUMP_N(m_name.c_str(), "Boost program_option gs_configfile with value '"<<vec->at(i).c_str()<<"'");
        std::cout << m_name.c_str() <<": Parse command line option --gs_configfile " << vec->at(i) << std::endl;
        config(vec->at(i).c_str());
      }
    }

  }
  
#else
  
  /// Parses the command line  using getopt and extracts the configfile option.
  /**
   * Throws a CommandLineException.
   *
   * The call getopt changes the used array argv so use a copy of the
   * command line arguments!
   *
   * @param argc The argc of main(...).
   * @param argv The argv of main(...).
   */
  void parseCommandLineWithGetOpt(const int argc, const char* const* argv) throw(CommandLineException) {
    GCNF_DUMP_N(m_name.c_str(), "Parse command line ("<<argc<<" arguments) with getopt");
 
    assert(argc < ARGC_COPY_SIZE); // if this fails, enlarge ARGC_COPY_SIZE
    
    // getopt changes the array: use a copy of the command line arguments!
    // copy **argv to **argv_cop
    char *argv_cop[ARGC_COPY_SIZE]; // ARGC_COPY_SIZE because argc not allowed in ISO C++
    for (int i = 0; i < argc; i++) {
      char *t = new char[strlen(argv[i]) + 1];
      strncpy(t, argv[i], strlen(argv[i]) + 1); // last character filled with \0
      //cout << " " << argv[i];
      argv_cop[i] = t;
    }

    for (int i = 0; i<argc; i++) {
      std::cout << "#"<< i << ": "<<argv_cop[i] << std::endl;
    }

    //
    // Info for getopt(...) and getopt_long(...):
    //   "man 3 getopt"
    //
    int c;
    //int digit_optind = 0;

    optind = 0; // reset of getopt!!
    while (1)
      {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        // avoid compiler warning
        int a = option_index;
        a = this_option_optind;
        static struct option long_options[] =
          {
            {"gs_configfile", 1, 0, 'c'},   // '--configfile filename' = '-c filename', expected value: '<filename>'
            {"help", 0, 0, 'h'},    // '--help' = '-h'
            {0, 0, 0, 0}
          };
        // Avoid error message for not recognized options:
        opterr = 0;
#ifdef ENABLE_SHORT_COMMAND_LINE_OPTIONS
        c = getopt_long (argc, argv_cop, "h:c:",
                         long_options, &option_index);
#else
        c = getopt_long (argc, argv_cop, "",
                         long_options, &option_index);
#endif
        if (c == -1) {
          //cout << "break"<<endl;
          break;
        }
        
        switch (c)
          {
          
          case 'c': // -c and --configfile
            {
              GCNF_DUMP_N(m_name.c_str(), "Option gs_configfile with value "<<optarg);
              std::cout << "Config file command line parser: parse option --gs_configfile " << optarg << std::endl<<std::flush;
              config(optarg);
              break;
            }
            
          case 'h': // -h and --help
            {
              std::cout << "Config file command line parser: parse option --help " << std::endl<<std::flush;
              std::cout << "  Command line usage for config file command line parser:" <<std::endl;
              std::cout << std::endl;
              std::cout << "     Possible Options/Arguments:" << std::endl;
              std::cout << std::endl;
              std::cout << "      --gs_configfile filename     multiple args possible" << std::endl;
              std::cout << std::endl;
              std::cout << "      --help    This help." << std::endl;
              std::cout << std::endl<<std::endl<<std::flush;
              break;
            }

          case '?':
            {
              GCNF_DUMP_N(m_name.c_str(), "Option ? not processed in config file command line parser: "<<optopt);
              break;
            }

          default:
            {
              //GCNF_DUMP_N(m_name.c_str(), "Option "<<c<<" not processed in config command line parser.");
              //printf ("?? getopt delivers code %o ??", c);
            }
          }
      }
    
    /*
    if (optind < argc)
      {
        std::stringstream ss;
        while (optind < argc)
          ss << argv_cop[optind++] << " ";
        GCNF_DUMP_N(m_name.c_str(), "No config options of ARGV: "<<ss.str().c_str());
      }
    */
 
  }
#endif

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
      parseresult p = parse(lstr);
      
      switch(p.second) {
        case parse_result_param: // set param value
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
          
        case parse_result_ignore: 
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
  cnf_api_if* mApi;

  /// Name of this object, given by constructor
  std::string m_name;
  
};

  
} // end namespace cnf
} // end namespace gs

#endif
