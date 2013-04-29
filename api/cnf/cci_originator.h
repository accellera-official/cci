// LICENSETEXT
//
//   Copyright (C) 2011 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


#ifndef __CCI_ORIGINATOR_H__
#define __CCI_ORIGINATOR_H__

__CCI_OPEN_CONFIG_NAMESPACE__


/// Originator class which is used to handle the originator information independently from its data type (object pointer or string)
/**
 * Can be used to set and handle originator information in a uniform way 
 * independently from its data type.
 * 
 * Use the appropriate setter (setting the object is preferred!).
 * Use the appropriate getter: 
 * - Use get object to make use of value the sc_object brings but check for NULL needs to be done
 * - Use get string to get the originator name, in the case an object had been set, its name will be returned. 
 *   No need to differentiate the between the underlying data type.
 *
 * Static setter function shall be used by the CCI implementation to update the originator.
 *
 * Static getter function can be used by the end user to get the current originator.
 *
 * Guidelines for CCI implementations: 
 * - Use the call stack for stacking originator information.
 *   Use the returned old originator to back it up and re-apply afterwards.
 * - It shall be unique/unambiguously to identify an originator using its name!
 *
 */
class cci_originator {

public:

  /// (Static) global setter function for updating the current Golbal Originator Pointer
  /**
   * @param current_originator  Pointer to the current Golbal Originator Pointer, might be NULL if there is no originator information
   * @return The old originator (NULL if there was none before)
   */
  static const cci_originator* set_global_originator(const cci_originator* current_originator) {
    const cci_originator* old_originator = handle_current_originator();
    handle_current_originator(current_originator, true);
    return old_originator;
  }
  
  /// (Static) global getter function for getting the current Golbal Originator Pointer
  /**
   * @param The current Golbal Originator Pointer (might be NULL if there is none)
   */
  static const cci_originator* get_global_originator() {
    return handle_current_originator();
  }
  
protected:
  /// Internal function storing the originator
  /**
   * Can be used to get and set the originator:
   * @param new_originator Pointer to the new originator (optional), only applied if param set_new_originator is true.
   * @param set_new_originator If the param new_originator shall be stored as new originator (guarded by this bool to allow setting of NULL)
   */
  static const cci_originator* handle_current_originator(const cci_originator* new_originator = NULL, bool set_new_originator = false) {
    static const cci_originator* global_current_originator = NULL;
    if (set_new_originator) global_current_originator = new_originator;
    return global_current_originator;
  }
  
public:

  /// Constructor with another originator whose content will be copied
  cci_originator(const cci_originator& originator) 
  : m_originator_obj(originator.m_originator_obj)
  , m_originator_str(originator.m_originator_str) {    
  }
  
  /// Constructor with an sc_object originator
  cci_originator(sc_core::sc_object& originator) 
  : m_originator_obj(&originator)
  , m_originator_str() {    
  }

  /// Convenience constructor with an sc_module originator
  cci_originator(sc_core::sc_module& originator) 
  : m_originator_obj(static_cast<sc_core::sc_object*>(&originator))
  , m_originator_str() {    
  }
  
  /// Constructor with an originator name
  cci_originator(const std::string& originator) 
  : m_originator_obj(NULL)
  , m_originator_str(originator) {    
  }
  
  /// Set the originator using its object (preferred!)
  /**
   * @param originator  Originator object
   */
  //void set_originator_obj(sc_core::sc_object& originator) {
  //  m_originator_obj = &originator;
  //}

  /// Set the originator using its name (only use if no sc_object available!)
  /**
   * @param originator  Originator name
   */
  //void set_originator_name(const std::string& originator) {
  //  m_originator_obj = NULL;
  //  m_originator_str = originator;
  //}

  //void delete_originator() {
  //  m_originator_obj = NULL;
  //  m_originator_str = "";
  //}
  
  /// Returns a pointer to the current originator
  /**
   * Might return NULL if there is no current originator or the current originator 
   * is only given by name (use get_originator_str() instead).
   *
   * @return Originator object pointer or NULL
   */
  sc_core::sc_object* get() const {
    return m_originator_obj;
  }
  
  /// Returns the name of the current originator
  /**
   * Might return empty if there is no current originator.
   * Automatically uses the originator object name if the originator is given by object pointer.
   *
   * @return Originator name or NULL
   */
  const char* name() const {
    if (m_originator_obj) return m_originator_obj->name();
    return m_originator_str.c_str();
  }

protected:
  
  /// Pointer to the current originator object (priority compared to name m_originator_str)
  sc_core::sc_object* m_originator_obj;
  
  /// Name of the current originator (no relevance if m_originator_obj not NULL)
  std::string m_originator_str;

};


__CCI_CLOSE_CONFIG_NAMESPACE__


#endif

