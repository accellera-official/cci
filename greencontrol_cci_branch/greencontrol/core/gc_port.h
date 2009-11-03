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

#ifndef __GC_PORT_H__
#define __GC_PORT_H__

#include "gc_globals.h"        // global settings
#include "gc_core.h"
#include "gc_port_if.h"

namespace gs {
namespace ctr {


/// Port to the GreenControl Core.
/**
 * Class gc_port is the port to the GreenControl Core.
 *
 *  It has a sc_port api_port which has to be bound by the API (either GCnf_Api or a User API adaptor) 
 *  to serve incoming transactions. Outgoing transactions can be sent with notify(transaction) in
 *  it's target and master ports.
 * 
 *  The target and initiator ports are tried to be bound automatically immediately by seaching the
 *  Control Core in the simulation context and calling the process_ports method.
 */
class gc_port
: public sc_object
{

  /// Connector to receive master calls from peq.
  /**
   * The master_connector's notify is called by the peq (where it is registered in GC_Core).
   * This makes the sensitivity of masterAccess dispensable and is FASTER than an event and
   * may be used during start_of_simulation because it avoids the usage of events.
   */
  class master_connector: public payload_event_queue_output_if<ControlTransactionContainer>{
  public:
    master_connector(gc_port* boss):m_boss(boss){}
    
    virtual void notify(ControlTransactionContainer& tp){
      m_boss->api_port->masterAccess(tp);
    }
    
  protected:
    gc_port* m_boss;
  };
  
  /// Connector to receive slave calls from peq.
  /**
   * The slave_connector's notify is called by the peq (where it is registered in GC_Core).
   * This makes the sensitivity of slaveAccess dispensable  is FASTER than an event and
   * may be used during start_of_simulation because it avoids the usage of events.
   */
  class slave_connector: public payload_event_queue_output_if<ControlTransactionContainer>{
  public: 
    slave_connector(gc_port* boss):m_boss(boss){}
    
    virtual void notify(ControlTransactionContainer& tp){
      m_boss->api_port->slaveAccess(tp);
    }
    
  protected:
    gc_port* m_boss;
  };
  

public:

  /// Port which has to be bound by the API.
  sc_port<gc_port_if> api_port;
  
  /// Control initiator port automatically connected to GC_Core.
  control_initiator_port   init_port;
  /// Control target port automatically connected to GC_Core.
  control_target_port      target_port;

  /// Constructor with information for control ports.
  /**
   * Constructor with information to be saved in the control ports to be read of core.
   *
   * Initiates automatic binding of the init and target ports by calling process_ports
   * in the Control Core (which is searched in the sim context).
   *
   * Must not be called after before end_of_elaboration.
   *
   * @param supported_service    Used by the plugin to announce the supported service.
   * @param api_OR_plugin_name   Name of the connected plugin or API, used for debug and future use.
   * @param is_plugin            Whether the connected module is a plugin.
   */
  gc_port(ControlService supported_service,
          const std::string api_OR_plugin_name, bool is_plugin)
    : sc_object(sc_gen_unique_name("__gc_port__")),
      init_port  (sc_gen_unique_name("ApiIPort"), supported_service, (cport_address_type) this, is_plugin, api_OR_plugin_name),
      target_port(sc_gen_unique_name("ApiTPort"), supported_service, (cport_address_type) this, is_plugin, api_OR_plugin_name),
      mMConn(this),
      mSConn(this)
  { 
    target_port.peq.out_port(mMConn); // binding of sc_port
    init_port.peq.out_port(mSConn);   // binding of sc_port

    // ensure Core is existing
    gs::ctr::GC_Core::get_instance();
    // Try to bind the target and init port to core and insert this port in address map.
    GC_Core_if* core;
    core = findCoreInSimContext(NULL);
    if (core) {
      core->process_ports();
    } else {
      SC_REPORT_WARNING(name(), "Cannot locate GC_Core instance in simulation context: automatic port binding and map generation will be done later. This port must not be used until then.");
    }
  }

protected:

  /// Returns a pointer to the GreenControl Core.
  /**
   * Allows immediate automatic binding and address mapping of the ports.
   */
  GC_Core_if *findCoreInSimContext(sc_object *node) {
    //cout << "findCoreInSimContext" << endl;
    // identify GC_Core
    GC_Core_if *co = dynamic_cast<GC_Core_if*>(node);
    if (co) {
      GC_DUMP_N(name(), "Found GC_Core");
      //cout << "Found GC_Core" << endl;
      return co;
    }
    
    // Kinder auslesen und alle durchsuchen
    const std::vector<sc_object *> *childs_stack = NULL;
    if (node) {
      //cout << "node is not NULL" << endl;
      // get child vector
#if SYSTEMC_API == 210
      // SystemC 2.1 Standard: (sec. 5.15) "Only module objects and process objects can have children."
      //cout << "node: name="<< node->name()<<", kind="<< node->kind() << endl;
      // Search in childs of the module
      sc_core::sc_module* node_as_module = dynamic_cast< sc_core::sc_module* >(node);
      if (node_as_module) childs_stack = &(node_as_module->get_child_objects());
      // sc_process_handle is not implemented standard conform in OCSI implementation
      // so we cannot search for process handles owning the core - which should never
      // happen anyway:
      //  sc_core::sc_process_handle node_as_process(node);
      //  if (node_as_process.valid()) childs_stack = &(node_as_process.get_child_objects());
#else
      childs_stack = &node->get_child_objects();
#endif
    } else { // Wenn NULL, dann Top-Level
      //cout << "start with top-level" << endl;
      // get child vector of sim context
#if SYSTEMC_API == 210
      sc_simcontext *sim; // deprecated with SystemC-2.2
      sim = sc_get_curr_simcontext(); // deprecated with SystemC-2.2
      childs_stack = &(sim->get_child_objects());
#else
      childs_stack = &sc_get_top_level_objects();
#endif
    }
    if (childs_stack != NULL) {
      // go through childs
      for (unsigned int i = 0; i < childs_stack->size(); i++) {
        sc_object *chnode = childs_stack->at(i);
        co = findCoreInSimContext(chnode);
        if (co) return co;
      }
    }
    return co;
  }


protected:

  /// Instance of master_connector which calls masterAccess in the api_port.
  master_connector mMConn;
  /// Instance of slave_connector which calls slaveAccess in the api_port.
  slave_connector  mSConn;

};


} // end namespace ctr
} // end namespace gs

#endif
