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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

// doxygen comments

#ifndef __MESSAGE_STREAMER_H__
#define __MESSAGE_STREAMER_H__

// for perror
#include <stdio.h>
#include <map>
#include <ostream>
#include "reportmsg_globals.h"
#include "repmsg_datatypes.h"
#include "repmsg_dump.h"

#include "greencontrol/analysis_file_outputplugin.h"
#include "greencontrol/gcnf/plugin/configset.h"

#include "MessageConfiguration.h"


BEGIN_GS_REPORTMSG_NAMESPACE

  /// Interface implemented by the report stream objects to allow enable/disable control by the MessageStreamer
  class report_stream_if {
  public:
    virtual ~report_stream_if() { }
    virtual void enable(bool) = 0;
    virtual const char* name() const = 0;
    virtual combined_message_level get_message_level() const = 0;
    virtual const char* get_parent_name() const = 0;
    virtual const char* get_id() const = 0;
  };

  /// Streamer back-end class for debug and system message streamer
  /**
   * Directs the incoming message to the correct output
   * dependent on the configuration
   */
  class MessageStreamer {
  protected:
    
    /// Contains the config parameter arrays
    gs_param<MessageConfiguration*> m_msg_config;

    /// Internal struct to store the overriding configuration in the map
    struct override_config_type {
      debug_msg_level dbglvl;
      bool info_en;
      bool warning_en;
      bool error_en;
      bool fatal_en;
    };

    /// Map storing the override information
    std::map<std::string, override_config_type> m_override_config;
    
    /// Vector storing all registered report stream to be able to handle enable/disable, the bool is the enabled value used temporary
    std::vector< std::pair<report_stream_if*, bool> > m_report_streams;
    
    /// protected Constructor, use static get_global_message_streamer() function
    MessageStreamer()
    : m_msg_config("MessageStreamer_config", 0, NULL, true, true) // array constructor with low priority default size
    { 
#ifdef MSG_VERBOSE
      sc_core::sc_object *obj = sc_core::sc_get_current_process_handle();
      if (obj) {
#if SYSTEMC_API == 210
        obj = obj->get_parent();
#else
        obj = obj->get_parent_object();
#endif
        REPMSG_DUMP_N("MessageStreamer", "constructor called within "<< obj->name());
      }
      else {
        REPMSG_DUMP_N("MessageStreamer", "constructor called on toplevel");
      }
#endif
      m_analysisAPI = gs::av::GAV_Api::getApiInstance(NULL);
      update_stream_enables();
    }
    
    /// Analysis API
    gs::av::shared_ptr<gs::av::GAV_Api>  m_analysisAPI;

  public:
    
    
    // ///////////////  functions called by streams  ////////////////// //
    
    
    /// Returns (and creates) the (one) global MessageStreamer object
    static MessageStreamer* get_global_message_streamer() {
      static MessageStreamer* global_message_streamer;
      if (!global_message_streamer)
        global_message_streamer = new MessageStreamer();
      return global_message_streamer;
    }
    

    /// Called by the debug or system stream object to do the outputs
    /**
     * @param module Name of the module which owns the streamer calling this.
     * @param streamer_id  Module-wide unique streamer id.
     * @paran level  Message level (severity level in case of system messages, verbosity level in case of debug messages).
     * @param raw_msg_string  Raw unformatted string that should be outputted
     * @param message_filename  Filename where this message was produced - may be used to format the string (dependent on output settings)
     * @param message_line  Line where this message was produced - may be used to format the string (dependent on output settings)
     * @param parent_name  Name of the sc_core::sc_module where this message was produced - may be used to format the string (dependent on output settings)
     */
    void out(const char* module_name, const char* streamer_id, 
             const combined_message_level level, const std::string& raw_msg_string,
             const std::string& message_filename, unsigned int message_line,
             const std::string& parent_name) {
      std::string nam;
      std::ostringstream ss;
      REPMSG_DUMP_N("MessageStreamer",
                 "out( from module "<<module_name<<", streamer id="<<streamer_id<<", level="<<message_level_to_string(level)<<","<<std::endl
                 <<"                  filename="<<message_filename<<", line="<<message_line<<", parent_name="<<parent_name<<std::endl
                 <<"                  message:  "<< raw_msg_string);
      for (unsigned int fileno = 0; fileno < m_msg_config.size(); fileno++) {
        // check filter
        if (check_config(fileno, module_name, streamer_id, level)) {
          REPMSG_DUMP_N("MessageStreamer", "check fileno " << fileno << " ("<<m_msg_config[fileno].get_MessageConfiguration().msgconfig_name<<") passed");

          // reset stream
          ss.clear();
          ss.str("");
          // prepare string
          if (m_msg_config[fileno].get_MessageConfiguration().msgconfig_printlevel) ss << message_level_to_string(level) << ": ";
          if (m_msg_config[fileno].get_MessageConfiguration().msgconfig_printfile) ss << message_filename << ":" << message_line << ": ";
          if (m_msg_config[fileno].get_MessageConfiguration().msgconfig_printtime) ss << "@" << sc_core::sc_time_stamp().to_string() << " /" << (unsigned)sc_core::sc_delta_count() << ": ";
          if (m_msg_config[fileno].get_MessageConfiguration().msgconfig_printname) ss << "(" << parent_name << "): ";
          ss << raw_msg_string << std::endl; // adds endline!
            
          // make output
          switch (m_msg_config[fileno].get_MessageConfiguration().stream_type) {
            case stdout_stream:
              std::cout << ss.str() << std::flush;
              break;
            case stderr_stream:
              perror(ss.str().c_str());
              break;
            case sc_report_info_stream:
              nam = module_name; if (nam.length() > 0) nam += "."; nam += streamer_id;
              SC_REPORT_INFO(nam.c_str(), ss.str().c_str());
              break;
            case sc_report_warning_stream:
              nam = module_name; if (nam.length() > 0) nam += "."; nam += streamer_id;
              SC_REPORT_WARNING(nam.c_str(), ss.str().c_str());
              break;
            case sc_report_error_stream:
              nam = module_name; if (nam.length() > 0) nam += "."; nam += streamer_id;
              SC_REPORT_ERROR(nam.c_str(), ss.str().c_str());
              break;
            case sc_report_fatal_stream:
              nam = module_name; if (nam.length() > 0) nam += "."; nam += streamer_id;
              SC_REPORT_FATAL(nam.c_str(), ss.str().c_str());
              break;
            default:
              // normal file output to the output plugin of this file no.
              assert(m_msg_config[fileno].get_MessageConfiguration().output_plugin != NULL);
              m_msg_config[fileno].get_MessageConfiguration().output_plugin->pure_output(ss.str());
              break;
          } // end switch
        } // end if
        else {
          REPMSG_DUMP_N("MessageStreamer", "check fileno " << fileno << " ("<<m_msg_config[fileno].get_MessageConfiguration().msgconfig_name<<") failed");
        }
      } // end for
    }

    /// Registers a report stream (debug or system) to allow handling of enable/disable by this
    void register_stream_object(report_stream_if& stream) {
      m_report_streams.push_back( std::make_pair(&stream, false) );
      // check enables/disables
      REPMSG_DUMP_N("MessageStreamer", "new registered stream "<< stream.name() <<": update_stream_enables");
      update_stream_enables();
    }
            
    /// Adds a report message file config
    /**
     * Adds a new report message configureation (e.g. file).
     * Error if the file name is empty.
     *
     * @param cnf  Configuration of the file that should be configured
     * @return     File number which identifies the file configuration within the MessageStreamer<br>4294967295 (=0xFFFFFFFF) on error
     */
    static unsigned int apply_configuration(msg_configuration &cnf) {
      return get_global_message_streamer()->apply_configuration_int(cnf);
    }

    /// Sets a system message config for module or id, overriding the message file settings
    static void set_override_sys_level_config(std::string module_or_fullid, system_msg_level level, bool enabled) {
      //combined_message_level l = (combined_message_level)( (int)(level + 10) );
      return get_global_message_streamer()->set_override_sys_level_config_int(module_or_fullid, level, enabled);
    }

    /// Sets a debug message config for module or id, overriding the message file settings
    static void set_override_dbg_level_conifg(std::string module_or_fullid, debug_msg_level level) {
      get_global_message_streamer()->set_override_dbg_level_conifg_int(module_or_fullid, level);
    }

    /// Applies a ConfigSet to add one file configuration
    /**
     * The ConfigSet should match the structure of the msg_configuration:<br>
     * Members:
     * - msgconfig_name (mandatory!)
     * - msgconfig_starttime_en
     * - msgconfig_starttime
     * - msgconfig_endtime
     * - msgconfig_dbglvl (at least the debug level or one system msg level should be set)
     * - msgconfig_info_en  (at least the debug level or one system msg level should be set)
     * - msgconfig_warn_en  (at least the debug level or one system msg level should be set)
     * - msgconfig_error_en  (at least the debug level or one system msg level should be set)
     * - msgconfig_fatal_en  (at least the debug level or one system msg level should be set)
     * - msgconfig_printtime
     * - msgconfig_printname
     * - msgconfig_printfile
     * - msgconfig_printlevel
     * - msgconfig_module_id
     * @param cs  ConfigSet that should be used to configure
     * @return    File number which identifies the file configuration within the MessageStreamer<br>4294967295 (=0xFFFFFFFF) on error
     */
    static unsigned int apply_config_set(const gs::cnf::ConfigSet& cs) {
      return get_global_message_streamer()->apply_config_set_int(cs);
    }
            
    /// Giving an user access to the output plugin used for message output to allow adding further parameters
    /**
     * @param fileno  File number whose output plugin should be returned
     * @return Output plugin pointer to the file's output plugin; NULL if no file (e.g. sc_report, cout,...)
     */
    static gs::av::File_OutputPlugin* get_output_plugin(unsigned int fileno) {
      return get_global_message_streamer()->get_output_plugin_int(fileno);
    }
            
  protected:
            
    // ////////////////////  Config functions (called by static functions)  ////////////////////////// //
    
    
    /// Adds a message file config
    /**
     * description see static function apply_configuration
     */
    unsigned int apply_configuration_int(msg_configuration &cnf) {
      // check for existing msgconfig_name
      //  if already existing: update the existing config
      //  else: resize all members of m_msg_config
      // copy all cnf members to (new/existing) m_msg_config members
      // return number of this file config (=position in the array)
      //
      // set stream_type!!
      // create output plugin! (on change, evtl. unregister this at output plugin)

      if (cnf.msgconfig_name.empty()) {
        SC_REPORT_WARNING("MessageStreamer", "apply_config_set failed: ConfigSet seems not to have a file configuration!");
        return 0xFFFFFFFF;
      }
      
      // check for existing msgconfig_name
      bool existing = false;
      unsigned int fileno;
      for (fileno = 0; fileno < m_msg_config.size(); fileno++) {
        if (m_msg_config[fileno].get_MessageConfiguration().msgconfig_name.getValue().compare(cnf.msgconfig_name) == 0) {
          existing = true;
          break;
        }
      }
      if (!existing) {
        fileno = m_msg_config.size();
        m_msg_config.resize(fileno + 1);
        m_msg_config[fileno].get_MessageConfiguration().msgconfig_name = cnf.msgconfig_name;
      }
      
      m_msg_config[fileno].get_MessageConfiguration().msgconfig_starttime_en = cnf.msgconfig_starttime_en;
      m_msg_config[fileno].get_MessageConfiguration().msgconfig_starttime = cnf.msgconfig_starttime;
      m_msg_config[fileno].get_MessageConfiguration().msgconfig_endtime = cnf.msgconfig_endtime;
      m_msg_config[fileno].get_MessageConfiguration().msgconfig_dbglvl = cnf.msgconfig_dbglvl;
      m_msg_config[fileno].get_MessageConfiguration().msgconfig_info_en = cnf.msgconfig_info_en;
      m_msg_config[fileno].get_MessageConfiguration().msgconfig_warn_en = cnf.msgconfig_warn_en;
      m_msg_config[fileno].get_MessageConfiguration().msgconfig_error_en = cnf.msgconfig_error_en;
      m_msg_config[fileno].get_MessageConfiguration().msgconfig_fatal_en = cnf.msgconfig_fatal_en;
      m_msg_config[fileno].get_MessageConfiguration().msgconfig_printtime = cnf.msgconfig_printtime;
      m_msg_config[fileno].get_MessageConfiguration().msgconfig_printname = cnf.msgconfig_printname;
      m_msg_config[fileno].get_MessageConfiguration().msgconfig_printfile = cnf.msgconfig_printfile;
      m_msg_config[fileno].get_MessageConfiguration().msgconfig_printlevel = cnf.msgconfig_printlevel;
      m_msg_config[fileno].get_MessageConfiguration().msgconfig_module_id = cnf.msgconfig_module_id;
      
      REPMSG_DUMP_N("MessageStreamer", "new file config "<< cnf.msgconfig_name<<": update_stream_enables");
      update_stream_enables();

      return fileno;
    }

    /// Sets a system message config for module or id, overriding the message file settings
    void set_override_sys_level_config_int(std::string module_or_fullid, system_msg_level level, bool enabled) {
      switch (level) {
        case sys_msg_INFO:
          m_override_config[module_or_fullid].info_en = enabled;
          break;
        case sys_msg_WARNING:
          m_override_config[module_or_fullid].warning_en = enabled;
          break;
        case sys_msg_ERROR:
          m_override_config[module_or_fullid].error_en = enabled;
          break;
        case sys_msg_FATAL:
          m_override_config[module_or_fullid].fatal_en = enabled;
          break;
        default:
          REPMSG_DUMP_N("MessageStreamer", "set_override_sys_level_config called with unexpected level!");
      }
      REPMSG_DUMP_N("MessageStreamer", "new override system level config for "<<module_or_fullid<<"update_stream_enables");
      update_stream_enables();
    }

    /// Sets a debug message config for module or id, overriding the message file settings
    void set_override_dbg_level_conifg_int(std::string module_or_fullid, debug_msg_level level) {
      switch (level) {
        case no_dbg_msg_level:
          REPMSG_DUMP_N("MessageStreamer", "set_override_debug_level_config called with unexpected level!");
          break;
        default:
          m_override_config[module_or_fullid].dbglvl = level;
      }
      REPMSG_DUMP_N("MessageStreamer", "new override debug level config for "<<module_or_fullid<<"update_stream_enables");
      update_stream_enables();
    }

    /// Applies a ConfigSet to add one file configuration
    /**
     * description see static function apply_configuration
     */
    unsigned int apply_config_set_int(const gs::cnf::ConfigSet& cs) {

      // First check if the top-level params are there like they should (at least the msgconfig_name!)
      bool config_file_specified = false;
      std::string tmp_msgconfig_name;
      for (gs::cnf::ConfigSet::cnf_data_map_type::const_iterator it = cs.get_config_map().begin();
           it != cs.get_config_map().end(); it++) {
        if (it->first == "msgconfig_name" && !it->second.empty()) {
          config_file_specified = true;
          tmp_msgconfig_name = it->second;
        }
      }  
      if (!config_file_specified) {
        SC_REPORT_WARNING("MessageStreamer", "apply_config_set failed: ConfigSet seems not to have a file configuration!");
        return 0xFFFFFFFF;
      }
      REPMSG_DUMP_N("MessageStreamer", "Apply ConfigSet for file name "<< tmp_msgconfig_name);
        
      // filter out the top-level params called like they should and set
      bool existing = false;
      unsigned int fileno;
      for (fileno = 0; fileno < m_msg_config.size(); fileno++) {
        if (m_msg_config[fileno].get_MessageConfiguration().msgconfig_name.getValue().compare(tmp_msgconfig_name) == 0) {
          existing = true;
          break;
        }
      }
      if (!existing) {
        fileno = m_msg_config.size();
        m_msg_config.resize(fileno + 1);
        m_msg_config[fileno].get_MessageConfiguration().msgconfig_name = tmp_msgconfig_name;
      }

      // go through the ConfigSet and set values when they are found
      for (gs::cnf::ConfigSet::cnf_data_map_type::const_iterator it = cs.get_config_map().begin();
           it != cs.get_config_map().end(); it++) {
        if (it->first == "msgconfig_name") {
          config_file_specified = true;
        }
        if (it->first == "msgconfig_starttime_en") m_msg_config[fileno].get_MessageConfiguration().msgconfig_starttime_en.setString(it->second);
        else if (it->first == "msgconfig_starttime") m_msg_config[fileno].get_MessageConfiguration().msgconfig_starttime.setString(it->second);
        else if (it->first == "msgconfig_endtime") m_msg_config[fileno].get_MessageConfiguration().msgconfig_endtime.setString(it->second);
        else if (it->first == "msgconfig_dbglvl") m_msg_config[fileno].get_MessageConfiguration().msgconfig_dbglvl.setString(it->second);
        else if (it->first == "msgconfig_info_en") m_msg_config[fileno].get_MessageConfiguration().msgconfig_info_en.setString(it->second);
        else if (it->first == "msgconfig_warn_en") m_msg_config[fileno].get_MessageConfiguration().msgconfig_warn_en.setString(it->second);
        else if (it->first == "msgconfig_error_en") m_msg_config[fileno].get_MessageConfiguration().msgconfig_error_en.setString(it->second);
        else if (it->first == "msgconfig_fatal_en") m_msg_config[fileno].get_MessageConfiguration().msgconfig_fatal_en.setString(it->second);
        else if (it->first == "msgconfig_printtime") m_msg_config[fileno].get_MessageConfiguration().msgconfig_printtime.setString(it->second);
        else if (it->first == "msgconfig_printname") m_msg_config[fileno].get_MessageConfiguration().msgconfig_printname.setString(it->second);
        else if (it->first == "msgconfig_printfile") m_msg_config[fileno].get_MessageConfiguration().msgconfig_printfile.setString(it->second);
        else if (it->first == "msgconfig_printlevel") m_msg_config[fileno].get_MessageConfiguration().msgconfig_printlevel.setString(it->second);
        else if (it->first == "msgconfig_module_id") m_msg_config[fileno].get_MessageConfiguration().msgconfig_module_id.setString(it->second); // TODO: this does not yet work!
      }
      
      REPMSG_DUMP_N("MessageStreamer", "new file config "<< tmp_msgconfig_name<<": update_stream_enables");
      update_stream_enables();
      
      return fileno;
      
    }
    
            
    // ///////////////////////  user functions (called by static functions)  /////////////////////// //
    
    
    /// Giving an user access to the output plugin used for message output to allow adding further parameters
    /**
     * description see static function get_output_plugin
     */
    gs::av::File_OutputPlugin* get_output_plugin_int(unsigned int fileno) {
      return m_msg_config[fileno].get_MessageConfiguration().output_plugin;
    }
    
            
    // ///////////////////  internal functions  ////////////////////// //

    /// Updates all registered report streams (1/ set stream type and OutputPlugin, 2/ enables/disables them)
    /**
     * Called each time the configuration changes or a new stream is registered
     */
    void update_stream_enables() {
      //REPMSG_DUMP_N("MessageStreamer", "update_stream_enables");
      // Search in the configuration and check for each stream if it will be needed (enable) or not (disable)
      std::vector< std::pair<report_stream_if*, bool> >::iterator iter;   

      // /////////////////////////////////////
      //  2/ first reset all tmp enables
      for( iter = m_report_streams.begin(); iter != m_report_streams.end(); iter++ ) {
        iter->second = false;
      }

      // now check and write to tmp enables
      for (unsigned int fileno = 0; fileno < m_msg_config.size(); fileno++) {
        REPMSG_DUMP_N("MessageStreamer", "  check config #" << fileno << ": "<< m_msg_config[fileno].get_MessageConfiguration().msgconfig_name);

        if (m_msg_config[fileno].get_MessageConfiguration().msgconfig_name.getValue().length() == 0)
          SC_REPORT_WARNING("MessageStreamer", "MessageStreamer contains configuration with empty name!");
        // /////////////////////////////////////
        //  1/ Set stream type and OutputPlugin
        if (m_msg_config[fileno].get_MessageConfiguration().stream_type == default_stream_type) {
          REPMSG_DUMP_N("MessageStreamer", "  set stream type and OutputPlugin for config #" << fileno << ": "<<std::endl << m_msg_config[fileno].get_MessageConfiguration().to_string());
          if      (m_msg_config[fileno].get_MessageConfiguration().msgconfig_name == "stdout")            m_msg_config[fileno].get_MessageConfiguration().stream_type = stdout_stream;
          else if (m_msg_config[fileno].get_MessageConfiguration().msgconfig_name == "stderr")            m_msg_config[fileno].get_MessageConfiguration().stream_type = stderr_stream;
          else if (m_msg_config[fileno].get_MessageConfiguration().msgconfig_name == "SC_REPORT_INFO")    m_msg_config[fileno].get_MessageConfiguration().stream_type = sc_report_info_stream;
          else if (m_msg_config[fileno].get_MessageConfiguration().msgconfig_name == "SC_REPORT_WARNING") m_msg_config[fileno].get_MessageConfiguration().stream_type = sc_report_warning_stream;
          else if (m_msg_config[fileno].get_MessageConfiguration().msgconfig_name == "SC_REPORT_ERROR")   m_msg_config[fileno].get_MessageConfiguration().stream_type = sc_report_error_stream;
          else if (m_msg_config[fileno].get_MessageConfiguration().msgconfig_name == "SC_REPORT_FATAL")   m_msg_config[fileno].get_MessageConfiguration().stream_type = sc_report_fatal_stream;
          else {
            m_msg_config[fileno].get_MessageConfiguration().stream_type = real_file;
            assert(m_msg_config[fileno].get_MessageConfiguration().output_plugin == NULL);
            REPMSG_DUMP_N("MessageStreamer", "  Create new OutputPlugin");
            // create output plugin
            gs::av::OutputPlugin_if* op = m_analysisAPI->create_OutputPlugin(gs::av::TXT_FILE_OUT, m_msg_config[fileno].get_MessageConfiguration().msgconfig_name);
            gs::av::File_OutputPlugin* fop = dynamic_cast<gs::av::File_OutputPlugin*>(op);
            assert(op != NULL);
            m_msg_config[fileno].get_MessageConfiguration().output_plugin = fop;
          }
        }

        // /////////////////////////////////////
        //  2/ Enable report stream is needed
        //REPMSG_DUMP_N("MessageStreamer", "  check file #" << fileno << ": " << m_msg_config.msgconfig_name[fileno]);
        for( iter = m_report_streams.begin(); iter != m_report_streams.end(); iter++ ) {
          if (check_config(fileno, iter->first->get_parent_name(), iter->first->get_id(), iter->first->get_message_level(), true)) {
            //REPMSG_DUMP_N("MessageStreamer", "    true for stream " << iter->first->name());
            iter->second = true;
          }
        }
      }
      // /////////////////////////////////////
      //  2/ apply all enables
      for( iter = m_report_streams.begin(); iter != m_report_streams.end(); iter++ ) {
        iter->first->enable(iter->second);
      }
    }
            
    /// Checks if this message has to go to the file
    /**
     * @param ignore_time_check is used by the update_stream_enables function which needs to check regardless of the time
     */
    bool check_config(unsigned int fileno, const char* module_name,
                      const char* streamer_id, const combined_message_level level,
                      bool ignore_time_check = false) {
      //std::cout << "check_config(fileno="<<fileno<<", module_name="<<module_name<<", "
      //              << "streamer_id="<<streamer_id<<", combined_message_level level="<<level<<", "
      //              << "bool ignore_time_check="<<ignore_time_check<<")" << std::endl;
      
      // if time control disabled or within time
      if (
          !m_msg_config[fileno].get_MessageConfiguration().msgconfig_starttime_en // if time control disabled
          || ( m_msg_config[fileno].get_MessageConfiguration().msgconfig_starttime.getValue().to_double() <= sc_core::sc_time_stamp().to_double()
              && m_msg_config[fileno].get_MessageConfiguration().msgconfig_endtime.getValue().to_double() >= sc_core::sc_time_stamp().to_double() )
          || ignore_time_check
          ) 
      {
        std::string mod_id(module_name); if (mod_id.length() > 0) mod_id += SC_NAME_DELIMITER; mod_id += streamer_id;
        // if (msgconfig_module_id contains value(s))
        //   if (NOT this module or mod_id is in it)
        //     return false;
        if (!m_msg_config[fileno].get_MessageConfiguration().msgconfig_module_id.getValue().empty()) {
          // Check if module_name or mod_id is in the vector
          std::vector<std::string>::const_iterator it;
          for (it = m_msg_config[fileno].get_MessageConfiguration().msgconfig_module_id.getValue().begin();
               it != m_msg_config[fileno].get_MessageConfiguration().msgconfig_module_id.getValue().end(); it++) {
            if ((*it).compare(module_name) == 0
                || (*it).compare(mod_id) == 0)
              break;
          }
          // if the for ran through without finding mod or id, return false
          if (it == m_msg_config[fileno].get_MessageConfiguration().msgconfig_module_id.getValue().end())
            return false;
        }
        switch (level) {
          case int_no_message_level:
            assert(false); exit(1);
          case int_sys_msg_INFO: // =10 if sys msg
            if (m_msg_config[fileno].get_MessageConfiguration().msgconfig_info_en) {
              if (m_override_config.find(module_name) != m_override_config.end()) {
                if (!m_override_config[module_name].info_en )// check overriding per module configuration
                  return false;
              }
              if (m_override_config.find(mod_id) != m_override_config.end()) {
                if (!m_override_config[mod_id]     .info_en) // check overriding per ID configuration
                  return false;
              }
              return true;
            }
            break;
          case int_sys_msg_WARNING: // =11 if sys msg
            if (m_msg_config[fileno].get_MessageConfiguration().msgconfig_warn_en) {
              if (m_override_config.find(module_name) != m_override_config.end()) {
                if (!m_override_config[module_name].warning_en )// check overriding per module configuration
                  return false;
              }
              if (m_override_config.find(mod_id) != m_override_config.end()) {
                if (!m_override_config[mod_id]     .warning_en) // check overriding per ID configuration
                  return false;
              }
              return true;
            }
            break;
          case int_sys_msg_ERROR: // =12 if sys msg
            if (m_msg_config[fileno].get_MessageConfiguration().msgconfig_error_en) {
              if (m_override_config.find(module_name) != m_override_config.end()) {
                if (!m_override_config[module_name].error_en )// check overriding per module configuration
                  return false;
              }
              if (m_override_config.find(mod_id) != m_override_config.end()) {
                if (!m_override_config[mod_id]     .error_en) // check overriding per ID configuration
                  return false;
              }
              return true;
            }
            break;
          case int_sys_msg_FATAL: // =13 if sys msg
            if (m_msg_config[fileno].get_MessageConfiguration().msgconfig_fatal_en) {
              if (m_override_config.find(module_name) != m_override_config.end()) {
                if (!m_override_config[module_name].fatal_en )// check overriding per module configuration
                  return false;
              }
              if (m_override_config.find(mod_id) != m_override_config.end()) {
                if (!m_override_config[mod_id]     .fatal_en) // check overriding per ID configuration
                  return false;
              }
              return true;
            }
            break;
          default: // if debug message
            if ((level >= 0 && level <= 9) // if this is a debug message
                && m_msg_config[fileno].get_MessageConfiguration().msgconfig_dbglvl != int_no_message_level
                && m_msg_config[fileno].get_MessageConfiguration().msgconfig_dbglvl >= level) { // if this file is for this debug message
              if (m_override_config.find(module_name) != m_override_config.end()) {
                if (m_override_config[module_name].dbglvl < (debug_msg_level)level) return false;
              }
              return true;
            }
        } // end switch
      } // end if timing
      return false;
    }
              
  };
  
  
END_GS_REPORTMSG_NAMESPACE              
              
#endif
