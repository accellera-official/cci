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


// doxygen comments

#ifndef __gav_plugin_h__
#define __gav_plugin_h__


#include <list>
#include <map>

// Core
#include "greencontrol/core/gc_port_if.h"
#include "greencontrol/core/gc_port.h"
#include "greencontrol/core/command_if.h"

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
 *
 * TODO: Make this not an sc_module. Problem: Automatically created params 
 *       created by the child Output Plugins need to get the correct name!
 */
class GAV_Plugin
: public sc_module, 
  public gc_port_if,
  public command_if
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
   * The constructor parameter name is because the Plugin needs (currently)
   * to be an sc_module, but shall have no other name than "AnalysisPlugin".
   *
   * @param name  Name of the instance. SHALL NOT BE CHANGED!!
   */
  GAV_Plugin(sc_module_name name = "AnalysisPlugin")
  : sc_module(name)
  , m_gc_port(AV_SERVICE, "GAV_Plugin", true, this)
  , m_OutpPl_event_listener("OutpPlEventListener")
  , m_trigger_event_listener("TriggerEventListener")
  , m_default_output_plugin_type(NULL_OUT)
  //, m_name("AnalysisPlugin")
  { 
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
   * The constructor parameter name is because the Plugin needs (currently)
   * to be an sc_module, but shall have no other name than "AnalysisPlugin".
   *
   * @param default_output_plugin_type  Default type for output plugins.
   * @param name  Name of the instance. SHALL NOT BE DIFFERENT FROM "AnalysisPlugin"!!
   */
  GAV_Plugin(sc_module_name name, OutputPluginType default_output_plugin_type)
  : sc_module(name)
  , m_gc_port(AV_SERVICE, "GAV_Plugin", true, this)
  , m_OutpPl_event_listener("OutpPlEventListener")
  , m_trigger_event_listener("TriggerEventListener")
  , m_default_output_plugin_type(default_output_plugin_type)
  //, m_name("GAV_Plugin")
  { 
    DEPRECATED_WARNING("GAV_Plugin", "DEPRECATED: GAV_Plugin Constructor with name is deprecated, use without name instead!");
    GCNF_DUMP_N("GAV_Plugin", "GAV_Plugin constructor!");

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
   * The constructor parameter name is because the Plugin needs (currently)
   * to be an sc_module, but shall have no other name than "AnalysisPlugin".
   *
   * @param default_output_plugin_type  Default type for output plugins.
   * @param name  Name of the instance. SHALL NOT BE CHANGED!!
   */
  GAV_Plugin(OutputPluginType default_output_plugin_type, sc_module_name name = "AnalysisPlugin")
  : sc_module(name)
  , m_gc_port(AV_SERVICE, "GAV_Plugin", true, this)
  , m_OutpPl_event_listener("OutpPlEventListener")
  , m_trigger_event_listener("TriggerEventListener")
  , m_default_output_plugin_type(default_output_plugin_type)
  //, m_name("GAV_Plugin")
  { 
    // create the default GAV_Api instance which is accessible via the NULL pointer
    GAV_Api::getApiInstance(NULL);
    
    create_all_default_output_plugins();
  }
  
  /// Destructor
  ~GAV_Plugin() {
    OutputPlugin_if* op = NULL;
    GAV_DUMP_N(name(), "Delete created Output Plugins:");
    IF_GAV_VERBOSE(showMyOutputPlugins();)
    // delete output plugins
    for (OutpPlMultiMap_type::iterator iter = m_OutputPlugins.begin(); iter != m_OutputPlugins.end(); iter++) {
      op = (*iter).second;
      delete op;
    }
    op = NULL;
    m_OutputPlugins.clear();
    for (OutpPlMap_type::iterator iter = m_DefaultOutputPlugins.begin(); iter != m_DefaultOutputPlugins.end(); iter++) {
      op = (*iter).second;
      delete op;
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
   * Implements gc_port_if. 
   * This method starts whenever a master triggers a payload-event.
   */
  void transport(ControlTransactionHandle &tr)
  {
    GAV_DUMP_N(name(), "got transaction atom from master, command="<<gavCommandToString(tr->get_mCmd()).c_str());
    // show received Transaction
    //GAV_DUMP_N(name(), "  received transaction: "<<tr->toString().c_str());      

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
        GAV_DUMP_N(name(), "Created OutputPlugin (id="<<op->get_id()<<").");
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
        SC_REPORT_WARNING(name(), "transport: unknown command!");
        tr->set_mError(1);
      }
    }
  }

  // //////////////// command_if methods ////////////////////////////////////

  /// Returns the name of the plugin.
  const std::string getName()
  {
    return name();
  }

  // This method should pass the request to a static method in this class, which could also be used by the GAV_Api, so the code providing the name- and
  // description-strings exists only once. However this is not possible, since GAV_Api.h would have to include gav_plugin.h, but gav_plugin.h already includes
  // GAV_Plugin.h. So this would lead to a cyclic inclusion. It could be solved by using the template technique like it is used in GCnf, but that wouldn't be
  // reasonable. So the code exists twice, in GAV_Api.h and in gav_plugin.h.

  /// Returns the name of the specified command.
  const std::string getCommandName(unsigned int cmd)
  {
    // Remember to repeat any changes made here in GAV_Api.h!
    switch (cmd) {
      case CMD_NONE:
        return std::string("CMD_NONE");
      case CMD_ADD_TO_OUTPUT_PLUGIN:
        return std::string("CMD_ADD_TO_OUTPUT_PLUGIN");
      case CMD_CREATE_OUTPUT_PLUGIN:
        return std::string("CMD_CREATE_OUTPUT_PLUGIN");
      case CMD_GET_EVENT_LISTENER:
        return std::string("CMD_GET_EVENT_LISTENER");
      default:
        return std::string("unknown");
    }
  }

  /// Return a description of the specified command.
  const std::string getCommandDescription(unsigned int cmd)
  {
    // Remember to repeat any changes made here in GAV_Api.h!
    switch (cmd) {
      case CMD_NONE:
        return std::string("No command.");
      case CMD_ADD_TO_OUTPUT_PLUGIN:
        return std::string("Adds a parameter to an OutputPlugin.");
      case CMD_CREATE_OUTPUT_PLUGIN:
        return std::string("Creates an OutputPlugin instance of the specified type.");
      case CMD_GET_EVENT_LISTENER:
        return std::string("Return an event listener for Trigger objects.");
      default:
        return std::string("unknown");
    }
  }

  // ///////////////////////////////////////////////////////////////////////// //

  /// Returns the name of this Plugin
  //const char* name() { return m_name.c_str(); }
  
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
  
  /// Map of Output Plugins hold by this plugin. Key: OutputPluginType, value: Pointer to the OutputPlugin
  OutpPlMultiMap_type m_OutputPlugins;
  
  //gs::cnf::cnf_api_if *m_cnf_api;
  
  /// Event listener for the OutputPlugins
  event_listener<OutputPlugin_base> m_OutpPl_event_listener;

  /// Event listener for the Triggers
  event_listener<trigger_if> m_trigger_event_listener;

  /// Default output plugin (accessed by OutputPluginType::DEFAULT_OUT)
  OutputPluginType m_default_output_plugin_type;

  /// Plugin name for debug
  //const std::string m_name;

public:
  /// Map of Output Plugins Fabrics. Key: OutputPluginType, value: Pointer to fabric function
  static OutpPlFabricMap_type& getFabricMap() {
    static OutpPlFabricMap_type fabricMap;
    return fabricMap;
  }
    
private:

#ifdef GAV_VERBOSE
  void showMyOutputPlugins() {
    std::multimap<unsigned int, OutputPlugin_if*>::iterator iter;
    sc_object* ob;
    // iterate over the map of output plugins searching for the correct pointer
    for( iter = m_OutputPlugins.begin(); iter != m_OutputPlugins.end(); iter++ ) {
      std::cout << "  (id=" << (*iter).second->get_id()<<")";
      ob = dynamic_cast<sc_object*> ((*iter).second);
      assert (ob != NULL && "An Output Plugin must be an sc_object!");
      if (ob) std::cout << " name: " << ob->name();
      std::cout << std::endl;
    }
  }
#endif
  
};

  /*inline void outputPluginDumpNames() {
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
  }*/
  
} // end namespace av
} // end namespace gs

#endif
