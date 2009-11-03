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

// doxygen comments

#ifndef __GCEXCEPTION_h__
#define __GCEXCEPTION_h__


namespace gs {
namespace ctr {


/// GreenControl exception class.
/**
 * Use the error text (errorText) to specify the error, 
 * use the argument ID (argId) to e.g. give a argument of the called method
 * to the error processing code.
 */
class GCException  
: public std::exception
{
public:

  /// Constructor.
  /**
   * With default string values for error text and argument id.
   *
   * @param errorText  Text which specifies the error.
   * @param argId      Identifier for the argument.
   */
  GCException(const std::string &errorText = "undefined GreenControl exception", 
                       const std::string &argId = "undefined")
    : std::exception(),
      m_errorText(errorText),
      m_argId(argId)
  {  }
  
  /// Destructor.
  virtual ~GCException() throw() { }
  
  /// Returns the error text.
  std::string error() const { return ( m_errorText ); }
  
  /// Returns the argument id.
  std::string argId() const  
  { 
    if ( m_argId == "undefined" )
      return " ";
    else
      return ( "Argument: " + m_argId ); 
  }
  
  /// Returns the arg id and error text. 
  const char* what() const throw() 
  {
    static std::string ex; 
    ex = m_argId + " -- " + m_errorText;
    return ex.c_str();
  }
  
private:
  /// The text of the exception message.
  std::string m_errorText;
  
  /// The argument related to this exception.
  std::string m_argId;
};


} // end namespace ctr
} // end namespace gs


namespace gs {
namespace cnf {


/// Exception class for wrong command line parameter.
class CommandLineException  
: public gs::ctr::GCException
{
public:

  /// Constructor.
  /**
   * With default string values for error text and argument id.
   *
   * @param errorText  Text which specifies the error.
   * @param argId      Identifier for the argument.
   */
  CommandLineException(const std::string &errorText = "undefined command line exception", 
                       const std::string &argId = "undefined")
    : GCException(errorText, argId)
  {  }
  
};


/// Exception class for wrong command line parameter.
class RegisterObserverFailedException  
: public gs::ctr::GCException
{
public:

  /// Constructor.
  /**
   * With default string values for error text and argument id.
   *
   * @param errorText  Text which specifies the error.
   * @param argId      Identifier for the argument.
   */
  RegisterObserverFailedException(const std::string &errorText = "undefined register observer failed exception", 
                                  const std::string &argId = "undefined")
    : GCException(errorText, argId)
  {  }
  
};


} // end namespace cnf
} // end namespace gs

#endif
