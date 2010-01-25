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

#ifndef __GCNF_PRIVATE_API_T_H__
#define __GCNF_PRIVATE_API_T_H__

#include <set>
#include <cstdarg>

#include "greencontrol/gcnf/apis/GCnf_private_Api/gcnf_private_plugin.h"  // internal private plugin

#define GCNF_P_API_NAME "GCnf_private_Api"
#undef DUMMY_NAME
#define DUMMY_NAME "**dummy.NewParamObservers**"

#define END_OF_PUBLIC_PARAM_LIST "0"

namespace gs {
namespace cnf {

  template<typename gs_param_base_T, template<class T> class gs_param_T, typename ConfigPlugin_T>
  class GCnf_private_Api_T
  : public sc_object,
    public initialize_if,
    public cnf_api
  {

    /// Friend class to make cmd_NOTIFY_NEW_PARAM_OBSERVER accessable
    template<class T1, class T2, class T3> friend class ConfigPrivatePlugin_T;
    
    /// Typedef for the standard config API
    typedef GCnf_Api_t<gs_param_base_T, GCnf_private_Api_T<gs_param_base, gs_param_T, ConfigPlugin_T>, gs_param_T, ConfigPlugin_T> GCnf_Api_TMPL;
  
    /// Typedef for a multimap saving pointers to callback functions for each parameter.
    /**
     * The multimap may contain several callback pointers for each parameter.
     *
     * The map saves CallbAdapt_b objects which contain the object pointer and the
     * member function pointer.
     */
    typedef std::multimap<std::string, CallbAdapt_b*> observerCallbackMap;
    
  protected:
    /// Default constructor not allowed
    GCnf_private_Api_T() { }

  public:

    /// Contructor private GCnf API with vector of parameter names that are public
    /**
     * @param owner_module   Pointer to owning module
     * @param pub_params   String-vector of parameter names (name underneath the owner hierarchical position) e.g. "par1" for a param "topmod.ModS.ModSub.par1" or "ModSub2.par1" for a param "topmod.ModS.ModSub.ModSub2.par1"
     */
    // * @param full_owner_name  Full hierarchical name of the owner module (name()) e.g. "topmod.ModS.ModSub".
    GCnf_private_Api_T(sc_core::sc_module* owner_module /*const char* full_owner_name*/,
                       std::vector<std::string> pub_params)
    : sc_object(sc_gen_unique_name("__gcnf_api__")),
      mPrivPlugin(this),
      m_registered_as_new_param_observer(false),
      m_new_param_event(NULL)
    {
      assert (owner_module != NULL);
      owner_name = owner_module->name(); // full_owner_name;
      this->owner_module = owner_module;
      GCNF_DUMP_N(name(), "Create private API for owner '"<<owner_name<<"'");
      std::string full_par_name;
      for (unsigned int i = 0; i < pub_params.size(); i++) {
        full_par_name = owner_name;
        full_par_name += SC_NAME_DELIMITER;
        full_par_name += pub_params[i];
        GCNF_DUMP_N(name(), "    public param: '"<<pub_params[i].c_str()<<"' = '"<<full_par_name.c_str()<<"'");
        public_params.insert(full_par_name);
      }
      // Register etc.
      init_this();
    }      

    /// Contructor private GCnf API without public parameters
    /**
     * @param owner_module   Pointer to owning module
     */
    //     * @param full_owner_name  Full hierarchical name of the owner module (name()) e.g. "topmod.ModS.ModSub".
    GCnf_private_Api_T(sc_core::sc_module* owner_module /*const char* full_owner_name*/)
    : sc_object(sc_gen_unique_name("__gcnf_api__")),
      mPrivPlugin(this),
      m_registered_as_new_param_observer(false),
      m_new_param_event(NULL)
    {
      assert (owner_module != NULL);
      owner_name = owner_module->name(); // full_owner_name;
      this->owner_module = owner_module;
      GCNF_DUMP_N(name(), "Create private API for owner '"<<owner_name.c_str()<<"'");
      // Register etc.
      init_this();
    }      

    /// Contructor private GCnf API with variable argument list containing parameter names that are public (terminated by END_OF_PUBLIC_PARAM_LIST)
    /**
     * The variable argument list of this constructor must be terminated
     * using <code>END_OF_PUBLIC_PARAM_LIST</code>.
     *
     * Example:
     * <code>
       ModSub(sc_core::sc_module_name name)
       : sc_core::sc_module(name),
         m_privApi(this, "par1", "ModSub2.par1", END_OF_PUBLIC_PARAM_LIST),
     * </code>
     *
     * @param owner_module   Pointer to owning module
     * @param pub_par   Variable arguments of parameter names (name underneath the owner hierarchical position) e.g. "par1" for a param "topmod.ModS.ModSub.par1" or "ModSub2.par1" for a param "topmod.ModS.ModSub.ModSub2.par1"
     */
    // * @param full_owner_name  Full hierarchical name of the owner module (name()) e.g. "topmod.ModS.ModSub".
    GCnf_private_Api_T(sc_core::sc_module* owner_module /* const char* full_owner_name*/, const char* pub_par ...)
    : sc_object(sc_gen_unique_name("__gcnf_api__")),
      mPrivPlugin(this),
      m_registered_as_new_param_observer(false),
      m_new_param_event(NULL)
    {
      assert (owner_module != NULL);
      owner_name = owner_module->name(); // full_owner_name;
      this->owner_module = owner_module;
      GCNF_DUMP_N(name(), "Create private API for owner '"<<owner_name.c_str()<<"' with vararg public params");

      std::string full_par_name;
      va_list list;
      va_start(list, pub_par);
      const char* p = pub_par;
      for(;;) { 
        if(std::string(p) == std::string(END_OF_PUBLIC_PARAM_LIST))
          break;
        full_par_name = owner_name;
        full_par_name += SC_NAME_DELIMITER;
        full_par_name += p;
        GCNF_DUMP_N(name(), "    public param: '"<<p<<"' = '"<<full_par_name.c_str()<<"'");
        public_params.insert(full_par_name);
        p = va_arg(list, char*);
      }
      va_end(list);
      // Register etc.
      init_this();
    }      
    
    /// Destructor
    ~GCnf_private_Api_T() {
      GCNF_DUMP_N(name(), "Destruct");
      if (m_new_param_event != NULL)
        delete m_new_param_event;
    }
    
    /// Returns the full hierarchical name of the owning module
    const std::string get_owner_name() const {
      return owner_name.c_str();
    }

    // ////////////// API functions (implements cnf_api) //////////////

    /// @see gs::cnf::GCnf_Api::addParam(const std::string&)
    bool addParam(const std::string &parname) {
      DEPRECATED_WARNING(name(), "DEPRECATED: addParam is deprecated due to simulation overhead! Use 'setInitValue(\"par_name\", \"value\")' instead or create a gs_param<type> object.");
      GCNF_DUMP_N(name(), "addParam("<<parname.c_str()<<"), deprecated");
      return addParam(parname, string(""));
    }
    
    /// @see gs::cnf::GCnf_Api::addParam(const std::string&, const std::string&)
    bool addParam(const std::string &parname, const std::string &default_val, std::string meta_data = "") {
      if (!is_local_param(parname) || is_public_param(parname))
        return mApi->addParam(parname, default_val, meta_data);
      DEPRECATED_WARNING(name(), "DEPRECATED: addParam is deprecated due to simulation overhead! Use 'setInitValue(\"par_name\", \"value\")' instead or create a gs_param<type> object.");
      GCNF_DUMP_N(name(), "addParam("<<parname.c_str()<<", "<<default_val.c_str()<<"), deprecated");
      if (!isHierarchicalParameterName(parname)) {
        SC_REPORT_INFO(name(), "Parameter names should be hierarchical names (expect this is a top-level parameter)!");
      }
     
      unsigned int error = mPrivPlugin.cmd_ADD_PARAM(parname, default_val, NULL);
      
      if (error == 0) {
        GCNF_DUMP_N(name(), "addParam: ... adding of param "<<parname.c_str()<<" successfull");
      } else {
        GCNF_DUMP_N(name(), "addParam: ... adding of param "<<parname.c_str()<<" failed (error "<<error<<")!") ;
        SC_REPORT_ERROR(name(), "addParam failed!");
        return false;
      }
      return true;
    }
    
    /// @see gs::cnf::GCnf_Api::addPar
    bool addPar(gs_param_base_T* par, std::string meta_data = "") {
      if (!is_local_param(par->getName()) || is_public_param(par->getName()))
        return mApi->addPar(par, meta_data);
      
      if (par == NULL) SC_REPORT_ERROR(name(), "addPar: Parameter is NULL!");
      GCNF_DUMP_N(name(), "addPar(par) name="<<par->getName().c_str()<<"");
      
      unsigned int error = mPrivPlugin.cmd_ADD_PARAM("", "", par);
      
      if (error == 0) {
        GCNF_DUMP_N(name(), "addPar: ... adding of param "<<par->getName().c_str()<<" successfull");
      } else {
        GCNF_DUMP_N(name(), "addPar: ... adding of param "<<par->getName().c_str()<<" failed (error "<<error<<")!");
        SC_REPORT_ERROR(name(), "addParam failed!");
        return false;
      }
      return true;
    }
    
    /// @see gs::cnf::GCnf_Api::removePar
    bool removePar(gs_param_base_T* par, std::string meta_data = "") {
      if (!is_local_param(par->getName()) || is_public_param(par->getName()))
        return mApi->removePar(par, meta_data);
      if (par == NULL) SC_REPORT_ERROR(name(), "removePar: Parameter is NULL!");
      GCNF_DUMP_N(name(), "removePar(par) name="<<par->getName().c_str()<<"");
      
      if (!par->is_destructing()) {
        GCNF_DUMP_N(name(), "removePar: This method may only be called by the parameter's destructor itself");
        SC_REPORT_ERROR(name(), "removePar: This method may only be called by the parameter's destructor itself.");
        sc_assert(false);
        return false;
      }
      
      unsigned int error = mPrivPlugin.cmd_REMOVE_PARAM(par);
      
      // handle returned errors
      if (error == 0) {
        GCNF_DUMP_N(name(), "removePar: ... remove of param "<<par->getName().c_str()<<" successfull");
      } else {
        GCNF_DUMP_N(name(), "removePar: ... remove of param "<<par->getName().c_str()<<" failed (error "<<error<<")!");
        SC_REPORT_ERROR(name(), "removeParam failed!");
        return false;
      }
      return true;
    }
    
    /// @see gs::cnf::GCnf_Api::setParam
    bool setParam(const std::string &parname, const std::string &value, std::string meta_data = "") {
      if (!is_local_param(parname) || is_public_param(parname))
        return mApi->setParam(parname, value, meta_data);
      DEPRECATED_WARNING(name(), "DEPRECATED: setParam is deprecated due to simulation overhead! Use 'getPar(\"par_name\") = value' or 'setInitValue(\"par_name\", \"init_value\")' instead.");
      GCNF_DUMP_N(name(), "setParam("<<parname.c_str()<<", "<<value.c_str()<<"), deprecated");
      gs_param_base_T* p = getPar(parname);
      bool succ = false;
      if (p == NULL)   succ = setInitValue(parname, value); // Add new parameter
      else succ = p->setString(value); // set existing parameter
      return succ;
    }
    
    /// @see gs::cnf::GCnf_Api::setInitValue
    bool setInitValue(const std::string &parname, const std::string &value, std::string meta_data = "") {
      if (!is_local_param(parname) || is_public_param(parname))
        return mApi->setInitValue(parname, value, meta_data);
      GCNF_DUMP_N(name(), "setInitValue("<<parname.c_str()<<", "<<value.c_str()<<")");      
      
      mPrivPlugin.cmd_SET_INIT_VAL(parname, value);
      
      GCNF_DUMP_N(name(), "setInitValue: ... setting successfull");
      return true;
    }
    
    /// @see gs::cnf::GCnf_Api::getParam
    const std::string& getParam(const std::string &parname, std::string meta_data = "" ) {
      // try to get the param from parent if it is not local or if it is public
      if (!is_local_param(parname) || is_public_param(parname))
        return mApi->getParam(parname, meta_data);
      // try to get the param local
      DEPRECATED_WARNING(name(), "DEPRECATED: getParam is deprecated due to simulation overhead! Use 'value = getPar(\"par_name\")' or 'getPar(...)->getValue()' or 'getPar(...)->getString()' instead.");
      GCNF_DUMP_N(name(), "getParam("<<parname.c_str()<<"), deprecated");      
      gs_param_base_T *p = getPar(parname);
      if (p == NULL) {
        static std::string u = string();
        return u;
      }
      return p->getString();
    }
    
    /// @see gs::cnf::GCnf_Api::getValue
    const std::string getValue(const std::string &parname, std::string meta_data = "") {
      // try to get the param from parent if it is not local or if it is public
      if (!is_local_param(parname) || is_public_param(parname))
        return mApi->getValue(parname, meta_data);
      // try to get the param local
      GCNF_DUMP_N(name(), "getValue("<<parname.c_str()<<")");      
      std::string value;
      if (mPrivPlugin.cmd_GET_VAL(parname, value)) {
        GCNF_DUMP_N(name(), "getValue: ... got value = "<<value.c_str());
      } else {
        GCNF_DUMP_N(name(), "getPar: ... getting failed!");
        SC_REPORT_INFO(name(), "getPar: ... getting value failed!");
      }
      return value;
    }

    /// @see gs::cnf::GCnf_Api::getPar
    gs_param_base_T* getPar(const std::string &parname, std::string meta_data = "") {
      // try to get the param from parent if it is not local or if it is public
      if (!is_local_param(parname) || is_public_param(parname))
        return mApi->getPar(parname, meta_data);
      // try to get the param local
      GCNF_DUMP_N(name(), "getPar("<<parname.c_str()<<")");
      
      gs_param_base_T *par = mPrivPlugin.cmd_GET_PARAM(parname);
      if (par != NULL) {
        GCNF_DUMP_N(name(), "getPar: ... got parameter");
      } else {
        GCNF_DUMP_N(name(), "getPar: ... getting failed!");
        SC_REPORT_INFO(name(), "getPar: ... getting failed!");
      }      
      return par;
    }
    
    /// @see gs::cnf::GCnf_Api::existsParam
    bool existsParam(const std::string &parname, std::string meta_data = "") {
      GCNF_DUMP_N(name(), "existsParam("<<parname.c_str()<<")");      
      
      if (mPrivPlugin.cmd_EXISTS_PARAM(parname)) {
        GCNF_DUMP_N(name(), "existsParam (private): ... yes");
        return true;
      }
      GCNF_DUMP_N(name(), "existsParam (private): ... no");

      // Try to find at parent (if not local or if public)
      if (!is_local_param(parname) || is_public_param(parname))
        return mApi->existsParam(parname, meta_data = "");
      
      return false;
    }

    /// @see gs::cnf::GCnf_Api::is_used
    /**
     * @TODO: implement is_used for private API
     */
    bool is_used(const std::string &parname, std::string meta_data = "") {
      GCNF_DUMP_N(name(), "is_used("<<parname.c_str()<<")");      
      SC_REPORT_ERROR(name(), "is_used not implemented for private config API!");
      // TODO:
      // First check in private plugin
      //if (mPrivPlugin.cmd_CMD_PARAM_HAS_BEEN_ACCESSED(parname)) {
      //  GCNF_DUMP_N(name(), "is_used (private): ... yes");
      //  return true;
      //}
      GCNF_DUMP_N(name(), "is_used (private): ... no");
      
      // Try to find at parent (if not local or if public)
      if (!is_local_param(parname) || is_public_param(parname))
        return mApi->is_used(parname, meta_data = "");

      return false;
    }
    
    /// @see gs::cnf::GCnf_Api::is_explicit
    /*bool is_explicit(const std::string &parname) {
      // try to get the param from parent if it is not local or if it is public
      if (!is_local_param(parname) || is_public_param(parname))
        return mApi->is_explicit(parname);
      GCNF_DUMP_N(name(), "is_explicit("<<parname.c_str()<<")");
      
      gs_param_base_T *par = mPrivPlugin.cmd_GET_PARAM(parname);
      if (par != NULL) {
        return true;
      }
      return false;
    }*/
      
    
    /// @see gs::cnf::GCnf_Api::getParamList()
    const std::vector<std::string> getParamList() {
      return getParamList("", false);
    }
    
    /// @see gs::cnf::GCnf_Api::getParamList(const std::string&)
    const std::vector<std::string> getParamList(const std::string &module_name, std::string meta_data = "") {
      return getParamList(module_name, false, meta_data);
    }
    
    /// @see gs::cnf::GCnf_Api::getParamList(const std::string&, bool)
    const std::vector<std::string> getParamList(const std::string &module_name, bool including_childs, std::string meta_data = "") {
      std::vector<std::string> parentParamList = mApi->getParamList(module_name, including_childs, meta_data);
      std::vector<std::string> myParamList;
      
      std::stringstream ss;
      GCNF_DUMP_N(name(), "getParamList(...)");
      
      ss << module_name;
      if (including_childs) ss << ".*";
      myParamList = mPrivPlugin.cmd_GET_PARAM_LIST_VEC(module_name);
     
#ifdef GC_VERBOSE
      // Show vector
      vector<string>::iterator iter;
      std::stringstream ss_show;
      iter = myParamList.begin();
      while( iter != myParamList.end() ) {
        if (iter != myParamList.begin()) ss_show << ", ";
        ss_show << *iter;
        iter++;
      }
      GCNF_DUMP_N(name(), "getParamList: ... got list for '"<<module_name.c_str()<<"': '"<<ss_show.str().c_str()<<"'");
#endif
      // combine parent list with private List
      myParamList.insert(myParamList.end(), parentParamList.begin(), parentParamList.end());
      return myParamList;
    }  

    /// @see gs::cnf::GCnf_Api::getParams(const std::string &module_name)
    const std::vector<gs_param_base*> getParams(const std::string &module_name = "") {
      std::vector<gs_param_base*> parentParamList = mApi->getParams(module_name);
      std::vector<gs_param_base*> myParamList;

      GCNF_DUMP_N(name(), "getParams("<<module_name.c_str()<<")");
      
      std::vector<std::string> vec = mPrivPlugin.cmd_GET_PARAM_LIST_VEC(module_name);
      std::vector<std::string>::iterator iter;
      gs_param_base *p;
      for (iter = vec.begin(); iter != vec.end(); iter++) {
        p = getPar(*iter);
        if (p != NULL)
          myParamList.push_back(p);
      }

      // combine parent list with private List
      myParamList.insert(myParamList.end(), parentParamList.begin(), parentParamList.end());
      return myParamList;
    }
    
    /// @see gs::cnf::GCnf_Api::getUpdateEvent
    sc_event& getUpdateEvent(const std::string &parname)  throw (RegisterObserverFailedException) {
      if (is_public_param(parname))
        return mApi->getUpdateEvent(parname);
      DEPRECATED_WARNING(name(), "DEPRECATED: getUpdateEvent is deprecated! Use 'my_param.getUpdateEvent()' or use callback 'REGISTER_PARAM_CALLBACK(&my_param, class, method)' instead.");
      SC_REPORT_ERROR(name(), "getUpdateEvent is deprecated (in general) and (especially) must not be used within private APIs!");
      return deprecatedUpdateEvent;
    }
    
    /// @see gs::cnf::GCnf_Api::registerCallback
    bool registerCallback(const std::string &parname, CallbAdapt_b *callb) {
      // if it is not local or if it is public
      if (!is_local_param(parname) || is_public_param(parname))
        return mApi->registerCallback(parname, callb);
      // else
      DEPRECATED_WARNING(name(), "DEPRECATED: registerCallback (and macro REGISTER_CALLBACK) is deprecated due to simulation overhead! Use 'REGISTER_PARAM_CALLBACK(&my_param, class, method)' instead.");
      SC_REPORT_ERROR(name(), "registerCallback is deprecated (in general) and (especially) must not be used within private APIs!");
      return false;
    }
    
    /// @see gs::cnf::GCnf_Api::getNewParamEvent
    sc_event& getNewParamEvent() throw (RegisterObserverFailedException) {
      /* 
       * Register this with a NewParamCallback at the parent API
       * and create a local new_param_event which will be notified 
       * when new params at the parent arrive.
       */
      DEPRECATED_WARNING("GCnf_Api", "DEPRECATED: The getNewParamEvent function is deprecated due to performance reasons! Register a callback instead using mApi.REGISTER_NEW_PARAM_CALLBACK(class_name, callback_function);");
      register_as_new_param_observer(); // automatically checks if already done
      if (m_new_param_event == NULL) {
        m_new_param_event = new sc_event();
      }
      return *m_new_param_event;
    }
    
    /// @see gs::cnf::GCnf_Api::registerNewParamCallback
    bool registerNewParamCallback(CallbAdapt_b *callb) {
      const std::string parname = DUMMY_NAME;
      GCNF_DUMP_N(name(), "registerNewParamCallback(func_ptr)"); 
      
      // Debug: Check if already a callback registered
      int cnt = m_observer_callback_map.count(parname);
      if (cnt > 0) {
        GCNF_DUMP_N(name(), "registerCallback: There is/are already "<<cnt<<" registered callback function(s) for the parameter "<<parname.c_str()<<".");
      }

      register_as_new_param_observer(); // automatically checks if already done
      
      // Add object and function pointer to multimap
      m_observer_callback_map.insert( pair<std::string, CallbAdapt_b*>( string(parname), callb ) );
      
      return true;
    }
    
    /// @see gs::cnf::GCnf_Api::legacy_callback_adapter_method
    void legacy_callback_adapter_method(gs_param_base_T &par) {
      DEPRECATED_WARNING(name(), "DEPRECATED: legacy_callback_adapter_method is deprecated due to simulation overhead!");
      SC_REPORT_ERROR(name(), "DEPRECATED: legacy_callback_adapter_method is deprecated due to simulation overhead!");
    }
    
    /// @see gs::cnf::GCnf_Api::unregisterAllCallbacks
    void unregisterAllCallbacks(void* observer, std::string meta_data = "") {
      // call at parent
      mApi->unregisterAllCallbacks(observer, meta_data);
      // process locally
      if (observer == NULL) SC_REPORT_ERROR(name(), "unregisterAllCallbacks: Observer pointer is NULL!");
      GCNF_DUMP_N(name(), "unregisterAllCallbacks(observer="<<observer<<")");
      
      // Do not send transaction if simulation has already stopped
      // (because some GreenConfig Core objects may already be destructed)
      // TODO!!!!
      //if (!sc_is_running()/*sc_end_of_simulation_invoked()*/) {
      //  GCNF_DUMP_N(name(), "removePar: Removing will NOT be performed after simulation (because some GreenConfig Core objects may already be destructed).");
      //  return false;
      //}
      mPrivPlugin.cmd_UNREGISTER_PARAM_CALLBACKS(observer);
      GCNF_DUMP_N(name(), "unregisterAllCallbacks: ... unregister callbacks for obserserver "<<observer<<" successfull");
    }
    
    // //////////////// end API functions //////////////////// //

    // ///////////// initialize_if functions ///////////////// //
    
    /// Implements initialize_if, deal the initialize-mode.
    virtual void start_initial_configuration() {
    }
    
    /// Implements initialize_if, deal the initialize-mode.
    virtual void end_initialize_mode() {
    }
    
    // /////////////////////////////////////////////////////// //

    /// Function being called by the parent API when new parameters are added
    void new_param_callback(const std::string parname, const std::string value) {
      GCNF_DUMP_N(name(), "   internal new param callback (param "<<parname.c_str()<<")");
      cmd_NOTIFY_NEW_PARAM_OBSERVER(parname, value, NULL);
    }
    
  protected:
    
    /// Checks if given parameter is a child of the module which owns this API
    /**
     * @param parname  Name of the parameter to check
     */
    bool is_local_param(const std::string& parname) {
      return is_child_of(parname, owner_module->name());
    }
    
    /// Handles notifies of new parameters from the private config plugin
    /**
     * @param pname     If implicit: parameter name that is notifed to be new.
     * @param init_val  Value of the new implicit parameter.
     * @param par       Pointer to the new explicit parameter
     * @return  Returns if error: 0 = no error, >0 = error
     */
    void cmd_NOTIFY_NEW_PARAM_OBSERVER(const std::string& pname, const std::string& init_val,
                                               gs_param_base_T* par) {
      const std::string dummyParName = DUMMY_NAME;
      
      // Notify the oberserver events
      GCNF_DUMP_N(name(), "cmd_NOTIFY_NEW_PARAM_OBSERVER: notify event for new added parameter "<<pname.c_str());      
      
      if (m_new_param_event != NULL) {
        // Attention: Only wait for these events after elaboration! (Before use callbacks.)
        // notify event
        m_new_param_event->notify();
      }
     
      // Call back all the callback observer methods for this parameter
      if (par != NULL)
        makeCallbacks(dummyParName, par->getName(), par->getString()  );
      else
        makeCallbacks(dummyParName, pname, init_val);
    }
    
    
    /// Initialized this API (Registers this API at the static function, etc.)
    /**
     * Important: get the next API before registering this one!!!
     */
    inline void init_this() {
      /// get next API upwards
      mApi = GCnf_Api_TMPL::getApiInstance(owner_module);
#ifdef GCNF_VERBOSE
      GCnf_private_Api_T* p = dynamic_cast<GCnf_private_Api_T*>(mApi);
      if (p) {
        GCNF_DUMP_N(name(), "  API upwards is private API of owning module '"<<p->get_owner_name().c_str()<<"'.");
      }
      else {
        GCNF_DUMP_N(name(), "  API upwards is NO private API (but e.g. the default one).");
      }
#endif
      /// register this API
      GCNF_DUMP_N(name(), "  register this private API");
      GCnf_Api_TMPL::register_private_ApiInstance(this);
    }
    
    /// Returns if the given parameter is a public one within this private API.
    /**
     * @param   par_name  Name of the parameter that should be checked.
     * @return  If the given parameter is a public one within this private API.
     */
    inline bool is_public_param(const std::string& par_name) {
      if (public_params.find(par_name) != public_params.end())
        return true;
      return false;
    }
    
    /// Iterates the observer callback map and makes the callbacks.
    /**
     * @param search     Search string for the map (parameter name or dummy for new parameters).
     * @param par_name   Name of the changed parameter.
     * @param value      New value of the changed parameter.
     */
    void makeCallbacks(const std::string &search, const std::string &par_name, const std::string &value) {
      CallbAdapt_b *callb;
      observerCallbackMap::iterator it;
      pair<observerCallbackMap::iterator, observerCallbackMap::iterator> begin_end;        
      begin_end = m_observer_callback_map.equal_range(search);
      for (it = begin_end.first;  it != begin_end.second;   ++it) {
        GCNF_DUMP_N(name(), "masterAccess: callback for parameter "<<search.c_str());      
        callb = (*it).second;
        // Make call
        callb->call(par_name, value);
      }
    }
    
    /// Help function which registers this API at the plugin to be informed about new parameters
    /**
     * Automatically checks if this is already done.
     */
    void register_as_new_param_observer() {
      // If we are not yet registered for observing new parameters
      if (!m_registered_as_new_param_observer) {
        // Register callback instead of event at parent API
        mApi->REGISTER_NEW_PARAM_CALLBACK(GCnf_private_Api_T, new_param_callback);
        
        // create Transaction an send it to config plugin
        mPrivPlugin.cmd_REGISTER_NEW_PARAM_OBSERVER();

        m_registered_as_new_param_observer = true;
      }
    }
    
  protected:
    /// Full hierarchical name of the owning module
    std::string owner_name;
    
    /// Module which owns this API
    sc_core::sc_module* owner_module;
    
    /// All public parameters
    std::set<std::string> public_params;
    
    /// Pointer to the GCnf_API this API uses to communicate upwards (e.g. to the Core). This may be a private or the default API
    cnf_api* mApi;

    /// private config plugin
    // TODO CS ConfigPrivatePlugin mPrivPlugin;
    ConfigPrivatePlugin_T<gs_param_base_T, gs_param_T<std::string>, GCnf_private_Api_T<gs_param_base_T, gs_param_T, ConfigPlugin_T> >  mPrivPlugin;
                                                      
    /// If this API is registered to observe new parameters
    bool m_registered_as_new_param_observer;
    
    /// Deprecated event pointer for new parameter event. If NULL, this API is not registered to observe new parameters
    sc_event *m_new_param_event;
    
    /// Map to save an observer callback function pointer for each parameter (if needed).
    observerCallbackMap m_observer_callback_map;
    
    sc_event deprecatedUpdateEvent;

  };
  

  //typedef GCnf_private_Api_T<gs_param_base, gs_param, ConfigPlugin, GCnf_Api> GCnf_private_Api;
  
  //typedef GCnf_Api_t<gs_param_base, GCnf_private_Api, gs_param, ConfigPlugin> GCnf_Api;

} // end namespace cnf
} // end namespace gs

#endif
