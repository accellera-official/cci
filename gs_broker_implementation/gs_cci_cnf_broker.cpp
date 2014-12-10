// LICENSETEXT
//
//   Copyright (C) 2009-2010 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//    Mark Burton, mark@greensocs.com
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


#include "gs_cci_cnf_broker.h"
#include "cci_core/cci_gen_unique_name.h"	// HACK; needs a proper home

// TODO: remove (this is for debug)
/*static void show_accessor_map(std::map<std::string, cci::cnf::cci_base_param*> mmap) {
  std::cout << mmap.size() << " accessors: " << std::endl;
  std::map<std::string, cci::cnf::cci_base_param*>::iterator iter;
  for( iter = mmap.begin(); iter != mmap.end(); iter++ ) {
    std::cout << iter->first << std::endl;
  }
}*/

// -----------------------------------------------------------------------------------

cci::cnf::gs_cci_cnf_broker::internal_callback_forwarder::internal_callback_forwarder
(cci::shared_ptr<cci::cnf::callb_adapt> _adapt, const gs::cnf::callback_type _type, my_type& _par)
: adapt(_adapt.get())
, type(_type)
, param(&_par)
, calling_gs_adapter() {
}
cci::cnf::gs_cci_cnf_broker::internal_callback_forwarder::~internal_callback_forwarder() {
  //cout << "Destructing callback forwarder for param "<<param->get_name() << endl;
  if (calling_gs_adapter) {
    //calling_gs_adapter->unregister_at_parameter();
  }
}
// This gets called by the GCnf
gs::cnf::callback_return_type cci::cnf::gs_cci_cnf_broker::internal_callback_forwarder::call(const std::string& parname, gs::cnf::callback_type cbtype) {
  gs::cnf::callback_return_type returned_gs_message = gs::cnf::return_nothing;
  cci::cnf::callback_return_type returned_cci_message = cci::cnf::return_nothing;
  switch(cbtype) {
    case gs::cnf::destroy_param:
      returned_cci_message = adapt->call(parname, cci::cnf::destroy_param);
      break;
    case gs::cnf::pre_read: // = cci::cnf::pre_read
      returned_cci_message = adapt->call(parname, cci::cnf::pre_read);
      break;
    case gs::cnf::reject_write: // = cci::cnf::reject_write
      returned_cci_message = adapt->call(parname, cci::cnf::reject_write);
      break;
    case gs::cnf::pre_write: // = cci::cnf::pre_write
      returned_cci_message = adapt->call(parname, cci::cnf::pre_write);
      break;
    case gs::cnf::post_write: // = cci::cnf::post_write
      returned_cci_message = adapt->call(parname, cci::cnf::post_write);
      break;
    case gs::cnf::create_param: // = cci::cnf::create_param
      returned_cci_message = adapt->call(parname, cci::cnf::create_param);
      break;
    default:
      returned_cci_message = cci::cnf::return_other_error;
      assert(false);
  }
  switch (returned_cci_message) {
    case cci::cnf::return_value_change_rejected:
      returned_gs_message = gs::cnf::return_value_change_rejected;
      break;
    case cci::cnf::return_other_error:
      returned_gs_message = gs::cnf::return_other_error;
      break;
    default:
      returned_gs_message = gs::cnf::return_nothing;
  }
  return returned_gs_message;
}
void cci::cnf::gs_cci_cnf_broker::internal_callback_forwarder::call_create_param(const std::string parname, const std::string value) {
   call(parname, gs::cnf::create_param);
}
const gs::cnf::callback_type cci::cnf::gs_cci_cnf_broker::internal_callback_forwarder::get_type() {
  return type;
}

// -----------------------------------------------------------------------------------

cci::cnf::gs_cci_cnf_broker::gs_cci_cnf_broker(const std::string& name) 
: gs::cnf::GCnf_Api(name.c_str()) 
, m_name(cci::cci_gen_unique_name(name.c_str()))
{ 
  assert (name.length() > 0 && "Name must not be empty");
  CCI_CNF_DUMP("Created new broker \""<<name<<"\" and GCnf_Api \""<< getName() << "\"");
}

