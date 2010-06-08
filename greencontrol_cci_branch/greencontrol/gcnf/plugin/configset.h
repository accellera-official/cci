// LICENSETEXT
// 
//   Copyright (C) 2008 : GreenSocs Ltd
//       http://www.greensocs.com/ , email: info@greensocs.com
// 
//   Developed by :
// 
//   Christian Schroeder
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
// 
// 
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


#ifndef __CONFIGSET_H__
#define __CONFIGSET_H__

#include "config_globals.h"
#include <map>

namespace gs {
  namespace cnf {
    
    /// Data structure carrying a set of configuration data that can be applied to the model or a module
    /**
     * Parsers can create and return such a ConfigSet read from a configuration file.
     * The GCnf_Api can apply the object to the model (top-level) or a specific module.
     */
    class ConfigSet
    {
    public:

      /// Type of the internal data. Should be used by user when iterating
      typedef std::map<std::string, std::string> cnf_data_map_type;

      /// Adds a new param - value pair to the config set
      /**
       * Overwrites existing values
       *
       * @param param_name  Name of the param
       * @param value       Value (string representation) of the param
       * @return        If the the added value was a new one. (false if value has been overwritten)
       */
      bool add(const std::string& param_name, const std::string& value) {
        std::pair< cnf_data_map_type::iterator, bool> ret;
        ret=m_cnf_data.insert( std::pair<std::string, std::string>(param_name, value) ); 
        if (!ret.second) {
          GCNF_DUMP_N("ConfigSet", "add("<<param_name<<", "<<value<<") replaces existing value in this ConfigSet!");
          m_cnf_data.erase(ret.first); // erase previous value
          m_cnf_data.insert( std::pair<std::string, std::string>(param_name, value) ); // add new value
        }
        return ret.second;
      }
      
      /// Returns the value of the given parameter stored in this ConfigSet
      std::string get(const std::string& param_name) const {
        cnf_data_map_type::const_iterator iter = m_cnf_data.find(param_name);
        if( iter != m_cnf_data.end() ) {
          return iter->second;
        }
        GCNF_DUMP_N("ConfigSet", "get("<<param_name<<") cannot return value of key not existing in ConfigSet!");
        SC_REPORT_INFO("ConfigSet", "get_value cannot return value of key not existing in ConfigSet!");
        return std::string("");
      }
      
      /// Returns IF the value of the given parameter stored in this ConfigSet
      bool has(const std::string& param_name) const {
        cnf_data_map_type::const_iterator iter = m_cnf_data.find(param_name);
        if( iter != m_cnf_data.end() ) {
          return true;
        }
        return false;
      }
      
      /// Returns the ConfigSet map, e.g. to iterate over
      const cnf_data_map_type& get_config_map() const {
        return m_cnf_data;
      }
      
    protected:
      /// actual config data map
      cnf_data_map_type m_cnf_data;

    };
    
  } // end namespace cnf
} // end namespace gs


#endif
