//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Marcus Bartholomeu <bartho@greensocs.com>
//     GreenSocs
//     http://www.greensocs.com
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

#ifndef __VCD_OutputPlugin_h__
#define __VCD_OutputPlugin_h__

#include <time.h>
#include <stdio.h>
#include <map>
#include "OutputPlugin_base.h"
#include "gav_plugin.h"

namespace gs {
namespace av {
      
  GAV_REGISTER_PLUGIN(6, VCD_FILE_OUT, VCD_OutputPlugin)
 
  /// Output Plugin which observes GreenConfig gs_param parameters and prints out changes to a VCD file
  /**
   * Due to the structure of VCD files all parameters should be registered before
   * writing the file! The plugin will begin writing the file on the first parameter change.
   *
   * Pausing the plugin does NOT write the current time slot to the file because the
   * plugin may be started again during the same time slot.
   */
  class VCD_OutputPlugin 
  : public OutputPlugin_base {
        
  public:
    
    /// Constructor with filename
    /**
     * Checks and manipulates the file name. 
     * Does NOT create the file etc.
     *
     * @param file_name Filename of the output file.
     * @param ev_listn  Pointer to event listener that may be used by this.
     */
    VCD_OutputPlugin(const char* file_name, event_listener<OutputPlugin_base> *ev_listn)
    : OutputPlugin_base(file_name, ev_listn, VCD_FILE_OUT, false),
      filename(file_name),
      started_once(false),
      current_time(sc_time(0,SC_NS)),
      current_delta(0)
    {
      std::string filename_str = filename;
      // Check for correct file extension
      if (filename_str.find(".vcd") != filename_str.length() - 4
          && filename_str.find(".VCD") != filename_str.length() - 4) {
        filename_str += ".vcd";
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
      GAV_DUMP_N(name(), "Init VCD Output Plugin: Open file '"<<filename.c_str()<<"' to write observed parameters to.");
      current_time = sc_core::sc_time_stamp();
      current_delta = sc_core::sc_delta_count();
      // create time
      time_t rawtime;
      struct tm * timeinfo;
      time ( &rawtime );
      timeinfo = localtime ( &rawtime );
      // write time to file
      outp_file = fopen(filename.c_str(), "w+t");
      fprintf (outp_file, "$date\n   %s$end\n", asctime (timeinfo) );   // note: the date string has a line break already
      // write version
      fprintf (outp_file, "$version\n   GreenSocs Analysis VCD output plugin version 1.\n$end\n");
      // write timescale
      fprintf (outp_file, "$timescale %s $end\n", sc_get_time_resolution().to_string().c_str());
    }      
  public:
    
    /// Destructor
    ~VCD_OutputPlugin() {
      if (OutputPlugin_base::is_used) {
        GAV_DUMP_N(name(), "Destructor, close VCD file.");
        write_time_slot();
        if ( fclose(outp_file) == -1 )
          SC_REPORT_ERROR(name(), "Closing output file failed!");
      }
    }
    
    virtual bool observe(gs_param_base& par) {
      if (!started_once) {
        // store name
        p_vec.push_back(par.getName());
        // find size
        unsigned size=3;
        std::string warn_msg;
        switch (par.getType()) {

        case gs::cnf::PARTYPE_INT: 
        case gs::cnf::PARTYPE_UINT:
          size = 32;
          break;

        case gs::cnf::PARTYPE_BOOL:
        case gs::cnf::PARTYPE_SC_BIT:
          size = 1;
          break;

        case gs::cnf::PARTYPE_SC_INT_BASE:
        case gs::cnf::PARTYPE_SC_INT:
        case gs::cnf::PARTYPE_SC_UINT_BASE:
        case gs::cnf::PARTYPE_SC_UINT:

        case gs::cnf::PARTYPE_LONGLONG:
        case gs::cnf::PARTYPE_ULONGLONG:
          size = 64;
          break;

        case gs::cnf::PARTYPE_CHAR:
        case gs::cnf::PARTYPE_UCHAR:
        case gs::cnf::PARTYPE_SIGNED_CHAR:
          size = 8;
          break;

        case gs::cnf::PARTYPE_USHORT:
          size = 16;
          break;

        case gs::cnf::PARTYPE_SC_SIGNED:
        case gs::cnf::PARTYPE_SC_UNSIGNED:
        case gs::cnf::PARTYPE_SC_BIGINT:
        case gs::cnf::PARTYPE_SC_BIGUINT:

        case gs::cnf::PARTYPE_STRING:
          size = 512; // max 64 bytes
          break;

        case gs::cnf::PARTYPE_SC_LOGIC:
          size = 1;
          break;

        // Unhandled
        case gs::cnf::PARTYPE_DOUBLE:
        case gs::cnf::PARTYPE_FLOAT:
        case gs::cnf::PARTYPE_SC_TIME:
        case gs::cnf::PARTYPE_SC_ATTRIBUTE:
        case gs::cnf::PARTYPE_SC_EVENT:
        case gs::cnf::PARTYPE_SMPL_ARRAY:
        case gs::cnf::PARTYPE_EXT_ARRAY:

        default:
          warn_msg = "Unable to use this parameter. No fitting datatype: ";
          warn_msg+= par.getName();
          warn_msg+= " with type ";
          warn_msg+= par.getTypeString();
          SC_REPORT_WARNING("VCD_OutputPlugin", warn_msg.c_str());
          break;
        }
        // store size
        p_vec_size.push_back(size);
        // store type
        p_vec_type.push_back(par.getType());
        // observe
        OutputPlugin_base::observe(par);
      }
      else {
        SC_REPORT_WARNING(name(), "Added parameter will be ignored because the header was already written. Do not add new parameters after begin of writing the file!");
        return false;
      }
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
        
        if (!started_once || sc_core::sc_time_stamp() != current_time )
          write_time_slot();
        
        // Store current value into current_storage
        current_line_values[par.getName()] = par.getString();
      }
    }

  protected:
    
    /// Writes an old time slot
    void write_time_slot() {
      // Print header of table on first callback!
      if (!started_once) {

        // write scope
        // TODO: check how this scope section works
        //fprintf (outp_file, "$scope module logic $end\n");

        // write each parameter
        for (unsigned i=0; i<p_vec.size(); i++)
        {
          fprintf (outp_file, "$var wire %d %c %s $end\n", p_vec_size[i], i+33, p_vec[i].c_str());
        }

        // write until the end of header
        //fprintf (outp_file, "$upscope $end\n");  // check scope above
        fprintf (outp_file, "$enddefinitions $end\n");

        // TODO: should write the initial parameter values here, using a $dumpvars section

        started_once = true;
      }
      else {
        // write time field
        fprintf(outp_file, "#%lld\n", current_time.value());
        // write fields
        for (unsigned i=0; i<p_vec.size(); i++)
        {
          std::map<std::string,std::string>::iterator iter_m = current_line_values.find(p_vec[i]);
          if( iter_m != current_line_values.end() ) {

            switch (p_vec_type[i]) {

            // treat string
            case gs::cnf::PARTYPE_STRING: {
              fprintf(outp_file, "b");
              for (int j=0; j<64 && iter_m->second.c_str()[j]; j++) {
                unsigned num = (unsigned) iter_m->second.c_str()[j];
                for (int k=7; k>=0; k--) {
                  fprintf(outp_file, "%u", (num << (31-k)) >> 31);
                }
              }
              // coded param name
              fprintf(outp_file, " %c\n", i+33);
            } break;

            // treat sc_logic
            case gs::cnf::PARTYPE_SC_LOGIC: {
              fprintf(outp_file, "%s%c\n", iter_m->second.c_str(), i+33);
            } break;

            // treat all others as numbers
            default: {
              // get value as unsigned
              uint64_t num=0;
              sscanf(iter_m->second.c_str(), "%llu", &num);
              // output single bit if num < 2
              if (num<2) fprintf(outp_file, "%llu", num);
              // output bit sequence otherwise
              else {
                fprintf(outp_file, "b");

                bool started=false;
                for (int j=63; j>=0; j--) {
                  bool bit = (num << (63-j)) >> 63;
                  if (bit) started=true;
                  if (started) fprintf(outp_file, "%u", bit);
                }
                // space from the identifier code
                fprintf(outp_file, " ");
              }

              // coded param name
              fprintf(outp_file, "%c\n", i+33);
            }
            } // switch
          }
        }
      }

      // store new time
      current_delta = (unsigned)sc_core::sc_delta_count();
      current_time = sc_core::sc_time_stamp();

      // clear params from current time group
      current_line_values.clear();
    }
    
    /// Output file pointer
    FILE *outp_file;
    
    /// Filename
    std::string filename;
    
    /// Status if already started output (to ignore new added parameters)
    bool started_once;

    /// Vector of names of all observed parameters (order of apperance in the file)
    std::vector<std::string> p_vec;
    /// Corresponding sizes in bits
    std::vector<unsigned> p_vec_size;
    /// Corresponding types
    std::vector<gs::cnf::Param_type> p_vec_type;
    
    /// Current time where the values in <code>current_line_values</code> belong to
    sc_time current_time;
    /// Current delta cycle
    unsigned current_delta;
    /// vector with the current line values
    std::map<std::string, std::string> current_line_values;
    
  };
  
  
} // end namespace av
} // end namespace gs

#endif
