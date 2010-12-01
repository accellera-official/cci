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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// doxygen comments


#ifndef __GS_PARAM_ARRAY_H__
#define __GS_PARAM_ARRAY_H__


// Do NOT include gs_param_base.h in this file!!

#include "greencontrol/gcnf/plugin/config_globals.h"


// define in config_globals.h #define GS_PARAM_ARRAY_VERBOSE
#ifdef GS_PARAM_ARRAY_VERBOSE
//# define GS_PARAM_ARRAY_TRACE(msg, ...) { printf("%s: ", this->name()); printf(msg, ##__VA_ARGS__); printf("\n"); } 
# define GS_PARAM_ARRAY_DUMP(msg) { std::cout<<this->name()<<": " << msg << std::endl; } 
#else
//# define GS_PARAM_ARRAY_TRACE(msg, ...)
# define GS_PARAM_ARRAY_DUMP(msg)
#endif


// includes the callback adapter which is needed because the callback register macro is not yet included at this point.
#include "greencontrol/gcnf/plugin/config_paramcallbadapt.h"


namespace gs {
namespace cnf {
  
 
// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// Extended Parameter Array ////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //

  
// For friend and typedef below.
class gs_param_base;  


/// Extended Parameter Array
/**
 * Extended parameter arrays. Support named members with
 * different types. Also allows nested arrays.
 *
 * This class is templated to resolve cyclic includes!! 
 * Never use with other specialization than gs_param_base.
 * Use the typedef gs_param_array!!
 */
template<typename gs_param_base_T>
class gs_param_array_T
: public gs_param_base_T
{
protected:
  
  friend class gs_param_base;
  
  typedef gs_param_array_T<gs_param_base_T> my_type;
  
  typedef typename std::map<std::string, gs_param_base_T*> memberMapType;
  /// Internal map which contains the names and pointers to the member parameters
  memberMapType m_MemberMap;
  
  typedef typename std::map<void*, CallbackBehavior::CallbackBehavior> callbackBehaviorMapType;
  /// Observer callback behavior map: stores the behavior of callback methods for each observer identified by it's this pointer. Default is: 'call on member changes'!
  callbackBehaviorMapType m_CallbackBehaviorMap;
  
protected:
  
  using gs_param_base_T::m_api;
  using gs_param_base_T::m_par_name;
  using gs_param_base_T::name;
  using gs_param_base_T::m_callback_lists;
  using gs_param_base_T::m_callback_list_pre_read;
  //using gs_param_base_T::m_callback_list_post_read;
  using gs_param_base_T::m_callback_list_reject_write;
  using gs_param_base_T::m_callback_list_pre_write;
  using gs_param_base_T::m_callback_list_post_write;
  using gs_param_base_T::m_callback_list_destroy_param;
  
public:
  // /////////////////////////////////////////////////////// //
  // ///////////////////  Iterator ///////////////////////// //
  // /////////////////////////////////////////////////////// //
  
  /// Parameter iterator for Extended Parameter Arrays
  class iterator {
  protected:
    typedef iterator it_my_type;
    friend class gs_param_array_T<gs_param_base_T>;
    
  public:

    iterator() { }

    ~iterator() { }

    it_my_type& operator = (const it_my_type& it) {
      m_iter = it.m_iter;
      return *this;
    }

    bool operator == (const it_my_type& it) {
      return m_iter == it.m_iter;
    }
    
    bool operator != (const it_my_type& it) {
      return m_iter != it.m_iter;
    }

    it_my_type& operator ++ () { // prefix ++x
      m_iter++;
      return *this;
    }

    it_my_type& operator -- () { // prefix --x
      m_iter--;
      return *this;
    }
    
    gs_param_base* operator* () {
     return (*m_iter).second;
    }

  protected:

    typename memberMapType::iterator m_iter;

  };
  
 
public:
  // Explicit constructors to avoid implicit construction of parameters.
  
  /// Empty constructor (and optional parent array pointer). Name will be set in base
  explicit gs_param_array_T(gs_param_array_T* parent_array = NULL) 
  : gs_param_base_T("", true, parent_array, false)
  {
    this->m_is_extended_array = true;
    m_api->addPar(this);
  }
  /// Constructor with parent array reference. Name will be set in base
  explicit gs_param_array_T(gs_param_array_T &parent_array) 
  : gs_param_base_T("", true, &parent_array, false)
  {
    this->m_is_extended_array = true;
    m_api->addPar(this);
  }
  
  /// Constructor with (local) name (and optional parent array pointer).
  explicit gs_param_array_T(const std::string &nam,
                              gs_param_array_T* parent_array = NULL)
  : gs_param_base_T(nam, true, parent_array, false)
  {
    this->m_is_extended_array = true;
    m_api->addPar(this);
  }
  /// Constructor with (local) name and parent array reference.
  explicit gs_param_array_T(const std::string &nam,
                            gs_param_array_T &parent_array)
  : gs_param_base_T(nam, true, &parent_array, false)
  {
    this->m_is_extended_array = true;
    m_api->addPar(this);
  }
  
  /// Constructor with (local) name (and optional parent array pointer).
  explicit gs_param_array_T(const char *nam,
                            gs_param_array_T* parent_array = NULL)
  : gs_param_base_T(string(nam), true, parent_array, false)
  {
    this->m_is_extended_array = true;
    m_api->addPar(this);
  } 
  /// Constructor with (local) name and parent array reference.
  explicit gs_param_array_T(const char *nam,
                            gs_param_array_T &parent_array)
  : gs_param_base_T(string(nam), true, &parent_array, false)
  {
    this->m_is_extended_array = true;
    m_api->addPar(this);
  } 
  
  
  /// Destructor
  virtual ~gs_param_array_T() {
    delete_array();
    gs_param_base_T::destruct_gs_param();
  }
  
  /// Overloads gs_param_base::getTypeString
  const std::string getTypeString() const {
    return string("ExtendedArray");
  }
  
  /// Overloads gs_param_base::getType
  const Param_type getType() const {
    return PARTYPE_EXT_ARRAY;
  }
  
  /// @see gs::cnf::gs_param_base::get_value_pointer
  const void* get_value_pointer() const {
    this->make_pre_read_callbacks();
    //this->make_post_read_callbacks(); // TODO not possible here
    return &m_MemberMap;
  }
  
  // //////////////////////////////////////////////////////////////////// //
  // ///////   set and get with value   ///////////////////////////////// //
  
  
  /// Set the member values of this parameter to the member values of another gs_param array.
  /**
   * @param v  Parameter where the values should be read from.
   * @return   Pointer to this.
   */
  // TODO: copy operator
  /*my_type& operator = (const my_type& v) { 
    this->delete_array();
    internal_ArrVec.resize(v.size());
    for (unsigned int i = 0; i < internal_ArrVec.size(); i++) {
      std::stringstream hierParName_ss;
      hierParName_ss << m_par_name << SC_NAME_DELIMITER "." << i;
      internal_ArrVec[i] = new gs_param<T>(hierParName_ss.str(), v[i].getValue(), true);
    }
    return *this;
  }*/
  
  
  // //////////////////////////////////////////////////////////////////// //
  // ///////   set and get with string representation   ///////////////// //

  /// Oberloads base method. Not implemented for Extended Parameter Arrays!
  /**
   * @param str  String to be set.
   * @return success = false, not implemented.
   */
  bool setString(const std::string &str) {
    SC_REPORT_WARNING(this->name(), "setString not implemented for Extended Parameter Arrays!");
    GCNF_DUMP_N(name(), "setString not implemented for Extended Parameter Arrays");
    return false;
  }
  
  /// Get the value of this parameter as a string.
  /**
   * @return String representation of the array.
   */
  const std::string& getString() const {
    this->make_pre_read_callbacks();
    std::ostringstream ss;
    ss << "{";
    // iterate over members and print names and values!
    typename memberMapType::const_iterator it;
    std::string local_n, n, tmp;
    int pos;
    for ( it=m_MemberMap.begin() ; it != m_MemberMap.end(); it++ ) {
      if (it != m_MemberMap.begin()) ss << ",";
      ss << std::endl;
      n = (*it).second->getName();
      local_n = n.substr(   n.find_last_of(SC_NAME_DELIMITER)+1, n.size()-n.find_last_of(SC_NAME_DELIMITER)+1   );
      ss << "   " << local_n << " = '";
      tmp = (*it).second->getString();
      // insert additional spaces
      pos = 0;
      while (tmp.find('\n', pos) != string::npos) {
        pos = tmp.find('\n', pos)+1; 
        tmp.insert(pos, "   ");
      }
      // add to output string
      ss << tmp;
      ss << "'";
    }
    ss << std::endl << "}";
    return_string = ss.str();
    //this->make_post_read_callbacks(); // TODO deactivated
    return return_string;
  }
  
  // //////////////////////////////////////////////////////////////////// //
  // ///////   array methods   ////////////////////////////////////////// //

  /// Returns the number of parameter members in this array.
  unsigned int size() const {
    return m_MemberMap.size();
  }
  
  /// Returns the member with the name member_name
  /**
   * Not secure!
   */
  gs_param_base_T& operator[] (const std::string member_name) { 
    this->make_pre_read_callbacks();
    //this->make_post_read_callbacks(); // TODO not possible here
    return *getMember(member_name);
  }
  
  /// Returns the member with the specified name.
  /**
   * @return Returns the member of this array with the specified name. 
   *         NULL if no member with the name.
   */
  gs_param_base_T* getMember(const std::string name) {
    this->make_pre_read_callbacks();
    std::string hier_name = m_par_name + SC_NAME_DELIMITER + name;
    typename memberMapType::iterator iter;
    iter = m_MemberMap.find(hier_name);
    if( iter == m_MemberMap.end() ) {
      GS_PARAM_ARRAY_DUMP("getMember("<<name<<"): Array has no member with the name '"<<hier_name<<"'!");
      return NULL;
    } 
    //this->make_post_read_callbacks(); // TODO not possible here
    return iter->second;
  }
  
  /// Returns the member array with the specified name if it is an Extended Parameter Array.
  /**
   * @return Returns the member ARRAY of this array with the specified name. 
   *         NULL if no member with the name or the member is not an array.
   */
  my_type* getMemberArray(const std::string name) {
    return dynamic_cast<my_type*>(this->getMember(name));
  }
  
/// Macro to register the callback behavior of this array for the calling module
#define SET_CALLBACK_BEHAVIOR(behav) set_callback_behavior((void*)this, behav);
  
  /// Stores the callback behavior for the observer with the this-pointer address observer
  /**
   * Stores the this pointer with the behavior in the behavior map.
   *
   * @param observer  Identification (this-)pointer of the observer.
   * @param behav     Callback behavior.
   */
  void set_callback_behavior(void* observer, CallbackBehavior::CallbackBehavior behav) {
    {
#ifdef GCNF_ENABLE_GS_PARAM_LOCK
      // Check the lock!
      if (gs_param_base_T::m_locked) {                                                
        GS_PARAM_ARRAY_DUMP("parameter is locked!");                                      
        SC_REPORT_INFO(GCNF_SC_REPORTER(this->getName()), "parameter is locked!");
        return;                                                               
      }                                                                             
#endif
      // check if already registered as an observer in any cb list and warn if not
      bool found = false;
      typename gs_param_base_T::callback_list_type::iterator observers_it;
      typename gs_param_base_T::all_callback_lists_type::iterator map_iter;
      for ( map_iter=m_callback_lists.begin(); map_iter != m_callback_lists.end(); map_iter++ ) {
        for (observers_it = map_iter->second->begin();  
             observers_it != map_iter->second->end();   
             ++observers_it) {
          if ((*observers_it).second->get_observer() == observer) {
            found = true;
            break;
          }
        }
      } 
      if (!found) {
        SC_REPORT_WARNING(name(), "SET_CALLBACK_BEHAVIOR: Don't forget to register the observer for the array!");
      }
    }
    // if already in behavior map
    if (m_CallbackBehaviorMap.find(observer) != m_CallbackBehaviorMap.end()) {
      if (m_CallbackBehaviorMap.find(observer)->second != behav) {
        m_CallbackBehaviorMap.find(observer)->second = behav;
        //GS_PARAM_ARRAY_DUMP("set_callback_behavior("<<(unsigned int)behav<<", "<<(unsigned long int) observer<<"): changed behavior!");
        GS_PARAM_ARRAY_DUMP("set_callback_behavior("<<(unsigned int)behav<<", <omitted ptr addr for diff>): changed behavior!");
      } else {
        //GS_PARAM_ARRAY_DUMP("set_callback_behavior("<<(unsigned int)behav<<", "<<(unsigned long int) observer<<"): no change in behavior!");
        GS_PARAM_ARRAY_DUMP("set_callback_behavior("<<(unsigned int)behav<<", <omitted ptr addr for diff>): no change in behavior!");
      }
    }
    // if new observer entry
    else {
      m_CallbackBehaviorMap.insert(  std::make_pair<void*, CallbackBehavior::CallbackBehavior>(observer, behav) );
      //GS_PARAM_ARRAY_DUMP("set_callback_behavior("<<(unsigned int)behav<<", "<<(unsigned long int) observer<<"): inserted new behavior entry!");
      GS_PARAM_ARRAY_DUMP("set_callback_behavior("<<(unsigned int)behav<<", <omitted ptr addr for diff>): inserted new behavior entry!");
    }
#ifdef GS_PARAM_ARRAY_VERBOSE
    dbg_show_CallbackBehaviorMap();
    dbg_show_ObserverMap();
#endif
  }
  
  /// Returns an iterator which points to the first member of the array.
  iterator begin() { 
    iterator it;
    it.m_iter = m_MemberMap.begin();
    return it; 
  }
  /// Returns an iterator which points behind(!) the last member of the array.
  iterator end() { 
    iterator it;
    it.m_iter = m_MemberMap.end();
    return it; 
  }
  /// Returns an iterator which points to the member with the (local) name
  iterator find(const std::string name) {
    std::string hier_name = m_par_name + SC_NAME_DELIMITER + name;
    iterator it;
    typename memberMapType::iterator iter;
    it.m_iter = m_MemberMap.find(hier_name);
    return it;
  }
  
#ifdef GS_PARAM_ARRAY_VERBOSE
  /// Debug function
  void dbg_show_CallbackBehaviorMap() {
    std::cout << "  " << name() << " '"<<m_par_name<<"' CallbackBehaviorMap: "<< std::endl;
    callbackBehaviorMapType::iterator it;
    for (it = m_CallbackBehaviorMap.begin(); it != m_CallbackBehaviorMap.end(); ++it) {
      //std::cout << "    observer "<< (unsigned long int)(*it).first << ", ";
      std::cout << "    observer <omitted ptr addr for diff>, ";
      if ( it->second == CallbackBehavior::CALLBACK_MEMBER_CHANGES)
        std::cout << "CALLBACK_MEMBER_CHANGES" << std::endl;       
      else
        std::cout << "NOT_CALLBACK_MEMBER_CHANGES" << std::endl;
    }
  }
  /// Debug function
  void dbg_show_ObserverMap() {
    std::cout << "  " << name() << " '"<<m_par_name<<"' ObserverVector (observer_ptr): "<< std::endl;
    typename gs_param_base_T::callback_list_type::iterator it;
    typename gs_param_base_T::all_callback_lists_type::iterator map_iter;
    for ( map_iter=m_callback_lists.begin(); map_iter != m_callback_lists.end(); map_iter++ ) {
      for (it =  map_iter->second->begin(); it != map_iter->second->end(); ++it) {
        //std::cout << "    "<< (unsigned long int)(*it).second->get_observer() << ", ";
        std::cout << "    <omitted ptr addr for diff>, ";
      }
    }
    std::cout << std::endl;
  }
  /// Debug function: return string with all members
  void dbg_show_members() {
    typename memberMapType::iterator it;
    for (it = m_MemberMap.begin();  
         it != m_MemberMap.end();   
         ++it) {
      if (it != m_MemberMap.begin()) std::cout << ", ";
      std::cout << (*it).second->getName();
      if ( (*it).first != (*it).second->getName())
        SC_REPORT_WARNING(name(), "Failure in member map: kex name does not match the member name!");
    }   
  }
#endif
  
protected:
  
  /// Member callback function to give through the callbacks to the array observers (if they enabled member callbacks).
  /**
   * - Callback my observers for member changes
   *   (if enabled in callback behavior map) and
   * - remove members when they are deleted.
   */
  callback_return_type array_config_callback(gs_param_base_T& changed_param, callback_type cb_reason) {
    GS_PARAM_ARRAY_DUMP("param array '"<<m_par_name<<"' got internal callback (of type "<<callback_type_to_string(cb_reason)<<") for param '"<<changed_param.getName()<<"'");
    
    // Choose correct list depending on cb reason
    typename gs_param_base_T::callback_list_type* cb_list = NULL;
    switch (cb_reason) {
      case pre_read:
        cb_list = &m_callback_list_pre_read;
        break;
      //case post_read:
      //  cb_list = &m_callback_list_post_read;
      //  break;
      case reject_write:
        cb_list = &m_callback_list_reject_write;
        break;
      case pre_write:
        cb_list = &m_callback_list_pre_write;
        break;
      case post_write:
        cb_list = &m_callback_list_post_write;
        break;
      case destroy_param:
        cb_list = &m_callback_list_destroy_param;
        break;
      default:
        GCNF_DUMP_N(name(), "array_config_callback: Cannot handle given callback type reason: "<< callback_type_to_string(cb_reason)<<"!");
        SC_REPORT_ERROR(name(), "array_config_callback: Cannot handle given callback type reason!");
        return return_other_error;
    }
    
    callback_return_type ret = return_nothing;
    callback_return_type tmp_ret;
    
    // go through all observers of this array's param base
    //   and call back IF enabled for that observer in the array's behavior map
    gs_param_base_T::m_currently_making_callbacks = true;
    ParamCallbAdapt_b *callb;
    typename gs_param_base_T::callback_list_type::iterator observers_it;
    for (observers_it = cb_list->begin();  
         observers_it != cb_list->end();   
         ++observers_it) {
      callb = (*observers_it).second.get();
      // if not disabled in behavior map
      callbackBehaviorMapType::iterator beh_it = m_CallbackBehaviorMap.find(callb->get_observer());
      if (  beh_it != m_CallbackBehaviorMap.end()  ) {
        if ( beh_it->second == CallbackBehavior::CALLBACK_MEMBER_CHANGES) { 
          //GS_PARAM_ARRAY_DUMP("observer "<<(unsigned long int)callb->get_observer()<<" is in behavior map - CALLBACK_MEMBER_CHANGES - perform member call back");
          GS_PARAM_ARRAY_DUMP("observer <omitted ptr addr for diff> is in behavior map - CALLBACK_MEMBER_CHANGES - perform member call back");
          // Make call with changed parameter
          tmp_ret = callb->call(changed_param, cb_reason);
          if (tmp_ret == return_value_change_rejected && ret == return_nothing) ret = tmp_ret;
          else if (tmp_ret == return_other_error) ret = tmp_ret;
        } 
        else {
          //GS_PARAM_ARRAY_DUMP("observer "<<(unsigned long int)callb->get_observer()<<" is in behavior map - NOT_CALLBACK_MEMBER_CHANGES - NOT perform member call back");
          GS_PARAM_ARRAY_DUMP("observer <omitted ptr addr for diff> is in behavior map - NOT_CALLBACK_MEMBER_CHANGES - NOT perform member call back");
        }
      }
      else {
        //GS_PARAM_ARRAY_DUMP("default: observer "<<(unsigned long int)callb->get_observer()<<" is NOT in behavior map - perform member call back");
        GS_PARAM_ARRAY_DUMP("default: <omitted ptr addr for diff> observer is NOT in behavior map - perform member call back");
        // Make call with changed parameter
        tmp_ret = callb->call(changed_param, cb_reason);
        if (tmp_ret == return_value_change_rejected && ret == return_nothing) ret = tmp_ret;
        else if (tmp_ret == return_other_error) ret = tmp_ret;
      }
    }
    gs_param_base_T::m_currently_making_callbacks = false;
    gs_param_base_T::cleanup_callback_list();
    // remove member if destroying
    if (changed_param.is_destructing()) {
      removeMember(changed_param.getName());
    }
    return ret;
  }
  
  /// Removes a member out of this array. Called by the callback method.
  /**
   * Only removes the pointer out of the internal vector. Does not
   * delete the pointer.
   *
   * This takes the name and not a pointer or reference to be able to remove the
   * member even during it's destruction.
   *
   * @param name   Name of the parameter that has to be removed.
   */
  bool removeMember(const std::string name) {
    GS_PARAM_ARRAY_DUMP("removeMember("<<name<<")");
    if (this->make_reject_write_callbacks() == return_value_change_rejected) {
      GS_PARAM_ARRAY_DUMP("  reject_write callback rejected remove member!");
      return false;
    }
    this->make_pre_write_callbacks();
#ifdef GCNF_ENABLE_GS_PARAM_LOCK
    // Check the lock!
    if (gs_param_base_T::m_locked) {                                                
      GS_PARAM_ARRAY_DUMP("parameter is locked!");                                      
      SC_REPORT_INFO(GCNF_SC_REPORTER(this->getName()), "parameter is locked!");
      return false;                                                               
    }                                                                             
#endif
    if (m_MemberMap.erase( name ) > 0) {
      this->make_post_write_callbacks();
      return true;
    }
    this->make_post_write_callbacks();
    return false;
  }

  /// Adds a member. Called by friend class gs_param_base.
  /**
   * Adds a new member to the internal member vector.
   * Checks if correct name: hierarchical name must be the name of this array.
   *
   * @param new_member  Parameter base object which should be added to this array.
   * @return Returns the success of adding (fails if wrong name).
   */
  bool addMember(gs_param_base_T &new_member) {
    GS_PARAM_ARRAY_DUMP("addMember("<<new_member.getName()<<")");
    // check for correct name
    std::string new_members_parent_name = new_member.getName().substr(0, new_member.getName().find_last_of(SC_NAME_DELIMITER));
    if (strcmp(m_par_name.c_str(), new_members_parent_name.c_str()) != 0) {
      GCNF_DUMP_N(name(), "addMember: The hierarchical name of the added member '"<<new_member.getName().c_str()<<"' has to start with the name of this parent array object ('"<<m_par_name.c_str()<<".<new_member_name>')!");
      SC_REPORT_WARNING(name(), "addMember: You tried to add a member which has the wrong hierarchical name!");
      return false;
    }
    // callback changes of this
    if (this->make_reject_write_callbacks() == return_value_change_rejected) {
      GS_PARAM_ARRAY_DUMP("  reject_write callback rejected add member!");
      return false;
    }
    this->make_pre_write_callbacks();
#ifdef GCNF_ENABLE_GS_PARAM_LOCK
    // Check the lock!
    if (gs_param_base_T::m_locked) {                                                
      GS_PARAM_ARRAY_DUMP("parameter is locked!");                                      
      SC_REPORT_INFO(GCNF_SC_REPORTER(this->getName()), "parameter is locked!");
      return false;
    }                                                                             
#endif
    // add member
    m_MemberMap.insert(   make_pair( new_member.getName(), &new_member )   );
    // register this as an observer (for callbacks and for destruction info)
    new_member.registerParamCallback(boost::shared_ptr<gs::cnf::ParamCallbAdapt_b>(new gs::cnf::ParamTypedCallbAdapt<gs_param_array_T>(this, &gs_param_array_T::array_config_callback, this, &new_member)), pre_read);
    //new_member.registerParamCallback(boost::shared_ptr<gs::cnf::ParamCallbAdapt_b>(new gs::cnf::ParamTypedCallbAdapt<gs_param_array_T>(this, &gs_param_array_T::array_config_callback, this, &new_member)), post_read); // TODO add when supported
    new_member.registerParamCallback(boost::shared_ptr<gs::cnf::ParamCallbAdapt_b>(new gs::cnf::ParamTypedCallbAdapt<gs_param_array_T>(this, &gs_param_array_T::array_config_callback, this, &new_member)), reject_write);
    new_member.registerParamCallback(boost::shared_ptr<gs::cnf::ParamCallbAdapt_b>(new gs::cnf::ParamTypedCallbAdapt<gs_param_array_T>(this, &gs_param_array_T::array_config_callback, this, &new_member)), pre_write);
    new_member.registerParamCallback(boost::shared_ptr<gs::cnf::ParamCallbAdapt_b>(new gs::cnf::ParamTypedCallbAdapt<gs_param_array_T>(this, &gs_param_array_T::array_config_callback, this, &new_member)), post_write);
    new_member.registerParamCallback(boost::shared_ptr<gs::cnf::ParamCallbAdapt_b>(new gs::cnf::ParamTypedCallbAdapt<gs_param_array_T>(this, &gs_param_array_T::array_config_callback, this, &new_member)), destroy_param);
    
    // callback changes of this
    this->make_post_write_callbacks();
    return true;
  }
  
  /// Delete all members and the internal array
  /**
   * @return If delete was successfull (or not because e.g. read-only param)
   */
  bool delete_array() {
    if (this->make_reject_write_callbacks() == return_value_change_rejected) {
      GS_PARAM_ARRAY_DUMP("delete_array: reject_write callback rejected delete member!");
      return false;
    }
    this->make_pre_write_callbacks();
#ifdef GCNF_ENABLE_GS_PARAM_LOCK
    // Check the lock!
    if (gs_param_base_T::m_locked) {                                                
      GS_PARAM_ARRAY_DUMP("parameter is locked!");                                      
      SC_REPORT_INFO(GCNF_SC_REPORTER(this->getName()), "parameter is locked!");
      return false;
    }                                                                             
#endif
    std::vector<gs_param_base*> pvec;
    // go through all members and store them to delete
    typename memberMapType::iterator it;
    for (it = m_MemberMap.begin();
         it != m_MemberMap.end(); ++it) {
      pvec.push_back((*it).second); // store pointer to delete later
      //m_MemberMap.erase(it);
      (*it).second = NULL;
    }
    m_MemberMap.clear();
    // now delete the members
    for (std::vector<gs_param_base*>::iterator it = pvec.begin(); it != pvec.end(); it++) {
      delete *it;
    }
    this->make_post_write_callbacks();
    return true;
  }

protected:

  /// String whose reference can be returned as string value
  mutable std::string return_string;

};

/// Typedef for gs_param_array_T with the only allowed template specialization
typedef gs_param_array_T<gs_param_base> gs_param_array;

  
} // end namespace cnf
using cnf::gs_param_array; // make gs_param_array available in gs namespace
} // end namespace gs

#endif
