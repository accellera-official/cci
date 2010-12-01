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

#ifndef __OBSERVERDATABASE_H__
#define __OBSERVERDATABASE_H__

#include <set>

#include "config_globals.h"
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
 *
 * Actually this class is not more than JUST A SET of observers.
 */
class ObserverDatabase
: public observer_db_if
{

public:

  /// std::set type containing the addresses of the observers
  typedef std::set<cport_address_type> addressSet;

  //               key=parname, value=observer set
  typedef std::map<std::string, addressSet > observer_map;
  typedef observer_map::iterator             observer_iterator;

  /// Constructor with name and parameter database
  /**
   * @param name    Name of this object.
   */
  ObserverDatabase(const char* name)
  : m_name(name)
  {
  }
  
public:

  /// @see gs::cnf::observer_db_if::registerObserver
/*  bool registerObserver(const std::string &hier_parname, const cport_address_type observer) {
    GCNF_DUMP_N(name(), "registerObserver("<<hier_parname.c_str()<<", "<<observer<<")");
    // TODO: What happens when a not existing parameter is observed?
    //if (!m_plugin->existsParam(hier_parname)) {
    //  GCNF_DUMP_N(name(), "registerObserver: parameter not yet exists: create implicit parameter (set with empty value).");
    //  m_plugin->setParam(hier_parname, "");
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
    std::pair< addressSet::iterator, bool > result;
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

  /// Return this name for debug purpose
  const char* name() { return m_name.c_str(); }
  
protected:
 
  /// Observer database
  observer_map m_observer_db;
  
  /// Name of this for debug
  std::string m_name;

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
