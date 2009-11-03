//   GreenConfig framework
//
// LICENSETEXT
//
//   Copyright (C) 2007-2009 : GreenSocs Ltd
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


#ifndef __CONFIG_GLOBALS_h__
#define __CONFIG_GLOBALS_h__

#include "greencontrol/core/gc_globals.h"


namespace gs {
namespace cnf {
      
  using namespace ctr;
  using std::string;
  using std::vector;

  // ////////////////// BEGIN User defines /////////////////////////////// //

  // if to show GreenConfig debug information
  //#define GCNF_VERBOSE
  
  // if to show the debug information of the parameters
  //#define GS_PARAM_VERBOSE
  
  // if to show the gsconfig parameter array debug information:
  //#define GS_PARAM_ARRAY_VERBOSE
  
  // define to enable the gs::cnf namespace of class scml_property<>
  // enable namespace to match GreenControl conventions, disable if needed to be linked as original scml_property (which originally is in no namespace)
  //#define ENABLE_SCML_NAMESPACE

  // if to show callback outputs for gs_params
  //#define GS_PARAM_CALLBACK_VERBOSE
  
  // if to use getopt in the command option parsers or not (boost_program_options instead)
  #ifndef USE_BOOST_PROG_OPT
  # define USE_GETOPT
  #endif
  
  // ////////////////// END User defines ///////////////////////////////// //

#define GCNF_SC_REPORT_PREFIX "/GreenSocs/GreenControl/GreenConfig"
#define GCNF_SC_REPORTER(_postfix) (std::string(GCNF_SC_REPORT_PREFIX) + "/" + _postfix).c_str()
  
  // if to enable avoiding nested gs_param callbacks per parameter
  // This may be dangerous because someone might be interested in these callbacks.
  // Example: During a pre_read callback the called function writes the value.
  //          This write should be notified (by callback) to someone who registered 
  //          for e.g. post_write events.
  //#define GCNF_AVOID_NESTED_CALLBACKS_IN_PARAM
  
  // if to enable avoiding nested gs_param callbacks per callback adapter
  // This ensures that the same callback adapter (registered callback) is never
  // called more than once within a cakkback call stack (reentrant protection)
  #define GCNF_AVOID_NESTED_CALLBACKS_IN_CALLBACK_ADAPTERS

  namespace CallbackBehavior {
    /// CallbackBehavior enumeration for setting of callback behavior by array observers
    enum CallbackBehavior {
      CALLBACK_MEMBER_CHANGES = 0, // default: this MUST be the default because arrays that register as an observer at other arrays must be calles back on member changes - an thex cannot use the set_callback_behavior call during addMember()
      NOT_CALLBACK_MEMBER_CHANGES
    };
  } // end namespace CallbackBehavior

  
// Output Config debug messages
#ifdef GCNF_VERBOSE
# include <stdio.h>
//# define GCNF_TRACE(name, msg, ...) { printf("@%s /%d (%s): ", sc_core::sc_time_stamp().to_string().c_str(), (unsigned)sc_core::sc_delta_count(), name); printf(msg, ##__VA_ARGS__); printf("\n"); } 
# define GCNF_DUMP(msg)         std::cout<<"@"<<sc_core::sc_time_stamp()<<" /"<<(unsigned)sc_core::sc_delta_count()<<" ("<<name()<<"): "<<msg<<std::endl
# define GCNF_DUMP_N(name, msg) std::cout<<"@"<<sc_core::sc_time_stamp()<<" /"<<(unsigned)sc_core::sc_delta_count()<<" ("<<name  <<"): "<<msg<<std::endl
#else
//# define GCNF_TRACE(name, msg, ...)
# define GCNF_DUMP(msg)
# define GCNF_DUMP_N(name, msg)
#endif

// Output of the parameter objects
#ifdef GS_PARAM_VERBOSE
//# define GS_PARAM_TRACE(msg, ...) { printf("%s: ", this->name()); printf(msg, ##__VA_ARGS__); printf("\n"); } 
//# define GS_PARAM_TRACE_WITHNAME(name, msg, ...) { printf("%s: ", name); printf(msg, ##__VA_ARGS__); printf("\n"); } 
# define GS_PARAM_DUMP(msg) { std::cout<<this->name()<<": "<<msg<<std::endl; } 
# define GS_PARAM_DUMP_WITHNAME(name, msg) { std::cout<<name<<": "<<msg<<std::endl; } 
#else
//# define GS_PARAM_TRACE(msg, ...)
//# define GS_PARAM_TRACE_WITHNAME(name, msg, ...)
# define GS_PARAM_DUMP(msg)
# define GS_PARAM_DUMP_WITHNAME(name, msg)
#endif
  
// Output of the callback handling for parameter objects
#ifdef GS_PARAM_CALLBACK_VERBOSE
//# define GS_PARAM_CALLBACK_TRACE(msg, ...) { printf("%s: ", this->name()); printf(msg, ##__VA_ARGS__); printf("\n"); } 
# define GS_PARAM_CALLBACK_DUMP(msg) { std::cout<<this->name()<<": "<<msg<<std::endl; } 
# define GS_PARAM_CALLBACK_DUMP_WITHNAME(name, msg) { std::cout<<name<<": "<<msg<<std::endl; } 
#else
//# define GS_PARAM_CALLBACK_TRACE(msg, ...)
# define GS_PARAM_CALLBACK_DUMP(msg)
# define GS_PARAM_CALLBACK_DUMP_WITHNAME(name, msg)
#endif
  
} // end namespace cnf
} // end namespace gs


#endif