cci::cnf::gs_cci_cnf_broker::gs_cci_cnf_broker(bool called_internally_for_creating_global_broker) 
: gs::cnf::GCnf_Api(__CCI_DEFAULT_BROKER_STRING__) 
, m_name(cci::cci_gen_unique_name(__CCI_DEFAULT_BROKER_STRING__))
{ 
  CCI_CNF_DUMP("Created new global standard broker \""<<m_name<<"\" and GCnf_Api \""<< getName() << "\"");
}

cci::cnf::gs_cci_cnf_broker::~gs_cci_cnf_broker() { 
  // remove this from all callbacks being called by this broker (to prevent them to remove themselves from this on their destruction)
  for (unsigned int i = 0; i < fw_vec.size(); ++i)
    fw_vec[i]->adapt->caller_broker = NULL;      

  // Delete all Callbacks in m_observer_callback_map
  for (observerCallbackMap::iterator iter = m_observer_callback_map.begin(); iter!=m_observer_callback_map.end(); ++iter) {
    delete iter->second;
  }
  m_observer_callback_map.clear();
}

const std::string &cci::cnf::gs_cci_cnf_broker::name() const {
  return m_name;
}

void cci::cnf::gs_cci_cnf_broker::json_deserialize_initial_value(const std::string &parname, const std::string &json_value) {
  // TODO: use JSON
  // TODO: if <parname> is implicit parameter, cause a reject_write-callback here!
  // TODO: if <parname> is implicit parameter, cause a pre_write-callback here!
  if ( !gs::cnf::GCnf_Api::setInitValue(parname, json_value) ) {
    cci_report_handler::set_param_failed("Setting initial value failed.");
  } else {
    m_implicit_originator_map.insert( std::pair<std::string, cci::cnf::cci_originator>(parname, *cci_originator::get_global_originator()));
  }
  // TODO: if <parname> is implicit parameter, cause a post_write-callback here!
}

const cci::cnf::cci_originator* cci::cnf::gs_cci_cnf_broker::get_latest_write_originator(const std::string &parname) const {
  cci::cnf::cci_base_param* p = get_param_const(parname);
  if (p) {
    if (p->get_latest_write_originator()) 
      return p->get_latest_write_originator();
  }
  implicitOriginatorMap::const_iterator it;
  it = m_implicit_originator_map.find(parname);
  if (it != m_implicit_originator_map.end()) {
    return &(it->second);
  }
  return NULL;
}

void cci::cnf::gs_cci_cnf_broker::lock_initial_value(const std::string &parname) {
  if ( !gs::cnf::GCnf_Api::lockInitValue(parname) ) {
    cci_report_handler::set_param_failed("Locking initial value failed.");
  }
}

const std::string cci::cnf::gs_cci_cnf_broker::json_serialize(const std::string &parname) {
  // TODO: use JSON
  // TODO: if <parname> is implicit parameter, cause a pre_read-callback here!
  return gs::cnf::GCnf_Api::getValue(parname);
}

const std::string cci::cnf::gs_cci_cnf_broker::json_serialize_keep_unused(const std::string &parname) {
  // TODO: use JSON
  return gs::cnf::GCnf_Api::getValue(parname, "", true);
}


cci::cnf::cci_base_param* cci::cnf::gs_cci_cnf_broker::get_param(const std::string &parname) {
  return get_param_const(parname);
}
cci::cnf::cci_base_param* cci::cnf::gs_cci_cnf_broker::get_param_const(const std::string &parname) const {
  //std::cout << name() << " (gs_cci_cnf_broker) get param " << parname << std::endl;
  std::map<std::string,cci_base_param*>::const_iterator iter = m_mirrored_registry.find(parname);
  if( iter != m_mirrored_registry.end() ) {
    cci::cnf::cci_base_param* ret = dynamic_cast<cci::cnf::cci_base_param*>(iter->second);
    assert(ret != NULL && "This param shall be a cci::cnf::cci_base_param!");
    return ret;
  }
  else return NULL;
  /*gs::gs_param_base* p = gs::cnf::GCnf_Api::getPar(parname);
   cci_base_param* cp = dynamic_cast<cci_base_param*> (p);
   assert(((p == NULL && cp == NULL) || (p != NULL && cp != NULL)) && "There shall only be cci params in the gs database!");
   if (cp != NULL) return cp;
   else return NULL;*/
}

