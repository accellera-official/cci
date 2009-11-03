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

#ifndef __CONFIGPLUGIN_T_H__
#define __CONFIGPLUGIN_T_H__

#include <map>
#include <list>
#include <vector>
#include <set>

#include "observerdatabase.h"  // with typedef address_set
#include "configdatabase.h" // ConfigDatabase for automatic creation to be bound to port of this plugin



namespace gs {
namespace cnf {

  
/// ConfigPlugin for service "ConfigPlugin" CONFIG_SERVICE.
/**
 * This is the GreenControl ConfigPlugin which holds the configurable parameters.
 * The plugin provides the service "ConfigPlugin" (enumeration entry CONFIG_SERVICE).
 */
class ConfigPlugin
: public sc_core::sc_module,
  public gc_port_if,
  public plugin_if  // to provide the parameter database to the observer database
{
protected:
  
  /// Returns and sets the Plugin singleton pointer
  static ConfigPlugin* access_plugin_singleton(ConfigPlugin* plugin = NULL, bool set = false) {
    /// Pointer to the Core singleton
    static ConfigPlugin* plugin_singleton = NULL;
    if (set) {
      assert(!access_plugin_instantiated());
      assert(plugin_singleton == NULL || plugin == NULL);
      plugin_singleton = plugin;
      access_plugin_instantiated(true);
    }
    return plugin_singleton;
  }
  
  /// If the Plugin has been instantiated, stays true when Plugin is destroyed
  static bool access_plugin_instantiated(bool set = false) {
    /// Instantiated status
    static bool plugin_instantiated = false;
    if (set) {
      assert(plugin_instantiated == false);
      plugin_instantiated = set;
    }
    return plugin_instantiated;
  }
  
public:
  
  /// Static function to get access to the Plugin singleton
  static ConfigPlugin& get_instance() {
    if (access_plugin_singleton() == NULL) {
      assert(!access_plugin_instantiated() && "Config Plugin has already been destroyed. Make sure you destroy the plugin after all confiuration objects (like APIs and parameters).");
      GCNF_DUMP_N("static ConfigPlugin::get_instance()", "Create not yet existing ConfigPlugin!");
      new ConfigPlugin("ConfigPlugin"); // will register itself with plugin_singleton 
    }
    return *access_plugin_singleton();
  }

  /// Port to communicate over the Core with the configurable modules.
  gc_port m_gc_port;
  
  /// Port to get access to the parameter database which implements the param_db_if and acts as channel.
  sc_port<param_db_if> m_param_db_port;

  /// Port to get access to the observer database which implements the observer_db_if and acts as channel.
  sc_port<observer_db_if> m_observer_db_port;

  /// Constructor
  /**
   * Constructor 
   * The service name is fixed to "ConfigPlugin" (not the module name is used).
   *
   * @param name  Name of the instance. 
   * @param db    Pointer to the parameter database, if NULL, default is instantiated automatically.
   */
  ConfigPlugin(sc_core::sc_module_name name, param_db_if *db = NULL)
  : sc_core::sc_module(name)
  , m_gc_port(CONFIG_SERVICE, "ConfigPlugin", true)
  , m_param_db_created(NULL)
  { 
    GCNF_DUMP_N(sc_core::sc_module::name(), "ConfigPlugin constructor!");

    // ensure singleton
    if (access_plugin_singleton() != NULL) {
      SC_REPORT_FATAL(sc_core::sc_module::name(), "Config Plugin has already been instantiated! Make sure a manual construction is done before a possible automatic construction.");
    }
    assert(access_plugin_singleton() == NULL && "Config Plugin has already been instantiated! Make sure a manual construction is done before a possible automatic construction.");
    assert(access_plugin_instantiated() == false && "Config Plugin has already been instantiated and destroyed!");
    access_plugin_singleton(this, true);

    if (db == NULL) {
      GCNF_DUMP_N(sc_core::sc_module::name(), "No parameter database given by user, create default one!");
      m_param_db_created = new ConfigDatabase("ConfigDatabase");
      db = m_param_db_created;
    }
    
    m_gc_port.api_port(*this); // bind sc_port of m_gc_port

    // Check if the database pointer is valid
    std::string test = db->getParameters();
    assert(test.length() == 0);
    
    // bind the (in the toplevel) user chosen database to the port
    m_param_db_port(*db);
    
    /// Observer database
    m_observer_db = new ObserverDatabase("ObserverDatabase", this);
    m_observer_db_port(*m_observer_db);
    
    // create the default GCnf_Api instance which is accessible via the NULL pointer
    // Note: Don't call GCnf_Api_t::getApiInstance(NULL) here, because this ConfigPlugin constructor
    //       is possibly called by exactly this function (the GCnf_Api_T constructor), the 
    //       create_default_instance() function has a guard to prevent this.
    GCnf_Api_t<gs_param_base, GCnf_private_Api_T<gs_param_base, gs_param, ConfigPlugin>, gs_param, ConfigPlugin>::create_default_instance();
  }

  /// Destructor
  ~ConfigPlugin() {
    delete m_observer_db; m_observer_db = NULL;
    if (m_param_db_created != NULL) {
      delete m_param_db_created; m_param_db_created = NULL;
    }
  }

  // //////////   GC_PORT_IF   //////////////////////////////////// //

  /// Called by gc_port through gc_port_if when notification arrives.
  /**
   * Implements pc_port_if. 
   * This method starts whenever a master triggers a payload-event.
   */
  void masterAccess(ControlTransactionContainer &t_p)
  {
    
    ControlTransactionHandle tr = t_p.first;
    ControlPhase ph = t_p.second;

    GCNF_DUMP_N(name(), "got "<<ph.toString()<<" atom from master");      
    // show received Transaction
    //GCNF_DUMP_N(name(), "  received transaction: "<< tr->toString().c_str());      

    //bool notify_observers = false;     // if a parameter was changed and notifiers need to notified (if existing)
    bool notify_new_parameter = false; // if a NEW parameter was added or set

    ControlPhase p(ControlPhase::CONTROL_RESPONSE);
    
    // According to the command fill the transaction or make actions
    switch (tr->get_mCmd()) {

      // ////////////   Command CMD_ADD_PARAM   ////////////////////////////////////// //
    // May only be used by module which owns the parameter
    /* If mAnyPointer is not set, create a new gs_param with name and value */
    /* Note: this method (and all related methods) must not call any of the 
     *       pure virtual functions in gs_param_base because this method is 
     *       called by the gs_param_base constructor.*/
    case CMD_ADD_PARAM:
      {
        GCNF_DUMP_N(name(), "CMD_ADD_PARAM: add param");
        gs_param_base *par = static_cast<gs_param_base*>(tr->get_mAnyPointer());
        notify_new_parameter = true;
        if (par != NULL) {
          if ( m_param_db_port->existsParam(par->getName())) notify_new_parameter = false;
        } else {
          if ( m_param_db_port->existsParam(tr->get_mSpecifier()) ) notify_new_parameter = false;
          GCNF_DUMP_N(name(), "Create new gs_param<string>: "<<tr->get_mSpecifier().c_str());
          // use special parameter constructor with register_at_db=false:
          par = new gs_param<std::string>(tr->get_mSpecifier(), tr->get_mValue(), NULL, true, false);
        }

        if ( ! m_param_db_port->addParam(par) ) {
          tr->set_mError(1);
        }
        break;
      }
    
      // ////////////   Command CMD_REMOVE_PARAM   ////////////////////////////////// //
    case CMD_REMOVE_PARAM:
      {
        GCNF_DUMP_N(name(), "CMD_REMOVE_PARAM: remove param");
        gs_param_base *par = static_cast<gs_param_base*>(tr->get_mAnyPointer());
        if (par != NULL) {
          if ( !m_param_db_port->removeParam(par) ) {
            tr->set_mError(1);
          }
        } else
          GCNF_DUMP_N(name(), "CMD_REMOVE_PARAM: failed to remove parameter NULL.");      
        break;
      }

        // ////////////   Command CMD_SET_INIT_VAL   ////////////////////////////////// //
    case CMD_SET_INIT_VAL:
      {
        GCNF_DUMP_N(name(), "CMD_SET_INIT_VAL: set init value to param");
        bool newAdded = m_param_db_port->setInitValue(tr->get_mSpecifier(), tr->get_mValue());
        if (newAdded) {
          notify_new_parameter = true;
        }
        break;
      }
        
        // ////////////   Command CMD_GET_VAL   ////////////////////////////////// //
    case CMD_GET_VAL:
      {
        GCNF_DUMP_N(name(), "CMD_GET_VAL: get value of param");
        if ( m_param_db_port->existsParam(tr->get_mSpecifier())) {
          tr->set_mValue(m_param_db_port->getValue(tr->get_mSpecifier()));
        } else {
          tr->set_mError(1);
          tr->set_mValue("");
        }
        break;
      }
        
        // ////////////   Command CMD_GET_PARAM   ////////////////////////////////////// //
    case CMD_GET_PARAM: 
      {
        GCNF_DUMP_N(name(), "CMD_GET_PARAM: get param");      
        if ( m_param_db_port->existsParam(tr->get_mSpecifier())) {
          gs_param_base *par = m_param_db_port->getParam( tr->get_mSpecifier() );
          tr->set_mAnyPointer(par);
        } else {
          tr->set_mError(1);
          tr->set_mAnyPointer(NULL);
        }
        break;
      }

      // ////////////   Command CMD_EXISTS_PARAM   ////////////////////////////////// //
    case CMD_EXISTS_PARAM:
      {
        GCNF_DUMP_N(name(), "CMD_EXISTS_PARAM: exists param");      
        if ( !m_param_db_port->existsParam(tr->get_mSpecifier())) {
          tr->set_mError(1);
        }
        break;
      }

      // ////////////   Command CMD_GET_PARAM_LIST   /////////////////////////////// //
    case CMD_GET_PARAM_LIST:
      {
        DEPRECATED_WARNING(name(), "DEPRECATED: One API you are using does use the deprectaed command 'CMD_GET_PARAM_LIST'. Use 'CMD_GET_PARAM_LIST_VEC' instead!");
        GCNF_DUMP_N(name(), "CMD_GET_PARAM_LIST: get param list");      

        // mSpecifier: empty : all parameters
        //             modulename.* : parameters of the module with children
        //             modulename   : parameters of the module (without children)

        std::vector<std::string> vec;
        std::string mod;

        // all parameters
        if (tr->get_mSpecifier().empty()) {
          GCNF_DUMP_N(name(), "     All parameters");      
          tr->set_mValue(m_param_db_port->getParameters());
        } 

        // module's parameters with children
        else if (size_t x = tr->get_mSpecifier().find(".*") != std::string::npos) {
          // Search all parameter including childrennames beginning with <modulename> (without ".*")
          mod = tr->get_mSpecifier().substr(0,  tr->get_mSpecifier().length() -x -1);
          GCNF_DUMP_N(name(), "     Parameters (incl. children) of the module "<< mod.c_str());      
          vec = m_param_db_port->getParametersVector();
          std::stringstream ss; bool first = true;
          std::vector<std::string>::iterator iter = vec.begin();
          while( iter != vec.end() ) {
            // if parametername begins with <mod>
            if (((std::string)*iter).find(mod) == 0) {
              if (!first) ss << ",";
              first = false;
              ss << *iter;
            }
            iter++;
          }
          tr->set_mValue(ss.str());
        } 

        // module's parameters without children
        else {
          std::string mod = tr->get_mSpecifier();
          // Search all parameter names beginning with <modulename> and no further point in the remaining part
          std::vector<std::string> vec = m_param_db_port->getParametersVector();
          GCNF_DUMP_N(name(), "     Parameters (without children) of the module "<<mod.c_str());      
          std::stringstream ss; bool first = true;
          std::vector<std::string>::iterator iter = vec.begin();
          while( iter != vec.end() ) {
            // if parametername begins with <mod>   AND  last '.'(SC_NAME_DELIMITER) is inside '<mod>.'
            if (  ((std::string)*iter).find(mod) == 0 
                  && ((string)*iter).find_last_of(SC_NAME_DELIMITER) <= mod.length()  ) {
              if (!first) ss << ",";
              first = false;
              ss << *iter;
            }
            iter++;
          }
          tr->set_mValue(ss.str());
        }

        break;
      }

        // ////////////   Command CMD_GET_PARAM_LIST_VEC   ////////////////////////// //
      case CMD_GET_PARAM_LIST_VEC:
      {
        GCNF_DUMP_N(name(), "CMD_GET_PARAM_LIST_VEC: get param list");      
        
        // mSpecifier: empty : all parameters
        //             modulename.* : parameters of the module with children
        //             modulename   : parameters of the module (without children)
        //             *.paramname  : named parameter from all modules
        
        //
        // mAnyPointer will return a pointer to a newed vector<string>
        // This has to be deleted after being used!
        //
        
        std::vector<std::string>* return_vec = new std::vector<std::string>();
        std::vector<std::string> vec;
        std::string mod;
        
        // all parameters
        if (tr->get_mSpecifier().empty()) {
          GCNF_DUMP_N(name(), "     All parameters");      
          *return_vec = m_param_db_port->getParametersVector(); // copies vector to newed vec
          tr->set_mAnyPointer(return_vec);
        } 
        
        // module's parameters with children
        else if (size_t x = tr->get_mSpecifier().find(".*") != std::string::npos) {
          // Search all parameter including childrennames beginning with <modulename> (without ".*")
          mod = tr->get_mSpecifier().substr(0,  tr->get_mSpecifier().length() -x -1);
          GCNF_DUMP_N(name(), "     Parameters (incl. children) of the module "<< mod.c_str());      
          vec = m_param_db_port->getParametersVector();
          std::vector<std::string>::iterator iter = vec.begin();
          while( iter != vec.end() ) {
            // if parametername begins with <mod>
            if (((std::string)*iter).find(mod) == 0) {
              return_vec->push_back(*iter);
            }
            iter++;
          }
          tr->set_mAnyPointer(return_vec);
        } 
        
        // named parameter from all modules
        else if (tr->get_mSpecifier().find("*.") == 0) {
          // Search all parameters named as specifier (without "*.")
          std::string paramname;
          paramname = tr->get_mSpecifier().substr(2);
          GCNF_DUMP_N(name(), "     All parameters named "<< paramname.c_str());      
          vec = m_param_db_port->getParametersVector();
          std::vector<std::string>::iterator iter = vec.begin();
          while( iter != vec.end() ) {
            // if parameter name ends with <paramname>
            if (iter->length() - iter->rfind(paramname) == paramname.length()) {
              return_vec->push_back(*iter);
            }
            iter++;
          }
          tr->set_mAnyPointer(return_vec);
        } 
        
        // module's parameters without children
        else {
          std::string mod = tr->get_mSpecifier();
          // Search all parameter names beginning with <modulename> and no further point in the remaining part
          std::vector<std::string> vec = m_param_db_port->getParametersVector();
          GCNF_DUMP_N(name(), "     Parameters (without children) of the module "<< mod.c_str());      
          std::vector<std::string>::iterator iter = vec.begin();
          while( iter != vec.end() ) {
            // if parametername begins with <mod>   AND  last '.'(SC_NAME_DELIMITER) is inside '<mod>.'
            if (  ((std::string)*iter).find(mod) == 0 
                && ((string)*iter).find_last_of(SC_NAME_DELIMITER) <= mod.length()  ) {
              return_vec->push_back(*iter);
            }
            iter++;
          }
          tr->set_mAnyPointer(return_vec);
        }
        
        break;
      }
        
        // ////////////   Command CMD_REGISTER_PARAM_OBSERVER   ////////////////////// //
     // REMOVED: APIs use registering at parameters themselves
     /*case CMD_REGISTER_PARAM_OBSERVER:
      {
        GCNF_DUMP_N(name(), "CMD_REGISTER_PARAM_OBSERVER: register observer");      
        if ( !m_observer_db_port->registerObserver(tr->get_mSpecifier(), tr->get_mID()) ) {
          tr->set_mError(1);
        }
        break;
      }*/

      // ////////////   Command CMD_REGISTER_NEW_PARAM_OBSERVER   ///////////////// //
    case CMD_REGISTER_NEW_PARAM_OBSERVER:
      {
        GCNF_DUMP_N(name(), "CMD_REGISTER_NEW_PARAM_OBSERVER: register observer for new parameters");
        if ( !m_observer_db_port->registerNewParamObserver( tr->get_mID()) ) {
          tr->set_mError(1);
        }
        break;
      }
       
      // ////////////   Command CMD_UNREGISTER_PARAM_CALLBACKS   ////////////////// //
    case CMD_UNREGISTER_PARAM_CALLBACKS:
      {
        std::vector<std::string> vec = m_param_db_port->getParametersVector();
        std::vector<std::string>::iterator iter;
        for( iter = vec.begin(); iter != vec.end(); iter++ ) {
          if (m_param_db_port->is_explicit(*iter)) {
            m_param_db_port->getParam(*iter)->unregisterParamCallbacks(tr->get_mAnyPointer());
          }
        }      
        break;
      }
        
      // ////////////   No Command (error)   ////////////////////////////////////// //
    default:
      {
        SC_REPORT_WARNING(name(), "masterAccess: unknown command!");
        p.state = ControlPhase::CONTROL_ERROR;
      }
    }
    
    // Answer with a CONTROL_RESPONSE or CONTROL_ERROR phase
    GCNF_DUMP_N(name(), "send "<<p.toString()<<" atom back to master");      

    // Do not Ack the transaction if command was a remove, because sender could already be destructed
    // TODO: What is with the notifications of these?
    if (tr->get_mCmd() != CMD_REMOVE_PARAM) {
      ControlTransactionContainer ctc = ControlTransactionContainer(t_p.first, p);
      // Ack transaction
      m_gc_port.target_port.out->notify(ctc, PEQ_IMMEDIATE);
    }

    // If observers for new parameters have to be notified, do that.
    if (notify_new_parameter) {
      sendNewParameterNotify(  static_cast<gs_param_base*>(tr->get_mAnyPointer()),
                               tr->get_mSpecifier(), tr->get_mValue()  );
    }
    
    // moved to parameter themseves!
    // If observers have to be notified, do that.
    //if (notify_observers) {
    //  sendObserverNotifies(tr->get_mSpecifier());
    //}

  }
  
  /// Called by gc_port through gc_port_if when notification arrives.
  /**
   * Implements pc_port_if. 
   * This method starts whenever a slave triggers a payload-event.
   * That happens when one of the GC-API methods below send a transaction
   */
  void slaveAccess(ControlTransactionContainer &t_p)
  {  
    ControlTransactionHandle tr = t_p.first;
    ControlPhase ph = t_p.second;

    GCNF_DUMP_N(name(), "got "<<ph.toString()<<" atom from slave");      
    
    switch (ph.state) {
    case ControlPhase::CONTROL_RESPONSE:
      // processed in the initiating methods
      break;
    }
  }


  // ////////// PLUGIN_IF Interface ///////////////////////////////////// //

  /// This gs::cnf::plugin_if method provides the gs::cnf::param_db_if::setParam method of the gs::cnf::param_db_if to the observer database.
  //bool setParam(const std::string &hier_parname, const std::string &value) {
  //  return m_param_db_port->setParam(hier_parname, value);
  //}

  /// This gs::cnf::plugin_if method provides the gs::cnf::param_db_if::existsParam method of the gs::cnf::param_db_if to the observer database.
  bool existsParam(const std::string &hier_parname) {
    return m_param_db_port->existsParam(hier_parname);
  }

  // /////////////////////////////////////////////////////////////////// //

protected:

  /// Sends all notify transactions to APIs which observe new parameters.
  /**
   * APIs observers for new parameters have to be notified each time an (explicit) parameter is added
   * (without being an implicit parameter before) to the Config Plugin -- or an (implicit) 
   * parameters init value is set (without being added before).
   *
   * New explicit parameters are submitted as pointers (mAnyPointer field).
   * New implicit parameters are submitted with name and init_value (mSpecifier and mValue fields).
   *
   * @param par      Pointer to the new parameter (may be NULL).
   * @param par_name  Name of the new parameter if par is NULL.
   * @param init_value  Init value of the new parameter if par is NULL.
   */
  void sendNewParameterNotify(gs_param_base *par, const std::string &par_name, const std::string &init_value) {
#ifdef GCNF_VERBOSE
    if (par != NULL) {
      GCNF_DUMP_N(name(), "sendNewParameterNotify: notify new parameter observers for new explicit parameter "<< par->getName().c_str());
    } else {
      GCNF_DUMP_N(name(), "sendNewParameterNotify: notify new parameter observers for new implicit parameter "<< par_name.c_str());
    }
#endif

    // Go through set and notify each new parameter observer (saved under name "**newParamObserver**")
    ObserverDatabase::addressSet observers = m_observer_db_port->getNewParamObservers();
    cport_address_type addr = 0;

    for (ObserverDatabase::addressSet::iterator iter = observers.begin(); iter!=observers.end(); ++iter) {
      
      addr = *iter;

      // create Transaction an send it to API
      ControlTransactionHandle th = m_gc_port.init_port.create_transaction();
      th->set_mService(CONFIG_SERVICE);
      th->set_mCmd(CMD_NOTIFY_NEW_PARAM_OBSERVER);
      th->set_mTarget(addr);
      th->set_mAnyPointer(par);
      if (par == NULL) {
        th->set_mSpecifier(par_name);
        th->set_mValue(init_value);
      }
      
      ControlPhase p(ControlPhase::CONTROL_REQUEST);
      ControlTransactionContainer ctc = ControlTransactionContainer(th,p);
      m_gc_port.init_port.out->notify(ctc, PEQ_IMMEDIATE);

      if (th->get_mError() > 0) {
#ifdef GC_VERBOSE
        if (par != NULL) {
          GCNF_DUMP_N(name(), "sendNewParameterNotify: Response for CMD_NOTIFY_NEW_PARAM_OBSERVER (param="<<par->getName()<<" ,value="<<par->getString()<<") returned error "<<th->get_mError()<<".");
        } else {
          GCNF_DUMP_N(name(), "sendNewParameterNotify: Response for CMD_NOTIFY_NEW_PARAM_OBSERVER (param="<<par_name<<" ,value="<<init_value<<") returned error "<<th->get_mError()<<".");
        }
#endif
        SC_REPORT_WARNING(name(), "sendNewParameterNotify: Response for CMD_NOTIFY_NEW_PARAM_OBSERVER returned error.");
      }

    } // end for
  }
  
  /// Sends all notify transactions to oberver APIs.
  /**
   * @param parname  Name of the parameter whose observers have to be notified.
   */
  // has been moved to the gs_param s
  /*void sendObserverNotifies(const std::string &parname) {
    GCNF_DUMP_N(name(), "sendObserverNotifies: notify observers for parameter "<< parname.c_str());

    // Go through set and notify each observer
    ObserverDatabase::addressSet observers = m_observer_db_port->getObservers(parname);
    cport_address_type addr = 0;
    std::string value = m_param_db_port->getParam( parname );

    for (ObserverDatabase::addressSet::iterator iter = observers.begin(); iter!=observers.end(); ++iter) {
      
      addr = *iter;

      // create Transaction an send it to API
      ControlTransactionHandle th = m_gc_port.init_port.create_transaction();
      th->set_mService(CONFIG_SERVICE);
      th->set_mCmd(CMD_NOTIFY_OBSERVER);
      th->set_mTarget(addr);
      th->set_mSpecifier(parname);
      th->set_mValue(value);
      
      ControlPhase p(ControlPhase::CONTROL_REQUEST);
      ControlTransactionContainer ctc = ControlTransactionContainer(th,p);
      m_gc_port.init_port.out->notify(ctc);

      if (th->get_mError() > 0) {
        GCNF_DUMP_N(name(), "sendObserverNotifies: Response for CMD_NOTIFY_OBSERVER (param="<<parname<<" ,value="<<value<<") returned error "<<th->get_mError()<<".");
        SC_REPORT_WARNING(name(), "sendObserverNotifies: Response for CMD_NOTIFY_OBSERVER returned error.");
      }

    } // end for

  }*/

protected:

  /// Observer database
  observer_db_if *m_observer_db;

  /// Pointer to the param db if created automatically, NULL else
  param_db_if* m_param_db_created;
  
};


//typedef GCnf_Api_t<gs_param_base, GCnf_private_Api, gs_param> GCnf_Api;

} // end namespace cnf
} // end namespace gs

#endif
