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

#ifndef __AVOwner_h__
#define __AVOwner_h__

#include "simple_analysis_globals.h"
#include "greencontrol/config.h"
#include "greencontrol/gav/apis/gav_api/GAV_Api.h"


/// Module which owns some gs_params.
class AVOwner
: public sc_core::sc_module
{
  
public:

  SC_HAS_PROCESS(AVOwner);
	
  /// Constructor
  AVOwner(sc_core::sc_module_name name)
    : sc_core::sc_module(name),
      int_param ("int_param", 50 ),
      str_param ("str_param", "This is a test string."),
      uint_param("uint_param", 12000),
      int_param1("int_param1", 1),
      int_param2("int_param2", 2),
      int_param3("int_param3", 3),
      int_param4("sub.int_param4", 4),
      int_param5("sub.int_param5", 5),
      logic_param("logic_param", 'X'),
      uint64_param("uint64_param", 10)
  { 
    SC_THREAD(main_action);
  }
  
  /// Main action to make tests with parameters.
  void main_action();

  /// Example parameter.
  gs::gs_param<int>             int_param;
  gs::gs_param<std::string>     str_param;
  gs::gs_param<unsigned int>    uint_param;
  gs::gs_param<int>             int_param1;
  gs::gs_param<int>             int_param2;
  gs::gs_param<int>             int_param3;
  gs::gs_param<int>             int_param4;
  gs::gs_param<int>             int_param5;
  gs::gs_param<sc_dt::sc_logic> logic_param;
  gs::gs_param<uint64_t>        uint64_param;

  gs::av::GAV_Api m_GAV_Api;
};


#endif

