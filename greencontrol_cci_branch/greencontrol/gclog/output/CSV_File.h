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

#ifndef __CSV_FILE_H__
#define __CSV_FILE_H__

#include "greencontrol/gclog/output_base.h"

namespace gs {
namespace log {


/// Output to a CSV file.
/**
 * This output module writes the transactions to a CSV file.
 */
class CSV_File : public OutputBase
{
public:

  /// Contructor.
  /**
   * Creates the file and prints the header using the standard filename.
   *
   * @param bUnique If set to true, a timestamp will be added to the filename.
   */
  CSV_File(bool bUnique = true)
  {
    init();

    openFile("csv_output.csv", bUnique);
    printHeader();
  }
   
  /// Contructor.
  /**
   * Creates the file and prints the header using the specified filename.
   *
   * @param filename  Name of the file, that will be created by the contructor.
   * @param bUnique   If set to true, a timestamp will be added to the filename.
   */
  CSV_File(const char* filename, bool bUnique = true)
  {
    init();

    if(filename)
      openFile(filename, bUnique);
    else
    {
      SC_REPORT_WARNING("GC_Logger CSV output module", "Filename passed for CSV output is not valid. Using standard filename.");
      openFile("csv_output.csv", bUnique);
    }

    printHeader();
  }

  /// Destructor that finishes and closes the file.
  ~CSV_File()
  {
    m_file << "\n\nRegular end of log.\nFinished on " << getTimeString();
    m_file.close();
  }

  /// Write the transaction to the file.
  void processTransaction(ControlTransactionHandle& th)
  {
    // use the static formatting if there's a LogConfig for the output
    if(m_pConf)
      writeStaticTransaction(th);
    else
    {
      // write out the full transaction if the LogConfig is true and hasn't changed
      if(!th->get_mLogConfig()->hasChanged() && (th->get_mLogConfig()->getStandardSetting() == true))
        writeFullTransaction(th);
      else
        writeDynamicTransaction(th);
    }
  }

  /// Return the number of filter ports needed by this filter.
  unsigned int numFilterPorts()
  {
    return 0;
  }

  /// Callback method to write the first line when the CSV_File is added to another filter.
  void activate()
  {
    writeFirstLine();
  }

  /// Set a static LogConfig object that is used for formatting.
  void setLogConfig(LogConfig* pConf)
  {
    m_pConf = pConf;
  }

  /// Specify if the log should be human readable.
  void setHumanReadable(bool bSet)
  {
    m_bHumanRd = bSet;
  }

  /// Enable or disable timing information.
  void enableTiming(bool bSet)
  {
    m_bTiming = bSet;
  }

  /// Set the delimiter.
  void setDelimiter(const char* delimiter)
  {
    m_del = delimiter;
  }

protected:

  /// Initialize the members.
  void init()
  {
    m_pConf = NULL;
    m_del = ";";
    m_bHumanRd = false;
    m_bTiming = true;
  }

  /// Writes out the transaction according to the LogConfig object of the CSV_File.
  void writeStaticTransaction(ControlTransactionHandle& th)
  {
    log_if* pLog = th->get_mLogPointer();

    if(m_bTiming)
      m_file << "\"" << sc_core::sc_time_stamp().to_string() << "/" << sc_core::sc_delta_count() << "\"" << m_del;

    if(!th->hasChanged())
      m_file << th << m_del;
    else
      m_file << "returning" << m_del;

    if(m_pConf->getService())
    {
      m_file << th->get_mService() << m_del;
      // write out the command_if and log_if information if needed
      if(m_bHumanRd)
        m_file << "\"" << getControlServiceString(th->get_mService()) << "\"" << m_del;
    }
    if(m_pConf->getTarget())  m_file << th->get_mTarget() << m_del;
    if(m_pConf->getCmd())
    {
      m_file << th->get_mCmd() << m_del;
      if(m_bHumanRd)
        m_file << "\"" << th->getCommandName() << "\"" << m_del;
    }
    if(m_pConf->getAnyPointer())  m_file << th->get_mAnyPointer() << m_del;
    if(m_pConf->getAnyPointer2()) m_file << th->get_mAnyPointer2() << m_del;
    if(m_pConf->getAnyUint())     m_file << th-> get_mAnyUint() << m_del;
    if(m_pConf->getSpecifier())   m_file << "\"" << th->get_mSpecifier() << "\"" << m_del;
    if(m_pConf->getValue())       m_file << "\"" << th->get_mValue() << "\"" << m_del;
    if(m_pConf->getID())
    {
      m_file << th->get_mID() << m_del;
      if(m_bHumanRd)
        m_file << "\"" << th->getSenderName() << "\"" << m_del;
    }
    if(m_pConf->getMetaData())    m_file << "\"" << th->get_mMetaData() << "\"" << m_del;
    if(m_pConf->getError())       m_file << th->get_mError() << m_del;
    if(m_pConf->getLogPointer())
    {
      m_file << pLog << m_del;
      if(m_bHumanRd)
      {
        if(pLog)
          m_file << "\"" << pLog->toString() << "\"" << m_del;
        else
          m_file << m_del;
      }
    }
    if(m_pConf->getCmdIf())     m_file << th->get_mCmdIf() << m_del;
    if(m_pConf->getLogConfig()) m_file << th->get_mLogConfig();
    m_file << std::endl;
  }

