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
//   Wolfgang Klingauf <klingauf@eis.cs.tu-bs.de>,
//   Michael Ruetz <m.ruetz@tu-bs.de>
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

#ifndef __CONTROLADDRESSMAP_H__
#define __CONTROLADDRESSMAP_H__

#include <map>

#include "gc_globals.h"
#include "gc_transaction.h"


namespace gs {
namespace ctr {

// forward declarations for typedef
template <typename gc_port_T> class ControlAddressMap_T;
class gc_port;

/// ControlAddressMap
typedef ControlAddressMap_T<gc_port> ControlAddressMap;

/// Address map for the GreenControl Core.
/**
 * Address map for the GreenControl Core which resolves the addresses (pointer to target APIs) to the names and services
 * and the services to the addresses of the plugins.
 */
template <typename gc_port_T>
class ControlAddressMap_T
{
  typedef std::map<cport_address_type, ControlService>  addressServiceMapType;
  typedef std::map<cport_address_type, std::string>     addressNameMapType;
  typedef std::multimap<ControlService, cport_address_type>  serviceAddressMapType;

public:

  ControlAddressMap_T() {}

  /// Destructor.
  ~ControlAddressMap_T() {}

  /// Inserts the gc_port into the address map.
  /**
   * Gets supported ConstrolService, plugin or API name etc. and inserts it into map.
   *
   * @param pPort  gc_port which should be inserted.
   */
  void addPort(gc_port_T* pPort)
  {
    const char* target_name = NULL;
    cport_address_type target_address;
    bool is_plugin;
    ControlService service = NO_SERVICE;
    std::string target_string;

    // get supported ControlService  and  plugin name or API name
    if(pPort)
    {
      target_address = pPort->getParent();
      target_string = pPort->getParentName(); // a temp var is used due to limitations caused by forward template declaration of gc_port
      target_name = target_string.c_str();    // otherwise target_name won't be filled
      service = pPort->getSupportedControlService();
      is_plugin = pPort->isPlugin();

      if(pPort->isPlugin()) {
        GC_DUMP_N("ControlAddressMap", "           Target-Plugin ["<<target_name<<"] is connected to core");
      }
      else {
        GC_DUMP_N("ControlAddressMap", "           Target-API ["<<target_name<<"] is connected to core");
      }
      GC_DUMP_N("ControlAddressMap", "                Target port supports control service "<<service<<" ("<<getControlServiceString(service).c_str()<<")");
        
      // insert into map if not yet in it
      if(!is_in_maps(target_address))
      {
          GC_DUMP_N("ControlAddressMap", "                Insert port in map.");
          insert(service, target_address, target_name, is_plugin); 
      }
      else
        GC_DUMP_N("ControlAddressMap", "                Port is already in map.");
    }
    else
      SC_REPORT_WARNING("ControlAddressMap", "Tried to add gc_port, but was NULL.");
  }

