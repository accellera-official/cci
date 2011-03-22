//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007-2009 : GreenSocs Ltd
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


#ifndef __gs_param_GenericTransaction_h__
#define __gs_param_GenericTransaction_h__

#ifndef USE_STATIC_CASTS
#  error transaction recording only works with the static cast variant. Please compile with '-DUSE_STATIC_CASTS'.
#endif

#include "greencontrol/config.h"

namespace gs {
namespace cnf {

  /// Class which allows gs_params which are a copy of the main attributes of tlm::gp::GenericTransaction
  class GenericTransactionCopy {
  public:
    tlm::MAddr mAddr;
    tlm::MID mID;
    tlm::MBurstLength mBurstLength, sBurstLength;    
    tlm::MSBytesValid msBytesValid; 
    tlm::MCmd mCmd;
    std::vector<gs_uint8> msData;
    tlm::SResp sResp;
    tlm::Error mError;
    tlm::Error sError;  
    tlm::TransID transID;
    bool is_extended;

    GenericTransactionCopy() {
    }
    
    GenericTransactionCopy& operator= (const tlm::gp::GenericTransaction& src_tr) {
      const tlm::gp::GenericSlaveAccess &src = const_cast<tlm::gp::GenericTransaction*>(&src_tr)->getTargetAccess();
      mAddr = src.getMAddr();
      mID = src.getMID();
      mBurstLength = src.getMBurstLength();
      sBurstLength = src.getMBurstLength();
      msBytesValid = src.getMSBytesValid();
      mCmd = src.getMCmd();
      msData = src.getMData().getData();
      sResp = src.getSResp();
      mError = src.getMError();
      sError = src.getSError();
      transID = src.getTransID();
      is_extended = src_tr.isExtended();
      return *this;
    }
    
    GenericTransactionCopy& operator= (const GenericTransactionCopy& src) {
      if (&src==this)
        return *this;
      mAddr = src.mAddr;
      mID = src.mID;
      mBurstLength = src.mBurstLength;
      sBurstLength = src.sBurstLength;
      msBytesValid = src.msBytesValid;
      mCmd = src.mCmd;
      msData = src.msData;
      sResp = src.sResp;
      mError = src.mError;
      sError = src.sError;
      transID = src.transID;
      is_extended = src.is_extended;
      return *this;
    }
    
    void reset() {
      mAddr = 0;
      mID = 0;
      mBurstLength = 0;
      sBurstLength = 0;
      msBytesValid = 0;
      mCmd = tlm::Generic_MCMD_IDLE;
      msData.clear();
      sResp = tlm::Generic_SRESP_NULL;
      mError = 0;
      sError = 0;
      transID = 0;
      is_extended = false;
    }
    
    std::string to_string() const {
      std::ostringstream ss;
      ss << "GenericTransactionCopy (transaction ID = " << transID << ")" << endl;
      ss << "    addr      = " << hex << mAddr << endl;
      ss << "    sender ID = " << hex << mID << endl;
      ss << "    command   = " << mCmd;
      switch ((tlm::GenericMCmdType)mCmd) {
        case tlm::Generic_MCMD_IDLE:
          ss << " (Idle)";
          break;
        case tlm::Generic_MCMD_WR:    
          ss << " (Write)";
          break;
        case tlm::Generic_MCMD_RD:
          ss << " (Read)";
          break;
        case tlm::Generic_MCMD_RDEX:
          ss << " (Exclusive read)";
          break;
        case tlm::Generic_MCMD_RDL:       
          ss << " (Read linked)";
          break;
        case tlm::Generic_MCMD_WRNP:
          ss << " (Non-posted write)";
          break;
        case tlm::Generic_MCMD_WRC: 
          ss << " (Write conditional)";
          break;
        case tlm::Generic_MCMD_BCST:
          ss << " (Broadcast)";
          break;
      }
      ss << endl;
      ss << "    burst length ma = " << dec << (gs_uint32)mBurstLength << ", sl = " << (gs_uint32)sBurstLength << ", valid = " << (gs_uint32)msBytesValid << endl;
      ss << "    response = " << (unsigned int)sResp << endl;
      ss << "    error ma = " << (gs_uint32)mError << ", sl = " << (gs_uint32)sError << endl;
      if (is_extended) 
        ss << "    extended = true" << endl;
      else
        ss << "    extended = false" << endl;
      ss << "    data: ";
      for (unsigned int i = 0; i < msData.size(); i++) {
        if (isprint(msData[i]))
          ss << msData[i];// << " ";
        else
          ss << "\\" << int(msData[i]);// << " ";
      }
      return ss.str();
    }
    
  };
  
  // /////////////////////////////////////////////////////////////////////////////// //
  // /////////////////// gs_param< GenericTransactionCopy > /////////////////// //
  // /////////////////////////////////////////////////////////////////////////////// //

  /// Template specialization for gs_param< GenericTransactionCopy >.
  /**
   * Default value = 0.
   */
  template<>
  class gs_param< GenericTransactionCopy >
  : public gs_param_t< GenericTransactionCopy >
  {
    /// Typedef for the value.
    typedef GenericTransactionCopy val_type;
    
  public:
    GS_PARAM_HEAD;
    
    // ///////////////////////
    //  operators
    
    /// Overloads gs_param_base::getTypeString
    const std::string getTypeString() const {
      return string("GenericTransactionCopy");
    }
    
    /// Overloads gs_param_base::getType
    const Param_type getType() const {
      return PARTYPE_NOT_AVAILABLE;
    }
    
    /// Operator= that allows GenericTransactions, Generic[Master|Slave|Router]Accesses to be assigned to this param
    my_type& operator= (const tlm::gp::GenericTransaction& src) {
      if (make_reject_write_callbacks() == return_value_change_rejected) {
        GS_PARAM_DUMP_WITHNAME("gs_param", "operator=: reject_write callback rejected value change!");
        throw string("Value change rejected"); // TODO: use exception
      } else {
        make_pre_write_callbacks();
        my_value = src;
        make_post_write_callbacks();
      }
      return *this;
    }
    
    /// Overloads gs_param_t<T>::serialize
    std::string serialize(const val_type &val) const {
      return static_serialize(val);
    }  
    inline static std::string static_serialize(const val_type &val) {
      return val.to_string();
    }

    /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
    inline static bool static_deserialize(val_type &target_val, const std::string& str) { 
      GS_PARAM_DUMP_WITHNAME("gs_param", "deserialize: string='"<< str.c_str()<<"'");
      if (str.length() == 0) {
        target_val.reset();
        GS_PARAM_DUMP_WITHNAME("gs_param", "getValue: reset transaction (default value)");
        return true;
      }
      // TODO
      SC_REPORT_ERROR("gs_param<GenericSlaveAccess>::deserialize()", "deserialize not implemented for GenericSlaveAccess");
      return false;
    }
    
  };
  
}} // end namespaces

#endif
