//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007-2010 : GreenSocs Ltd
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

// doygen comments

#ifndef __WRONG_TRANSACTIONS_USER_API_H__
#define __WRONG_TRANSACTIONS_USER_API_H__

#include "greencontrol/config.h"

#define WRONG_TRANSACTIONS_USER_API_NAME "Wrong_Transactions_User_Api"

namespace gs {
namespace cnf {

using namespace ctr;

/// Simple User API which generates wrong transactions for negative regression tests.
class Wrong_Transactions_User_Api
: public sc_object,
  public initialize_if,
  public gc_port_if
{
  
public:

  SC_HAS_PROCESS(Wrong_Transactions_User_Api);
	
  /// Constructor
  Wrong_Transactions_User_Api()
    : sc_object(sc_gen_unique_name("__wrong_transactions_user_api__")),
      m_gc_port(CONFIG_SERVICE, WRONG_TRANSACTIONS_USER_API_NAME, false, this)
  { 
  }

  // //////////////////// GC_PORT_IF ///////////////////////////////////
  /**
   * Implements gc_port_if.
   * This method starts whenever a master sends a transaction.
   */
  void transport(ControlTransactionHandle &tr)
  {
    GCNF_DUMP_N(name(), "got transaction atom from master");      
    GCNF_DUMP_N(name(), "  received transaction: "<<(tr->toString()).c_str());      
  }

  // ////// API methods //////////////////////////////////////////////////

  ControlTransactionHandle getTransaction() {
    return m_gc_port.createTransaction();
  }

  /// Creates a transaction which has a (hopefully) not existing target field.
  bool sendTransaction(ControlTransactionHandle th) {
    GC_DUMP_N(name(), "sendTransaction()");
    bool success = false;

    GC_DUMP_N(name(), "createWrongTargetField: notify init_port");
    //ControlTransactionHandle th = m_gc_port.createTransaction();
    m_gc_port->transport(th);

    if (th->get_mError() == 0) {
      GC_DUMP_N(name(), "createWrongTargetField: ... successfull");
      success = true;
    } else {
      GC_DUMP_N(name(), "createWrongTargetField: ... failed (error "<<th->get_mError()<<")!");
      success = false;
    }
    
    return success;
  }


  // ////////////////////////////////////////////////////////////////////////////////
  // / deal the initialize-mode

  /// implements initialize_if 
  void start_initialize_mode() {
  }

  /// implements initialize_if 
  void end_initialize_mode() {
  }
  // ////////////////////////////////////////////////////////////////////////////////

private:
  /// gc_port to send transaction containers
  gc_port m_gc_port;

#ifdef GC_VERBOSE
  /**
   * Helpfunction for GC_DUMP_Ns
   */
  const char * name() {
    std::stringstream ss;
    ss << this->get_parent()->name() << SC_NAME_DELIMITER << "__Wrong_Transactions_User_Api__";
    return ss.str().c_str();
  }
#endif

};


} // end namespace cnf
} // end namespace gs

#endif
