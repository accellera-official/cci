//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2010 : GreenSocs Ltd
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
//
// This is similar to the TLM-2.0 mechanism

#ifndef __GC_TRANSACTION_EXTENSION_H__
#define __GC_TRANSACTION_EXTENSION_H__


namespace gs {
namespace ctr {
  

/// Helper function managing the maximum number of transaction extensions existing
/**
 * This function counts the global maximum number of gc_transaction_extension
 * available in the system. This is incremented each time a new class
 * of gc_transaction_extension is defined. Hence it is changed during static 
 * initialization and can be used to size the extenstion array (vector)
 * within the ControlTransaction after static initialization.
 */
inline unsigned int max_num_gc_transaction_extensions(bool increment=false)
{
  static unsigned int max_num = 0;
  if (increment) ++max_num;
  return max_num;
}

/// Class to store extensions in the gc_transaction_extensions
/**
 * The base class log_if is optional but recommended to allow logging with the GC_Logger!
 */
class gc_transaction_extension_base
  : public gs::ctr::log_if 
{
protected:
  virtual ~gc_transaction_extension_base() {}
  static unsigned int register_extension()
  {
    return (max_num_gc_transaction_extensions(true) - 1);
  };
};

/// Base class to derive transaction extensions from to store them in the gc_transaction
/**
 * Derive your transaction extension class in the following way:                                                       <br>
 * \verbatim
 class my_extension
  : public gs::ctr::gc_transaction_extension<my_extension>
 {
    ...
 }
 \endverbatim
 *
 * This triggers proper transaction extension registration during C++ static
 * contruction time. my_extension::ID will hold the unique index in the
 * ControlTransaction::m_extensions array.
 */
template <typename T>
class gc_transaction_extension : public gc_transaction_extension_base
{
public:
  virtual ~gc_transaction_extension() {}
  const static unsigned int ID;
};

template <typename T>
const unsigned int gc_transaction_extension<T>::ID = gc_transaction_extension_base::register_extension();

  
} // end namespace
} // end namespace


#endif
