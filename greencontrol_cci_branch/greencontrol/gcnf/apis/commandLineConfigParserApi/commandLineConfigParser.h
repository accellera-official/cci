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

#ifndef __COMMANDLINECONFIGPARSER_H__
#define __COMMANDLINECONFIGPARSER_H__

// if this should use the unix getopt function for the parsing of the command line options
// or (if not) the boost program_options should be used (don't forget to link the lib boost_program_options!)
// default: do NOT define this!
// #define USE_GETOPT

//#define ENABLE_SHORT_COMMAND_LINE_OPTIONS  // enables the short synonyms for the gs_ options

#include "greencontrol/gcnf/plugin/config_globals.h" // may define USE_GETOPT

#ifdef USE_GETOPT
# include <getopt.h>
# ifdef ARGC_COPY_SIZE
#  undef ARGC_COPY_SIZE
# endif
# define ARGC_COPY_SIZE 20
#else
# include <boost/program_options.hpp>
#endif

#include "greencontrol/config.h"


namespace gs {
namespace cnf {

#ifndef USE_GETOPT
  namespace po = boost::program_options;
#endif

/// Command line parser for parameter configuration
/**
 * Parses the overgiven command line and starts the setting of 
 * the parameters over the GCnf_Api.
 *
 * Command line usage:
 *
 *   program_name [-other switches or arguments] [--gs_param <param_name>=<param_value>]* [others]
 *
 *
 *   Possible Options/Arguments:
 *
 *   -  --gs_param parname=value     multiple args possible
 *   -  --gs_param "parname=value"
 *   -  --gs_param parname="value"
 *   -  Values with quotes: '--gs_param parname="this is a value string with \"quotes\""
 *
 *   -  --help
 *
 *    
 *   Example:
 *
 *     ./myProgram.x --gs_param IP2.scmlIntParam=1000 --gs_param IP1.scmlStringParam=testValueString --gs_param IP3.stringPar="This is a string with \"quotes\" and spaces"
 *
 */
class CommandLineConfigParser
{

public:

  /// DEPRECATED Constructor with parse.
  CommandLineConfigParser(char* name, const int argc, const char* const* argv)
  { 
    DEPRECATED_WARNING("CommandLineConfigParser", "DEPRECATED: CommandLineConfigParser Constructor with name is deprecated, use without name instead.");
    gcnf_api = GCnf_Api::getApiInstance(NULL);
    parse(argc, argv);  
  }
  /// DEPRECATED Constructor without parse.
  CommandLineConfigParser(char* name)
  { 
    DEPRECATED_WARNING("CommandLineConfigParser", "DEPRECATED: CommandLineConfigParser Constructor with name is deprecated, use without name instead.");
    gcnf_api = GCnf_Api::getApiInstance(NULL);
  }
  
  /// Constructor with parse.
  /**
   * Constructor which calls the parse method immediately.
   *
   * @param argc The argc of main(...).
   * @param argv The argv of main(...).
   */
  CommandLineConfigParser(const int argc, const char* const* argv)
  { 
    gcnf_api = GCnf_Api::getApiInstance(NULL);
    parse(argc, argv);  
  }
  
  /// Constructor without parse.
  /**
   * Constructor which does not call the parse method immediately.
   * Parsing has to be done by calling the parse(...)-method.
   */
  CommandLineConfigParser()
  { 
    gcnf_api = GCnf_Api::getApiInstance(NULL);
  }
  
  /// Destructor
  ~CommandLineConfigParser() {
  }

  /// Parses the command line.
  /**
   * The command line may include arguments not used by this
   * command line parser.
   *
   * Possible switches/arguments see CommandLineConfigParser.
   *
   * Throws a CommandLineException.
   *
   * Works on a copy of argv.
   *
   * @param argc The argc of main(...).
   * @param argv The argv of main(...).
   */
  void parse(const int argc, const char* const* argv) throw(CommandLineException) {
#ifdef USE_GETOPT
    parseCommandLineWithGetOpt(argc, argv);
#else
    parseCommandLineWithBoost(argc, argv);
#endif
  }


private:

#ifndef USE_GETOPT
  /// Parses the command line using boost::program_options.
  /**
   * Throws a CommandLineException.
   *
   * @param argc The argc of main(...).
   * @param argv The argv of main(...).
   */
  void parseCommandLineWithBoost(const int argc, const char* const* argv) throw(CommandLineException) {
    GCNF_DUMP_N("CommandLineConfigParser", "Parse command line ("<<argc<<" arguments) with boost program_options");

    po::options_description desc("Allowed options");
#ifdef ENABLE_SHORT_COMMAND_LINE_OPTIONS
    desc.add_options()
      ("help,h", "  Command line usage for command line Config parser")
      ("gs_param,p", po::value< std::vector<std::string> >(), "parname=value     multiple args possible")
      ;
#else
    desc.add_options()
    ("help", "  Command line usage for command line Config parser")
    ("gs_param", po::value< std::vector<std::string> >(), "parname=value     multiple args possible")
    ;
#endif
    
    po::variables_map vm;
    //po::store(po::parse_command_line(argc, argv, desc), vm); // without allowing unknown options
    po::store(po::command_line_parser(argc, const_cast<char**>(argv)).options(desc).allow_unregistered().run(), vm); // allows unknown options
    
    if (vm.count("help")) {
      std::cout << "Command line config parser: parse option --help" << std::endl;
      std::cout << "Usage: options_description [options]" << std::endl;
      std::cout << desc;
      std::cout << "      --gs_param parname=value     multiple args possible" << std::endl;
      std::cout << "      --gs_param \"parname=value\"" << std::endl;
      std::cout << "      --gs_param parname=\"value\"" << std::endl;
      std::cout << "          Values with quotes: \'--gs_param parname=\"this is a value string with \\\"quotes\\\"\"\'" << std::endl;
      return;
    }
    
    if (vm.count("gs_param"))
    {
      const std::vector<std::string> *vec = &vm["gs_param"].as< std::vector<std::string> >();
      for (unsigned int i = 0; i < vec->size(); i++) {
        GCNF_DUMP_N("CommandLineConfigParser", "Boost program_option gs_param with value '"<<vec->at(i).c_str()<<"'");
        std::cout << "CommandLineConfigParser" <<": Parse command line option --gs_param " << vec->at(i) << std::endl;
        parseAndSetParam(vec->at(i));
      }
    }
    
  }
  
#else

