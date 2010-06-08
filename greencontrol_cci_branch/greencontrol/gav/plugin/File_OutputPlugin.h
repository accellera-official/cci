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

#ifndef __File_OutputPlugin_h__
#define __File_OutputPlugin_h__

#include <time.h>
#include <stdio.h>
#include "OutputPlugin_base.h"
#include "gav_plugin.h"

namespace gs {
namespace av {
      
  GAV_REGISTER_PLUGIN(2, TXT_FILE_OUT, File_OutputPlugin)
 
  /// Output Plugin which observes GreenConfig gs_param parameters and prints out changes to a human-readable text file
  class File_OutputPlugin 
  : public OutputPlugin_base {
    
  public:
    
    /// Constructor with filename
    /**
     * Checks and manipulates the file name:
     * If the given file name has no extension (no dot in it)
     * the extension '.log' will be added automatically.
     *
     * Does NOT create the file etc.
     *
     * @param file_name  Filename of the output file.
     * @param ev_listn   Pointer to event listener that may be used by this.
     */
    File_OutputPlugin(const char* file_name, event_listener<OutputPlugin_base> *ev_listn)
    : OutputPlugin_base(file_name, ev_listn, TXT_FILE_OUT),
      filename(file_name)
    {
      // Check for any file extension
      std::string filename_str = filename;
      if (filename_str.find(".") == string::npos) {
        filename_str += ".log";
        filename = filename_str.c_str();
        GAV_DUMP_N(name(), "Add file extension: "<<filename.c_str());
      }
    }
    
  protected:
    /// Init function called on first usage, not called during construction!
    /**
     * Opens the file, prints time and date in it.
     */
    void init() {
      GAV_DUMP_N(name(), "Init TXT file Output Plugin: Open file '"<<filename.c_str()<<"' to write observed parameters to.");
      // create time
      time_t rawtime;
      struct tm * timeinfo;
      time ( &rawtime );
      timeinfo = localtime ( &rawtime );
      // write time to file
      outp_file = fopen(filename.c_str(), "w+t");
      fprintf (outp_file, "Simulation time: %s\n", asctime (timeinfo) );
    }
  public:
    
    /// Destructor
    ~File_OutputPlugin() {
      if (is_used) {
        if ( fclose(outp_file) == -1 )
          SC_REPORT_ERROR(name(), "Closing output file failed!");
      }
    }

    /// Callback function
    void config_callback(gs_param_base &par) {
      // check for destructing parameters: remove them from callback list if removed
      if (par.is_destructing()) {
        remove(par);
      }
      // only perform output action when not paused (or stopped)
      else if (is_running) {
        fprintf(outp_file, "@%s /%d: ", sc_core::sc_time_stamp().to_string().c_str(), (unsigned)sc_core::sc_delta_count());
        fprintf(outp_file, "%s = %s", par.getName().c_str(), par.getString().c_str()); 
        fprintf(outp_file, "\n");
      }
    }
    
    /// Special function to write a raw string to the file (needed by debug/system messages)
    void pure_output(const std::string& outp) {
      if (!is_used) { this->init(); is_used = true; } // needed because observe() may be not called
      if (is_running)
        fprintf(outp_file, "%s", outp.c_str());
    };
    
  protected:
    
    /// Output file pointer
    FILE *outp_file;
    
    /// Filename
    std::string filename;
    
  };
  
  
} // end namespace av
} // end namespace gs

#endif