bool cci::cnf::gs_cci_cnf_broker::param_exists(const std::string &parname) {
  std::map<std::string,cci_base_param*>::iterator iter = m_mirrored_registry.find(parname);
  if( iter != m_mirrored_registry.end() ) {
    cci::cnf::cci_base_param* ret = dynamic_cast<cci::cnf::cci_base_param*>(iter->second);
    assert(ret != NULL && "This param shall be a cci::cnf::cci_base_param!");
    return true;
  }
  return gs::cnf::GCnf_Api::existsParam(parname);
}

bool cci::cnf::gs_cci_cnf_broker::is_used(const std::string &parname) {
  return gs::cnf::GCnf_Api::is_used(parname);
}

const std::vector<std::string> cci::cnf::gs_cci_cnf_broker::get_param_list() {
  return gs::cnf::GCnf_Api::getParamList();
}

cci::shared_ptr<cci::cnf::callb_adapt> cci::cnf::gs_cci_cnf_broker::register_callback(const std::string& parname, const callback_type type, cci::shared_ptr<cci::cnf::callb_adapt> callb) {

  internal_callback_forwarder *fw = NULL;
  cci_base_param *p = NULL;
  switch (type) {
    case cci::cnf::pre_read:
      // forward to explicit parameter
      p = get_param(parname); // TODO: get_param_list(pattern) and iterate registering callbacks to all of them
      if (p) {
        p->register_callback(type, callb);
      } else {
        SC_REPORT_WARNING("GreenSocs/cci/broker", "pre_read callback not possible for implicit parameter");  
      }
      break;
    case cci::cnf::reject_write:
      // forward to explicit parameter
      p = get_param(parname); // TODO: get_param_list(pattern) and iterate registering callbacks to all of them
      if (p) {
        p->register_callback(type, callb);
      } else {
        SC_REPORT_WARNING("GreenSocs/cci/broker", "reject_write callback not possible for implicit parameter");  
      }
      break;
    case cci::cnf::pre_write:
      // forward to explicit parameter
      p = get_param(parname); // TODO: get_param_list(pattern) and iterate registering callbacks to all of them
      if (p) {
        p->register_callback(type, callb);
      } else {
        SC_REPORT_WARNING("GreenSocs/cci/broker", "pre_write callback not possible for implicit parameter");  
      }
      break;
    case cci::cnf::post_write:
      // forward to explicit parameter or handle internally
      p = get_param(parname); // TODO: get_param_list(pattern) and iterate registering callbacks to all of them
      if (p) {
        p->register_callback(type, callb);
      } else {
        SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "TODO: register locally for implicit parameters or forward to explicit parameter");
      }
      break;
    case cci::cnf::destroy_param:
      // forward to explicit parameter
      p = get_param(parname); // TODO: get_param_list(pattern) and iterate registering callbacks to all of them
      if (p) {
        p->register_callback(type, callb);
      } else {
        SC_REPORT_WARNING("GreenSocs/cci/broker", "destroy_param callback not possible for implicit parameter");  
      }
      break;
    case cci::cnf::create_param:
      if (parname.compare("*") != 0)
        SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "This implementation cannot handle patterns / listen for special new parameters - will register for all new params");      
      fw = new internal_callback_forwarder(callb, gs::cnf::no_callback, *this);
      fw_vec.push_back(fw);
      //assert(fw->caller_broker && "create_param callbacks only can be used with string callback functions!");
      assert(get_gs_cnf_api() != NULL && "This must be available!");
      // ** callback forwarded to GCnf_Api
      // We cannot use the GreenConfig callback because it comes before the param is available / registered with the broker
      //cb = gs::cnf::create_param;
      //fw->calling_gs_adapter = cci::shared_ptr< ::gs::cnf::CallbAdapt_b>( 
      //                              new ::gs::cnf::CallbAdapt<internal_callback_forwarder>( fw, &internal_callback_forwarder::call_create_param ));
      //get_gs_cnf_api()->registerNewParamCallback( fw->calling_gs_adapter.get() );
      
      // ** local handling of new param callbacks
      // Add object and function pointer to multimap
      m_observer_callback_map.insert( std::pair<std::string, internal_callback_forwarder*>( "**dummy.NewParamObservers**", fw ) );

      break;
    default:
      break;
  }
  return callb;
}

