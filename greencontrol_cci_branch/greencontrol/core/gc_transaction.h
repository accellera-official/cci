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

#ifndef __GC_TRANSACTION_H__
#define __GC_TRANSACTION_H__

#include <sstream>

#include "boost/shared_ptr.hpp"
#include "transport/unevenpair.h"
#include "transport/gs_datatypes.h"

// for output:
#ifdef GC_VERBOSE
# include "greencontrol/gcnf/plugin/gcnf_datatypes.h"
#endif


namespace gs {
namespace ctr {

using boost::shared_ptr;
  
// based on the GernericTransaction

/// Control service specifier.
/**
 * Control service to route the transactions in the GreenControl Core 
 * and announce supported service from API or plugin to the Core.
 */
enum ControlService {
  /// No service.
  NO_SERVICE=0,
  // Config Service, provided by ConfigPlugin
  CONFIG_SERVICE=1,
  // Analysis and Visibility Service
  AV_SERVICE=2
};

/// Static method to get the string representation of the ControlService enumeration.
/**
 * @param cs  ControlService enumeration.
 * @return    String representation of cs.
 */
static const string getControlServiceString(enum ControlService cs) {
  switch (cs) {
    case NO_SERVICE:
      return string("NO_SERVICE");
      break;
    case CONFIG_SERVICE:
      return string("CONFIG_SERVICE");
      break;
    case AV_SERVICE:
      return string("AV_SERVICE");
      break;
    default:
      return string("unknown service");
  }
}

/// Type of the address for APIs and plugins.
typedef void* cport_address_type;
   
/// Pointer
typedef void* AnyPointer;

/// Target object (e.g. parameter name).
typedef std::string ControlSpecifier;

/// Transported value.
typedef std::string ControlValue;

  

// //////////////   ControlTransaction   /////////////////////

/// Control transaction class.
/**
 * The default control transaction class is the container for submitted 
 * transactions. The command field is set with specialized enumerations
 * for the special service.
 */
class ControlTransaction
{
public:

  /// Constructor without parameters
  /**
   * The members are set to initial values.
   */
  ControlTransaction()
    : mService(NO_SERVICE),
      mCmd(0)
  {
    mTarget = 0;
    mValue  = "";
    mSpecifier = "";
    mError = 0;
    mAnyPointer = NULL;
    mAnyPointer2 = NULL;
    mMetaData = "";
  }

  /// Copy constructor.
  ControlTransaction(const ControlTransaction &t) {
    *this = t; // use copy-operator
  }

  /// Destructor.
  ~ControlTransaction() {
  }
    
protected:
  // quarks (also add in copy operator!)

  /// Command (equal to plugin address).
  ControlService mService; 
  /// Address of the target API or plugin (pointer address of the target gc_port).
  cport_address_type  mTarget;  
  /// Command enumeration, specialized by the service. Default for no command is '0'!
  unsigned int mCmd;
  /// void Pointer to be used for any kind of pointer transmission
  AnyPointer mAnyPointer;
  /// another void Pointer to be used for any kind of pointer transmission
  AnyPointer mAnyPointer2;
  /// unsigned int to be used for any kind of unsigned int transmission
  unsigned int mAnyUint;
  /// Specifier for specifier dependent on service and command (e.g. parameter name).
  ControlSpecifier  mSpecifier;  
  /// Transported data dependent on service and command (e.g. parameter value).
  ControlValue   mValue;   
  // ID (API's or plugin's address) of sending master.
  cport_address_type   mID;
  /// Error flag (mError != 0: error).
  unsigned int   mError;  
  /// String to hold meta data
  std::string   mMetaData;  

//     operator GenericTargetAccess() {
//       return reinterpret_cast<GenericTargetAccess&>(*this);
//     }
//     operator GenericMasterAccess() {
//       return reinterpret_cast<GenericMasterAccess&>(*this);
//     }

public:
  // quark access
  void set_mService(const ControlService& _mService){mService=_mService;}
  void set_mTarget(const cport_address_type& _mTarget){mTarget=_mTarget;}
  void set_mCmd(const unsigned int& _mCmd){mCmd=_mCmd;}
  void set_mAnyPointer(const AnyPointer& _mAnyPointer){mAnyPointer=_mAnyPointer;}
  void set_mAnyPointer2(const AnyPointer& _mAnyPointer2){mAnyPointer2=_mAnyPointer2;}
  void set_mAnyUint(const unsigned int _mAnyUint){mAnyUint=_mAnyUint;}
  void set_mSpecifier(const ControlSpecifier& _mSpecifier){mSpecifier=_mSpecifier;}
  void set_mValue(const ControlValue& _mValue){mValue=_mValue;}
  void set_mID(const cport_address_type& _mID){mID=_mID;}
  void set_mError(const unsigned int _mError){mError=_mError;}
  void set_mMetaData(std::string _mMetaData){mMetaData=_mMetaData;}

