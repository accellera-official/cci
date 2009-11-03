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

#ifndef __CONTROLADDRESSMAP_H__
#define __CONTROLADDRESSMAP_H__

#include <map>

#include "gc_globals.h"
#include "gc_transaction.h"
//#include "initialize_if.h"


namespace gs {
namespace ctr {
  

/// Address map for the GreenControl Core.
/**
 * Address map for the GreenControl Core which resolves the
 * addresses (pointer to target APIs) to the port number
 */
class ControlAddressMap {
  
  typedef sc_export<payload_event_queue_if<ControlTransactionContainer> > * destinationPort;  

  typedef std::map<cport_address_type, unsigned int>    addressMapType;
  typedef std::map<cport_address_type, ControlService>  addressServiceMapType;
  typedef std::map<cport_address_type, std::string>     addressNameMapType;
  typedef std::multimap<ControlService, cport_address_type>  serviceAddressMapType;

public:

  ControlAddressMap() {}

  /// Destructor.
  ~ControlAddressMap() {
  }

  /// Generate the address map.
  /**
   * Generate an address map from the connected targets.
   *
   * @param port The control_initiator_port which has the target ports.
   */
  void generateMap(control_initiator_multi_port* port)
  {
    if(port == NULL) 
    {
      SC_REPORT_ERROR("ControlAddressMap", "Null pointer passed to generateMap(port).");
    }
    
    GC_DUMP_N("ControlAddressMap", "Generating address map:");
    GC_DUMP_N("ControlAddressMap", "Number of connected slaves: "<<(int)( *port ).connected_in_ports.size());
    
    // Go through all connected ports of the multi_port and insert them in one of the addressMaps
    //  read out the supported service and plugin names 
    for (unsigned int i=0; i<(*port).connected_in_ports.size(); i++) {
      // get target_port
      destinationPort destination = (*port).connected_in_ports[i];

      // get name
      sc_object *o = dynamic_cast<sc_object*>(destination->get_parent());
      if (!o->get_parent()){
        SC_REPORT_WARNING("ControlAddressMap", "Port has no parent..");
      }

      control_target_port *tport = dynamic_cast<control_target_port*>(o);
      //control_initiator_port *iport = dynamic_cast<control_initiator_port*>(o);
      if (tport) // TODO: is this if needed?
        insert_port(tport, i);

    }
  }

  /// Inserts the target port into the address map.
  /**
   * Gets supported ConstrolService, plugin or API name etc. and
   * inserts it with port i into map.
   *
   * @param tport  Target port which should be inserted.
   * @param i      The number of the port (in the core) which belongs to the address.
   */
  void insert_port(control_target_port *tport, unsigned int i) {
    const char* target_name = NULL;
    cport_address_type target_address;
    bool is_plugin;
    ControlService service = NO_SERVICE;

      // get supported ControlService  and  plugin name or API name
      if (tport) {
        service = tport->m_supported_ControlService;
        target_address = tport->m_parent_target_address;
        target_name = tport->m_parent_name.c_str();
        is_plugin = tport->m_is_plugin;

        GC_DUMP_N("ControlAddressMap", "           Target ["<<target_name<<"] is connected to port number "<<i);
        GC_DUMP_N("ControlAddressMap", "                Target port supports control service "<<service<<" ("<<getControlServiceString(service).c_str()<<")");
        if (!tport->m_is_plugin) {
          GC_DUMP_N("ControlAddressMap", "                Target port belongs to plugin "<<tport->m_parent_name.c_str() );
        }
        
        // insert into map if not yet in it
        if (!is_in_maps(target_address)) {
          GC_DUMP_N("ControlAddressMap", "                Insert port in map.");
          insert(service, target_address, i, target_name, is_plugin); 
        } else {
          GC_DUMP_N("ControlAddressMap", "                Port is already in map.");
        }
      }
      /*else if (iport) {
        service = iport->m_supported_ControlService;
        GC_DUMP_N("ControlAddressMap", "                Initiator port supports control service "<<service );
        if (!iport->m_plugin_name.empty()) {
        GC_DUMP_N("ControlAddressMap", "                Initiator port belongs to plugin (gc_service.)%s", 
        iport->m_plugin_name.c_str() );
        std::stringstream ss;
        ss << "gc_service." << iport->m_plugin_name;
        target_name = ss.str().c_str();
        }
        if (!iport->m_api_type.empty()) {
        // TODO
        }
        }*/
      else {
        SC_REPORT_WARNING("ControlAddressMap", "Unexpected port type (no control_target_port or control_initiator_port).");
      }
  }

