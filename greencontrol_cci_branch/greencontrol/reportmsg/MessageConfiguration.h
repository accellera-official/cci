//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2008-2009 : GreenSocs Ltd
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

#ifndef __MESSAGE_CONFIGURATION_H__
#define __MESSAGE_CONFIGURATION_H__

#include "greencontrol/config.h"

BEGIN_GS_REPORTMSG_NAMESPACE

  // forward declaration
  class MessageStreamer;

  /// internal stream type for fast switch, mapped to msgconfig_name
  enum special_stream_types {
    default_stream_type = 0,
    real_file,
    stdout_stream, // if msgconfig_name == "stdout"
    stderr_stream, // if msgconfig_name == "stderr"
    sc_report_info_stream, // if msgconfig_name == "SC_REPORT_INFO"
    sc_report_warning_stream, // if msgconfig_name == "SC_REPORT_WARNING"
    sc_report_error_stream, // if msgconfig_name == "SC_REPORT_ERROR"
    sc_report_fatal_stream // if msgconfig_name == "SC_REPORT_FATAL"
  };

  /// Report Message Configuration object
  /**
   * used for user configuration as well as for internal storage in gs_param
   */
  class MessageConfiguration
  {
    friend class MessageStreamer;
    friend class gs::cnf::gs_param<MessageConfiguration>;
    
  protected:
    
    /// if the members are registered at db
    bool m_registered_at_db;
    
    special_stream_types       stream_type;
    gs::av::File_OutputPlugin* output_plugin;

    /// Constructor creating gs_param members being registered at database (for MessageStreamer only)
    MessageConfiguration(std::string nam)
    : m_registered_at_db(true)
    , stream_type(default_stream_type)
    , output_plugin(NULL)
    , msgconfig_name(nam+".msgconfig_name")
    , msgconfig_starttime_en(nam+".msgconfig_starttime_en")
    , msgconfig_starttime(nam+".msgconfig_starttime")
    , msgconfig_endtime(nam+".msgconfig_endtime")
    , msgconfig_dbglvl(nam+".msgconfig_dbglvl")
    , msgconfig_info_en(nam+".msgconfig_info_en")
    , msgconfig_warn_en(nam+".msgconfig_warn_en")
    , msgconfig_error_en(nam+".msgconfig_error_en")
    , msgconfig_fatal_en(nam+".msgconfig_fatal_en")
    , msgconfig_printtime(nam+".msgconfig_printtime")
    , msgconfig_printname(nam+".msgconfig_printname")
    , msgconfig_printfile(nam+".msgconfig_printfile")
    , msgconfig_printlevel(nam+".msgconfig_printlevel")
    , msgconfig_module_id(nam+".msgconfig_module_id")
    {   
      std::cout << "protected MessageConfiguration constructor with name '"<<nam<<"' and registration to db" << std::endl;
      std::cout << "MessageConfiguration content after construction is:" << std::endl << to_string() << std::endl;
    }

    /// Constructor creating gs_param members being registered at database (for MessageStreamer only)
    MessageConfiguration(const char* nam)
    : m_registered_at_db(true)
    , stream_type(default_stream_type)
    , output_plugin(NULL)
    , msgconfig_name(std::string(nam)+".msgconfig_name")
    , msgconfig_starttime_en(std::string(nam)+".msgconfig_starttime_en")
    , msgconfig_starttime(std::string(nam)+".msgconfig_starttime")
    , msgconfig_endtime(std::string(nam)+".msgconfig_endtime")
    , msgconfig_dbglvl(std::string(nam)+".msgconfig_dbglvl")
    , msgconfig_info_en(std::string(nam)+".msgconfig_info_en")
    , msgconfig_warn_en(std::string(nam)+".msgconfig_warn_en")
    , msgconfig_error_en(std::string(nam)+".msgconfig_error_en")
    , msgconfig_fatal_en(std::string(nam)+".msgconfig_fatal_en")
    , msgconfig_printtime(std::string(nam)+".msgconfig_printtime")
    , msgconfig_printname(std::string(nam)+".msgconfig_printname")
    , msgconfig_printfile(std::string(nam)+".msgconfig_printfile")
    , msgconfig_printlevel(std::string(nam)+".msgconfig_printlevel")
    , msgconfig_module_id(std::string(nam)+".msgconfig_module_id")
    {
      std::cout << "protected MessageConfiguration constructor with name '"<<nam<<"' and registration to db" << std::endl;
      std::cout << "MessageConfiguration content after construction is:" << std::endl << to_string() << std::endl;
    }
    
  public:
    
    /// Contructor creating gs_param members NOT being registered at database (for user)
    MessageConfiguration()
    : m_registered_at_db(false)
    , stream_type(default_stream_type)
    , output_plugin(NULL)
    , msgconfig_name(std::string("msgconfig_name"), std::string(), NULL, false, false) // do not register at db
    , msgconfig_starttime_en(std::string("msgconfig_starttime_en"), std::string(), NULL, false, false) // do not register at db
    , msgconfig_starttime(std::string("msgconfig_starttime"), std::string(), NULL, false, false) // do not register at db
    , msgconfig_endtime(std::string("msgconfig_endtime"), std::string(), NULL, false, false) // do not register at db
    , msgconfig_dbglvl(std::string("msgconfig_dbglvl"), std::string(), NULL, false, false) // do not register at db
    , msgconfig_info_en(std::string("msgconfig_info_en"), std::string(), NULL, false, false) // do not register at db
    , msgconfig_warn_en(std::string("msgconfig_warn_en"), std::string(), NULL, false, false) // do not register at db
    , msgconfig_error_en(std::string("msgconfig_error_en"), std::string(), NULL, false, false) // do not register at db
    , msgconfig_fatal_en(std::string("msgconfig_fatal_en"), std::string(), NULL, false, false) // do not register at db
    , msgconfig_printtime(std::string("msgconfig_printtime"), std::string(), NULL, false, false) // do not register at db
    , msgconfig_printname(std::string("msgconfig_printname"), std::string(), NULL, false, false) // do not register at db
    , msgconfig_printfile(std::string("msgconfig_printfile"), std::string(), NULL, false, false) // do not register at db
    , msgconfig_printlevel(std::string("msgconfig_printlevel"), std::string(), NULL, false, false) // do not register at db
    , msgconfig_module_id(std::string("msgconfig_module_id"), std::string(), NULL, false, false) // do not register at db
    {  
      std::cout << "public MessageConfiguration constructor without name and NO registration to db" << std::endl;
    }
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////   members   //////////////////////////////////////////////// //

    // message config name
    gs_param<std::string> msgconfig_name; // filename
    // time options
    gs_param<bool> msgconfig_starttime_en; // if start-/end-time enabled
    gs_param<sc_core::sc_time> msgconfig_starttime;
    gs_param<sc_core::sc_time> msgconfig_endtime;
    // debug level
    gs_param<int> msgconfig_dbglvl;  // max. debug level
    // system level
    gs_param<bool> msgconfig_info_en;
    gs_param<bool> msgconfig_warn_en;
    gs_param<bool> msgconfig_error_en;
    gs_param<bool> msgconfig_fatal_en;
    // output options
    gs_param<bool> msgconfig_printtime;
    gs_param<bool> msgconfig_printname;
    gs_param<bool> msgconfig_printfile;
    gs_param<bool> msgconfig_printlevel;
    /// lists of modules/streams to be captured (comma separated list!)
    gs_param< std::vector<std::string> > msgconfig_module_id;
    
    void reset() {
      msgconfig_name.setString("");
      msgconfig_starttime_en.setString("");
      msgconfig_starttime.setString("");
      msgconfig_endtime.setString("");
      msgconfig_dbglvl.setString("");
      msgconfig_info_en.setString("");
      msgconfig_warn_en.setString("");
      msgconfig_error_en.setString("");
      msgconfig_fatal_en.setString("");
      msgconfig_printtime.setString("");
      msgconfig_printname.setString("");
      msgconfig_printfile.setString("");
      msgconfig_printlevel.setString("");
      msgconfig_module_id.setString("");
    }
    
    std::string to_string() {
      std::ostringstream ss;
      ss << msgconfig_name.getName() << " = " << msgconfig_name.getString() << std::endl;
      ss << msgconfig_starttime_en.getName() << " = " << msgconfig_starttime_en.getString() << std::endl;
      ss << msgconfig_starttime.getName() << " = " << msgconfig_starttime.getString() << std::endl;
      ss << msgconfig_endtime.getName() << " = " << msgconfig_endtime.getString() << std::endl;
      ss << msgconfig_dbglvl.getName() << " = " << msgconfig_dbglvl.getString() << std::endl;
      ss << msgconfig_info_en.getName() << " = " << msgconfig_info_en.getString() << std::endl;
      ss << msgconfig_warn_en.getName() << " = " << msgconfig_warn_en.getString() << std::endl;
      ss << msgconfig_error_en.getName() << " = " << msgconfig_error_en.getString() << std::endl;
      ss << msgconfig_fatal_en.getName() << " = " << msgconfig_fatal_en.getString() << std::endl;
      ss << msgconfig_printtime.getName() << " = " << msgconfig_printtime.getString() << std::endl;
      ss << msgconfig_printname.getName() << " = " << msgconfig_printname.getString() << std::endl;
      ss << msgconfig_printfile.getName() << " = " << msgconfig_printfile.getString() << std::endl;
      ss << msgconfig_printlevel.getName() << " = " << msgconfig_printlevel.getString() << std::endl;
      ss << msgconfig_module_id.getName() << " = " << msgconfig_module_id.getString() << std::endl;
      return ss.str();
    }
    
  };

