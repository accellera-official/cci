// LICENSETEXT
//
//   Copyright (C) 2009-2010 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//    Mark Burton, mark@greensocs.com
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




#ifndef __CCI_H__
#define __CCI_H__


#include <systemc>

/**
 * @mainpage Configuration, Control & Inspection (CCI) Configuration API proposal
 * This is the working group internal API documentation of a proposal API for the configuration part of the CCI standard.
 *
 * Additionally to this documentation there are some text files documenting a changelog, discussions, rules etc.<br>
 * <a href="../../README.txt">README.txt</a> - ChangeLog for the different committed versions.<br>
 * <a href="../changelog.txt">changelog.txt</a> - ChangeLog for the different committed versions.<br>
 * <a href="../discussions.txt">discussions.txt</a> - Motivation for some important discussions.<br>
 * <a href="../guidelines.txt">guidelines.txt</a> - Guidelines for implementations of the proposed API.<br>
 * <a href="../notes.txt">notes.txt</a> - Some general notes and hints on the API and the example implementation.<br>
 * <a href="../proposal_status.txt">proposal_status.txt</a> - Status and TODOs of the example implementation.<br>
 * 
 *
 *
 * Surely there are unsufficient parts in this proposal which are all open for discussions and changes!
 *
 * Please post discussions to the working group reflector.
 *
 * Christian Schröder
 * christian.schroeder@greensocs.com
 */


#define CCI_CNF_VERBOSE

#ifdef CCI_CNF_VERBOSE
# define CCI_CNF_DUMP(msg) { std::cout<<msg<<std::endl; } 
#else
# define CCI_CNF_DUMP(msg) {  } 
#endif


/// Namespace for OSCI Configuration, Control & Inspection (CCI) standard
namespace cci {

  
  // forward declaration
  class cci_cnf_api;
  
  /// Returns a configuration API instance responsible for handling the requests of the given module 
  /**
   * Forward declaration, to be implemented by the tool.
   *
   * @param mod Pointer to the module who wants access the config API, can be NULL if e.g. top-level or non-SystemC code is requesting the API.
   * @return Configuration API.
   */
  cci_cnf_api* get_cnf_api_instance(sc_core::sc_module* mod = NULL);
  
  
} // end namespace cci


#include "cci_error_types.h"
#include "cci_datatypes.h"
#include "cci_callbacks.h"
//#include "cci_base_param.h"
//#include "cci_param.h"
#include "cci_cnf_api.h"

#include "cci_base_param_if.h"
#include "cci_base_param.h"

#include "cci_param_if.h"

namespace cci_impl {
  
  template<class T, cci::param_mutable_type TM>
  cci::cci_param_if<T, TM>* CreateParam(const char* nam, T& val, bool force_top_level_name);

  template<class T, cci::param_mutable_type TM>
  cci::cci_param_if<T, TM>* CreateParam(const char* nam, bool force_top_level_name);

  template<class T, cci::param_mutable_type TM>
  void DestroyParam(cci::cci_param_if<T, TM>* param);

} // end namespace cci_impl

#include "cci_param.h"

#include "cci_broker_module.h"


#endif
