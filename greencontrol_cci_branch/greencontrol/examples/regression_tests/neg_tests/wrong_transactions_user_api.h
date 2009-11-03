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

// doygen comments

#ifndef __WRONG_TRANSACTIONS_USER_API_H__
#define __WRONG_TRANSACTIONS_USER_API_H__

#include "greencontrol/core/gc_core.h"
#include "greencontrol/gcnf/plugin/gcnf_datatypes.h"

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
      m_gc_port(CONFIG_SERVICE, WRONG_TRANSACTIONS_USER_API_NAME, false)
  { 
    m_gc_port.api_port(*this);  // bind sc_port of m_gc_port
  }

  // //////////////////// GC_PORT_IF ///////////////////////////////////
  /**
   * Implements gc_port_if.
   * This method starts whenever a master triggers a payload-event.
   */
  void masterAccess(ControlTransactionContainer &t_p)
  {
    ControlTransactionHandle tr = t_p.first;
    ControlPhase ph = t_p.second;

    GC_DUMP_N(name(), "got "<<ph.toString().c_str()<<" atom from master");      
  }
  
  /**
   * Implements gc_port_if.
   * This method starts whenever a slave triggers a payload-event.
   */
  void slaveAccess(ControlTransactionContainer &t_p)
  {  
    ControlTransactionHandle tr = t_p.first;
    ControlPhase ph = t_p.second;

    GC_DUMP_N(name(), "got "<<ph.toString().c_str()<<" atom from slave");      

    switch (ph.state) {
    case ControlPhase::CONTROL_RESPONSE:
      {
        // processed directly in the method which initiated the transaction
        break;
      }
    case ControlPhase::CONTROL_ERROR:
      {
        SC_REPORT_WARNING(name(), "slaveAccess got phase ControlError!");
        break;
      }
    default:
      {
        SC_REPORT_WARNING(name(), "slaveAccess got not processed phase!");
      }
    }
  }

  // ////// API methods //////////////////////////////////////////////////

  ControlTransactionHandle getTransaction() {
    return m_gc_port.init_port.create_transaction();
  }

  /// Creates a transaction which has a (hopefully) not existing target field.
  bool sendTransaction(ControlTransactionHandle th) {
    GC_DUMP_N(name(), "sendTransaction()");
    bool success = false;

    ControlPhase p(ControlPhase::CONTROL_REQUEST);
    GC_DUMP_N(name(), "createWrongTargetField: notify init_port");
    ControlTransactionContainer ctc = ControlTransactionContainer(th,p);
    m_gc_port.init_port.out->notify(ctc);

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
  void start_initial_configuration() {
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
