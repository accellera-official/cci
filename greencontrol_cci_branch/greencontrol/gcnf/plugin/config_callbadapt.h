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

// doxygen comments

#ifndef __GC_CALLBADAPT_H__
#define __GC_CALLBADAPT_H__

#include "greencontrol/core/gc_globals.h"


namespace gs {
namespace cnf {


/// Deprecated! Adapter base class which can be used to call a arbitrary callback function.
/**
 * Base class for CallbAdapt to allow access by the back calling API without
 * knowing the template parameter.
 */
class CallbAdapt_b
{
public:
  virtual ~CallbAdapt_b() { }
  /// Virtual call method to make the call in the template specialized class.
  virtual void call(const std::string parname, const std::string value) = 0;
};


/// Deprecated! Template specialized adapter class which can be used to call a arbitrary callback function.
/**
 * This class saves an object pointer and a member function pointer to the object's
 * callback function. The call method is called by the virtual call method of the
 * base class when the API calls it.
 */
template<class T>
class CallbAdapt
: public CallbAdapt_b
{
  /// Typedef for the member function pointer.
  /**
   * Callback functions must have the signature: void method_name(const std::string, const std::string)
   */
  typedef void (T::*callb_func_ptr)(const std::string, const std::string);

public:

  /// Constructor with pointers to be saved in this object.
  /**
   * @param ob    Pointer to the object where the callback method should be called.
   * @param _ptr  Member function pointer to the callback method (must match signature CallbAdapt::callb_func_ptr).
   */
  CallbAdapt(T *ob, callb_func_ptr _ptr) {
    obj = ob;
    ptr = _ptr;
    // not deprecated for new param observers! DEPRECATED_WARNING("Callback", "DEPRECATED: Callbacks that are registered in the GCnf_Api getting strings (parameter name and value) are deprecated due to performance reasons! Register your callbacks in the gs_params instead with getting a gs_param_base reference instead of strings.");
  }

  /// Pointer to the object where the callback method should be called.
  T *obj;
  /// Member function pointer to the callback method (must match signature CallbAdapt::callb_func_ptr).
  callb_func_ptr ptr;

  /// Makes the callback, called by the base class CallbAdapt_b.
  void call(const std::string parname, const std::string value) {
    (obj->*ptr)(parname, value);
  }
};


} // end namespace cnf
} // end namespace gs

#endif
