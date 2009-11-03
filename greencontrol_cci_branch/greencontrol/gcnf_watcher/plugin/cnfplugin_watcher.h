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
      m_gc_port(CONFIG_SERVICE, "ConfigPluginWatcher", true)
  { 
    m_gc_port.api_port(*this); // bind sc_port of m_gc_port

    // create the default GCnf_Api instance which is accessible via the NULL pointer
    GCnf_Api::getApiInstance(NULL);
  }

  /// Destructor
  ~ConfigPluginWatcher() {
  }

  // //////////   GC_PORT_IF   //////////////////////////////////// //

  /// Called by gc_port through gc_port_if when notification arrives.
  /**
   * Implements pc_port_if. 
   * This method starts whenever a master triggers a payload-event.
   */
  void masterAccess(ControlTransactionContainer &t_p)
  {
	//sc_object *port_addr; 
	sc_module *port_addr; 
	// control_target_port *port_addr; 

   	std::cout<< "ConfigPluginWatcher :: masterAccess called "  << std::endl;

	std::cout<< "Service name = " << gs::ctr::getControlServiceString((t_p.first)->get_mService()) << std::endl;

	port_addr = reinterpret_cast<sc_module * > ((t_p.first)->get_mID());
	//std::cout<< "ID = " << (t_p.first)->get_mID() << std::endl;
	//std::cout<< "Port Name = " << (port_addr->get_parent())->name() << std::endl;

	while (port_addr != NULL)
	{
		std::cout<< "Port Name = " << port_addr->name() << std::endl;
		port_addr = get_parent_sc_module(port_addr);
	}


  }
  
  /// Called by gc_port through gc_port_if when notification arrives.
  /**
   * Implements pc_port_if. 
   * This method starts whenever a slave triggers a payload-event.
   * That happens when one of the GC-API methods below send a transaction
   */
  void slaveAccess(ControlTransactionContainer &t_p)
  {  
   	std::cout<< "ConfigPluginWatcher :: slaveAccess called "  << std::endl;
  }

};

  
} // end namespace cnf
} // end namespace gs

#endif
