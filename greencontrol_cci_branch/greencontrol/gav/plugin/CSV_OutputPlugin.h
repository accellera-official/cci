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

#ifndef __CSV_OutputPlugin_h__
#define __CSV_OutputPlugin_h__

// If to allow the adding of parameters after the header was written
// => results in unnamed columns for afterwards added parameters
//    and in rows of different length in the file!
#define ALLOW_ADDING_PARAMETERS_AFTER_HEADER_WRITTEN

// For standard-conform CSV-files this separator has to be a ','.
// For files to be imported by Excel ';' should be used.
#define SEPARATOR ";"

#include <time.h>
#include <stdio.h>
#include <map>
#include "OutputPlugin_base.h"
#include "gav_plugin.h"

namespace gs {
namespace av {
      
  GAV_REGISTER_PLUGIN(4, CSV_FILE_OUT, CSV_OutputPlugin)
 
  /// Output Plugin which observes GreenConfig gs_param parameters and prints out changes to a CSV file that can be used by Excel
  /**
   * Due to the structure of CSV files (first line (header) should contain all param names)
   * all parameters should be registered before writing the file! 
   * That's why this plugin will begin logging and writing the file on the automatic end_of_elaboration call.
   * Optionally it can be switched to begin logging and running immediately on first param adding. 
   * See OutputPlugin_base for details on how to handle and influence running behavior.
   *
   * The behavior how to react to newly added parameters after began writing can be
   * adapted with the <code>define ALLOW_ADDING_PARAMETERS_AFTER_HEADER_WRITTEN</code>.
   * If not defined the output plugin will refuse (ignore) all register requests 
   * for new parameters after having written the first value! If defined the output
   * plugin will add this new observed parameter to each following line. Accordingly
   * the row witdh may increase within the CSV file - which is imported by Excel without
   * problems. But the head of the table will <em>not</em> include the parameter name,
   * the last columns will remain unnamed!
   *
   * Pausing the plugin does NOT write the current time slot to the file because the
   * plugin may be started again during the same time slot.
   *
   * The separator character can be specified with the macro <code>define SEPARATOR ';'</code>.
   * For standard-conform CSV-files this separator has to be a ','
   * for files to be imported by Excel ';' should be used.
   *
   * <code>define ALLOW_ADDING_PARAMETERS_AFTER_HEADER_WRITTEN</code>
   * If to allow the adding of parameters after the header was written
   * => results in unnamed columns for afterwards added parameters
   *    and in rows of different length in the file!
   *
   * TODO Alternative: read in existing file and create new one containing the new 
   *   parameter.
   */
  class CSV_OutputPlugin 
  : public OutputPlugin_base {
        
  public:
    
    /// Constructor with filename
    /**
     * Checks and manipulates the file name. 
     * Does NOT create the file etc.
     *
     * @param file_name Filename of the output file.
     * @param ev_listn  Pointer to event listener that may be used by this.
     * @param start_running  If this output plugin (start_running=false:) begins to output on end_of_elaboreation (default) or (start_running=true) starts running immediately on first param adding
     */
    CSV_OutputPlugin(const char* file_name, event_listener<OutputPlugin_base> *ev_listn, bool start_running = false)
    : OutputPlugin_base(file_name, ev_listn, CSV_FILE_OUT, start_running),
      filename(file_name),
      started_once(false),
      current_time(sc_time(0,SC_NS)),
      current_delta(0),
      separator(SEPARATOR)
    {
      std::string filename_str = filename;
      // Check for correct file extension
      if (filename_str.find(".csv") != filename_str.length() - 4
          && filename_str.find(".CSV") != filename_str.length() - 4) {
        filename_str += ".csv";
        filename = filename_str.c_str();
        GAV_DUMP_N(name(), "Add file extension: "<<filename.c_str());
      }
    }

  protected:
    /// Init function called before first output, not called during construction!
    /**
     * Opens the file, prints time and date in it.
     */
    void init() {
      GAV_DUMP_N(name(), "Init CSV Output Plugin: Open file '"<<filename.c_str()<<"' to write observed parameters to.");
      current_time = sc_core::sc_time_stamp();
      current_delta = sc_core::sc_delta_count();
      // create time
      time_t rawtime;
      struct tm * timeinfo;
      time ( &rawtime );
      timeinfo = localtime ( &rawtime );
      // write time to file
      outp_file = fopen(filename.c_str(), "w+t");
      fprintf (outp_file, "Simulation time: %s\n", asctime (timeinfo) );
      // write table name
      fprintf (outp_file, "%s\n \n", filename.c_str());
    }      
  public:
    
    /// Destructor
    ~CSV_OutputPlugin() {
      if (is_used) {
        GAV_DUMP_N(name(), "Destructor, close file.");
        write_time_slot();
        if ( fclose(outp_file) == -1 )
          SC_REPORT_ERROR(name(), "Closing output file failed!");
      }
    }
    
    virtual bool observe(gs_param_base& par) {
#ifdef ALLOW_ADDING_PARAMETERS_AFTER_HEADER_WRITTEN
      p_vec.push_back(par.getName());
      OutputPlugin_base::observe(par);
      if (started_once) {
        SC_REPORT_INFO(name(), "Added parameter will be in an unnamed column because the header was already written. Avoid adding new parameters after begin of writing the file!");
      }
#else
      if (!started_once) {
        p_vec.push_back(par.getName());
        OutputPlugin_base::observe(par);
      }
      else {
        SC_REPORT_WARNING(name(), "Added parameter will be ignored because the header was already written. Do not add new parameters after begin of writing the file!");
        return false;
      }
#endif
      return true;
    }
    
    /// Callback function
    void config_callback(gs_param_base &par) {

      // check for destructing parameters: remove them from callback list if removed
      if (par.is_destructing()) {
        remove(par);
      }
      
      // only perform output action when not paused (or stopped)
      else if (is_running) {
        
        if (!started_once
            || sc_core::sc_time_stamp() != current_time || sc_core::sc_delta_count() != current_delta )
          write_time_slot();
        
        // Store current value into current_storage
        std::pair<std::map<std::string, std::string>::iterator, bool> mp
          = current_line_values.insert(make_pair(par.getName(), par.getString()));
        if (!mp.second)
          (mp.first)->second = par.getString();
        
      }
    }

  protected:
    
    /// Writes an old time slot
    void write_time_slot() {
      // Print header of table on first callback!
      if (!started_once) {
        fprintf (outp_file, "\"time /delta\"%s", separator);
        for (std::vector<std::string>::iterator iter = p_vec.begin();
             iter != p_vec.end(); iter++)
        {
          fprintf (outp_file, "\"%s\"%s", (*iter).c_str(), separator);
        }
        fprintf (outp_file, "\n");
        started_once = true;
        current_delta = (unsigned)sc_core::sc_delta_count();
        current_time = sc_core::sc_time_stamp();
        current_line_values.clear();
      }
      else {
        // write time field
        fprintf(outp_file, "\"%s /%d\"", current_time.to_string().c_str(), (unsigned)current_delta);
        // write field
        for (std::vector<std::string>::iterator iter = p_vec.begin();
             iter != p_vec.end(); iter++)
        {
          fprintf(outp_file, separator);
          std::map<std::string,std::string>::iterator iter_m = current_line_values.find(*iter);
          if( iter_m != current_line_values.end() ) {
            fprintf(outp_file, "\"%s\"", iter_m->second.c_str());
          }
        }
        // close record
        fprintf(outp_file, "\n");
        current_delta = (unsigned)sc_core::sc_delta_count();
        current_time = sc_core::sc_time_stamp();
        current_line_values.clear();
      }
    }
    
    /// Output file pointer
    FILE *outp_file;
    
    /// Filename
    std::string filename;
    
    /// Status if already started output (to ignore new added parameters)
    bool started_once;

    /// Vector of names of all observed parameters (order of apperance in the file)
    std::vector<std::string> p_vec;
    
    /// Current time where the values in <code>current_line_values</code> belong to
    sc_time current_time;
    /// Current delta cycle
    unsigned current_delta;
    /// vector with the current line values
    std::map<std::string, std::string> current_line_values;
    
    const char* separator;
  };
  
  
} // end namespace av
} // end namespace gs

#endif
