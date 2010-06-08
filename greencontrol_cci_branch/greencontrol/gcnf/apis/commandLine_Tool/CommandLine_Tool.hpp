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

#include <typeinfo>
#include <stdexcept>
#include <sched.h> // for sched_yield for mac

#include "greencontrol/core/helpfunctions.h"

namespace gs {
namespace cnf {

inline void CommandLine_Tool::show_command_line() {
  std::cout << "toolThread (h=help) > " <<std::flush;
}

inline void CommandLine_Tool::start_thread() {
  GCNF_DUMP_N(name(), "This is the main action in CommandLine_Tool");
#ifdef ACTIVATE_COMMAND_THREAD
  // Thread for key input
  pthread_t key_threadID;
  if (pthread_create(&key_threadID, NULL, CommandLine_Tool::toolThread, (void *) this) != 0) {
    SC_REPORT_ERROR(name(), "Error creating thread for key input.");
  }
  GCNF_DUMP_N(name(), "toolThread created.");
  mThreadShowInputLine = true;
//#else // already in constructor
//    SC_REPORT_INFO(name(), "ACTIVATE_COMMAND_THREAD is not defined! The command line Thread will not be started!");
#endif
}

inline void *CommandLine_Tool::toolThread(void *generic_ptr) {
  GCNF_DUMP_N("static toolThread", "toolThread startet.");

#ifdef ACTIVATE_COMMAND_THREAD
  CommandLine_Tool *tool = (CommandLine_Tool*)generic_ptr;

  if (!tool) {
    SC_REPORT_ERROR("static toolThread", "Thread started with wrong pointer!");
  }
  else {
    tool->mInputThreadRunning = true;

    while (true) {
      //pthread_yield(); // give control to SystemC-sim. // TODO does not work with Christian's Mac
      sched_yield(); // give control to SystemC-sim.
      // Wait for the ok to show command line
      while (!tool->mThreadShowInputLine) { 
      }
      if (tool->mNewThreadDataAvailable) {
        GCNF_DUMP_N("static toolThread", "Old data have not yet been processed! Wait...");
        while (tool->mNewThreadDataAvailable) {
        }
        GCNF_DUMP_N("static toolThread", "Old data now have been processed!");
      }

      std::ostringstream ss;
      int c;
      std::cout << "toolThread (h=help) > " <<std::flush;
            
      // Record input until return pressed 
      while((c=getchar()) != '\n') {
        ss << char(c);
        //pthread_yield();
      }
      
      if (ss.str() == "q") {
        GCNF_DUMP_N("static toolThread", "quit thread loop");
        break;
      }

      // instead of tool->process_input(ss.str())
      tool->mThreadData = ss.str();
      tool->mNewThreadDataAvailable = true;

    } // end while
    tool->mInputThreadRunning = false;
  } // end tool ptr
#endif

  GCNF_DUMP_N("static toolThread", "toolThread ends.");
  return NULL;
}


inline void CommandLine_Tool::poll_thread_data() {
  // wait for toolThread to be started
  while (!mInputThreadRunning) {
    wait(TEST_TOOL_POLL_TIME);
  }
  GCNF_DUMP_N(name(), "Begin poll pthread data.");
  while(true) {
    //GCNF_DUMP_N(name(), "poll pthread data.");
    if (mNewThreadDataAvailable) {
      GCNF_DUMP_N(name(), "poll_thread_data: new data available: mNewThreadDataAvailable");
      process_input(mThreadData);
      mNewThreadDataAvailable = false;
    }
    else {
      //GCNF_DUMP_N(name(), "no new data available: !mNewThreadDataAvailable");
    }
    //GCNF_DUMP_N(name(), "wait");
    wait(TEST_TOOL_POLL_TIME);
    // Exit SC_THREAD when toolThread not running any longer
    if (!mInputThreadRunning) {
      GCNF_DUMP_N(name(), "Exit SC_THREAD poll_thread_data");
      break;
    }
  }
}


inline void CommandLine_Tool::process_input(const std::string &token_) {
  GCNF_DUMP_N(name(), "got input "<<token_.c_str());
  bool parsed = false;

  // parse and action
  std::string::size_type idx;
  std::vector<std::string> tokenlist;
  std::string token(token_);
  
  // test for empty token --> ignore
  if (token == "") parsed = false;

  // test for comment --> remove 
  idx = token.find("#"); 
  if (idx != std::string::npos)
    token = token.substr(0, idx);
   
  // test for tabs and spaces and create token list
  while((idx = token.find_first_of(" \t")) != std::string::npos) {
    if (idx > 0) { // is a word
      tokenlist.push_back(token.substr(0,idx));
    }    
    token = token.substr(idx+1);
  }
  if (token.size() > 0) {
    tokenlist.push_back(token);
  }

  if (tokenlist.size() > 0) {
    if (tokenlist[0] == "set") {
      if (tokenlist.size() == 3) {
        parsed = true;
        gs::gs_param_base *par = toolApi.getPar(tokenlist[1].c_str());
        if (par) {
          par->setString(tokenlist[2].c_str());
          std::cout << "Param " << tokenlist[1].c_str() << " set to '" << tokenlist[2].c_str() <<  "'" << std::endl;
        } else {
          std::cout << "Error setting value for not existing param " << tokenlist[1].c_str() << std::endl;
        }
      }
    }
    else if (tokenlist[0] == "get") {
      if (tokenlist.size() == 2) {
        parsed = true;
        gs::gs_param_base *par = toolApi.getPar(tokenlist[1].c_str());
        if (par) {
          std::cout << "Got value for param " << tokenlist[1].c_str() << ": '";
          std::cout << par->getString();
          std::cout << "'" << std::endl;
        } else {
          std::cout << "Error getting value for not existing param " << tokenlist[1].c_str() << std::endl;
        }
      }
    }
    else if (tokenlist[0] == "list") {
      if (tokenlist.size() == 1) {
        parsed = true;
        std::cout << "Got list:" << std::endl;
        show_vector(toolApi.getParamList());
      }
      if (tokenlist.size() == 2) {
        parsed = true;
        std::cout << "Got value for module " << tokenlist[1].c_str() << ":" << std::endl;
        show_vector(toolApi.getParamList(tokenlist[1]));
      }
    }
  }

  // show help
  if (!parsed) {
    std::cout << "  h   : this help" << std::endl
         << "  q   : quit" <<std::endl
         << "  set <param_name> <value>  : Set value <value> of parameter <param_name> " << std::endl
         << "  get <param_name>          : Get value of parameter <param_name>" <<std::endl
         << "  list                      : List all parameters"<<std::endl
         << "  list <modname>            : List all parameters of module <modname>"<<std::endl
         << "  list <modname>.*          : List all parameters of module <modname> and its children"<<std::endl<<std::flush;
  } 
}

} // end namespace cnf
} // end namespace gs
