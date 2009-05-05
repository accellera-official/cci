//   OSCI CCI WG
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// 
// ENDLICENSETEXT


#ifndef __CCI_PARAM_BASE_HPP__
#define __CCI_PARAM_BASE_HPP__

namespace cci {

  
  static sc_core::sc_module* get_sc_module(sc_core::sc_object *object) {
    sc_core::sc_object *obj = object;
    sc_core::sc_module *mod = NULL;
    for(int i = 0; i<10; i++) {
      if (!obj) break;
      mod = dynamic_cast<sc_core::sc_module*>(obj);
      if (mod) break;
#if SYSTEMC_API == 210
      obj = obj->get_parent();
#else
      obj = obj->get_parent_object();
#endif
    }
    return mod;
  }
  
  

  // ////////   constructors, destructor   /////////// //

  cci_param_base::cci_param_base(const std::string &n,
                                 const bool register_at_db,
                                 const bool force_top_level_name)
  : m_destruction_flag(false)
  , m_register_at_db(register_at_db)
  {
    CCI_PARAM_DUMP("cci_param_base(name='"<<n<<"', register_at_db="<<(unsigned int)register_at_db<<", force_top_level_name="<<(unsigned int)force_top_level_name<<") constructor");

    // Name counter to make unnamed params unique
    static int cnt = 0;

    // Set the api pointer
    sc_core::sc_module *mod = get_sc_module(sc_core::sc_get_current_process_handle().get_parent_object()); // TODO
    m_api = GCnf_Api::getApiInstance(mod);

    if (isHierarchicalParameterName(n) && parent_array != NULL) {
      SC_REPORT_FATAL(name(), "Do not use hierarchical parameter names for Extended Parameter Array members!");
    }

    // Set the name
    std::stringstream ss;
    if (!force_top_level_name) ss << get_name_prefix(); 
    if (n.length() == 0)       ss << "no_name_" << cnt++;
    else                       ss << n;
    CCI_PARAM_DUMP("Set parameter name = '"<< ss.str().c_str()<<"',");
    m_par_name = ss.str();

  }

  ~cci_param_base::cci_param_base() {
    // delete pointer to this parameter in all callback adapters called by this parameter
    observerParamCallback_type::iterator it;
    for (it = m_observer_param_callback_mmap.begin();  it != m_observer_param_callback_mmap.end();   ++it) {
      (*it).second->caller_param=NULL;
    }
  }
    
  void cci_param_base::destruct_cci_param() {
    // set destruction flag
    m_destruction_flag = true;

    CCI_PARAM_DUMP("Destructing param '"<< m_par_name.c_str()<<"'.");

    makeSecureCallbacks();

    // remove this from database
    if (m_register_at_db)
      m_api->removePar(this);
  }
  
  // ////////   set, get   /////////// //
  
  template<class T>
  const bool cci_param_base::getValue(T &value) const {
    return cci_param<T>::static_deserialize(value, this->get());
  }
  
  template<class T>
  const T cci_param_base::getValue() const {
    T val;
    cci_param<T>::static_deserialize(val, this->getString());
    return val;
  }
  
  
  template<class T>
  cci_param<T>* cci_param_base::get_cci_param() {
    return dynamic_cast<cci_param<T>*>(this);
  }
  
  // ////////   Type   /////////// //

  const std::string cci_param_base::getTypeString() const {
    return string("PARTYPE_NOT_AVAILABLE");
  }
  
  const Param_type cci_param_base::getType() const{
    return PARTYPE_NOT_AVAILABLE;
  }

  
  // //////  param_attributes  ////// //

  /*bool cci_param_base::add_param_attribute(const param_attribute attr) {
    CCI_PARAM_DUMP("Add param attribute '"<<(unsigned int) attr<<"'");
    return (m_attributes.insert(attr)).second;
  }
  
  bool cci_param_base::has_param_attribute(const param_attribute attr) const {
    return (m_attributes.find(attr) != m_attributes.end());
  }
  
  const std::set<param_attribute> cci_param_base::get_param_attributes() const {
    return m_attributes;
  }
  
  bool cci_param_base::remove_param_attribute(param_attribute attr) {
    CCI_PARAM_DUMP("Remove param attribute '"<<(unsigned int) attr<<"'");
    return (m_attributes.erase(attr) > 0);
  }
  
  void cci_param_base::remove_all_param_attributes() {
    CCI_PARAM_DUMP("Remove all param attributes");
    m_attributes.clear();
  }*/
  
  
  // ////////   diverse   /////////// //

  
  std::string &cci_param_base::getName() const { 
    return m_par_name; 
  }

  const void* cci_param_base::get_value_pointer() const = 0;

  const bool cci_param_base::is_destructing() const {
    return m_destruction_flag;
  }
  
  
  // ///////////   Observer and callbacks   //////////////


  // //////////////////////////////////////////////////////////////////// //
  // ///////   Callback handling methods   ////////////////////////////// //
  

  virtual boost::shared_ptr<ParamCallbAdapt_b> registerParamCallback(boost::shared_ptr<ParamCallbAdapt_b> callb) {
    GS_PARAM_CALLBACK_DUMP("registerParamCallback(shared_ptr<ParamCallbAdapt_b>)");      
    GS_PARAM_CALLBACK_DUMP("   param '"<<callb->get_caller_param()->getName()<<"' registerParamCallback(ParamCallbAdapt callb="<<(void*)callb.get()<<")");
    // Add function pointer to callbacks
    m_observer_param_callback_mmap.insert( pair<void*, boost::shared_ptr<ParamCallbAdapt_b> >(callb->get_observer(), callb) );
    //show_callbacks();
    return callb;
  }


