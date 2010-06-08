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

// doxygen comments

#ifndef __COMMANDLINE_TOOL_H__
#define __COMMANDLINE_TOOL_H__

// for CommandLine_Tool: timed or not timed models
//  value to be put into wait(...) statement, e.g.:
//    SC_ZERO_TIME or
//    sc_time(1, SC_NC)
// #define TEST_TOOL_POLL_TIME sc_time(1, SC_MS)

#ifndef TEST_TOOL_POLL_TIME
#  warning Warning: CommandLine_Tool will define default poll time for thread! 
#  warning          To avoid this please define TEST_TOOL_POLL_TIME.
#  define TEST_TOOL_POLL_TIME sc_time(1, SC_MS)
#endif

#include <sstream>

#include "greencontrol/gcnf/plugin/config_globals.h"
#include "greencontrol/gcnf/apis/toolApi/tool_gcnf_api.h"

// To (de)activate the command line
//#define ACTIVATE_COMMAND_THREAD
#ifdef ACTIVATE_COMMAND_THREAD
  #include <pthread.h>
#endif

// On Linux, you must compile with the -D_REENTRANT option.  This tells
// the C/C++ libraries that the functions must be thread-safe
#ifndef _REENTRANT
#error ACK! You need to compile with _REENTRANT defined since this uses threads
#endif


namespace gs {
namespace cnf {


/// Tool which provides a command line in the simulation terminal for access to parameters.
/**
 * Tool which uses the Tool_GCnf_Api to set, get and list parameters.
 *
 * This tool provides a command line to the user during simulation runtime and is able to 
 * - set and get parameters
 * - get the parameter list
 * 
 * Get help and a list of the available commands with the command 'h'.
 *
 * You have to define TEST_TOOL_POLL_TIME with the time the SC_THREAD should
 * use to poll for data from the pthread. For untimed models use SC_ZERO_ZIME,
 * for timed models use a reasonable time, e.g. sc_time(1, SC_MS)
 * \code
 * #define TEST_TOOL_POLL_TIME sc_time(1, SC_MS)
 * \endcode
 */
class CommandLine_Tool
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(CommandLine_Tool);
	
  /// Constructor
  CommandLine_Tool(sc_core::sc_module_name name)
    : sc_core::sc_module(name),
      mNewThreadDataAvailable(false),
      mThreadShowInputLine(false),
      mInputThreadRunning(false)
  { 
    GCNF_DUMP_N("CommandLine_Tool", "construct CommandLine_Tool");
#ifdef ACTIVATE_COMMAND_THREAD
    SC_THREAD(poll_thread_data);
#else
    SC_REPORT_INFO("CommandLine_Tool", "ACTIVATE_COMMAND_THREAD is not defined! The command line Thread will not be started!");
#endif
    SC_THREAD(start_thread);

  }

  /// SC_THREAD for polling data from pthread
  /**
   * Polls each time TEST_TOOL_POLL_TIME for new data.
   *
   * If mNewThreadDataAvailable is true, in mThreadData are new input data.
   * These data are processed and then mNewThreadDataAvailable is set to false.
   *
   * TEST_TOOL_POLL_TIME has to be set in global preferences (e.g. gs_globals.h)
   * - TEST_TOOL_POLL_TIME = SC_ZERO_TIME:     wait SC_ZERO_TIME, no time passes
   * - TEST_TOOL_POLL_TIME = sc_time(1,SC:NS): wait 1 ns, time passes (for timed models)
   */
  void poll_thread_data();

  /// Shows the command line.
  /**
   * May be used after another thread made outputs to reprint the command line.
   */
  void show_command_line();

private:

  /// starts the toolThread.
  void start_thread();

  /// Thread for console input
  /**
   * Thread which gets the key-inputs for the Tool API.
   *
   * When a full line was entered (new line entered) the data are written into the 
   * tool member variable mThreadData and the member variable mNewThreadDataAvailable
   * is set to true.
   *
   * In the tool the SC_THREAD poll_thread_data checks these variables and calls 
   * process_input() if new data are avalable.
   *
   * This is done to allow the pthread to act fully independent from the simulation.
   * Synchronisation is done throug mNewThreadDataAvailable.
   *
   * The thread waits with getting new input until mNewThreadDataAvailable is false.
   *
   * @param generic_ptr  Data which is provided to the thread. Here it is a pointer 
   *                     to the TestTool
   */
  static void *toolThread(void *generic_ptr);

  /// Process the input.
  /**
   * @param inp  Input string which should be processed.
   */
  void process_input(const std::string &inp);

private:

  /// Tool API which is used by this test tool
  gs::cnf::Tool_GCnf_Api toolApi;

  /// Set by pthread when new input data have been stored in mThreadData
  bool mNewThreadDataAvailable;

  /// Set by TestTool to announce to the thread that the command line should be shown
  bool mThreadShowInputLine;

  /// Data which are transmitted between pthread and SystemC
  std::string mThreadData;

  /// Indicates that the toolThread is running (poll_thread_data may exit when false)
  bool mInputThreadRunning;

};

} // end namespace cnf
} // end namespace gs

#include "CommandLine_Tool.hpp"

#endif
