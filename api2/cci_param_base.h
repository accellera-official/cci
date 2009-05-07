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


#ifndef __CCI_PARAM_BASE_H__
#define __CCI_PARAM_BASE_H__


#include <string>
#include <iostream>
#include <map>
#include <set>

#include <boost/shared_ptr.hpp>
#include "cci_datatypes.h"


namespace cci {

  template <typename T> 
  class cci_param;

  /// Base class for all cci_param template specialisations.
  /**
   * Manages the name of the parameter 
   *
   * Requires set and get with the string representation of the value.
   */
  class cci_param_base
  {

  public:

    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Construction / Destruction   ///////////////////// //

    /// Constructor with (local or hierarchical) name
    /**
     * - Sets the hierarchical parameter name (default name if name is empty).
     *   (If the parent_array is != NULL: 
     *            hierarchical name is the parent's name + local name)
     * - Sets the API pointer.
     * - Explicit constructor to avoid implicit construction of parameters.
     *
     * @param n  The local (or full hierarchical) parameter name (local: should not but may include points) 
     *           (local: unique inside a module, hierarchical: unique in the system).
     *           May be empty: name will be chosen automatically.
     * @param register_at_db   If this parameter should be registered at the database, 
     *                         default true, 
     *                         only the database itself may use false when creating 
     *                         parameters while adding them.
     * @param force_top_level_name If the given name n should be a top-level name (then no prefeix is attached to the name),
     *                             default false,
     *                             be carefull in using this.
     */
    explicit cci_param_base(const std::string& n, const bool register_at_db = true,
                            const bool force_top_level_name = false);
    
    /// Destructor.
    virtual ~cci_param_base();
    

    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Set and Get with Value   ///////////////////////// //

    
    /// Get this parameter's value converted to the type.
    /**
     * This function can be called if the user knows the type.
     *
     * The string value is converted to the user-chosen template type
     * by using the cci_param template specialization.
     *
     * This function does only work for types of cci_param<T>, not for 
     * any kind of cci_param_base, e.g. parameter arrays.
     *
     * @param value    The value of the parameter will be written to this parameter
     * @return         If the convertion was successfull.
     */
    virtual template<class T>
    const bool get_value(T& value) const = 0;
    
    /// Get a parameter's value. Independent of the implicit or explicit status.
    /**
     * Better use getValue(value) to avoid copying of the value.
     *
     * This function can be called if the user knows the type.
     *
     * The string value is converted to the user-chosen template type
     * by using the cci_param template specialization.
     *
     * @return         Value of the parameter, converted to the user-chosen type.
     */
    virtual template<class T>
    const T get_value() const = 0;
    
    /// Convenience function to get a parameter pointer of the cci_param<T> type defined by the caller (does a dynamic_cast).
    /**
     * @return  Pointer to the parameter object (NULL if not existing or wrong type).
     */ 
    virtual template<class T>
    cci_param<T>* get_cci_param() = 0;
    
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////   Set and Get with String Representation   ///////////////// //
    
    
    /// Sets the value of this parameter given by a string.
    /** 
     * @param str the new value represented as a string.
     * @return If setting was successful.
     */
    virtual bool set_string(const std::string& str) = 0;
    
    /// Get the string representation of this parameter's value.
    /**
     * @return  The value of this parameter represented as a string.
     */
    virtual const std::string& get_string() const = 0;
    
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Parameter Data Type   //////////////////////////// //

    
    /// Returns the type identifying enum of the type this parameter stores
    virtual const Param_type get_type() const = 0;
    
    /// Returns the string representation of the type this parameter stores
    virtual const std::string get_type_string() const = 0;
    
    
    // //////////////////////////////////////////////////////////////////// //
    // /////////////////////   Miscellaneous   //////////////////////////// //

    
    /// Get the name of this parameter.
    /**
     * @return   Name of the parameter.
     */
    virtual const std::string& get_name() const = 0;
   
    /// Returns the destrcution flag status: if this parameter is to be destroyed (for callbacks)
    virtual const bool is_destructing() const = 0;
    
  };

} // namespace cci

#include "cci_param_base.hpp"

#endif
