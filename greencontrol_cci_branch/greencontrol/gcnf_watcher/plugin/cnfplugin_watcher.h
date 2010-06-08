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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// doxygen comments

#ifndef __CONFIGPLUGIN_WATCHER_H__
#define __CONFIGPLUGIN_WATCHER_H__

#include <map>
#include <list>
#include <vector>
#include <set>

#include "../../gcnf/plugin/config_globals.h"

#include "greencontrol/core/gc_port_if.h"
#include "greencontrol/core/gc_port.h"
#include "../../gcnf/plugin/gcnf_datatypes.h"    // Command enumeration


namespace gs {
namespace cnf {

  
/// ConfigPluginWatcher to watch traffic on GreenConfig.
/**
 * This is the GreenControl ConfigPluginWatcher which registers itself with the CONFIG_SERVICE
 * with the greencontrol, so that it can watch the greenconfig traffic.
 */
class ConfigPluginWatcher
: public sc_core::sc_module,
  public gc_port_if
{

public:

  /// Port to communicate over the Core with the configurable modules.
  gc_port m_gc_port;
  
  /// Constructor
  /**
   * Constructor 
   * @param name  Name of the instance. 
   */
  ConfigPluginWatcher(sc_core::sc_module_name name)
    : sc_core::sc_module(name),
      m_gc_port(CONFIG_SERVICE, "ConfigPluginWatcher", true, this)
  { 
    // create the default GCnf_Api instance which is accessible via the NULL pointer
    GCnf_Api::getApiInstance(NULL);
  }

  /// Destructor
  ~ConfigPluginWatcher() {
  }

  // //////////   GC_PORT_IF   //////////////////////////////////// //

  /// Called by gc_port through gc_port_if when notification arrives.
  /**
   * Implements gc_port_if. 
   * This method starts whenever a master triggers a payload-event.
   */
  void transport(ControlTransactionHandle &tr)
  {
    //sc_object *port_addr; 
    //sc_module *port_addr; 
    // control_target_port *port_addr; 

    std::cout<< "ConfigPluginWatcher :: transport called "  << std::endl;

    std::cout<< "Service name = " << gs::ctr::getControlServiceString(tr->get_mService()) << std::endl;

    /* This does not work any longer with new gc_ports:
    port_addr = reinterpret_cast<sc_module * > (tr->get_mID());
    while (port_addr != NULL) {
      std::cout<< "Port Name = " << port_addr->name() << std::endl;
      port_addr = get_parent_sc_module(port_addr);
    }*/


  }
};

  
} // end namespace cnf
} // end namespace gs

#endif
