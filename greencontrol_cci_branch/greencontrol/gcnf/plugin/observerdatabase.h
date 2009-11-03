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

#ifndef __OBSERVERDATABASE_H__
#define __OBSERVERDATABASE_H__

#include <set>

#include "config_globals.h"
#include "plugin_if.h"
#include "observer_db_if.h"


namespace gs {
namespace cnf {

  
#undef DUMMY_NAME
#define DUMMY_NAME "**NewParamObservers**"


/// Observer database stores observers watching for new added parameters.
/**
 * OLD: This database stores for each parameter which is stored in the parameter
 * database the observer addresses (when they were set).
 *
 * @todo: remove prameter names in this database
 * The observers for new added (or without add first time set) parameters
 * are stored under the dummy entry parameter name DUMMY_NAME (see define).
 */
class ObserverDatabase
: public sc_object,
  public observer_db_if
{

public:

  /// std::set type containing the addresses of the observers
  typedef std::set<cport_address_type> addressSet;

  //               key=parname, value=observer set
  typedef std::map<std::string, addressSet > observer_map;
  typedef observer_map::iterator             observer_iterator;

  /// Port to access the parameter database of the ConfigPlugin
  sc_port<plugin_if> m_plugin_port;

  /// Constructor with name and parameter database
  /**
   * @param name    Name of this object.
   * @param plugin  Pointer to the config plugin.
   */
  ObserverDatabase(const char* name, plugin_if *plugin)
  : sc_object(name)
  {
    // bind port to ConfigPlugin acting as channel
    m_plugin_port(*plugin);
  }
  
private:
  /// Constructor without parameter must not be used!
  ObserverDatabase() { }
  
public:

  /// @see gs::cnf::observer_db_if::registerObserver
/*  bool registerObserver(const std::string &hier_parname, const cport_address_type observer) {
    GCNF_DUMP_N(name(), "registerObserver("<<hier_parname.c_str()<<", "<<observer<<")");
    // TODO: What happens when a not existing parameter is observed?
    //if (!m_plugin_port->existsParam(hier_parname)) {
    //  GCNF_DUMP_N(name(), "registerObserver: parameter not yet exists: create implicit parameter (set with empty value).");
    //  m_plugin_port->setParam(hier_parname, "");
    //}
    if (m_observer_db.count(hier_parname) == 0) {
      GCNF_DUMP_N(name(), "registerObserver: Parameter not found in observer map, add it.");
      m_observer_db.insert( make_pair(hier_parname, addressSet()) );
    }
    observer_iterator pos = m_observer_db.find(hier_parname);
    addressSet *obset = &pos->second;
    pair< addressSet::iterator, bool > result;
    result = obset->insert(observer);
    // If address already in set: warning!
    if (!result.second) {
      SC_REPORT_WARNING(name(), "registerObserver: Only one observer per API address allowed!");
    }
    return result.second;
  }*/

  /// @see gs::cnf::observer_db_if::getObservers
  /*const std::set<cport_address_type> getObservers(const std::string &hier_parname) {
    GCNF_DUMP_N(name(), "getObservers("<<hier_parname.c_str()<<")");
    if (m_observer_db.count(hier_parname) == 0) {
      GCNF_DUMP_N(name(), "getObservers: parameter not yet exist in observer map: create implicit parameter.");
      m_observer_db.insert( make_pair(hier_parname, addressSet()) );
    }
    observer_iterator pos = m_observer_db.find(hier_parname);
    if (pos == m_observer_db.end()) {
      SC_REPORT_WARNING(name(), "getObservers: Parameter not found in map.");
      return std::set<cport_address_type>();
    }
    return pos->second;
  }*/

  /// @see gs::cnf::observer_db_if::registerNewParamObserver
  bool registerNewParamObserver(const cport_address_type observer) {
    const std::string dummyParName = DUMMY_NAME;
    GCNF_DUMP_N(name(), "registerNewParamObserver("<<observer<<")");
    if (m_observer_db.count(dummyParName) == 0) {
      GCNF_DUMP_N(name(), "registerNewParamObserver: dummy entry not found in observer map, add it.");
      m_observer_db.insert( make_pair(dummyParName, addressSet()) );
    }
    observer_iterator pos = m_observer_db.find(dummyParName);
    addressSet *obset = &pos->second;
    pair< addressSet::iterator, bool > result;
    result = obset->insert(observer);
    // If address already in set: warning!
    if (!result.second) {
      SC_REPORT_WARNING(name(), "registerNewParamObserver: Only one observer per API address allowed!");
    }
    return result.second;
  }
  
  /// @see gs::cnf::observer_db_if::getNewParamObservers
  const std::set<cport_address_type> getNewParamObservers() {
    const std::string dummyParName = DUMMY_NAME;
    GCNF_DUMP_N(name(), "getNewParamObservers()");
    if (m_observer_db.count(dummyParName) == 0) {
      GCNF_DUMP_N(name(), "getNewParamObservers: dummy entry not yet exist in observer map: create implicit parameter.");
      m_observer_db.insert( make_pair(dummyParName, addressSet()) );
    }
    observer_iterator pos = m_observer_db.find(dummyParName);
    if (pos == m_observer_db.end()) {
      SC_REPORT_WARNING(name(), "getNewParamObservers: dummy entry not found in map.");
      return std::set<cport_address_type>();
    }
    GCNF_DUMP_N(name(), "getNewParamObservers: size of returned observer set = "<<(unsigned int)pos->second.size());
    IF_VERBOSE(show_observer_set(pos););
    return pos->second;
  }

protected:
 
  /// Observer database
  observer_map m_observer_db;

#ifdef GC_VERBOSE
  void show_observer_set(observer_iterator &obs) { 
    std::cout << "Observers for " << obs->first << ": " << std::endl << "   ";
    addressSet observers = obs->second;
    //cport_address_type addr = 0;
    for (addressSet::iterator iter = observers.begin(); iter!=observers.end(); ++iter) {
      std::cout << *iter << ", ";
    }
    std::cout << std::endl;
  }
#endif

};


} // end namespace cnf
} // end namespace gs

#endif