  /// Print the address map.
  /**
   * Prints the address map including debug information.
   */
  void dumpMap()
  {
    std::cout<<"  ControlAddressMap: printing the map:"<<std::endl<<std::flush;
    std::string name;
    ControlService service;
    cport_address_type addr;
    addressNameMapType::iterator nameIter;
    addressServiceMapType::iterator serviceIter;
    addressMapType::iterator iter;
    for( iter = m_address_port_map.begin(); iter != m_address_port_map.end(); iter++ ) {
      addr = iter->first;
      serviceIter = m_address_service_map.find(addr);
      service = serviceIter->second;
      nameIter = m_address_name_map.find(addr);
      name = nameIter->second;
      std::cout << "       Address: " << iter->first << ",  port: " << iter->second <<
        ", name: " << name << ", control service: "<< service << " ("<< getControlServiceString(service) << ")" << std::endl;
    }
  }
 
  /// Decode an address to a port.
  /**
   * If the address is '0' the decoding is done with the service.
   *
   * @param _addr    A target address.
   * @param service  The expected service.
   * @return The decoded target port number.
   */
  std::vector<unsigned int> decode(const cport_address_type _addr, ControlService service)
  {
    cport_address_type addr;
    std::vector<unsigned int> ret_vec_int;
    if (_addr == 0) {
      //GC_DUMP_N("ControlAddressMap", "Service "<<service<<" is supported by address "<<addr);
      return decode(service);
    } else {
      addr = _addr;
    }

    if (m_address_port_map.count(addr) < 1) {
      dumpMap();
      std::stringstream ss;
      ss << "Decode attempt for address "<< addr << " failed. Address not in port map.\n";
      SC_REPORT_ERROR("ControlAddressMap", ss.str().c_str());
    }
    addressMapType::iterator iter = m_address_port_map.find(addr);

    ret_vec_int.push_back(iter->second);
    return ret_vec_int;
  }

  /// Decode a service to a port(s).
  /**
   * @param service  The expected service.
   * @return The decoded target port number in a vector.
   */
  std::vector<unsigned int> decode(ControlService service)
  {
    std::pair<serviceAddressMapType::iterator, serviceAddressMapType::iterator> addr_range;
    serviceAddressMapType::iterator serv_addr_iter;
    addressMapType::iterator addr_port_iter;
    std::vector<unsigned int> ret_vec_int;

    addr_range = getAddrFromService(service);

    // Actually this check is not needed, it has already been performed in getAddrFromService()
    if ( addr_range.first == addr_range.second ) {
      //dumpMap();
      GC_DUMP_N("ControlAddressMap", "Decode attempt for service "<<service<<" failed:\n");
      SC_REPORT_ERROR("ControlAddressMap", "Service not in name map.");
    }

    for (serv_addr_iter = addr_range.first; serv_addr_iter!=addr_range.second; ++serv_addr_iter) 
    {
      addr_port_iter = m_address_port_map.find(serv_addr_iter->second);
      ret_vec_int.push_back(addr_port_iter->second);
    }

    // return all the port number in a vector
    return ret_vec_int;
  }

  /// Decode an address to it's supported control service.
  /**
   * @param addr A target address.
   * @return The decoded control service.
   */
  const ControlService getService(const cport_address_type addr)
  {
    if (m_address_service_map.count(addr) < 1) {
      //dumpMap();
      GC_DUMP_N("ControlAddressMap", "Decode attempt (service) for address "<<addr<<" failed:\n");
      SC_REPORT_ERROR("ControlAddressMap", "Address not in service map.");
    }
    addressServiceMapType::iterator iter = m_address_service_map.find(addr);
    return iter->second;
  }

  /// Decode an address to the parent's name.
  /**
   * @param addr A target address.
   * @return The decoded name.
   */
  const std::vector<std::string> getName(const cport_address_type addr)
  {
    std::vector<std::string> ret_vec_str;
    if (m_address_name_map.count(addr) < 1) {
      //dumpMap();
      GC_DUMP_N("ControlAddressMap", "Decode attempt (name) for address "<<addr<<" failed:\n");
      SC_REPORT_ERROR("ControlAddressMap", "Address not in name map.");
    }
    addressNameMapType::iterator iter = m_address_name_map.find(addr);
    ret_vec_str.push_back(iter->second);
    return ret_vec_str;
  }

