//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2009-2010 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Michael Ruetz <m.ruetz@tu-bs.de>,
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>
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

#ifndef __LOG_IF_H__
#define __LOG_IF_H__

namespace gs {
namespace ctr {


/// Interface to be implemented by parameters or classes, that are passed via a ControlTransaction.
/**
 * This interface is to be implemented by parameters or classes, that are passed via an AnyPointer or an extension in a
 * ControlTransaction. These classes have to provide some information about their content, type etc. in
 * the form of std::strings, so the GC_Logger is able to get further information about the payload in
 * a ControlTransaction.
 */
class log_if
{
public:
  virtual ~log_if() {}

  /// Should return a string with full information about the parameter (e.g. name, type and value).
  virtual std::string toString() = 0;

  /// Should return the value of the parameter as a string.
  virtual std::string getValueLog() = 0;

  /// Should return the type of the parameter as a string, if available.
  virtual std::string getTypeLog() = 0;

  /// Should return a string with the name of the parameter, if available.
  virtual std::string getNameLog() = 0;
};


} // end namespace ctr
} // end namespace gs

#endif