  const ControlService& get_mService() const {return mService; }
  const cport_address_type& get_mTarget() const {return mTarget; }
  const unsigned int& get_mCmd() const {return mCmd; }
  const AnyPointer& get_mAnyPointer() const {return mAnyPointer; }
  const AnyPointer& get_mAnyPointer2() const {return mAnyPointer2; }
  const unsigned int get_mAnyUint() const {return mAnyUint; }
  const ControlSpecifier& get_mSpecifier() const {return mSpecifier; }
  const ControlValue& get_mValue() const {return mValue; }
  const cport_address_type& get_mID() const {return mID; }
  const unsigned int get_mError() const {return mError; }
  std::string get_mMetaData() const {return mMetaData; }

  /// Resets a transaction so it can be reused.
  /**
   * mID is not resetted because it is set automatically in the port.
   *
   * Only reset quarks that need to be resetted!!
   */
  void reset() {
    // TODO: check if quarks are missing
    mService= NO_SERVICE; // Do not remove this reset. Ensured in documetation to be reseted
    //mCmd    = 0;
    //mTarget = 0;
    //mValue  = "";
    mAnyPointer = NULL;  // must be resetted because of the config command CMD_ADD_PARAM which is used by legacy APIs  with string and by current APIs with this pointer. If a legacy API gets a transaction for an add command which was used by another API with this pointer before, the legacy API will not reset the pointer but only set the specifier.
    //mAnyPointer2 = NULL;
    // not reset mAnyUint
    //mSpecifier = "";
    mError = 0; // Do not remove this reset. Ensured in documetation to be reseted
    mMetaData = ""; 
  }
  
  /// Copy operator.
  ControlTransaction & operator=(const ControlTransaction &t) {
    if (&t==this)
      return *this;

    mService= t.mService;
    mTarget = t.mTarget;
    mCmd    = t.mCmd;
    mAnyPointer = t.mAnyPointer;
    mAnyPointer2 = t.mAnyPointer2;
    mAnyUint = t.mAnyUint;
    mSpecifier = t.mSpecifier;
    mValue  = t.mValue;
    mID     = t.mID;
    mError  = t.mError;
    mMetaData  = t.mMetaData;

    // TODO: check if quarks are missing
    
    return *this;
  }

  /// Get human readable string of the transaction.
  std::string toString() const { 
    std::stringstream ss;
    ss << "  Transaction " << this << ":"  << std::endl 
       << "    mService   = " << mService << " ("<< getControlServiceString(mService) <<")"  << std::endl
       << "    mTarget    = " << mTarget      << std::endl 
       << "    mCmd       = " << mCmd; 
#ifdef GC_VERBOSE
# ifdef __GCNF_DATATYPES_H__
    if (mService == CONFIG_SERVICE)
      ss << " (" << cnf::gcnfCommandToString(mCmd) << ")";
# endif
# ifdef __gav_datatypes_h__
    if (mService == AV_SERVICE)
      ss << " (" << av::gavCommandToString(mCmd) << ")";
# endif
#endif
    ss << std::endl
       << "    mAnyPointer = " << mAnyPointer   << std::endl
       << "    mAnyPointer2= " << mAnyPointer2  << std::endl
       << "    mAnyUint    = " << mAnyUint     << std::endl
       << "    mSpecifier  = " << mSpecifier   << std::endl 
       << "    mValue      = " << mValue       << std::endl 
       << "    mID         = " << mID          << std::endl 
       << "    mMetaData   = " << mMetaData          << std::endl 
       << "    mError      = " << mError;
    return ss.str();
  }

  // TODO: GenericTransaction in generic.static_casts.h provides more functionality
  
};




// //////////////   ControlPhase   /////////////////////////////////////

/// Phases of the communication with transaction containers.
/**
 * Phase of the communication with access methods.
 */
class ControlPhase
{

public:

  /// Enumeration of phases
  /**
   * usage: ControlPhase::IDLE ...
   */
  enum {      
    /// Idle: no communication.  
    IDLE = 0,
    /// Request phase.
    CONTROL_REQUEST,
    /// Response phase (with response data) as reaction to a request phase.
    CONTROL_RESPONSE,
    /// Error phase indicates a communication error.
    CONTROL_ERROR
  };

  // member variables (also add in copy operator!)
  /// Actual phase of this phase object.
  int state;

  /// Constructor without parameters: standart phase is IDLE.
  ControlPhase()     : state(IDLE) {};
  /// Constructor with phase.
  ControlPhase(int s): state(s)    {};

  /// Copy constructor; uses copy-operator.
  ControlPhase(const ControlPhase &p) {
    *this = p; // use copy-operator
  }

  /// Copy operator.
  ControlPhase & operator=(const ControlPhase &p) {
    if (&p==this)
      return *this;
    state=p.state;
    return *this;
  }

  /// Get a string description of the current phase.
  std::string toString() { 
    if (state == IDLE) return std::string("Idle");
    else if (state == CONTROL_REQUEST) return std::string("ControlRequest");
    else if (state == CONTROL_RESPONSE) return std::string("ControlResponse");
    else if (state == CONTROL_ERROR) return std::string("ControlError");
    else {
      std::stringstream ss;
      ss << "unknown phase (state=" << state << ")";
      return ss.str();
    }
  }
  
};



/// Shared pointer type for GenericTransaction. 
typedef shared_ptr<ControlTransaction>               ControlTransactionHandle;
typedef gs::ctr::unevenpair<ControlTransactionHandle,ControlPhase> ControlTransactionContainer;


} // end namespace ctr
} // end namespace gs   
  
#endif
