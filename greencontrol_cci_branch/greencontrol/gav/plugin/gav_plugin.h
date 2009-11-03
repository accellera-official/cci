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

#ifndef __gav_plugin_h__
#define __gav_plugin_h__


#include <list>
#include <map>

// Core
#include "greencontrol/core/gc_port_if.h"
#include "greencontrol/core/gc_port.h"

// AV
#include "gav_globals.h"
#include "gav_datatypes.h"    // Command enumeration, OutputPlugin enumeration
#include "event_listener.h"
#include "trigger_if.h"
#include "greencontrol/gav/apis/gav_api/GAV_Api.h"
#include "OutputPlugin_base.h"

// OutputPlugin Interface
#include "OutputPlugin_if.h"


namespace gs {
namespace av {

  /// Interface of a plugin fabric function
  typedef OutputPlugin_if* (*OutpPlFabric_if)(const char* name, event_listener<OutputPlugin_base> *ev_listn);

  /// Map a plugin ID with a fabric function
  typedef std::map<OutputPluginType, OutpPlFabric_if> OutpPlFabricMap_type;
  
  /// Plugin fabric function generator (call plugin constructor with file name and event listener)
  template <class T>
  OutputPlugin_if* OutpPlFabric(const char* name, event_listener<OutputPlugin_base> *ev_listn) {
    return new T(name, ev_listn);
  }