  /// Parses the command line using getopt.
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
    GCNF_DUMP_N("CommandLineConfigParser", "Parse command line ("<<argc<<" arguments) with getopt");

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
            {"gs_param", 1, 0, 'p'},   // '--param value' = '-p value', expected value: '<parname>=<parvalue>'
            {"help", 0, 0, 'h'},    // '--help' = '-h'
            {0, 0, 0, 0}
          };
        
        // Avoid error message for not recognized options:
        opterr = 0;
#ifdef ENABLE_SHORT_COMMAND_LINE_OPTIONS
        c = getopt_long (argc, argv_cop, "h:p:",
                         long_options, &option_index);
#else
        c = getopt_long (argc, argv_cop, "",
                         long_options, &option_index);
#endif
        if (c == -1) {
          //std::cout << "break"<<std::endl<<std::flush;
          break;
        }
        
        switch (c)
          {
          
          case 'p': // -p and --param
            {
              GCNF_DUMP_N("CommandLineArgumentParser", "Option gs_param with value "<<optarg);
              std::cout << "CommandLineArgumentParser: Parse command line argument --gs_param " << optarg << std::endl<<std::flush;
              parseAndSetParam(optarg);
              break;
            }
            
          case 'h': // -h and --help
            {
              std::cout << "Command line config parser: parse option --help " << std::endl<<std::flush;
              std::cout << "  Command line usage for command line Config parser:" <<std::endl;
              std::cout << std::endl;
              std::cout << "     Possible Options/Arguments:" << std::endl;
              std::cout << std::endl;
              std::cout << "      --gs_param parname=value     multiple args possible" << std::endl;
              std::cout << "      --gs_param \"parname=value\"" << std::endl;
              std::cout << "      --gs_param parname=\"value\"" << std::endl;
              std::cout << "          Values with quotes: \'--gs_param parname=\"this is a value string with \\\"quotes\\\"\"\'" << std::endl;
              std::cout << std::endl;
              std::cout << "      --help    This help." << std::endl;
              std::cout << std::endl<<std::endl<<std::flush;
              break;
            }

          case '?':
            {
              GCNF_DUMP_N("CommandLineConfigParser", "Option ? not processed in config command line parser: "<<optopt);
              break;
            }

          default:
            {
              //GCNF_DUMP_N("CommandLineConfigParser", "Option "<<c<<" not processed in config command line parser.");
              //printf ("?? getopt delivers code %o ??", c);
            }
          }
      }
    
    /*
    if (optind < argc)
      {
        std::stringstream ss;
        while (optind < argc)
          ss << argv[optind++] << " ";
        GCNF_DUMP_N("CommandLineConfigParser", "No config options of ARGV: "<<ss.str().c_str());
      }
    */
  }
#endif

  /// Parse and set a single parameter.
  /**
   * This method can be called with one command line option argument:
   * 'parname=parvalue' or 'parname=this is a parameter value string with spaces'
   * or even 'parname=string value with "quotes"'.
   *
   * The argument is parsed (find '=') and the parameter is set in the GCnf_Api.
   *
   * @param argval  Value of the option --param or -p.
   */
  void parseAndSetParam(const std::string argval) throw(CommandLineException) {
    GCNF_DUMP_N("CommandLineConfigParser", "parseAndSetParam("<<argval.c_str()<<")");
    std::string parname;
    std::string parvalue;

    // *** Check right format (parname=value)
    // of no space
    if (argval.find_first_of("=") == std::string::npos) {
      throw CommandLineException("Option value in command line option has no '='. Type '--help' for help.", argval);
    }
    // if not space before equal sign
    if (argval.find_first_of(" ") < argval.find_first_of("=")) {
      throw CommandLineException("Option value in command line option may not contain a space before '='.", argval);
    }

    // Parse parameter name
    parname = argval.substr(0,argval.find_first_of("="));
    // Parse parameter value
    parvalue = argval.substr(argval.find_first_of("=")+1);

    // Set parameter
    std::cout  <<"CommandLineConfigParser: Configuring value of param '"<<parname.c_str()<<"' to '"<< parvalue.c_str() <<"'"<<std::endl;
    gcnf_api->setInitValue(parname, parvalue);
  }


private:

  /// Tool API which is used by this API
  cnf_api_if *gcnf_api;

};


} // end namespace cnf
} // end namespace gs

#endif