  bool cci_param_base::unregisterParamCallback(ParamCallbAdapt_b *callb) {
    //show_callbacks();
    GS_PARAM_CALLBACK_DUMP("param '"<<m_par_name.c_str()<<"' unregisterParamCallback(ParamCallbAdapt callb="<<(void*)callb<<")");
    observerParamCallback_type::iterator it;
    ParamCallbAdapt_b *cb;
    for ( it=m_observer_param_callback_mmap.begin() ; it != m_observer_param_callback_mmap.end(); it++ ) {
      if (((*it).second).get() == callb) {
        cb = ((*it).second).get();  // get out of shared_ptr
        m_observer_param_callback_mmap.erase(it);
        cb->caller_param = NULL;
        GS_PARAM_CALLBACK_DUMP("             removed parameter callback adapter "<< (void*)cb);
        return true;
      }
    }
    return false;
  }
  
  bool cci_param_base::unregisterParamCallbacks(void* observer) {
    //show_callbacks();
    GS_PARAM_CALLBACK_DUMP("param '"<<m_par_name<<"' unregisterParamCallbacks(void* observer="<<observer<<")");
    bool success = false;
    ParamCallbAdapt_b *cb;
    observerParamCallback_type::iterator it;
    // Remove all function pointer from callbacks variable that have the key observer and delete the adapter object(s)
    it = m_observer_param_callback_mmap.find(observer);
    while (it != m_observer_param_callback_mmap.end()) {
      success = true;
      cb = ((*it).second).get();
      // remove and delete
      m_observer_param_callback_mmap.erase(it);
      cb->unregister_at_parameter();
      GS_PARAM_CALLBACK_DUMP("             removed parameter callback adapter "<< (void*)cb);
      // search for next one
      it = m_observer_param_callback_mmap.find(observer);
    }
    return success;
  }

  
  /// Returns if this param has registered callbacks
  bool cci_param_base::has_callbacks() { return !m_observer_param_callback_mmap.empty(); }
  
 
protected:

  /*void cci_param_base::show_callbacks() {
    cout << endl << "**** Callback map for parameter "<<m_par_name << " - size = "<<m_observer_param_callback_mmap.size() << endl;
    observerParamCallback_type::iterator it;
    printf("   observer   -  callb adapt\n");
    for ( it=m_observer_param_callback_mmap.begin() ; it != m_observer_param_callback_mmap.end(); it++ ) {
      printf("   0x%x - 0x%x\n", (unsigned) (*it).first, (unsigned) ((*it).second).get());
    }        
    cout << endl;
  }*/
  
  const std::string cci_param_base::get_name_prefix() {
    std::string ret;
    // TODO sc_core::sc_module *mod = gs::cnf::get_parent_sc_module(this);
    sc_core::sc_module *mod = get_sc_module(sc_core::sc_get_current_process_handle().get_parent_object());
    if (mod != NULL) {
      ret = mod->name();
      ret += SC_NAME_DELIMITER;
      CCI_PARAM_DUMP("Setting parent name: '"<< ret.c_str()<< "'");
    } else {
      ret = ""; // old: "N/A"; 
      CCI_PARAM_DUMP("Setting parent name: Parameter is at top-level (has no parent module)!");
    }
    return ret;
  }

protected:

  void cci_param_base::makeCallbacks() {
    GS_PARAM_CALLBACK_DUMP("Make callbacks and notify event"); 
    observerParamCallback_type::iterator it;
    boost::shared_ptr<ParamCallbAdapt_b> b;
    for (it = m_observer_param_callback_mmap.begin();  it != m_observer_param_callback_mmap.end();   ) {
      b = (*it).second;  // 1/ store pointer
      it++;              // 2/ increment iterator to allow the b to be deleted during the call
      GS_PARAM_CALLBACK_DUMP("   call adapter "<< (void*)b.get());
      b->call(*this);    // 3/ call on b and allow here to erase it from the map (unregister callback)
    }
  }

  void cci_param_base::makeSecureCallbacks() {
    GS_PARAM_CALLBACK_DUMP("Make secure callbacks and notify event"); 
    observerParamCallback_type tmp_map = m_observer_param_callback_mmap;
    
    observerParamCallback_type::iterator it;
    //boost::shared_ptr<ParamCallbAdapt_b> val;
    //void* key;
    for (it = tmp_map.begin();  it != tmp_map.end();   ) {
      // 1/ check if the new call is still in the current original map 
      //      (so it was not deleted during the last callback)
      bool is_still_in = false;
      observerParamCallback_type::iterator tmp_it;
      for (tmp_it = m_observer_param_callback_mmap.begin(); tmp_it !=  m_observer_param_callback_mmap.end(); tmp_it++) {
        if (tmp_it->second == it->second)
          is_still_in = true;
      }
      //    if still in the original map, call:
      if (is_still_in) {
        GS_PARAM_CALLBACK_DUMP("   call adapter "<< (void*)(*it).second.get());
        (*it).second->call(*this);  // 2/ call, this may change the m_observer_param_callback_mmap
      }
      it++;              // 3/ increment iterator on copied map (which was NOT changed!)
      //key = it->first;
      //val = it->second;
    }
  }
  
};


} // namespace cci

#endif
