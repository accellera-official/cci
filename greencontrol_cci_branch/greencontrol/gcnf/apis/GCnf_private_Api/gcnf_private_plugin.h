//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2008 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
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

#ifndef __CONFIG_PRIVATE_PLUGIN_H__
#define __CONFIG_PRIVATE_PLUGIN_H__

#include <map>
#include <list>
#include <vector>
#include <set>

#include "greencontrol/gcnf/plugin/config_globals.h"

#include "greencontrol/gcnf/plugin/gcnf_datatypes.h"

#include "greencontrol/gcnf/plugin/param_db_if.h" // allows connection of database through SystemC port
//#include "greencontrol/gcnf/plugin/plugin_if.h"
#include "greencontrol/gcnf/plugin/observerdatabase.h"  // with typedef address_set
#include "greencontrol/gcnf/plugin/configdatabase.h" // ConfigDatabase to be bound to port of ConfigPlugin


namespace gs {
namespace cnf {

  // forward declarations
  class gs_param_base;
  template<typename T> class gs_param;
  template<typename T, template<class T> class gs_param_T, typename ConfigPlugin_T> class GCnf_private_Api_T;
  
  /// private Config Plugin to be used within a private config API
  /**
   * This class has similar functionality as the ConfigPlugin
   *
   * This has no observer database because the only possible 
   * observer is the parent API (which sets the member 
   * m_new_params_observed_by_API).
   */
  template<typename gs_param_base_T, typename gs_param_STRING_T, typename GCnf_private_Api_T>
  class ConfigPrivatePlugin_T
  //: public plugin_if  // to provide the parameter database to the observer database
  {
    
  public:
    
    /// Constructor
    ConfigPrivatePlugin_T(GCnf_private_Api_T* parent_api)
    : m_parent_api(parent_api)
    , m_new_params_observed_by_API(false)
    { 
      assert(parent_api != NULL);
      m_param_db = new ConfigDatabase("ConfigDatabase");
    }
    
    /// Destructor
    ~ConfigPrivatePlugin_T() {
      delete m_param_db;
    }
    
    // ////////// Functions related to commands ////////////////////////////// //

    // ////////////   Command CMD_ADD_PARAM   ////////////////////////////////////// //
    // May only be used by module which owns the parameter
    /* If mAnyPointer is not set, create a new gs_param with name and value */
    /* Note: this method (and all related methods) must not call any of the 
     *       pure virtual functions in gs_param_base because this method is 
     *       called by the gs_param_base constructor.*/
    unsigned int cmd_ADD_PARAM(const std::string& pname, const std::string& def_val, gs_param_base_T* par) {
      GCNF_DUMP_N(name(), "cmd_ADD_PARAM: add param");
      bool notify_new_parameter = true;
      if (par != NULL) {
#ifdef GCNF_OLD_NEW_PARAM_CALLBACK_BEHAVIOR_ENABLED
        if ( m_param_db->existsParam(par->getName())) notify_new_parameter = false;
#endif
      } else {
#ifdef GCNF_OLD_NEW_PARAM_CALLBACK_BEHAVIOR_ENABLED
        if ( m_param_db->existsParam(pname) ) notify_new_parameter = false;
#endif
        GCNF_DUMP_N(name(), "Create new gs_param<string>: "<<pname.c_str());
        // use special parameter constructor with register_at_db=false:
        par = new gs_param_STRING_T(pname, def_val, NULL, true, false);
      }
      
      if ( ! m_param_db->addParam(par) ) {
        return 1;
      }

      if (notify_new_parameter) {
        sendNewParameterNotify(par, pname, def_val);
      }
      return 0;
    }
          
    // ////////////   Command CMD_REMOVE_PARAM   ////////////////////////////////// //
    unsigned int cmd_REMOVE_PARAM(gs_param_base_T* par) {
      GCNF_DUMP_N(name(), "CMD_REMOVE_PARAM: remove param");
      if (par != NULL) {
        if ( !m_param_db->removeParam(par) ) {
          return 1;
        }
      } else
        GCNF_DUMP_N(name(), "CMD_REMOVE_PARAM: failed to remove parameter NULL.");
      return 0;
    }
      
    // ////////////   Command CMD_SET_INIT_VAL   ////////////////////////////////// //
    bool cmd_SET_INIT_VAL(const std::string& pname, const std::string& val) {
      GCNF_DUMP_N(name(), "CMD_SET_INIT_VAL: set init value to param");
      if (m_param_db->initValueLocked(pname)) {
        GCNF_DUMP_N(name(), "CMD_SET_INIT_VAL: init value was locked, thus not being set!");
        return false;
      } else {
        if (  m_param_db->setInitValue(pname, val)  ) {
          sendNewParameterNotify(NULL, pname, val);
        }
      }
      return true;
    }

    // ////////////   Command CMD_LOCK_INIT_VAL   ////////////////////////////////// //
    bool cmd_LOCK_INIT_VAL(const std::string& pname) {
      GCNF_DUMP_N(name(), "CMD_LOCK_INIT_VAL: lock init value");
      return m_param_db->lockInitValue(pname);
    }
    
    // ////////////   Command CMD_GET_VAL   ////////////////////////////////// //
    bool cmd_GET_VAL(const std::string& pname, std::string& val, const bool not_impact_is_used_status) {
      GCNF_DUMP_N(name(), "CMD_GET_VAL: get value of param");
      if ( m_param_db->existsParam(pname)) {
        val = m_param_db->getValue(pname, not_impact_is_used_status);
      } else {
        val = "";
        return false;
      }
      return true;
    }
    
    // ////////////   Command CMD_GET_PARAM   ////////////////////////////////////// //
    gs_param_base_T* cmd_GET_PARAM(const std::string& pname) {
      GCNF_DUMP_N(name(), "CMD_GET_PARAM: get param");      
      if ( m_param_db->existsParam(pname)) {
        return m_param_db->getParam( pname );
      }
      return NULL;
    }

    // ////////////   Command CMD_EXISTS_PARAM   ////////////////////////////////// //
    bool cmd_EXISTS_PARAM (const std::string pname) {
      GCNF_DUMP_N(name(), "CMD_EXISTS_PARAM: exists param");      
      if ( !m_param_db->existsParam(pname))
        return false;
      return true;
    }

    bool cmd_CMD_PARAM_HAS_BEEN_ACCESSED(const std::string pname) {
      GCNF_DUMP_N(name(), "CMD_PARAM_HAS_BEEN_ACCESSED: is/had been param used");      
      if ( m_param_db->is_used(pname) ) {
        // used
        return true;
      }
      return false; // not used
    }
    
    // ////////////   Command CMD_GET_PARAM_LIST_VEC   /////////////////////////// //
    const std::vector<std::string> cmd_GET_PARAM_LIST_VEC(const std::string& spec) {
      GCNF_DUMP_N(name(), "CMD_GET_PARAM_LIST_VEC: get param list");      
      
      // spec:       empty : all parameters
      //             modulename.* : parameters of the module with children
      //             modulename   : parameters of the module (without children)
      
      // all parameters
      if (spec.empty()) {
        GCNF_DUMP_N(name(), "     All parameters");      
        return m_param_db->getParametersVector();
      } 
      
      // module's parameters with children
      else if (size_t x = spec.find(".*") != string::npos) {
        // Search all parameter including childrennames beginning with <modulename> (without ".*")
        string mod = spec.substr(0,  spec.length() -x -1);
        GCNF_DUMP_N(name(), "     Parameters (incl. children) of the module "<< mod.c_str());      
        vector<string> vec = m_param_db->getParametersVector();
        vector<string> result;
        vector<string>::iterator iter = vec.begin();
        while( iter != vec.end() ) {
          // if parametername begins with <mod>
          if (((string)*iter).find(mod) == 0) {
            result.push_back(*iter);
          }
          iter++;
        }
        return result;
      } 
      
      // module's parameters without children
      else {
        // Search all parameter names beginning with <modulename> and no further point in the remaining part
        GCNF_DUMP_N(name(), "     Parameters (without children) of the module "<<spec.c_str());      
        vector<string> vec = m_param_db->getParametersVector();
        std::vector<std::string> result;
        std::vector<std::string>::iterator iter = vec.begin();
        while( iter != vec.end() ) {
          // if parametername begins with <spec>   AND  last '.'(SC_NAME_DELIMITER) is inside '<spec>.'
          if (  ((std::string)*iter).find(spec) == 0 
              && ((std::string)*iter).find_last_of(SC_NAME_DELIMITER) <= spec.length()  ) {
            result.push_back(*iter);
          }
          iter++;
        }
        return result;
      }
    }
          
    // ////////////   Command CMD_REGISTER_NEW_PARAM_OBSERVER   ///////////////// //
    void cmd_REGISTER_NEW_PARAM_OBSERVER() {
      GCNF_DUMP_N(name(), "CMD_REGISTER_NEW_PARAM_OBSERVER: register observer for new parameters");
      m_new_params_observed_by_API = true;
    }
      
    // ////////////   Command CMD_UNREGISTER_PARAM_CALLBACKS   ////////////////// //
    void cmd_UNREGISTER_PARAM_CALLBACKS(void* observer) {
      std::vector<std::string> vec = m_param_db->getParametersVector();
      std::vector<std::string>::iterator iter;
      for( iter = vec.begin(); iter != vec.end(); iter++ ) {
        if (m_param_db->is_explicit(*iter))
          static_cast<gs_param_base_T*>(m_param_db->getParam(*iter))->unregisterParamCallbacks(observer);
      }      
    }
     
    // ////////// PLUGIN_IF Interface ///////////////////////////////////// //
    
    /// This gs::cnf::plugin_if method provides the gs::cnf::param_db_if::existsParam method of the gs::cnf::param_db_if to the observer database.
    //bool existsParam(const std::string &hier_parname) {
    //  return m_param_db->existsParam(hier_parname);
    //}
    
    // /////////////////////////////////////////////////////////////////// //
    
  protected:
    
    /// Sends all notify to observing API (which observe new parameters).
    /**
     * The observing API has to be notified each time an (explicit) parameter is added
     * (without being an implicit parameter before) to the Private Config Plugin -- or an (implicit) 
     * parameters init value is set (without being added before).
     *
     * New explicit parameters are submitted as pointers (mAnyPointer field).
     * New implicit parameters are submitted with name and init_value (mSpecifier and mValue fields).
     *
     * @param par         Pointer to the new parameter (may be NULL).
     * @param par_name    Name of the new parameter if par is NULL.
     * @param init_value  Init value of the new parameter if par is NULL.
     */
    void sendNewParameterNotify(gs_param_base_T *par, const std::string &par_name, const std::string &init_value) {
      if (m_new_params_observed_by_API) {
#ifdef GCNF_VERBOSE
        if (par != NULL) {
          GCNF_DUMP_N(name(), "sendNewParameterNotify: notify new parameter observer for new explicit parameter "<<par->getName().c_str());
        } else {
          GCNF_DUMP_N(name(), "sendNewParameterNotify: notify new parameter observer for new implicit parameter "<<par_name.c_str());
        }
#endif
        if (par)
          m_parent_api->cmd_NOTIFY_NEW_PARAM_OBSERVER("", "", par);
        else
          m_parent_api->cmd_NOTIFY_NEW_PARAM_OBSERVER(par_name, init_value, NULL);
      }
    }
        
    const char* name() {
      return "ConfigPrivatePlugin";
    }
    
  protected:
    
    /// Parameter database
    param_db_if* m_param_db;
    
    /// Parent private API
    GCnf_private_Api_T* m_parent_api;
    
    /// If the parent API does observe new parameters
    bool m_new_params_observed_by_API;
  };
  
  //class ConfigPlugin;
  
  /// Typedef for ConfigPrivatePlugin (with the only allowed templates)
  //typedef ConfigPrivatePlugin_T<gs_param_base, gs_param<std::string>, GCnf_private_Api_T<gs_param_base, gs_param, ConfigPlugin> > ConfigPrivatePlugin;
  
} // end namespace cnf
} // end namespace gs

#endif