  /// Removes the gc_port from the address map.
  /**
   * @param pPort gc_port which should be removed.
   */
  void removePort(gc_port_T* pPort)
  {
    const char* target_name = NULL;
    cport_address_type target_address;
    ControlService service = NO_SERVICE;
    std::string target_string;

    if(pPort)
    {
      target_address = pPort->getParent();
      target_string = pPort->getParentName(); // a temp var is used due to limitations caused by forward template declaration of gc_port
      target_name = target_string.c_str();    // otherwise target_name won't be filled
      service = pPort->getSupportedControlService();

      if(pPort->isPlugin())
        GC_DUMP_N("ControlAddressMap", "           Plugin ["<<target_name<<"] supporting control service "<<service<<" ("<<getControlServiceString(service).c_str()<<") is removed from core");
      else
        GC_DUMP_N("ControlAddressMap", "           API ["<<target_name<<"] supporting control service "<<service<<" ("<<getControlServiceString(service).c_str()<<") is removed from core");

      // remove port from internal maps
      if(m_address_service_map.erase(target_address) != 1)
        SC_REPORT_WARNING("ControlAddressMap", "Tried to remove gc_port, but was not in address/service map.");
      if(m_address_name_map.erase(target_address) != 1)
        SC_REPORT_WARNING("ControlAddressMap", "Tried to remove gc_port, but was not in address/name map.");

      // service/address map contains only plugins
      if(pPort->isPlugin())
      {
        serviceAddressMapType::iterator it, lastElement;
        // find first and last element (since this is a multimap)
        it = m_service_address_map.find(service);
        if(it == m_service_address_map.end())
          SC_REPORT_WARNING("ControlAddressMap", "Tried to remove gc_port, but was not in service/address map.");
        else
        {
          lastElement = m_service_address_map.upper_bound(service);
          // find the one element containing the target_address of the gc_port
          // iterator is increased by hand in for-loop, because erase() invalidates the iterator
          for( ; it != lastElement; )
          {
            if(it->second == target_address)
              m_service_address_map.erase(it++);
            else
              ++it;
          }
        }
      }
    }
    else
      SC_REPORT_WARNING("ControlAddressMap", "Tried to remove gc_port, but was NULL.");
  }

  /// Method to check if there are any ports connected.
  bool isEmpty()
  {
    return m_address_service_map.empty();
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
    addressServiceMapType::iterator iter;
    for( iter = m_address_service_map.begin(); iter != m_address_service_map.end(); ++iter )
    {
      addr = iter->first;
      service = iter->second;
      nameIter = m_address_name_map.find(addr);
      name = nameIter->second;
      std::cout << "       Address: " << iter->first << ", name: " << name << ", control service: "<< service
        << " ("<< getControlServiceString(service) << ")" << std::endl;
    }
  }
 
  /// Decode a service to an address.
  /**
   * @param service  The expected service.
   * @return The decoded target addresses in a vector (might be empty).
   */
  std::vector<cport_address_type> decode(ControlService service)
  {
    std::pair<serviceAddressMapType::iterator, serviceAddressMapType::iterator> addr_range;
    serviceAddressMapType::iterator serv_addr_iter;
    std::vector<cport_address_type> ret_vec_addr;

    addr_range = getAddrFromService(service);

    // Actually this check is not needed, it has already been performed in getAddrFromService()
    //if ( addr_range.first == addr_range.second ) {
      //dumpMap();
    //  GC_DUMP_N("ControlAddressMap", "Decode attempt for service "<<service<<" failed:\n");
    //  SC_REPORT_ERROR("ControlAddressMap", "Service not in name map.");
    //}

    for (serv_addr_iter = addr_range.first; serv_addr_iter!=addr_range.second; ++serv_addr_iter) {
      ret_vec_addr.push_back(serv_addr_iter->second);
    }

    // return all the addresses in a vector
    return ret_vec_addr;
  }

  /// Decode an address to it's supported control service.
  /**
   * @param addr A target address.
   * @return The decoded control service (might be NO_SERVICE).
   */
  const ControlService getService(const cport_address_type addr)
  {
    if (m_address_service_map.count(addr) < 1) {
      //dumpMap();
      GC_DUMP_N("ControlAddressMap", "Decode attempt (service) for address "<<addr<<" failed:\n");
      //SC_REPORT_WARNING("ControlAddressMap", "Address not in service map.");
      return NO_SERVICE;
    }
    addressServiceMapType::iterator iter = m_address_service_map.find(addr);
    return iter->second;
  }

  /// Decode an address to the parent's name.
  /**
   * @param addr A target address.
   * @return The decoded name (might be empty).
   */
  const std::vector<std::string> getName(const cport_address_type addr)
  {
    std::vector<std::string> ret_vec_str;
    if (m_address_name_map.count(addr) < 1) {
      //dumpMap();
      GC_DUMP_N("ControlAddressMap", "Decode attempt (name) for address "<<addr<<" failed:\n");
      SC_REPORT_WARNING("ControlAddressMap", "Address not in name map.");
      return ret_vec_str;
    }
    addressNameMapType::iterator iter = m_address_name_map.find(addr);
    ret_vec_str.push_back(iter->second);
    return ret_vec_str;
  }

