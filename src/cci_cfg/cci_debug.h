// LICENSETEXT
//
//   Copyright (C) 2011 : GreenSocs Ltd
//    http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
//
// ENDLICENSETEXT

/**
 * @file
 * @author Christian Schroeder, GreenSocs Ltd.
 * @author Mark Burton, GreenSocs Ltd.
 * @author Thomas Goodfellow, OFFIS
 * @brief Debugging support for the SystemC CCI library
 *
 */


#ifndef CCI_CCI_DEBUG_H_INCLUDED_
#define CCI_CCI_DEBUG_H_INCLUDED_


#include <cassert>

#if defined(CCI_CNF_VERBOSE) || defined(CCI_PARAM_VERBOSE)
#include <string>
#include <iostream>
#endif

#ifdef CCI_CNF_VERBOSE
# define CCI_CNF_DUMP(msg) { std::cout<<msg<<std::endl; } 
#else
# define CCI_CNF_DUMP(msg) {  } 
#endif

// Debug output of the parameter objects
#ifdef CCI_PARAM_VERBOSE
# define CCI_PARAM_DUMP(msg) { std::cout<<this->name()<<": "<<msg<<std::endl; } 
# define CCI_PARAM_DUMP_WITHNAME(name, msg) { std::cout<<name<<": "<<msg<<std::endl; } 
#else
# define CCI_PARAM_DUMP(msg)
# define CCI_PARAM_DUMP_WITHNAME(name, msg)
#endif


#endif
