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

#ifndef __LOG_CONFIG_H__
#define __LOG_CONFIG_H__

// This header is included by greencontrol/core/gc_transaction.h, but before the class ControlTransaction is declared. To be able to make ControlTransaction
// a friend class of LogConfig, the class ControlTransaction has to be declared in its namespace.
namespace gs {
namespace ctr {
    class ControlTransaction;
}
}

namespace gs {
namespace log {


/// This class holds configurations for the GC_Logger output modules.
/**
 * This class holds configurations used by the GC_Logger output modules. It contains boolean counterparts to the quarks in the ControlTransaction class. If these
 * are set to true, the respective quarks are used by the GC_Logger output modules that make use of this feature, otherwise they are ignored. Filters can use this
 * to control the information usage of the output modules. This configuration is also used to mark if quarks have been changed by the receiver of the transaction
 * before it was returned, so the output modules are able to recognize these quarks and handle them accordingly.
 *
 * These configurations can be stacked, so different filters can use different configs. Configurations have to be activated by the activateConfig() method of the
 * ControlTransaction class. By using the deactivateConfig() method, the previous settings can be restored. So every activateConfig() should be followed by an
 * associated deactivateConfig() after the work was done, since different filters can be combined and a config belonging to a specific filter could affect other
 * filters or output modules, if it isn't deactivated.
 */
class LogConfig
{
  friend class gs::ctr::ControlTransaction;

public:

  /// Constructor.
  LogConfig()
  {
    m_pNext = NULL;
    m_bIsActivated = false;

    reset(true);
  }

  /// Copy constructor.
  LogConfig(const LogConfig &lc)
  {
    *this = lc; // use copy-operator
  }

  /// Destructor.
  virtual ~LogConfig() {}

  // single quark access
  void setService(bool bSet) { m_bHasChanged = true; m_bService = bSet; }
  void setTarget(bool bSet) { m_bHasChanged = true; m_bTarget = bSet; }
  void setCmd(bool bSet) { m_bHasChanged = true; m_bCmd = bSet; }
  void setAnyPointer(bool bSet) { m_bHasChanged = true; m_bAnyPointer = bSet; }
  void setAnyPointer2(bool bSet) { m_bHasChanged = true; m_bAnyPointer2 = bSet; }
  void setAnyUint(bool bSet) { m_bHasChanged = true; m_bAnyUint = bSet; }
  void setSpecifier(bool bSet) { m_bHasChanged = true; m_bSpecifier = bSet; }
  void setValue(bool bSet) { m_bHasChanged = true; m_bValue = bSet; }
  void setID(bool bSet) { m_bHasChanged = true; m_bID = bSet; }
  void setError(bool bSet) { m_bHasChanged = true; m_bError = bSet; }
  void setMetaData(bool bSet) { m_bHasChanged = true; m_bMetaData = bSet; }
  void setLogPointer(bool bSet) { m_bHasChanged = true; m_bLogPointer = bSet; }
  void setCmdIf(bool bSet) { m_bHasChanged = true; m_bCmdIf = bSet; }
  void setLogConfig(bool bSet) { m_bHasChanged = true; m_bLogConfig = bSet; }

  bool getService() { return m_bService; }
  bool getTarget() { return m_bTarget; }
  bool getCmd() { return m_bCmd; }
  bool getAnyPointer() { return m_bAnyPointer; }
  bool getAnyPointer2() { return m_bAnyPointer2; }
  bool getAnyUint() { return m_bAnyUint; }
  bool getSpecifier() { return m_bSpecifier; }
  bool getValue() { return m_bValue; }
  bool getID() { return m_bID; }
  bool getError() { return m_bError; }
  bool getMetaData() { return m_bMetaData; }
  bool getLogPointer() { return m_bLogPointer; }
  bool getCmdIf() { return m_bCmdIf; }
  bool getLogConfig() { return m_bLogConfig; }

  /// Returns true if this object is followed by another LogConfig.
  bool hasNextConfig()
  {
    if(m_pNext != NULL)
      return true;
    else
      return false;
  }

  /// Returns whether the LogConfig was modified since the last reset or not.
  bool hasChanged()
  {
    return m_bHasChanged;
  }

  /// Returns the boolean value used with the last reset.
  bool getStandardSetting()
  {
    return m_bStandardSetting;
  }

  /// Shows whether the LogConfig is already used inside a stack or not.
  bool isActive()
  {
    return m_bIsActivated;
  }

  /// Reset all quarks of the LogConfig to true or false.
  void reset(bool bSet)
  {
    m_bHasChanged = false;
    m_bStandardSetting = bSet;

    m_bService = bSet;
    m_bTarget = bSet;
    m_bCmd = bSet;
    m_bAnyPointer = bSet;
    m_bAnyPointer2 = bSet;
    m_bAnyUint = bSet;
    m_bSpecifier = bSet; 
    m_bValue = bSet;
    m_bID = bSet;
    m_bError = bSet;
    m_bMetaData = bSet;
    m_bLogPointer = bSet;
    m_bCmdIf = bSet;
    m_bLogConfig = bSet;
  }

