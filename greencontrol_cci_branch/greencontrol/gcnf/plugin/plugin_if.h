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

// doygen comments

#ifndef __PLUGIN_IF_H__
#define __PLUGIN_IF_H__

#include "greencontrol/core/gc_globals.h"


namespace gs {
namespace cnf {


/// Interface to allow the observer database to get access to the parameter database of the ConfigPlugin.
/**
 * The ConfigPlugin has to implement this interface to be connected to the port of the
 * observer database.
 */
struct plugin_if
{
public:

  virtual ~plugin_if() { }

  /// @see gs::cnf::param_database_if::setParam
  //virtual bool setParam(const std::string &hier_parname, const std::string &value) = 0;

  /// @see gs::cnf::param_database_if::existsParam
  virtual bool existsParam(const std::string &hier_parname) = 0;

};


} // end namespace cnf
} // end namespace gs

#endif