void cci::cnf::gs_cci_cnf_broker::make_create_callbacks(const std::string &search, const std::string &par_name, const std::string &value) {
  internal_callback_forwarder *callb;
  observerCallbackMap::iterator it;
  std::pair<observerCallbackMap::iterator, observerCallbackMap::iterator> begin_end;        
  begin_end = m_observer_callback_map.equal_range(search);
  for (it = begin_end.first;  it != begin_end.second;   ++it) {
    //CCI_DUMP_N(name(), "transport: callback for parameter "<<search.c_str());      
    callb = (*it).second;
    // Make call
    callb->call_create_param(par_name, value);
  }
}

void cci::cnf::gs_cci_cnf_broker::unregister_all_callbacks(void* observer) {
  bool succ = true;
  internal_callback_forwarder* fw;
  while (succ) {
    succ = false;
    for (unsigned int i = 0; i < fw_vec.size(); ++i) {
      if (fw_vec[i]->adapt->get_observer() == observer) {
        fw = fw_vec[i];
        fw_vec.erase(fw_vec.begin()+i);
        delete fw;
        succ = true;
        break;
      }
    }
  }
}

bool cci::cnf::gs_cci_cnf_broker::unregister_callback(callb_adapt* callb) {
  internal_callback_forwarder* fw;
  for (unsigned int i = 0; i < fw_vec.size(); ++i) {
    if (fw_vec[i]->adapt == callb) {
      fw = fw_vec[i];
      fw_vec.erase(fw_vec.begin()+i);
      delete fw;
      return true;
    }
  }
  return false;
}

bool cci::cnf::gs_cci_cnf_broker::has_callbacks(const std::string& parname) {
  return (fw_vec.size() > 0);
}

void cci::cnf::gs_cci_cnf_broker::add_param(cci_base_param* par) {
  bool new_element = m_mirrored_registry.insert(std::pair<std::string, cci_base_param*>(par->get_name(), par)).second;
  //std::cout << name() << " (gs_cci_cnf_broker) add param " << par->get_name() << std::endl;
  assert(new_element && "The same parameter had been added twice!!");
  /*gs::gs_param_base* p = dynamic_cast<gs::gs_param_base*> (par);
   assert(p != NULL && "This demo example implementation only works with the gs parameter implementation (TODO: internally to be changed!)");
   gs::cnf::GCnf_Api::addPar(p);*/
  // Make create_param callbacks if there are any
  make_create_callbacks("**dummy.NewParamObservers**", par->get_name(), par->json_serialize());
}

void cci::cnf::gs_cci_cnf_broker::remove_param(cci_base_param* par) {
  m_mirrored_registry.erase(par->get_name());
  /*gs::gs_param_base* p = dynamic_cast<gs::gs_param_base*> (par);
   assert(p != NULL && "This demo example implementation only works with the gs parameter implementation (TODO: internally to be changed!)");
   gs::cnf::GCnf_Api::removePar(p);*/
}

const std::vector<std::string> cci::cnf::gs_cci_cnf_broker::get_param_list(const std::string& pattern) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "pattern limited to GreenConfig capabilities");
  return gs::cnf::GCnf_Api::getParamList(pattern);
}

const std::vector<cci::cnf::cci_base_param*> cci::cnf::gs_cci_cnf_broker::get_params(const std::string& pattern) {
  SC_REPORT_WARNING("GreenSocs/cci/not_implemented", "pattern limited to GreenConfig capabilities");
  std::vector<cci::cnf::cci_base_param*> parvec;
  std::vector<std::string> strvec;
  strvec = get_param_list(pattern);
  std::vector<std::string>::iterator striter;
  for (striter = strvec.begin(); striter != strvec.end(); striter++) {
    cci::cnf::cci_base_param* p = get_param(*striter);
    if (p) parvec.push_back(p);
  }
  return parvec;
}

bool cci::cnf::gs_cci_cnf_broker::is_private_broker() const {
  return false;
}

gs::cnf::cnf_api_if* cci::cnf::gs_cci_cnf_broker::get_gs_cnf_api() {
  return this;
}
