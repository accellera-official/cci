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

#ifndef __CONTROLPORTS_H__
#define __CONTROLPORTS_H__

// These port use the tlm ports without gs_params inside them (see tlm_port.h) to provide control_ports

#include "gc_globals.h"

#include "transport/gc_b_if.h"
#include "transport/gc_base_port.h"

#include "transport/gc_memoryutils.h"

#include "gc_transaction.h"


namespace gs {
namespace ctr {
  

using gs::shared_ptr;
  
/// Target port for APIs to connect to GC Core.
/**
 * The control_target_port is the port that is provided by IPs and plugins to allow
 * the GreenControl Core to connect to.
 */
class control_target_port 
: public gc_base_port<gc_b_if<ControlTransactionHandle>, ControlTransactionContainer>,
  public gc_b_if<ControlTransactionHandle> // for binding of gc_b_if (sc_export)
{
  // Note: This Port needs no target address because during binding to
  //       the GreenControl Core the address can be saved in a map.
  
public:
  // member variables needed for Core routing
  /// Supported ControlService.
  ControlService m_supported_ControlService; // needed for binding to GreenControl Core
  /// Name of the parent API or Plugin (for debugging).
  std::string m_parent_name;
  /// Unique address for the parent API or plugin, to be read of GC Core (to be able to route transactions to the correct API or plugin).
  cport_address_type m_parent_target_address;
  /// If the owner is a plugin (to be read of GC Core).
  bool m_is_plugin;

public:

  typedef gc_base_port<gc_b_if<ControlTransactionHandle>, ControlTransactionContainer> base_type;

  // Contructor with information to be saved in the control port to be read of core.
  /**
   * Constructor with supported 
   * - ControlService,
   * - parent's target address,
   * - if is plugin and
   * - parent API's/plugin's name.
   *
   * @param port_name          Name of the control port.
   * @param supported_service  Used by the API or plugin to announce the supported service.
   * @param par_addr           Unique address of the parent API or plugin.
   * @param is_plugin          If the parent is a plugin (or not, e.g. an API).
   * @param par_name           Name of the parent - used for debugging.
   */
  control_target_port( sc_core::sc_module_name port_name, ControlService supported_service,
                       const cport_address_type par_addr, bool is_plugin, const std::string par_name)
    : base_type(port_name),
      m_supported_ControlService(supported_service),
      m_parent_name(par_name),
      m_parent_target_address(par_addr),
      m_is_plugin(is_plugin),
      mIsBound(false)
  {
    base_type::bind_b_if(*this);
    //base_type::bind_b_if(tlm_master_b_transact_error); // does not work with DUST, because reference between b_out and b_in cannot be resolved
  }

  // functions
  
  /* /// Get a shared_ptr to the current transaction container.
     ControlTransactionHandle get_transaction() {
     boost::shared_ptr<void> im = boost::static_pointer_cast<void>(this->get_payload().first); // simulate reinterpret_cast
     return boost::static_pointer_cast<ControlTransaction>(im);
     //return boost::static_pointer_cast<ControlTransaction>(this->get_payload().first);
     }*/
  
  /*/// Get the current phase of this port.
    ControlPhase get_phase() {
    return this->get_payload().second;
    }*/
    
  // einzige Methode, die fuer gc_b_if notwendig ist
  /// The gc_b_if PV transaction implementation is not implemented.
  inline virtual void b_transact(ControlTransactionHandle th) {
    SC_REPORT_ERROR(name(), "react(): blocking transaction not supported");
  }

  /// Mark this port as bound.
  /**
   * This method should be invoked if the port is bound (e.g. to the Core).
   */
  void markAsBound() {
    mIsBound = true;
  }

  /// Returns if the port is already bound.
  /**
   * @return If the port is bound.
   */
  bool is_Bound() const {
    return mIsBound;
  }

protected:

  /// Marks if the port is already bound.
  bool mIsBound;

};


/// Target multi port for the GC Core.
/**
 * The control_target_multi_port is the port that is provided by the GreenControl Core
 * where IPs and plugins can connect to
 */
class control_target_multi_port 
: public gc_multi_base_port<gc_b_if<ControlTransactionHandle>, ControlTransactionContainer>,
  public gc_b_if<ControlTransactionHandle>
{ 
  // Note: This Port needs no target address because during binding to
  //       the GreenControl Core the address can be saved in a map.

public:

  typedef gc_multi_base_port<gc_b_if<ControlTransactionHandle>, ControlTransactionContainer> base_type;
  
  /// Contructor with port name.
  control_target_multi_port( sc_core::sc_module_name port_name ) :
    base_type(port_name)
  {
    base_type::bind_b_if(*this);
  }
  
  // functions
  
  /*/// Get a shared_ptr to the current transaction container.
    ControlTransactionHandle get_transaction() {
    boost::shared_ptr<void> im = boost::static_pointer_cast<void>(this->get_payload().first); // simulate reinterpret_cast
    return boost::static_pointer_cast<ControlTransaction>(im);
    //return boost::static_pointer_cast<ControlTransaction>(this->get_payload().first);
    }

    /// Get the current phase of this port.
    ControlPhase get_phase() {
    return this->get_payload().second;
    }*/

  /// Rejects usage of PV (we want to use the peq).
  void b_transact( ControlTransactionHandle ) {
    SC_REPORT_ERROR( sc_core::SC_ID_INTERNAL_ERROR_, "Tryed to access blocking interface from a master as a target" );
  }

  /*/// Returns the default event.
    virtual const sc_event& default_event()  {
    return base_type::default_event();
    }*/

};

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

/// Initiator port for APIs to connect to GC Core.
/**
 * The control_initiator_port is the port that is used by the IPs and plugins
 * to access the GreenControl Core
 */
class control_initiator_port 
: public gc_base_port<gc_b_if<ControlTransactionHandle>, ControlTransactionContainer>,
  public gc_b_if<ControlTransactionHandle >
{
public:
  // member variables needed for Core routing
  /// Supported ControlService.
  ControlService m_supported_ControlService; // needed for binding to GreenControl Core
  /// Name of the parent API or Plugin (for debugging).
  std::string m_parent_name;
  /// Unique address for the parent API or plugin, to be read of GC Core (to be able to route transactions to the correct API or plugin).
  cport_address_type m_parent_target_address;
  /// If the owner is a plugin (to be read of GC Core).
  bool m_is_plugin;

public:

  typedef gc_base_port<gc_b_if<ControlTransactionHandle>, ControlTransactionContainer> base_type;

  // Contructor with information to be saved in the control port to be read of core.
  /**
   * Constructor with supported 
   * - ControlService,
   * - parent's target address,
   * - if is plugin and
   * - parent API's/plugin's name.
   *
   * @param port_name          Name of the control port.
   * @param supported_service  Used by the API or plugin to announce the supported service.
   * @param par_addr           Unique address of the parent API or plugin.
   * @param is_plugin          If the parent is a plugin (or not, e.g. an API).
   * @param par_name           Name of the parent - used for debugging.
   */
  control_initiator_port( sc_core::sc_module_name port_name, ControlService supported_service,
                          const cport_address_type par_addr, bool is_plugin, const std::string par_name)
    : base_type(port_name),
      m_supported_ControlService(supported_service),
      m_parent_name(par_name),
      m_parent_target_address(par_addr),
      m_is_plugin(is_plugin),
      mIsBound(false)
  {
    // mMasterPortNumber = (gs_uint32) &this->in;
    mMasterPortNumber = par_addr;
#ifdef DUST_ENABLE
    this->out.setMasterID(mMasterPortNumber);
#endif
    // bind gc_b_if
    base_type::bind_b_if(*this);
  }
  
  /*/// Returns the default event.
    virtual const sc_event& default_event()  {
    return base_type::default_event();
    }
  */

  /// Create a transaction.
  /**
   * Create a transaction out of pool.
   */
  ControlTransactionHandle create_transaction() {
    ControlTransactionHandle sp_tmp = TransactionFactory.create();
    sp_tmp->set_mID(mMasterPortNumber);
    return sp_tmp;
  }

  /*/// Get the current phase of this port.
    ControlPhase get_phase() {
    return this->get_payload().second;
    };*/

  /// Returns the port number of this port.
  void* get_master_port_number() const {
    return mMasterPortNumber;
  }
  
  /// Overwrite bind-operator '()' to save binding in mIsBound.
  void operator() (gc_multi_base_port<b_if_type,if_type>& other) {
    // the target multi ports is not needed to be marked
    // mark this port as bound
    mIsBound = true;
    // bind
    base_type::operator()(other);
  }

  /// Returns if the port is already bound.
  bool is_Bound() const {
    return mIsBound;
  }

protected:

  /// Rejects usage of PV (we want to use the peq).
  void b_transact( ControlTransactionHandle ) {
    SC_REPORT_ERROR( sc_core::SC_ID_INTERNAL_ERROR_, "Tried to access blocking interface from a master as a target" );
  }

  //ObjectFactory<ControlTransactionHandle> TransactionFactory;
  gs::ctr::ObjectFactory<ControlTransactionHandle> TransactionFactory; // faster factory from GreenBus

  /// Marks if the port is already bound.
  bool mIsBound;
  void* mMasterPortNumber;
    
};


/// Target multi port for the GC Core.
/**
 * The control_initiator_multi_port is the port that is used by the GreenControl Core
 * to access the IPs and plugins.
 */
class control_initiator_multi_port 
: public gc_multi_base_port<gc_b_if<ControlTransactionHandle>, ControlTransactionContainer>,
  public gc_b_if<ControlTransactionHandle>
{
public:

  typedef gc_multi_base_port<gc_b_if<ControlTransactionHandle>, ControlTransactionContainer> base_type;

  /// Constructor.
  control_initiator_multi_port( sc_core::sc_module_name port_name ) 
    : base_type(port_name)
  {
    base_type::bind_b_if(*this);
  }

  /*/// Returns the default event.
    virtual const sc_event& default_event() {
    return base_type::default_event();
    }
  */

  /// Overwrite bind-operator '()' to save binding in target port.
  void operator() (gc_base_port<b_if_type,if_type>& other) {
    // mark the target port as bound
    control_target_port *po = dynamic_cast<control_target_port*>( &other );
    if (po) {
      po->markAsBound();
    } else {
      SC_REPORT_ERROR( name(), "Bounded port is not a control_target_port!" );
    }
    // bind
    base_type::operator()(other);
  }
  
private:
  
  /// Rejects usage of PV (we want to use the peq).
  void b_transact( ControlTransactionHandle ) {
    SC_REPORT_ERROR( sc_core::SC_ID_INTERNAL_ERROR_, "Tried to access blocking interface from a master as a target" );
  }
  
};
    
  
} // end namespace ctr
} // end namespace gs

#endif