  /// Decode an address and service to the parent's name.
  /**
   * @param _addr    A target address.
   * @param service  The service of the searched target.
   * @return The decoded name.
   */
  const std::vector<std::string> getName(const cport_address_type _addr, ControlService service)
  {
    cport_address_type addr = _addr;
    std::vector<std::string> ret_vec_str;
    if (addr == 0) {
      //addr = getAddrFromService(service);
      return getName(service);
    }
    if (m_address_name_map.count(addr) < 1) {
      //dumpMap();
      GC_DUMP_N("ControlAddressMap", "Decode attempt (name) for address "<<addr<<" failed:\n");
      SC_REPORT_ERROR("ControlAddressMap", "Address not in name map.");
    }
    addressNameMapType::iterator iter = m_address_name_map.find(addr);
    ret_vec_str.push_back(iter->second);
    return ret_vec_str;
  }

  /// Decode a service to the parent's name.
  /**
   * @param service  The service of the searched target.
   * @return The decoded name of all plugin name in a vector 
   */
  const std::vector<std::string> getName(ControlService service)
  {
    std::pair<serviceAddressMapType::iterator, serviceAddressMapType::iterator> addr_range;
    serviceAddressMapType::iterator serv_addr_iter;
    addressNameMapType::iterator addr_name_iter;
    std::vector<std::string> ret_vec_str;
    
    addr_range = getAddrFromService(service);

    // Actually not needed, this check has been performed in getAddrFromService()
    if ( addr_range.first == addr_range.second ) {
      //dumpMap();
      GC_DUMP_N("ControlAddressMap", "Decode attempt for service "<<service<<" failed:\n");
      SC_REPORT_ERROR("ControlAddressMap", "Service not in name map.");
    }

    for (serv_addr_iter = addr_range.first; serv_addr_iter!=addr_range.second; ++serv_addr_iter) 
    {
      addr_name_iter = m_address_name_map.find(serv_addr_iter->second);
      ret_vec_str.push_back(addr_name_iter->second);
    }

    // return all the service names in a vector
    return ret_vec_str;
  }

  /// Decode an service to the address of the supporting plugin.
  /**
   * @param service Service to which the address should be returned.
   * @return The address range which belongs to all the plugins which supports the service.
   */
  std::pair<serviceAddressMapType::iterator, serviceAddressMapType::iterator> getAddrFromService(ControlService service)
  {
    if (m_service_address_map.count(service) < 1) {
      //dumpMap();
      GC_DUMP_N("ControlAddressMap", "Decode attempt (service->address) for service "<<service<<" failed:\n");
      std::stringstream ss;
      ss << "Service '" << getControlServiceString(service) << "' ("<< (unsigned int)service <<") is not available. Please instantiate plugin first.";
      SC_REPORT_ERROR("ControlAddressMap", ss.str().c_str());
    }
    return m_service_address_map.equal_range(service);
  }

protected:

  /// Insert a slave into the address map.
  /**
   * - Add the port to the address-port-map.
   * - Add the address with the service to the address-service-map.
   * - Add the address with the name to the address-name-map.
   *
   * @param service      The service the port provides.
   * @param addr         The address of the port (of the port's parent).
   * @param port_number  The number of the port which belongs to the address.
   * @param target_name  Name of the port's parent (for debug information).
   * @param is_plugin    If the slave port belongs to a plugin.
   */
  void insert(ControlService service, const cport_address_type addr, 
              unsigned int port_number, const char* target_name, bool is_plugin)
  {
    if (m_address_port_map.count(addr) > 0) {
      SC_REPORT_ERROR("ControlAddressMap", "insert: Address already exists.");
    }
    // add the entry to the different maps
    m_address_port_map.   insert( addressMapType::       value_type(addr, port_number)  );
    m_address_service_map.insert( addressServiceMapType::value_type(addr, service)     );
    m_address_name_map.   insert( addressNameMapType::   value_type(addr, target_name) );

    if (is_plugin) {
      if (m_service_address_map.count(service) > 0) {
        SC_REPORT_INFO("ControlAddressMap", "insert: Service already exists.");
      }
      // add the target address to the service map
      m_service_address_map.insert( serviceAddressMapType::value_type(service, addr));
    }
  }

  /// Internal method to check if port is already in the maps.
  /**
   * @param addr   Address of the target port which should be checked.
   */
  bool is_in_maps(const cport_address_type addr) const {
    if (m_address_port_map.count(addr) > 0) 
      return true;
    else
      return false;
  }

protected:

  /// Map to resolve address -> ConfigService for debug and future use.
  addressServiceMapType m_address_service_map;

  /// Map to resolve ConfigService -> address
  serviceAddressMapType m_service_address_map;

  /// Map to resolve target address -> port number.
  addressMapType m_address_port_map;

  /// Map to resolve target address -> target name.
  addressNameMapType m_address_name_map;

};


} // end namespace ctr
} // end namespace gs

#endif