  // Macro to simplify registering a plugin class
  #define GAV_REGISTER_PLUGIN(id, alias, clazz) \
       GAV_REGISTER_PLUGIN_NAME(id, alias) \
       class clazz; \
       namespace { \
         std::pair<OutpPlFabricMap_type::iterator,bool> clazz##Dummy = \
            GAV_Plugin::getFabricMap().insert(std::make_pair(alias,OutpPlFabric<clazz>)); \
       }


/// GAV_Plugin for service "GAV_Plugin" AV_SERVICE.
/**
 * This is the GreenControl GAV_Plugin which does analysis and visibility.
 * The plugin provides the service "GAV_Plugin" (enumeration entry AV_SERVICE).
 *
 * The plugin owns some event_listeners to provide to its childs
 * (e.g. StatisticsCalculators, OutputPlugins) the callback service being
 * called after an event was notified. This is needed because the childs
 * are created during simulation runtime and cannot be sc_core::sc_modules but need
 * to be 'sensitive' to events.
 */
class GAV_Plugin
: public sc_core::sc_module,
  public gc_port_if  
{

  typedef std::map<unsigned int, OutputPlugin_if*>      OutpPlMap_type;
  typedef std::multimap<unsigned int, OutputPlugin_if*> OutpPlMultiMap_type;
  
public:

  /// Port to communicate over the Core with the configurable modules.
  gc_port m_gc_port;

  SC_HAS_PROCESS(GAV_Plugin);

  /// Constructor
  /**
   * The service name is fixed to "GAV_Plugin" (not the module name is used).
   *
   * Sets the default output plugin to NULL_OUT
   *
   * @param name  Name of the instance. 
   */
  GAV_Plugin(sc_core::sc_module_name name)
  : sc_core::sc_module(name),
    m_gc_port(AV_SERVICE, "GAV_Plugin", true),
    m_OutpPl_event_listener("OutpPlEventListener"),
    m_trigger_event_listener("TriggerEventListener"),
    m_default_output_plugin_type(NULL_OUT)
  { 
    m_gc_port.api_port(*this); // bind sc_port of m_gc_port

    // create the default GAV_Api instance which is accessible via the NULL pointer
    GAV_Api::getApiInstance(NULL);

    create_all_default_output_plugins();
  }

  /// Constructor which sets the default output plugin
  /**
   * The service name is fixed to "GAV_Plugin" (not the module name is used).
   *
   * The default output can be accessed (within the following 
   * simulation) by using the DEFAULT_OUT.
   * The function set_default_output_plugin can be used to modify
   * the default output plugin.
   *
   * @param name  Name of the instance. 
   * @param default_output_plugin_type  Default type for output plugins.
   */
  GAV_Plugin(sc_core::sc_module_name name, OutputPluginType default_output_plugin_type)
  : sc_core::sc_module(name),
    m_gc_port(AV_SERVICE, "GAV_Plugin", true),
    m_OutpPl_event_listener("OutpPlEventListener"),
    m_trigger_event_listener("TriggerEventListener"),
    m_default_output_plugin_type(default_output_plugin_type)
  { 
    m_gc_port.api_port(*this); // bind sc_port of m_gc_port
    
    // create the default GAV_Api instance which is accessible via the NULL pointer
    GAV_Api::getApiInstance(NULL);
    
    create_all_default_output_plugins();
  }
  
  /// Destructor
  ~GAV_Plugin() {
    OutputPlugin_if* op = NULL;
    // delete output plugins
    for (OutpPlMultiMap_type::iterator iter = m_OutputPlugins.begin(); iter != m_OutputPlugins.end(); iter++) {
      delete op;
      op = (*iter).second;
    }
    op = NULL;
    m_OutputPlugins.clear();
    for (OutpPlMap_type::iterator iter = m_DefaultOutputPlugins.begin(); iter != m_DefaultOutputPlugins.end(); iter++) {
      delete op;
      op = (*iter).second;
    }
    m_DefaultOutputPlugins.clear();
  }
  
  /// Modifies the default output plugin
  /**
   * Calling this function after initialization of GreenControl and GreenAV
   * can lead to strange behavior if the default changes while modules
   * access them.
   *
   * @param default_output_plugin_type  Default output plugin type.
   */
  void set_default_output_plugin(OutputPluginType default_output_plugin_type) {
    m_default_output_plugin_type = default_output_plugin_type;
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

    GAV_DUMP_N(name(), "got "<<ph.toString().c_str()<<" atom from master, command="<<gavCommandToString(tr->get_mCmd()).c_str());
    // show received Transaction
    //GAV_DUMP_N(name(), "  received transaction: "<<tr->toString().c_str());      

    ControlPhase p(ControlPhase::CONTROL_RESPONSE);
    
    // According to the command fill the transaction or make actions
    switch (tr->get_mCmd()) {
        
        // ////////////   Command CMD_ADD_TO_OUTPUT_PLUGIN   /////////////////////////////// //
      case CMD_ADD_TO_OUTPUT_PLUGIN:
      {
        GAV_DUMP_N(name(), "CMD_ADD_TO_OUTPUT_PLUGIN: add param to output plugin");

        // if default output plugin
        if (tr->get_mAnyPointer() == NULL) {
          OutputPlugin_if *op;
          OutputPluginType chosen_plugin_type = tr->get_mAnyUint();
          if (chosen_plugin_type == DEFAULT_OUT)
            chosen_plugin_type = m_default_output_plugin_type;
          std::map<unsigned int, OutputPlugin_if*>::iterator iter 
                                          = m_DefaultOutputPlugins.find(chosen_plugin_type);
          // if not yet instantiated
          if (iter != m_DefaultOutputPlugins.end()) {
            op = (*iter).second;
          } else {
            //this should never happen
            SC_REPORT_ERROR(name(), "No default output plugin of needed type available! Please contact GreenSocs about this error.");
            assert(false); exit(666);
            op = NULL;
          }
          if (op) {
            // if param was submitted, add param to output
            gs_param_base* par = static_cast<gs_param_base*>(tr->get_mAnyPointer2());
            if (par) op->observe(*par);
            // Set any pointer to the output plugin
            tr->set_mAnyPointer(op);
          } 
          else {
            tr->set_mError(1);
            tr->set_mAnyPointer(NULL);
          }
        } 
        
        // if not default output plugin
        else {
          bool succ = false;
          OutputPlugin_if *op;
          std::multimap<unsigned int, OutputPlugin_if*>::iterator iter;
          // iterate over the map of output plugins searching for the correct pointer
          for( iter = m_OutputPlugins.begin(); iter != m_OutputPlugins.end(); iter++ ) {
            // check for the correct pointer
            if ((void*)(*iter).second == tr->get_mAnyPointer()) {
              op = (*iter).second;
              // add param to output
              gs_param_base* par = static_cast<gs_param_base*>(tr->get_mAnyPointer2());
              if (par) op->observe(*par);
              else SC_REPORT_ERROR(name(), "Wrong usage of command CMD_ADD_TO_OUTPUT_PLUGIN: param missing.");
              succ = true;
              // Set any pointer to the output plugin
              tr->set_mAnyPointer(op);
              break;
            }
          }
          if (!succ) {
            SC_REPORT_ERROR(name(), "Command CMD_ADD_TO_OUTPUT_PLUGIN used with not existing Output Plugin!");
            tr->set_mAnyPointer(NULL);
          }
        }
        break;
      }
        
        // ////////////   Command CMD_CREATE_OUTPUT_PLUGIN   /////////////////////////////// //
      case CMD_CREATE_OUTPUT_PLUGIN:
      {
        GAV_DUMP_N(name(), "CMD_CREATE_OUTPUT_PLUGIN: create new output plugin");
        OutputPlugin_if *op;
        // create output plugin
        op = outputPluginFabricCreator(tr->get_mAnyUint(), tr->get_mValue().c_str());
        m_OutputPlugins.insert(std::make_pair(tr->get_mAnyUint(), op));
        // Set any pointer to the created output plugin
        tr->set_mAnyPointer(op);
        GAV_DUMP_N(name(), "Created OutputPlugin.");
        break;
      }
      
        // ////////////   Command CMD_GET_EVENT_LISTENER   //////////////////////////////// //
      case CMD_GET_EVENT_LISTENER:
      {
        // Set any pointer to the event listener
        tr->set_mAnyPointer(&m_trigger_event_listener);
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
    GAV_DUMP_N(name(), "send "<<p.toString().c_str()<<" atom back to master");      

    ControlTransactionContainer ctc = ControlTransactionContainer(t_p.first, p);
    // Ack transaction
    m_gc_port.target_port.out->notify(ctc, PEQ_IMMEDIATE);

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

    GAV_DUMP_N(name(), "got "<<ph.toString().c_str()<<" atom from slave");      
    
    switch (ph.state) {
    case ControlPhase::CONTROL_RESPONSE:
      // processed in the initiating methods
      break;
    }
  }


  // /////////////////////////////////////////////////////////////////// //

protected:
  
  /// Create all default output plugins
  /**
   * Adds one default output plugin instance for each type
   * in the internal map. This is done so plugin constructors is called
   * during elaboration phase.
   */
  void create_all_default_output_plugins() {
    OutputPlugin_if *op;
    // create default output plugins and add them to the default map
    OutpPlFabricMap_type::iterator i;
    for(i = getFabricMap().begin(); i != getFabricMap().end(); ++i) {
      op = (*i).second("default", &m_OutpPl_event_listener);
      if (op)
        m_DefaultOutputPlugins.insert(std::make_pair((*i).first, op));
      else
        //this should never happen
        SC_REPORT_ERROR(name(), "Error in a plugin constructor (return null pointer).  Please contact GreenSocs about this error.");
    }
  }
  
  /// Fabric which creates the correct OutputPlugin corresponding to the given OutputPluginType
  /**
   * @param outputPluginType  Type (enum OutputPluginType) identifier for the OutputPlugin that should be created.
   * @param constructParam    Parameter for the OutputPlugin's constructor.
   * @return  Pointer to the created OutputPlugin instance.
   */
  OutputPlugin_if* outputPluginFabricCreator(const unsigned int outputPluginType,
                                             const char* constructParam) {
    OutputPlugin_if *op = NULL;
    unsigned int _outputPluginType = outputPluginType;
    if (_outputPluginType == DEFAULT_OUT) // handle default
      _outputPluginType = m_default_output_plugin_type;

    if (getFabricMap().find(_outputPluginType) != getFabricMap().end())
      op = getFabricMap()[_outputPluginType](constructParam, &m_OutpPl_event_listener);
    else {
      SC_REPORT_ERROR(name(), "OutputPlugin fabric: unknown output plugin!");
    }
    return op;
  }
  
  /// Map of the DEFAULT Output Plugins hold by this plugin. Key: OutputPluginType, value: Pointer to the OutputPlugin
  OutpPlMap_type m_DefaultOutputPlugins; 
  
  /// Map of Output Plugins hold by this plugin.Key: OutputPluginType, value: Pointer to the OutputPlugin
  OutpPlMultiMap_type m_OutputPlugins;
  
  //gs::cnf::cnf_api *m_cnf_api;
  
  /// Event listener for the OutputPlugins
  event_listener<OutputPlugin_base> m_OutpPl_event_listener;

  /// Event listener for the Triggers
  event_listener<trigger_if> m_trigger_event_listener;

  /// Default output plugin (accessed by OutputPluginType::DEFAULT_OUT)
  OutputPluginType m_default_output_plugin_type;

public:
  /// Map of Output Plugins Fabrics. Key: OutputPluginType, value: Pointer to fabric function
  static OutpPlFabricMap_type& getFabricMap() {
    static OutpPlFabricMap_type fabricMap;
    return fabricMap;
  }
    
};


  inline void outputPluginDumpNames() {
    std::cout << "List of registered output plugins:" << std::endl;

    // hard-coded ones (NULL and DEFAULT)
    std::cout << "id 0: " << outputPluginTypeToString(0) << " (hardcoded)" << std::endl;
    std::cout << "id 1: " << outputPluginTypeToString(1) << " (hardcoded)" << std::endl;
    
    // iterate over the registered plugins
    OutpPlFabricMap_type::iterator i;
    for(i = GAV_Plugin::getFabricMap().begin(); i != GAV_Plugin::getFabricMap().end(); ++i) {
      std::cout << "id " << (*i).first << ": " << outputPluginTypeToString((*i).first) << std::endl;
    }
    std::cout << std::endl;
  }

} // end namespace av
} // end namespace gs

#endif