  /// Write out the transaction according to the LogConfig object specified by the transaction.
  void writeDynamicTransaction(ControlTransactionHandle& th)
  {
    log_if* pLog = th->get_mLogPointer();

    if(m_bTiming)
      m_file << "\"" << sc_core::sc_time_stamp().to_string() << "/" << sc_core::sc_delta_count() << "\"" << m_del;

    if(!th->hasChanged())
      m_file << th << m_del;
    else
      m_file << "returning" << m_del;

    if(th->get_mLogConfig()->getService())
    {
      m_file << th->get_mService() << m_del;
      if(m_bHumanRd)
        m_file << "\"" << getControlServiceString(th->get_mService()) << "\"" << m_del;
    }
    else
    {
      // don't forget delimiters for empty fields
      m_file << m_del;
      if(m_bHumanRd)
        m_file << m_del;
    }
    if(th->get_mLogConfig()->getTarget())  m_file << th->get_mTarget();
    m_file << m_del;
    if(th->get_mLogConfig()->getCmd())
    {
      m_file << th->get_mCmd() << m_del;
      if(m_bHumanRd)
        m_file << "\"" << th->getCommandName() << "\"" << m_del;
    }
    else
    {
      m_file << m_del;
      if(m_bHumanRd)
        m_file << m_del;
    }
    if(th->get_mLogConfig()->getAnyPointer())  m_file << th->get_mAnyPointer();
    m_file << m_del;
    if(th->get_mLogConfig()->getAnyPointer2()) m_file << th->get_mAnyPointer2();
    m_file << m_del;
    if(th->get_mLogConfig()->getAnyUint())     m_file << th-> get_mAnyUint();
    m_file << m_del;
    if(th->get_mLogConfig()->getSpecifier())   m_file << "\"" << th->get_mSpecifier() << "\"";
    m_file << m_del;
    if(th->get_mLogConfig()->getValue())       m_file << "\"" << th->get_mValue() << "\"";
    m_file << m_del;
    if(th->get_mLogConfig()->getID())
    {
      m_file << th->get_mID() << m_del;
      if(m_bHumanRd)
        m_file << "\"" << th->getSenderName() << "\"" << m_del;
    }
    else
    {
      m_file << m_del;
      if(m_bHumanRd)
        m_file << m_del;
    }
    if(th->get_mLogConfig()->getMetaData())    m_file << "\"" << th->get_mMetaData() << "\"";
    m_file << m_del;
    if(th->get_mLogConfig()->getError())       m_file << th->get_mError();
    m_file << m_del;
    if(th->get_mLogConfig()->getLogPointer())
    {
      m_file << pLog << m_del;
      if(m_bHumanRd)
      {
        if(pLog)
          m_file << "\"" << pLog->toString() << "\"" << m_del;
        else
          m_file << m_del;
      }
    }
    else
    {
      m_file << m_del;
      if(m_bHumanRd)
        m_file << m_del;
    }
    if(th->get_mLogConfig()->getCmdIf())     m_file << th->get_mCmdIf();
    m_file << m_del;
    if(th->get_mLogConfig()->getLogConfig()) m_file << th->get_mLogConfig();
    m_file << std::endl;
  }