  /// Copy operator.
  LogConfig & operator=(const LogConfig &lc)
  {
    if(&lc == this)
      return *this;

    // Don't copy m_pNext or m_bIsActivated!!! This could compromise the internal stack. (C->B->A, A=C, ==> C->B->A->B->A...)
    m_bHasChanged = lc.m_bHasChanged;
    m_bStandardSetting = lc.m_bStandardSetting;

    m_bService = lc.m_bService;
    m_bTarget = lc.m_bTarget;
    m_bCmd = lc.m_bCmd;
    m_bAnyPointer = lc.m_bAnyPointer;
    m_bAnyPointer2 = lc.m_bAnyPointer2;
    m_bAnyUint = lc.m_bAnyUint;
    m_bSpecifier = lc.m_bSpecifier; 
    m_bValue = lc.m_bValue;
    m_bID = lc.m_bID;
    m_bError = lc.m_bError;
    m_bMetaData = lc.m_bMetaData;
    m_bLogPointer = lc.m_bLogPointer;
    m_bCmdIf = lc.m_bCmdIf;
    m_bLogConfig = lc.m_bLogConfig;

    return *this;
  }

  /// Returns a string representation of the LogConfig.
  std::string toString()
  {
    std::stringstream ss;
    ss << "    LogConfig " << this << ":" << std::endl
       << "      m_bHasChanged      = " << m_bHasChanged      << std::endl
       << "      m_bStandardSetting = " << m_bStandardSetting << std::endl
       << "      m_bService         = " << m_bService         << std::endl
       << "      m_bTarget          = " << m_bTarget          << std::endl
       << "      m_bCmd             = " << m_bCmd             << std::endl
       << "      m_bAnyPointer      = " << m_bAnyPointer      << std::endl
       << "      m_bAnyPointer2     = " << m_bAnyPointer2     << std::endl
       << "      m_bAnyUint         = " << m_bAnyUint         << std::endl
       << "      m_bSpecifier       = " << m_bSpecifier       << std::endl
       << "      m_bValue           = " << m_bValue           << std::endl
       << "      m_bID              = " << m_bID              << std::endl
       << "      m_bError           = " << m_bError           << std::endl
       << "      m_bMetaData        = " << m_bMetaData        << std::endl
       << "      m_bLogPointer      = " << m_bLogPointer      << std::endl
       << "      m_bCmdIf           = " << m_bCmdIf           << std::endl
       << "      m_bLogConfig       = " << m_bLogConfig       << std::endl
       << "      m_pNext            = " << m_pNext;
    return ss.str();
  }

protected:

  /// Set the next LogConfig to set up the stack.
  void setNextConfig(LogConfig* pConfig)
  {
    m_pNext = pConfig;
  }

  /// Get the next LogConfig object.
  LogConfig* getNextConfig()
  {
    return m_pNext;
  }

  /// Mark the LogConfig as active if it is used inside a stack.
  void setActive(bool bSet)
  {
    m_bIsActivated = bSet;
  }

protected:
  /// If the LogConfig has changed since the last reset.
  bool m_bHasChanged;

  /// The value of the last reset.
  bool m_bStandardSetting;

  /// If the LogConfig is already used inside a stack.
  /**
   * Is set, if this object is part of the internal stack structure by using it with ControlTransaction::activateConfig().
   * This is used to avoid multiple activations, which could lead to loops in the stack.
   */
  bool m_bIsActivated;

  // Boolean counterparts to the quarks in the ControlTransaction class. If these are set to true, the respective quarks are used by the GC_Logger output modules,
  // otherwise they are ignored. Filters can use this to advise output modules to ignore specific information. They are also used to mark if quarks have been
  // changed by the receiver of the transaction before it is returned, so the output modules are able to write out only quarks that changed during processing.
  /// If the mService quark of the transaction should be logged.
  bool m_bService;
  /// If the mTarget quark of the transaction should be logged.
  bool m_bTarget;
  /// If the mCmd quark of the transaction should be logged.
  bool m_bCmd;
  /// If the mAnyPointer quark of the transaction should be logged.
  bool m_bAnyPointer;
  /// If the mAnyPointer2 quark of the transaction should be logged.
  bool m_bAnyPointer2;
  /// If the mAnyUint quark of the transaction should be logged.
  bool m_bAnyUint;
  /// If the mSpecifier quark of the transaction should be logged.
  bool m_bSpecifier;
  /// If the mValue quark of the transaction should be logged.
  bool m_bValue;
  /// If the mID quark of the transaction should be logged.
  bool m_bID;
  /// If the mError quark of the transaction should be logged.
  bool m_bError;
  /// If the mMetaData quark of the transaction should be logged.
  bool m_bMetaData;
  /// If the mLogPointer quark of the transaction should be logged.
  bool m_bLogPointer;
  /// If the mCmdIf quark of the transaction should be logged.
  bool m_bCmdIf;
  /// If the mLogConfig quark of the transaction should be logged.
  bool m_bLogConfig;

  /// Pointer to another configuration, to work as an integrated stack.
  /**
   * Pointer to another configuration, to work as an integrated stack. So different filters can use their own configs and
   * restore the original settings afterwards.
   */
  LogConfig* m_pNext;
};


} // end namespace ctr
} // end namespace gs

#endif
