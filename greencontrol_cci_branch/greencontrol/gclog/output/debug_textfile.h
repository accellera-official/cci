//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
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

#ifndef __DEBUG_TEXTFILE_H__
#define __DEBUG_TEXTFILE_H__

#include "greencontrol/gclog/output_base.h"

namespace gs {
namespace log {


/// Debug output to a text file.
/**
 * This output module writes the transactions to a text file. It can be used for debugging, since it uses the most detailed (and slowest) string representation
 * of the transactions. By flushing the buffer after every write command the output is preserved, even if a crash occurs. This might be helpfull for debugging,
 * but it also slows down the simulation. So for normal logging the standard textfile output should be used.
 */
class DebugTextfile : public OutputBase
{
public:

  /// Contructor.
  /**
   * Creates the file and prints the header using the standard filename.
   *
   * @param bUnique If set to true, a timestamp will be added to the filename.
   */
  DebugTextfile(bool bUnique = true)
  {
    openFile("debug_output.txt", bUnique);
    printHeader();
  }

  /// Contructor.
  /**
   * Creates the file and prints the header using the specified filename.
   *
   * @param filename  Name of the file, that will be created by the contructor.
   * @param bUnique   If set to true, a timestamp will be added to the filename.
   */
  DebugTextfile(const char* filename, bool bUnique = true)
  {
    if(filename)
      openFile(filename, bUnique);
    else
    {
      SC_REPORT_WARNING("GC_Logger debug textfile output module", "Filename passed for textfile output is not valid. Using standard filename.");
      openFile("debug_output.txt", bUnique);
    }

    printHeader();
  }

  /// Destructor that finishes and closes the file.
  ~DebugTextfile()
  {
    m_file << "\nRegular end of log.\nFinished on " << getTimeString() << "\nClosing log.";
    m_file.close();
  }

  /// Write a user output to the file.
  void write(std::string message)
  {
    m_file << "User output: " << message << std::endl;
  }

  /// Write a string directly into the file.
  void writePure(std::string message)
  {
    m_file << message << std::endl;
  }

  /// Write the transaction to the file.
  void processTransaction(ControlTransactionHandle& th)
  {
    // std::endl / flush() flushes information and preserves all output if a crash occures, but is slower
    if(!th->hasChanged())
      m_file << "\n";
    m_file << th->toDebugString() << std::endl;
//    m_file << th->getCommandDescription() << "\n";
  }

  /// Return the number of filter ports needed by this filter.
  unsigned int numFilterPorts()
  {
    return 0;
  }

protected:

  /// Open the file with the specified name and generate unique filename if needed.
  void openFile(const char* filename, bool bUnique)
  {
    if(bUnique)
      m_file.open(generateUniqueFilename(filename).c_str());
    else
      m_file.open(filename);
  }

  /// Print the fileheader.
  void printHeader(void)
  {
    m_file << getTimeString();
    m_file << "\nDebug text output used with GC_Logger.\nStarting log.\n" << std::endl;
  }

protected:
  /// The output file.
  std::ofstream m_file;
};

  
} // end namespace log
} // end namespace gs

#endif
