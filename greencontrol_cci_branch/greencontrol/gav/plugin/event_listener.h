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
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


#ifndef __event_listener_h__
#define __event_listener_h__

#include <map>
#include <sstream>
#include <algorithm>

#ifndef SC_INCLUDE_DYNAMIC_PROCESSES
# error You need to define SC_INCLUDE_DYNAMIC_PROCESSES (e.g. as a compiler flag) to use GAV event listeners.
#endif

namespace gs {
namespace av {

  /// Event listener sc_core::sc_object which manages event listenings for non-sc_modules of the template type
  /**
   * The event_lister creates one dynamic process per event
   * given to it by an initiator. The method is sensitive to that event
   * and on notification of that event the listener calls back the
   * initiator.
   */ 
  template<typename Initiator_t>
  class event_listener
  : public sc_core::sc_object
  {

  protected:
    
    /// Typedef for the member function pointer (<code>void function()</code>).
    typedef void (Initiator_t::*event_listener_callb_func_ptr)();

    /// Internal struct to store the callback pointer
    /*struct callType {
      sc_process_handle* proc_h;
    };*/
  
    /// Type of the internal map
    //typedef std::map<int, callType> callMapType;

  public:
    
    /// Event listener constructor
    event_listener(const char* name)
    : sc_core::sc_object(name),
      id_cnt(0) 
    {
    }
    
    /// Creates a dynamic process which is sensitive to the given event and calls back to the given function when notified
    /**
     * @param obj      Object where the function should be called back (this-pointer of initiator)
     * @param cb_func  Function pointer to the member function of the initiator that should be called back
     * @param ev       Event for which the dynamic process should be sensitive to
     * @return      ID of the created process. May be used in future to delete the process.
     */
    int create_event_listener(Initiator_t *obj, event_listener_callb_func_ptr cb_func, sc_event &ev) {

      std::stringstream ssSpProcName;
      std::string ns = name();
      std::replace(ns.begin(), ns.end(), SC_NAME_DELIMITER, '_'); // TODO SC_NAME_DELIMITER must not be '-' in this case
      ssSpProcName << ns << "_eventListenerProcNo_" << id_cnt;

      sc_core::sc_spawn_options opt;
      opt.spawn_method();
      opt.dont_initialize();
      opt.set_sensitivity(&ev);
      //c.proc_h = &
      sc_core::sc_spawn(/*void*/sc_bind(&event_listener::spawned_thread, this, id_cnt, obj, cb_func), ssSpProcName.str().c_str(), &opt); 

      /*callType c;
      m_cbMap.insert( std::make_pair(id_cnt, c) );*/

      return id_cnt++;
    }
    
    /*bool remove_event_listener(int listerner_id) {
      typename callMapType::iterator iter = m_cbMap.find(listerner_id);
      if( iter != m_cbMap.end() ) {
        //sc_process_handle *ph = &((iter->second).proc_h);
        //delete ph;
        //m_cbMap.erase(iter);
        return true;
      } 
      return false;
    }*/
    
    /// Function to be used as dynamic process. Calls the given function when event is notified.
    /**
     * @param listener_id  The processe's ID. Used to look up the function in the map
     * @param obj          Object in which the function should be called.
     * @param cb_func      Member function pointer that should be called.
     */
    void spawned_thread(unsigned int listener_id, Initiator_t *obj, event_listener_callb_func_ptr cb_func) {
      GAV_DUMP_N(name(), "spawned method called (id="<<listener_id<<")");
      (obj->*cb_func)(); // call function
    }

  protected:
    /// Counts the used ids
    int id_cnt;
    
    /// Map conaining all function pointers
    //callMapType m_cbMap;
  };

} // end namespace av
} // end namespace gs

#endif
