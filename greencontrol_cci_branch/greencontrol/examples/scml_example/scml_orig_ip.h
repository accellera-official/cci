//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007-2010 : GreenSocs Ltd
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

#ifndef __SCML_ORIG_IP_H__
#define __SCML_ORIG_IP_H__

#include <boost/tokenizer.hpp> // Because of buggy boost integrated in SystemC 2.1
#include <systemc>

//#define COWARE

// Here is included either the CoWare scml or the GreenSocs scml file
#ifdef COWARE
  // The CoWare scml.h file is part of the free downloadable CoWare package scml 1.1.
  #include <scml.h> // CoWare scml
#else
  #include "greencontrol/gcnf/apis/scmlApi/scml.h" // GreenSocs scml
#endif


#include <string>
using std::string;

/// Test IP which uses original scml properties.
/**
 * This is a SystemC module which uses the CoWare scml_property framework.
 * Compatible to (at least) scml 1.4 and 2.0
 */
class Scml_Orig_IP
: public sc_core::sc_module
{
public:

  SC_HAS_PROCESS(Scml_Orig_IP);
	
  /// Constructor
  Scml_Orig_IP(sc_core::sc_module_name name)
  : sc_core::sc_module(name)
  , m_int_prop("m_int_prop",       -100  )
  , m_bool_prop("m_bool_prop",     false )
  , m_double_prop("m_double_prop", 100   )
  , m_string_prop("m_string_prop", "constructor default value")
  { 
    SC_THREAD(main_action);
    
    show_properties();
  }

  /// Destructor
  ~Scml_Orig_IP();

  /// Main action to make tests.
  void main_action();

  scml_property<int>    m_int_prop;
  scml_property<bool>   m_bool_prop;
  scml_property<double> m_double_prop;
  scml_property<string> m_string_prop;

private:

  /// Displays the properties and values
  void show_properties();
 
};


#endif
