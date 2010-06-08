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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// doxygen comments

#ifndef __CONFIG_TEXTFILE_H__
#define __CONFIG_TEXTFILE_H__

#include "greencontrol/gclog/output_base.h"

namespace gs {
namespace log {


/// Configurable output to a text file.
/**
 * This configurable output module writes the transactions to a text file.
 */
class ConfigTextfile : public OutputBase
{
public:

  /// Contructor.
  /**
   * Creates the file and prints the header using the standard filename.
   *
   * @param bUnique If set to true, a timestamp will be added to the filename.
   */
  ConfigTextfile(bool bUnique = true)
  {
    openFile("output.txt", bUnique);
    printHeader();
  }

  /// Contructor.
  /**
   * Creates the file and prints the header using the specified filename.
   *
   * @param filename  Name of the file, that will be created by the contructor.
   * @param bUnique   If set to true, a timestamp will be added to the filename.
   */
  ConfigTextfile(const char* filename, bool bUnique = true)
  {
    if(filename)
      openFile(filename, bUnique);
    else
    {
      SC_REPORT_WARNING("GC_Logger configurable textfile output module", "Filename passed for textfile output is not valid. Using standard filename.");
      openFile("output.txt", bUnique);
    }

    printHeader();
  }

  /// Destructor that finishes and closes the file.
  ~ConfigTextfile()
  {
    m_file << "\nRegular end of log.\nFinished on " << getTimeString() << "\nClosing log.";
    m_file.close();
  }

  /// Write the transaction to the file.
  void processTransaction(ControlTransactionHandle& th)
  {
    if(!th->hasChanged())
      m_file << "\n";
    m_file << th->toConfigString();
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
    m_file << "\nConfigurable text output used with GC_Logger.\nStarting log.\n" << std::endl;
  }

protected:
  /// The output file.
  std::ofstream m_file;
};

  
} // end namespace log
} // end namespace gs

#endif