  /// Decode an address and service to the parent's name.
  /**
   * @param _addr    A target address.
   * @param service  The service of the searched target.
   * @return The decoded name (might be empty).
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
      SC_REPORT_WARNING("ControlAddressMap", "Address not in name map.");
      return ret_vec_str;
    }
    addressNameMapType::iterator iter = m_address_name_map.find(addr);
    ret_vec_str.push_back(iter->second);
    return ret_vec_str;
  }

  /// Decode a service to the parent's name.
  /**
   * @param service  The service of the searched target.
   * @return The decoded name of all plugin name in a vector (might be empty)
   */
  const std::vector<std::string> getName(ControlService service)
  {
    std::pair<serviceAddressMapType::iterator, serviceAddressMapType::iterator> addr_range;
    serviceAddressMapType::iterator serv_addr_iter;
    addressNameMapType::iterator addr_name_iter;
    std::vector<std::string> ret_vec_str;
    
    addr_range = getAddrFromService(service);

    // Actually not needed, this check has been performed in getAddrFromService()
    //if ( addr_range.first == addr_range.second ) {
      //dumpMap();
    //  GC_DUMP_N("ControlAddressMap", "Decode attempt for service "<<service<<" failed:\n");
    //  SC_REPORT_ERROR("ControlAddressMap", "Service not in name map.");
    //}

    for (serv_addr_iter = addr_range.first; serv_addr_iter!=addr_range.second; ++serv_addr_iter) 
    {
      addr_name_iter = m_address_name_map.find(serv_addr_iter->second);
      ret_vec_str.push_back(addr_name_iter->second);
    }

    // return all the service names in a vector
    return ret_vec_str;
  }

  /// Decode a service to the address of the supporting plugin.
  /**
   * @param service Service to which the address should be returned.
   * @return The address range which belongs to all the plugins which supports the service (might be empty).
   */
  std::pair<serviceAddressMapType::iterator, serviceAddressMapType::iterator> getAddrFromService(ControlService service)
  {
    if (m_service_address_map.count(service) < 1) {
      //dumpMap();
      GC_DUMP_N("ControlAddressMap", "Decode attempt (service->address) for service "<<service<<" failed:\n");
      std::stringstream ss;
      ss << "Service '" << getControlServiceString(service) << "' ("<< (unsigned int)service <<") is not available. Please instantiate plugin first.";
      SC_REPORT_WARNING("ControlAddressMap", ss.str().c_str());
    }
    return m_service_address_map.equal_range(service);
  }

  /// Method to check if address is already in the maps.
  /**
   * @param addr   Address of the target which should be checked.
   */
  bool is_in_maps(const cport_address_type addr) const
  {
    if (m_address_service_map.count(addr) > 0) 
      return true;
    else
      return false;
  }

protected:

  /// Insert the port info into the address map.
  /**
   * - Add the address with the service to the address-service-map.
   * - Add the address with the name to the address-name-map.
   * - Add the service with the address to the service-address-map.
   *
   * @param service      The service the port provides.
   * @param addr         The address of the port (of the port's parent).
   * @param target_name  Name of the port's parent (for debug information).
   * @param is_plugin    If the slave port belongs to a plugin.
   */
  void insert(ControlService service, const cport_address_type addr, const char* target_name, bool is_plugin)
  {
    if (m_address_service_map.count(addr) > 0) {
      SC_REPORT_ERROR("ControlAddressMap", "insert: Address already exists.");
    }
    // add the entry to the different maps
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

protected:

  /// Map to resolve address -> ConfigService for debug and future use.
  addressServiceMapType m_address_service_map;

  /// Map to resolve ConfigService -> address
  serviceAddressMapType m_service_address_map;

  /// Map to resolve target address -> target name.
  addressNameMapType m_address_name_map;
};


} // end namespace ctr
} // end namespace gs

#endif
