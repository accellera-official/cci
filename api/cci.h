/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2009 by all Contributors.
  All Rights reserved.

  Developed by GreenSocs : http://www.greensocs.com/
   Christian Schroeder, schroeder@eis.cs.tu-bs.de
   Mark Burton, mark@greensocs.com

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 3.0 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/


#ifndef __CCI_H__
#define __CCI_H__


#include <systemc>

/**
 * @mainpage Configuration, Control & Inspection (CCI) Config API proposal
 * This is the working group internal API documentation of a proposal API for the configuration part of the CCI standard.
 *
 * Additionally to this documentation there are some text files documenting a changelog, discussions, rules etc.<br>
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
 * Post discussions to the working group reflector.
 *
 * Christian Schröder
 */


/// Namespace for OSCI Configuration, Control & Inspection (CCI) standard
namespace cci {

  
  // forward declaration
  class cci_cnf_api;
  
  // forward declaration, to be implemented by tool
  cci_cnf_api* get_cnf_api_instance(sc_core::sc_module* = NULL);
  
  
} // end namespace cci


#include "cci_error_types.h"
#include "cci_datatypes.h"
#include "cci_callbacks.h"
#include "cci_base_param.h"
#include "cci_param.h"
#include "cci_cnf_api.h"


#endif
