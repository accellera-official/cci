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

#ifndef __SIMPLE_CSV_FILE_H__
#define __SIMPLE_CSV_FILE_H__

#include "greencontrol/gclog/output_base.h"

namespace gs {
namespace log {


/// Output to a simple CSV file.
/**
 * This output module writes the transactions to a CSV file. LogConfigs are ignored, only full transactions are written with a fixed delimiter.
 *
 * Basically, the purpose of this class is to show performance differences between a hard coded and a flexible design. The performance difference in this case
 * is around 3% if the same output is written. So there is only a small performance advantage by using the hard coded version. In contrast to that it is
 * possible to write out only the elements that are really needed, instead of the whole transaction, by using a flexible design. In this case, leaving out only
 * one element of the transaction balances out the performance.
 *
 * 100000x getPar(), 10 measurements, average value, compiled Release VS2008, Intel C2D E6600: ~6.95s <-> ~6.73s
 */
class Simple_CSV_File : public OutputBase
{
public:

  /// Contructor.
  /**
   * Creates the file and prints the header using the standard filename.
   *
   * @param bUnique If set to true, a timestamp will be added to the filename.
   */
  Simple_CSV_File(bool bUnique = true)
  {
    openFile("csv_output_simple.csv", bUnique);
    printHeader();
  }

  /// Contructor.
  /**
   * Creates the file and prints the header using the specified filename.
   *
   * @param filename  Name of the file, that will be created by the contructor.
   * @param bUnique   If set to true, a timestamp will be added to the filename.
   */
  Simple_CSV_File(const char* filename, bool bUnique = true)
  {
    if(filename)
      openFile(filename, bUnique);
    else
    {
      SC_REPORT_WARNING("GC_Logger CSV output module", "Filename passed for CSV output is not valid. Using standard filename.");
      openFile("csv_output_simple.csv", bUnique);
    }

    printHeader();
  }

  /// Destructor that finishes and closes the file.
  ~Simple_CSV_File()
  {
    m_file << "\n\nRegular end of log.\nFinished on " << getTimeString();
    m_file.close();
  }

  /// Write the transaction to the file.
  void processTransaction(ControlTransactionHandle& th)
  {
    log_if* pLog = th->get_mLogPointer();

    m_file << "\"" << sc_core::sc_time_stamp().to_string() << "/" << sc_core::sc_delta_count() << "\";";

    if(!th->hasChanged())
      m_file << th << ";";
    else
      m_file << "returning;";

    // print the single quarks separated bby the delimiter
    m_file << th->get_mService() << ";\"" << getControlServiceString(th->get_mService()) << "\";" << th->get_mTarget() << ";" << th->get_mCmd() << ";\"" << th->getCommandName() << "\";" << th->get_mAnyPointer() << ";"
           << th->get_mAnyPointer2() << ";" << th->get_mAnyUint() << ";\"" << th->get_mSpecifier() << "\";\"" << th->get_mValue() << "\";" << th->get_mID() << ";";

    if(th->get_mCmdIf())
      m_file << "\"" << th->get_mCmdIf()->getName() << "\";";
    else
      m_file << ";";

    m_file << "\"" << th->get_mMetaData() << "\";" << th->get_mError() << ";" << pLog << ";";

    // check if the quark is valid
    if(pLog)
      m_file << "\"" << pLog->toString() << "\";";
    else
      m_file << ";";

    m_file << th->get_mCmdIf() << ";" << th->get_mLogConfig() << std::endl;
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
    m_file << "\nCSV output used with GC_Logger.\n\n" << std::endl;
    m_file << "\"Time / Delta\";\"Transaction:\";Service;\"Service Name\";Target;Command;\"Command Name\";AnyPointer;AnyPointer2;AnyUint;Specifier;Value;ID;\"Sender Name\";MetaData;Error;LogPointer;"
           << "\"LogPointer Content\";CommandIf;LogConfig" << std::endl;
  }

protected:
  /// The output file.
  std::ofstream m_file;
};

  
} // end namespace log
} // end namespace gs

#endif
