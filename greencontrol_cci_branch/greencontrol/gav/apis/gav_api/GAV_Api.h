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

#ifndef __GAV_Api_h__
#define __GAV_Api_h__

#include "greencontrol/gav/plugin/gav_globals.h"
#include "greencontrol/gav/plugin/gav_datatypes.h"
#include "greencontrol/gav/plugin/OutputPlugin_if.h"
#include "greencontrol/config.h"
#include "greencontrol/gav/plugin/event_listener.h"
#include "greencontrol/gav/plugin/trigger_if.h"

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
    public gc_port_if
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
          mInstanceMap->insert(pair<sc_module*, shared_ptr<GAV_Api> >(mod, newapi_sp));
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
        m_gc_port(AV_SERVICE, GAV_API_NAME, false)
    { 
      m_gc_port.api_port(*this);  // bind sc_port of m_gc_port
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
    void masterAccess(ControlTransactionContainer &t_p)
    {
      
      ControlTransactionHandle tr = t_p.first;
      ControlPhase ph = t_p.second;

      GAV_DUMP_N(name(), "got "<<ph.toString().c_str()<<" atom from master");      

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

      ControlPhase p(ControlPhase::CONTROL_RESPONSE);
      if (!success) p.state = ControlPhase::CONTROL_ERROR;
      // Answer with a CONTROL_RESPONSE or CONTROL_ERROR phase
      GAV_DUMP_N(name(), "send "<<p.toString().c_str()<<" atom back to master");      

      ControlTransactionContainer ctc = ControlTransactionContainer(t_p.first, p);
      m_gc_port.target_port.out->notify(ctc, PEQ_IMMEDIATE);

    }
    
    /// Called by gc_port through gc_port_if when notification arrives.
    /**
     * Implements pc_port_if.
     * This method starts whenever a slave triggers a payload-event.
     * That happens when one of the GC-API methods below send a transaction.
     */
    void slaveAccess(ControlTransactionContainer &t_p)
    {  
  #ifdef GAV_VERBOSE
      ControlTransactionHandle tr = t_p.first;
      ControlPhase ph = t_p.second;

      GAV_DUMP_N(name(), "got "<<ph.toString().c_str()<<" atom from slave");      

      switch (ph.state) {
        // No notification for response state is needed because caller directly gets the 
        // changes in the transaction since it is immediate communication
      }
  #endif
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
      ControlTransactionHandle th = m_gc_port.init_port.create_transaction();
      th->set_mService(AV_SERVICE);
      th->set_mCmd(CMD_CREATE_OUTPUT_PLUGIN);
      //th->set_mAnyPointer();
      //th->set_mAnyPointer2();
      th->set_mAnyUint(type);
      //th->set_mSpecifier();
      th->set_mValue(ctor_param);
      
      ControlPhase p(ControlPhase::CONTROL_REQUEST);
      ControlTransactionContainer ctc = ControlTransactionContainer(th,p);
      m_gc_port.init_port.out->notify(ctc, PEQ_IMMEDIATE);
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
      ControlTransactionHandle th = m_gc_port.init_port.create_transaction();
      th->set_mService(AV_SERVICE);
      th->set_mCmd(CMD_ADD_TO_OUTPUT_PLUGIN);
      th->set_mAnyPointer(NULL);
      th->set_mAnyPointer2(static_cast<void*>(par));
      th->set_mAnyUint(type);
      //th->set_mSpecifier();
      //th->set_mValue();
      
      ControlPhase p(ControlPhase::CONTROL_REQUEST);
      ControlTransactionContainer ctc = ControlTransactionContainer(th,p);
      m_gc_port.init_port.out->notify(ctc, PEQ_IMMEDIATE);
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
      ControlTransactionHandle th = m_gc_port.init_port.create_transaction();
      th->set_mService(AV_SERVICE);
      th->set_mCmd(CMD_ADD_TO_OUTPUT_PLUGIN);
      th->set_mAnyPointer(NULL);
      th->set_mAnyPointer2(NULL);
      th->set_mAnyUint(type);
      
      ControlPhase p(ControlPhase::CONTROL_REQUEST);
      ControlTransactionContainer ctc = ControlTransactionContainer(th,p);
      m_gc_port.init_port.out->notify(ctc, PEQ_IMMEDIATE);
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
      if (par == NULL) {
        SC_REPORT_WARNING(name(), "add_to_output: parameter is NULL.");
        return NULL;
      }
      GAV_PRINT_SPACE;
      GAV_DUMP_N(name(), "add_to_output(id="<<(void*)outputPluginID<<", '"<<par->getName().c_str()<<"')");
      if (outputPluginID == 0) {
        SC_REPORT_WARNING(name(), "add_to_output failed because of invalid OutputPlugin ID (=0)!");
        return NULL;
      }
      
      // Create Transaction and send it to analysis plugin
      ControlTransactionHandle th = m_gc_port.init_port.create_transaction();
      th->set_mService(AV_SERVICE);
      th->set_mCmd(CMD_ADD_TO_OUTPUT_PLUGIN);
      th->set_mAnyPointer(reinterpret_cast<void*>(outputPluginID));
      th->set_mAnyPointer2(static_cast<void*>(par));
      //th->set_mAnyUint(type);
      //th->set_mSpecifier();
      //th->set_mValue();
      
      ControlPhase p(ControlPhase::CONTROL_REQUEST);
      ControlTransactionContainer ctc = ControlTransactionContainer(th,p);
      m_gc_port.init_port.out->notify(ctc, PEQ_IMMEDIATE);
      if (th->get_mError() == 0) {
        GAV_DUMP_N(name(), "add_to_output: ... adding of param "<<par->getName().c_str()<<" to Output Plugin with id "<<(void*)outputPluginID<<" successfull");
        return static_cast<OutputPlugin_if*>(th->get_mAnyPointer());
      } else {
        GAV_DUMP_N(name(), "add_to_output: ... adding of param "<<par->getName().c_str()<<" to Output Plugin with if "<<(void*)outputPluginID<<" failed (error "<<th->get_mError()<<")!");
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
      ControlTransactionHandle th = m_gc_port.init_port.create_transaction();
      th->set_mService(AV_SERVICE);
      th->set_mCmd(CMD_GET_EVENT_LISTENER);
      //th->set_mAnyPointer();
      //th->set_mAnyPointer2();
      //th->set_mAnyUint();
      //th->set_mSpecifier();
      //th->set_mValue();
      
      ControlPhase p(ControlPhase::CONTROL_REQUEST);
      ControlTransactionContainer ctc = ControlTransactionContainer(th,p);
      m_gc_port.init_port.out->notify(ctc, PEQ_IMMEDIATE);
      
      if (th->get_mError() != 0) {
        GAV_DUMP_N(name(), "get_event_listener: ... getting list failed!");
        SC_REPORT_WARNING(name(), "get_event_listener failed!");
        return NULL;
      }
      GAV_DUMP_N(name(), "get_event_listener: ... got event listener successfully.");
      return static_cast<event_listener<trigger_if>*>(th->get_mAnyPointer());
    }

    // /////////////////    END GAV-API methods     /////////////////////////////////// //
     
  protected:

  };

    
} // end namespace av
} // end namespace gs

#endif