  /// Write out the complete transaction.
  void writeFullTransaction(ControlTransactionHandle& th)
  {
    log_if* pLog = th->get_mLogPointer(); // temp var

    if(m_bTiming)
      m_file << "\"" << sc_core::sc_time_stamp().to_string() << "/" << sc_core::sc_delta_count() << "\"" << m_del;

    if(!th->hasChanged())
      m_file << th << m_del;
    else
      m_file << "returning" << m_del;

    m_file << th->get_mService() << m_del;
    if(m_bHumanRd)
      m_file << "\"" << getControlServiceString(th->get_mService()) << "\"" << m_del;

    m_file << th->get_mTarget() << m_del << th->get_mCmd() << m_del;

    if(m_bHumanRd)
      m_file << "\"" << th->getCommandName()   << "\"" << m_del;

    // Specifier, Value and MetaData are strings and could contain special characters. therefore use "\""
    m_file << th->get_mAnyPointer() << m_del << th->get_mAnyPointer2() << m_del << th->get_mAnyUint() << m_del << "\"" << th->get_mSpecifier() << "\"" << m_del
           << "\"" << th->get_mValue() << "\"" << m_del << th->get_mID() << m_del;
    
    if(m_bHumanRd)
        m_file << "\"" << th->getSenderName() << "\"" << m_del;

    m_file << "\"" << th->get_mMetaData() << "\"" << m_del << th->get_mError() << m_del << pLog << m_del;

    if(m_bHumanRd)
    {
      if(pLog)
        m_file << "\"" << pLog->toString() << "\"" << m_del;
      else
        m_file << m_del;
    }

    m_file << th->get_mCmdIf() << m_del << th->get_mLogConfig() << std::endl;
  }

  /// Write the first line of the CSV output.
  void writeFirstLine()
  {
    // if there's a LogConfig object specified for the CSV_File only the specified fields are written.
    if(m_pConf)
    {
      if(m_bTiming)
        m_file << "\"Time / Delta\"" << m_del;
      m_file << "\"Transaction:\"" << m_del;
      if(m_pConf->getService())
      {
        m_file << "Service" << m_del;
        if(m_bHumanRd)
          m_file << "\"Service Name\"" << m_del;
      }
      if(m_pConf->getTarget())    m_file << "Target" << m_del;
      if(m_pConf->getCmd())
      {
        m_file << "Command" << m_del;
        if(m_bHumanRd)
          m_file << "\"Command Name\"" << m_del;
      }
      if(m_pConf->getAnyPointer())  m_file << "AnyPointer" << m_del;
      if(m_pConf->getAnyPointer2()) m_file << "AnyPointer2" << m_del;
      if(m_pConf->getAnyUint())     m_file << "AnyUint" << m_del;
      if(m_pConf->getSpecifier())   m_file << "Specifier" << m_del;
      if(m_pConf->getValue())       m_file << "Value" << m_del;
      if(m_pConf->getID())
      {
        m_file << "ID" << m_del;
        if(m_bHumanRd)
          m_file << "\"Sender Name\"" << m_del;
      }
      if(m_pConf->getMetaData())    m_file << "MetaData" << m_del;
      if(m_pConf->getError())       m_file << "Error" << m_del;
      if(m_pConf->getLogPointer())
      {
        m_file << "LogPointer" << m_del;
        if(m_bHumanRd)
          m_file << "\"LogPointer Content\"" << m_del;
      }
      if(m_pConf->getCmdIf())       m_file << "CommandIf" << m_del;
      if(m_pConf->getLogConfig())   m_file << "LogConfig";
      m_file << std::endl;
    }
    else
    {
      // write the first line
      if(m_bTiming)
        m_file << "\"Time / Delta\"" << m_del;
      m_file << "\"Transaction:\"" << m_del << "Service" << m_del;
      if(m_bHumanRd)
        m_file << "\"Service Name\"" << m_del;
      m_file << "Target" << m_del << "Command" << m_del;
      if(m_bHumanRd)
        m_file << "\"Command Name\"" << m_del;
      m_file << "AnyPointer" << m_del << "AnyPointer2" << m_del << "AnyUint" << m_del << "Specifier" << m_del << "Value" << m_del << "ID" << m_del;
      if(m_bHumanRd)
        m_file << "\"Sender Name\"" << m_del;
      m_file << "MetaData" << m_del << "Error" << m_del << "LogPointer" << m_del;
      if(m_bHumanRd)
        m_file << "\"LogPointer Content\"" << m_del;
      m_file << "CommandIf" << m_del << "LogConfig" << std::endl;
    }
  }

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
  }

protected:
  /// The output file.
  std::ofstream m_file;

  /// The delimiter used in the output file.
  const char* m_del;

  /// A LogConfig pointer that can hold an own configuration for the CSV_File.
  LogConfig* m_pConf;

  /// If the log should be human readable.
  bool m_bHumanRd;

  /// If the timing information should be written.
  bool m_bTiming;
};

  
} // end namespace log
} // end namespace gs

#endif
