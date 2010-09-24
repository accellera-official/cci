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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// doxygen comments

#ifndef __GAV_Api_h__
#define __GAV_Api_h__

#include "greencontrol/gav/plugin/gav_globals.h"
#include "greencontrol/gav/plugin/gav_datatypes.h"
#include "greencontrol/gav/plugin/OutputPlugin_if.h"
#include "greencontrol/config.h"
#include "greencontrol/gav/plugin/event_listener.h"
#include "greencontrol/gav/plugin/trigger_if.h"

#include "greencontrol/core/command_if.h"

#define GAV_API_NAME "GAV_Api"


namespace gs {
namespace av {

  using gs::cnf::gs_param_base;
  using boost::shared_ptr;
    
  /// GreenAV main access API
  /**
   * This API is the connection to the GAV_Plugin via the 
   * GreenControl framework.
   *
   * Mainly this API allows access to StatisticsCalculators
   * and OutputPlugins
   */
  class GAV_Api
  : public sc_object,
    public gc_port_if,
    public command_if
  {
    // Typedef for static map containing pointers to the Apis
    //        sc_core::sc_module-pointer, Api-instance-smart-pointer
    typedef std::map<sc_module*, shared_ptr<GAV_Api> >  apiMap;
    
  public:

    static shared_ptr<GAV_Api> getApiInstance(sc_core::sc_module *mod = NULL) 
    {
      if (!mod) {
        //sc_assert(false);
        //SC_REPORT_ERROR("static GAV_Api::getApiInstance", "no pointer address to parent module");
        GAV_DUMP_N("static GAV_Api::getApiInstance", "No pointer address to parent module, use/create default GAV_Api.");
      }
#ifdef GAV_VERBOSE
      else {
        GAV_DUMP_N("static GAV_Api::getApiInstance", "module pointer: i"<<(void*)mod<<" and name: "<<mod->name());
      }
#endif
      /// static map sc_core::sc_module-pointer -> Scml_Api-instance-pointer
      static apiMap* mInstanceMap = NULL;
      if (!mInstanceMap) {
        mInstanceMap = new apiMap();
        GAV_DUMP_N("static GAV_Api::getApiInstance", "create new apiMap");
      } // TODO: delete this instance!?
      
      shared_ptr<GAV_Api> gcapi;
      // If for the overgiven module <mod> an API instance exists, return it
      apiMap::iterator pos;
      pos = mInstanceMap->find(mod);
      if (pos != mInstanceMap->end()) {
        return pos->second;
      }
      // if it does not yet exist,
      //  - create one if api_to_register == NULL (should only be used before sc_start_of_simulation_invoked() = true)
      //  - store pointer if api_to_register != NULL
      else {
        // Check if the default is not already after ene_of_elab 
        if (!gs::cnf::GCnf_Api::initialize_mode && mod == NULL) {
          SC_REPORT_WARNING("static GAV_Api::getApiInstance", "The default GAV_Api has not been created before end_of_elaboration! You have to instantiate the GAV_Plugin first!");
        }
        //if (api_to_register == NULL) {
          GAV_DUMP_N("static GAV_Api::getApiInstance", "create new GAV_Api");
          // If not any longer in elaboration and if not explicitely the default shall be returned
        if (!gs::cnf::GCnf_Api::initialize_mode && mod != NULL) { 
            //SC_REPORT_WARNING("static GAV_Api::getApiInstance", "Creating GAV_Api during simulation not allowed. Use gs::cnf::GAV_Api::use_GAV_Api(this) in your constructor!");
            SC_REPORT_WARNING("static GAV_Api::getApiInstance", "Creating GAV_Api during simulation not allowed. You should use gs::cnf::GAV_Api::getApiInstance(this) in your constructor! If needed, use gs::cnf::GAV_Api::getApiInstance(NULL) to get the default GAV_Api instance even after elaboration.");
            return getApiInstance(NULL);
          }
          shared_ptr<GAV_Api> newapi_sp(new GAV_Api());
          mInstanceMap->insert(std::pair<sc_module*, shared_ptr<GAV_Api> >(mod, newapi_sp));
          return newapi_sp;
        /*} else {
          GAV_DUMP_N("static GAV_Api::getApiInstance", "register existing GAV_Api");
          shared_ptr<GAV_Api> api_sp(api_to_register);
          mInstanceMap->insert(pair<sc_module*, shared_ptr<GAV_Api> >(mod, api_sp));
          return api_sp;
        }*/
      }
    }

    /// Port to send and receive ControlTransactionContiners.
    gc_port m_gc_port;

  public:

    /// Constructor.
    /**
     */
    GAV_Api()
      : sc_object(sc_gen_unique_name("__gav_api__")),
        m_gc_port(AV_SERVICE, GAV_API_NAME, false, this)
    { 
      GAV_DUMP_N(name(), "constructor GAV_Api()");
    }

    /// Destructor
    ~GAV_Api() {
    }
    

    ////////////////////// GC_PORT_IF ///////////////////////////////////

    /// Called by gc_port through gc_port_if when notification arrives.
    /**
     * Implements pc_port_if.
     * This method starts whenever a master triggers a payload-event.
     */
    void transport(ControlTransactionHandle &tr)
    {
      GAV_DUMP_N(name(), "got transaction atom from master");      

      // show received Transaction
      GAV_DUMP_N(name(), "  received transaction: "<<(tr->toString()).c_str());      

      bool success = false;
      // According to the command fill the transaction or make actions
      switch (tr->get_mCmd()) {

      // TODO cases
          
      default:
        SC_REPORT_WARNING(name(), "Unknown command in transaction");
        success = false;
      }

      if (!success)
        tr->set_mError(1);
    }
    

    // /////////////////    BEGIN GAV-API methods   /////////////////////////////////// //

    
    /// Creates a new OutputPlugin instance of the specified type in the plugin.
    /**
     * @param type  Type of the OutputPlugin that should be created: 
     *              member of enum OutputPluginType (see gav_datatypes.h)
     * @param ctor_param  Constructor parameter (string) that should be given to
     *                    the OutputPlugin's constructor (containing e.g. filename
     *                    dependent on the OutputPlugin type).
     * @return Identifier of the created OutputPlugin. This is needed to access it.
     */
    OutputPlugin_if* create_OutputPlugin(const OutputPluginType type, std::string ctor_param) {
      GAV_PRINT_SPACE;
      GAV_DUMP_N(name(), "create_OutputPlugin(type="<<outputPluginTypeToString(type).c_str()<<", ctor='"<<ctor_param.c_str()<<"')");
      
      // Create Transaction and send it to analysis plugin
      ControlTransactionHandle th = m_gc_port.createTransaction();
      th->set_mService(AV_SERVICE);
      th->set_mCmd(CMD_CREATE_OUTPUT_PLUGIN);
      //th->set_mAnyPointer();
      //th->set_mAnyPointer2();
      th->set_mAnyUint(type);
      //th->set_mSpecifier();
      th->set_mValue(ctor_param);
      
      m_gc_port->transport(th);
      if (th->get_mError() == 0) {
        GAV_DUMP_N(name(), "create_OutputPlugin: ... created OutputPlugin of type "<<outputPluginTypeToString(type).c_str()<<" (constructor param '"<<ctor_param.c_str()<<"') successfully.");
        return static_cast<OutputPlugin_if*>(th->get_mAnyPointer());
      } else {
        GAV_DUMP_N(name(), "create_OutputPlugin: ... creating OutputPlugin of type "<<outputPluginTypeToString(type).c_str()<<" (constructor param '"<<ctor_param.c_str()<<"') failed!");
        SC_REPORT_WARNING(name(), "create_OutputPlugin failed!");
        return NULL;
      }
      return NULL;
    }

    /// Add a gs_param to be outputted by the simulation-wide default output plugin.
    /**
     * @param par   Pointer to the gs_param that should be outputted
     * @return      Pointer to the OutputPlugin, NULL if adding failed
     */
    OutputPlugin_if* add_to_default_output(gs_param_base *par) {
      return add_to_default_output(DEFAULT_OUT, par);
    }
    OutputPlugin_if* add_to_default_output(gs_param_base &par) {
      return add_to_default_output(DEFAULT_OUT, &par);
    }
    
    /// Add a gs_param to be outputted by the (implicit existing) default OutputPlugin of the specified type.
    /**
     * For each OutputPluginType type there is existing one object in the
     * plugin. (The object will be created at its first usage.)
     *
     * @param type  Type of the default OutputPlugin
     * @param par   Pointer to the gs_param that should be outputted
     * @return                Pointer to the OutputPlugin, NULL if adding failed
     */
    OutputPlugin_if* add_to_default_output(const OutputPluginType type, gs_param_base *par) {
      if (par == NULL) {
        SC_REPORT_WARNING(name(), "add_to_default_output: parameter is NULL.");
        return NULL;
      }
      GAV_PRINT_SPACE;
      GAV_DUMP_N(name(), "add_to_default_output(type="<<outputPluginTypeToString(type).c_str()<<", '"<<par->getName().c_str()<<"')");
      
      // Create Transaction and send it to analysis plugin
      ControlTransactionHandle th = m_gc_port.createTransaction();
      th->set_mService(AV_SERVICE);
      th->set_mCmd(CMD_ADD_TO_OUTPUT_PLUGIN);
      th->set_mAnyPointer(NULL);
      th->set_mAnyPointer2(static_cast<void*>(par));
      th->set_mAnyUint(type);
      //th->set_mSpecifier();
      //th->set_mValue();
      
      m_gc_port->transport(th);
      if (th->get_mError() == 0) {
        GAV_DUMP_N(name(), "add_to_default_output: ... adding of param "<<par->getName().c_str()<<" to default Output Plugin "<<outputPluginTypeToString(type).c_str()<<" successfull.");
        return static_cast<OutputPlugin_if*>(th->get_mAnyPointer());
      } else {
        GAV_DUMP_N(name(), "add_to_default_output: ... adding of param "<<par->getName().c_str()<<" to default Output Plugin "<<outputPluginTypeToString(type).c_str()<<" failed (error "<<th->get_mError()<<")!");
        SC_REPORT_WARNING(name(), "add_to_default_output failed!");
        return NULL;
      }
      return NULL;
    }
    OutputPlugin_if* add_to_default_output(const OutputPluginType type, gs_param_base &par) {
      return add_to_default_output(type, &par);
    }
    
    /// Get the default OutputPlugin of the specified type or the simulation-wide default.
    /**
     * For each OutputPluginType type there is existing one object in the
     * plugin. (The object will be created at its first usage.)
     *
     * Not specifying the Output Plugin type will return the 
     * simulation-wide default one.
     *
     * @param type  Type of the default OutputPlugin
     * @return      Pointer to the OutputPlugin, NULL if getting failed
     */
    OutputPlugin_if* get_default_output(const OutputPluginType type = DEFAULT_OUT) {
      GAV_PRINT_SPACE;
      GAV_DUMP_N(name(), "get_default_output(type="<<outputPluginTypeToString(type).c_str()<<")");
      
      // Create Transaction and send it to analysis plugin
      ControlTransactionHandle th = m_gc_port.createTransaction();
      th->set_mService(AV_SERVICE);
      th->set_mCmd(CMD_ADD_TO_OUTPUT_PLUGIN);
      th->set_mAnyPointer(NULL);
      th->set_mAnyPointer2(NULL);
      th->set_mAnyUint(type);
      
      m_gc_port->transport(th);
      if (th->get_mError() == 0) {
        GAV_DUMP_N(name(), "get_default_output: ... getting default Output Plugin "<<outputPluginTypeToString(type).c_str()<<" successfull.");
        return static_cast<OutputPlugin_if*>(th->get_mAnyPointer());
      } else {
        GAV_DUMP_N(name(), "get_default_output: ... getting default Output Plugin "<<outputPluginTypeToString(type).c_str()<<" failed (error "<<th->get_mError()<<")!");
        SC_REPORT_WARNING(name(), "get_default_output failed!");
        return NULL;
      }
      return NULL;
    }

    /// Add a gs_param to be outputted by the specified OutputPlugin.
    /**
     * The specified OutputPlugin has to be created by calling create_OutputPlugin.
     *
     * @param outputPluginID  OutpPlugin ptr (returned by create_OutputPlugin) to the
     *                        OutputPlugin the parameter should be outputted.
     * @param par             Pointer to the gs_param that should be outputted
     * @return                Pointer to the OutputPlugin, NULL if adding failed
     */
    OutputPlugin_if* add_to_output(OutputPlugin_if* outputPluginID, gs_param_base *par) {
      if (par == NULL || outputPluginID == NULL) {
        SC_REPORT_WARNING(name(), "add_to_output: parameter or Output Plugin is NULL.");
        return NULL;
      }
      GAV_PRINT_SPACE;
      GAV_DUMP_N(name(), "add_to_output(id="<<outputPluginID->get_id()<<", '"<<par->getName().c_str()<<"')");
      
      // Create Transaction and send it to analysis plugin
      ControlTransactionHandle th = m_gc_port.createTransaction();
      th->set_mService(AV_SERVICE);
      th->set_mCmd(CMD_ADD_TO_OUTPUT_PLUGIN);
      th->set_mAnyPointer(reinterpret_cast<void*>(outputPluginID));
      th->set_mAnyPointer2(static_cast<void*>(par));
      //th->set_mAnyUint(type);
      //th->set_mSpecifier();
      //th->set_mValue();
      
      m_gc_port->transport(th);
      if (th->get_mError() == 0) {
        GAV_DUMP_N(name(), "add_to_output: ... adding of param "<<par->getName().c_str()<<" to Output Plugin with id "<<outputPluginID->get_id()<<" successfull");
        return static_cast<OutputPlugin_if*>(th->get_mAnyPointer());
      } else {
        GAV_DUMP_N(name(), "add_to_output: ... adding of param "<<par->getName().c_str()<<" to Output Plugin with if "<<outputPluginID->get_id()<<" failed (error "<<th->get_mError()<<")!");
        SC_REPORT_WARNING(name(), "add_to_output failed!");
        return NULL;
      }
      return NULL;
    }
    
    /// Returns an event listerner for Triggers
    /**
     * @return Pointer to the plugin's event listener
     */
    event_listener<trigger_if>* get_event_listener() {
      GAV_PRINT_SPACE;
      GAV_DUMP_N(name(), "get_event_listener()");
      
      // Create Transaction and send it to analysis plugin
      ControlTransactionHandle th = m_gc_port.createTransaction();
      th->set_mService(AV_SERVICE);
      th->set_mCmd(CMD_GET_EVENT_LISTENER);
      //th->set_mAnyPointer();
      //th->set_mAnyPointer2();
      //th->set_mAnyUint();
      //th->set_mSpecifier();
      //th->set_mValue();
      
      m_gc_port->transport(th);
      
      if (th->get_mError() != 0) {
        GAV_DUMP_N(name(), "get_event_listener: ... getting list failed!");
        SC_REPORT_WARNING(name(), "get_event_listener failed!");
        return NULL;
      }
      GAV_DUMP_N(name(), "get_event_listener: ... got event listener successfully.");
      return static_cast<event_listener<trigger_if>*>(th->get_mAnyPointer());
    }

    // /////////////////    END GAV-API methods     /////////////////////////////////// //

    // //////////////// command_if methods ////////////////////////////////////

    /// Returns the name of the API.
    const std::string getName()
    {
      return name();
    }

    // This method should call a static method in the GAV_Plugin class, so the code providing the name- and description-strings exists only once. However this
    // is not possible, since GAV_Api.h would have to include gav_plugin.h, but gav_plugin.h already includes GAV_Plugin.h. So this would lead to a cyclic
    // inclusion. It could be solved by using the template technique like it is used in GCnf, but that wouldn't be reasonable. So the code exists twice, in
    // GAV_Api.h and in gav_plugin.h.
    /// Returns the name of the specified command.
    const std::string getCommandName(unsigned int cmd)
    {
      // Remember to repeat any changes made here in gav_plugin.h!
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
      // Remember to repeat any changes made here in gav_plugin.h!
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
     
  protected:

  };

    
} // end namespace av
} // end namespace gs

#endif
