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

#ifndef __COMMAND_IF_H__
#define __COMMAND_IF_H__

namespace gs {
namespace ctr {


/// Interface to be implemented by each API/Plugin.
/**
 * This interface has to be implemented by the APIs and Plugins, so other modules can get the name
 * and a descrition of the commands provided by an API/Plugin pair. The APIs and Plugins should 
 * internally use the same functions (e.g. static functions provided by the plugins, or same base 
 * class) so the code that provides the information exists only once.
 */
class command_if
{
public:

  virtual ~command_if() {}

  /**
   * This method has to return the name of the API/Plugin implementing command_if.
   */
  virtual const std::string getName() = 0;

  /**
   * This method has to return the name of the specified command.
   */
  virtual const std::string getCommandName(unsigned int cmd) = 0;

  /**
   * This method has to return a description of the specified command.
   */ 
  virtual const std::string getCommandDescription(unsigned int cmd) = 0;  
};


} // end namespace ctr
} // end namespace gs

#endif