END_GS_REPORTMSG_NAMESPACE

namespace gs {
namespace cnf {

  /// gs_param template specialization for MessageConfiguration
  /**
   * This specialization does not derive from gs_param_t because it needs to
   * call the my_value's constructor with a name, which is not possible with 
   * gs_param_t!!!
   */
  template<>
  class gs_param< gs::report::MessageConfiguration >
  : public gs_param_base
  {
  protected:
    
    /// Typedef for the value.
    typedef gs::report::MessageConfiguration val_type;

    typedef gs_param<gs::report::MessageConfiguration> my_type;
    
    val_type my_value;
    
  protected:
    
    using gs_param_base::m_api;
    using gs_param_base::m_par_name;
    using gs_param_base::name;
    
  private:
    /// Not allowed constructor!
    explicit gs_param(const val_type &val) : gs_param_base(string(""), true, NULL, false) { sc_assert(false); }
    
  public:
    // Explicit constructors to avoid implicit construction of parameters.
    
    /// Empty constructor. Name will be set in base
    explicit gs_param(const bool force_top_level_name = false) : gs_param_base("", true, NULL, force_top_level_name) { init();  }
    
    /// Constructor with (local/hierarchical) name.
    explicit gs_param(const std::string &nam) : gs_param_base(nam        , true, NULL, false), my_value(nam) { init(); }
    explicit gs_param(const char *nam       ) : gs_param_base(string(nam), true, NULL, false), my_value(nam)  { init(); }
    
    /// Constructor with (local/hierarchical) name and string representation of initial value.
    explicit gs_param(const std::string &nam, const std::string &val, const bool force_top_level_name = false) : gs_param_base(nam        , true, NULL, force_top_level_name ), my_value(nam) { init(convertStringToValue(val));         } 
    explicit gs_param(const char *nam,        const char *val       , const bool force_top_level_name = false) : gs_param_base(string(nam), true, NULL, force_top_level_name ), my_value(nam) { init(convertStringToValue(string(val))); } 
    explicit gs_param(const std::string &nam, const char *val       , const bool force_top_level_name = false) : gs_param_base(nam        , true, NULL, force_top_level_name ), my_value(nam) { init(convertStringToValue(string(val))); } 
    explicit gs_param(const char *nam,        const std::string &val, const bool force_top_level_name = false) : gs_param_base(string(nam), true, NULL, force_top_level_name ), my_value(nam) { init(convertStringToValue(val));         } 
    
    /// Constructor with (local/hierarchical) name and initial value.
    explicit gs_param(const std::string &nam, const val_type &val, const bool force_top_level_name = false) : gs_param_base(nam        , true, NULL, force_top_level_name ), my_value(nam)  { init(val); }    
    explicit gs_param(const char *nam,        const val_type &val, const bool force_top_level_name = false) : gs_param_base(string(nam), true, NULL, force_top_level_name ), my_value(nam)  { init(val); }    
    
    // Constructors with parent array
    explicit gs_param(                                                gs_param_array* parent_array) : gs_param_base(string("") , true, parent_array, false), my_value("") { init(); }
    explicit gs_param(const std::string &nam                        , gs_param_array* parent_array) : gs_param_base(nam        , true, parent_array, false), my_value(nam) { init(); } 
    explicit gs_param(const char *nam                               , gs_param_array* parent_array) : gs_param_base(string(nam), true, parent_array, false), my_value(nam) { init(); } 
    explicit gs_param(const std::string &nam, const std::string &val, gs_param_array* parent_array) : gs_param_base(nam        , true, parent_array, false), my_value(nam) { init(convertStringToValue(val));         } 
    explicit gs_param(const char *nam,        const char *val       , gs_param_array* parent_array) : gs_param_base(string(nam), true, parent_array, false), my_value(nam) { init(convertStringToValue(string(val))); } 
    explicit gs_param(const std::string &nam, const char *val       , gs_param_array* parent_array) : gs_param_base(nam        , true, parent_array, false), my_value(nam) { init(convertStringToValue(string(val))); } 
    explicit gs_param(const char *nam,        const std::string &val, gs_param_array* parent_array) : gs_param_base(string(nam), true, parent_array, false), my_value(nam) { init(convertStringToValue(val));         } 
    explicit gs_param(const std::string &nam, const val_type &val   , gs_param_array* parent_array) : gs_param_base(nam        , true, parent_array, false), my_value(nam) { init(val); }    
    explicit gs_param(const char *nam,        const val_type &val   , gs_param_array* parent_array) : gs_param_base(string(nam), true, parent_array, false), my_value(nam) { init(val); }    
    
    // Constructors with parent array
    explicit gs_param(                                                gs_param_array& parent_array) : gs_param_base(string("") , true, &parent_array, false), my_value("") { init(); }
    explicit gs_param(const std::string &nam                        , gs_param_array& parent_array) : gs_param_base(nam        , true, &parent_array, false), my_value(nam) { init(); } 
    explicit gs_param(const char *nam                               , gs_param_array& parent_array) : gs_param_base(string(nam), true, &parent_array, false), my_value(nam) { init(); } 
    explicit gs_param(const std::string &nam, const std::string &val, gs_param_array& parent_array) : gs_param_base(nam        , true, &parent_array, false), my_value(nam) { init(convertStringToValue(val));         } 
    explicit gs_param(const char *nam,        const char *val       , gs_param_array& parent_array) : gs_param_base(string(nam), true, &parent_array, false), my_value(nam) { init(convertStringToValue(string(val))); } 
    explicit gs_param(const std::string &nam, const char *val       , gs_param_array& parent_array) : gs_param_base(nam        , true, &parent_array, false), my_value(nam) { init(convertStringToValue(string(val))); } 
    explicit gs_param(const char *nam,        const std::string &val, gs_param_array& parent_array) : gs_param_base(string(nam), true, &parent_array, false), my_value(nam) { init(convertStringToValue(val));         } 
    explicit gs_param(const std::string &nam, const val_type &val   , gs_param_array& parent_array) : gs_param_base(nam        , true, &parent_array, false), my_value(nam) { init(val); }    
    explicit gs_param(const char *nam,        const val_type &val   , gs_param_array& parent_array) : gs_param_base(string(nam), true, &parent_array, false), my_value(nam) { init(val); }    
    
    // Constructors with register_at_db bool
    explicit gs_param(const std::string &nam, const std::string &val, gs_param_array* parent_array, const bool force_top_level_name, const bool register_at_db) : gs_param_base(nam, register_at_db,  parent_array, force_top_level_name), my_value(nam) { init(convertStringToValue(val));         } 
    explicit gs_param(const std::string &nam, const std::string &val, gs_param_array& parent_array, const bool force_top_level_name, const bool register_at_db) : gs_param_base(nam, register_at_db, &parent_array, force_top_level_name), my_value(nam) { init(convertStringToValue(val));         } 
    
    
    /// Overloads gs_param_t<T>::deserialize in gs_param_t<T>
    const bool deserialize(val_type &target_val, const std::string& str) {
#ifdef GCNF_ENABLE_GS_PARAM_LOCK
      // Check the lock!
      if (gs_param_base::m_locked) {
        GS_PARAM_DUMP("parameter is locked!");
        SC_REPORT_INFO(GCNF_SC_REPORTER(this->getName()), "parameter is locked!");
        return false;
      }                                                                             
#endif
      return static_deserialize(target_val, str);
    }
    
    /// Destructor
    virtual ~gs_param() {
      gs_param_base::destruct_gs_param();
    }
    

    // //////////////////////////////////////////////////////////////////// //
    // ///////   set and get with string representation   ///////////////// //
    
    
    /// Set the value of this parameter with a string.
    /**
     * @param str  The new value for this parameter, represented as a string.
     * @return If setting was successful
     */ 
    bool setString(const std::string &str) {
      if (make_pre_write_callbacks() == return_value_change_rejected) {
        GS_PARAM_DUMP("pre_write callback rejected value change!");
        return false;
      }
      bool success = deserialize(my_value, str);
      make_post_write_callbacks();
      return success;
    }
    
    /// Get the value of this parameter as a string.
    /**
     * @return String representation of the current value of this parameter.
     */
    const std::string& getString() const {
      make_pre_read_callbacks();
      return_string = convertValueToString(my_value);
      make_post_read_callbacks();
      return return_string;
    }
  
    // ///////////////////////
    //  override functions
    
    
    /// Overrides gs_param_base::getTypeString
    const std::string getTypeString() const {
      return std::string("gs::report::MessageConfiguration");
    }
    
    /// Overrides gs_param_base::get_value_pointer
    const void* get_value_pointer() const {
      SC_REPORT_WARNING(gs_param_base::name(), "get_value_pointer not implemented in MessageConfiguration!");
      return NULL;
    }
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////   set and get with string representation   ///////////////// //
    
    inline static bool static_deserialize(val_type &target_val, const std::string& str) {
      if (str.length() == 0) {
        //target_val.reset();
        return true;
      }
      SC_REPORT_WARNING("MessageConfiguration", "static_deserialize (setString) not implemented for MessageConfiguration parameters! Please set the members individually!");
      GCNF_DUMP_N("MessageConfiguration", "setString not implemented for MessageConfiguration parameters");
      return false;
    }
    
    
    /// Get the value of this parameter as a string.
    std::string convertValueToString(const val_type &val) const{
      // TODO
      //SC_REPORT_WARNING("MessageConfiguration", "convertValueToString (getString) not implemented for MessageConfiguration parameters!");
      std::string str("Warning: getString not implemented for MessageConfiguration parameters");
      GCNF_DUMP_N(name(), "convertValueToString (getString) not implemented for MessageConfiguration parameters");
      return str;
    }
    
     
    // //////////////////////////////////////////////////////////////////// //
    // ///////   array methods   ////////////////////////////////////////// //
    
    
    /// Returns the member with the name member_name
    /**
     * Not secure!
     */
    gs_param_base& operator[] (const std::string member_name) { 
      return *getMember(member_name);
    }
    
    /// Returns the member with the specified name.
    /**
     * @return Returns the member of this array with the specified name. 
     *         NULL if no member with the name.
     */
    gs_param_base* getMember(const std::string name) {
      make_pre_read_callbacks();
      //make_post_read_callbacks(); // TODO not possible here!
      if (name == "msgconfig_name") return &my_value.msgconfig_name;
      if (name == "msgconfig_starttime_en") return &my_value.msgconfig_starttime_en;
      if (name == "msgconfig_starttime") return &my_value.msgconfig_starttime;
      if (name == "msgconfig_endtime") return &my_value.msgconfig_endtime;
      if (name == "msgconfig_dbglvl") return &my_value.msgconfig_dbglvl;
      if (name == "msgconfig_info_en") return &my_value.msgconfig_info_en;
      if (name == "msgconfig_warn_en") return &my_value.msgconfig_warn_en;
      if (name == "msgconfig_error_en") return &my_value.msgconfig_error_en;
      if (name == "msgconfig_fatal_en") return &my_value.msgconfig_fatal_en;
      if (name == "msgconfig_printtime") return &my_value.msgconfig_printtime;
      if (name == "msgconfig_printname") return &my_value.msgconfig_printname;
      if (name == "msgconfig_printfile") return &my_value.msgconfig_printfile;
      if (name == "msgconfig_printlevel") return &my_value.msgconfig_printlevel;
      if (name == "msgconfig_module_id") return &my_value.msgconfig_module_id;
      return NULL;
    }
    
    
    /// Return a reference to the MessageConfiguration (which is non-const!)
    /**
     * Note: If the returned object is changed, no callback for this parameter
     *       will be made!! The callback of the member is done anyway.
     */
    val_type& get_MessageConfiguration() {
      make_pre_read_callbacks();
      //make_post_read_callbacks(); // TODO not possible here, returning tmp object would change behavior!
      return my_value;
    }
   
    
    
  protected:

    /// Init method to set the value and add the parameter to the plugin db.
    /** 
     * Has to be called by the gs_param class after construction!
     * Does set the value and add the parameter to the plugin db.
     */
    void init(const val_type &default_val) {
      // set my_value
      my_value = default_val;
      // add to plugin database
      if (m_register_at_db) {
        m_api->addPar(this);
      }
    }
    /// Init method without value @see gs::cnf::gs_param_t::init(val_type &default_val)
    void init() {
      GS_PARAM_DUMP("Init gs_param<MessageConfiguration> " << m_par_name.c_str());
      my_value = convertStringToValue(string(""));
      // add to plugin database
      if (m_register_at_db) {
        m_api->addPar(this);
      }
    }
    
    /// Get the value the string. Needed for construction of gs_param.
    /**
     * Conversion string --> value type.
     *
     * @param  str String that should be converted to a value
     * @return If the convert was successfull
     */
    const val_type& convertStringToValue(const std::string& str) {    
      deserialize(my_value, str);                   
      return my_value;                              
    }                                                                 
    
    /// String whose reference can be returned as string value
    mutable std::string return_string;

    
    
    
    
    
  };


} // end namespace cnf
} // end namespace gs

#endif
