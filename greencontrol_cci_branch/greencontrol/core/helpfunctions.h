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

#ifndef __helpfunctions_h__
#define __helpfunctions_h__

#include <vector>
#include <string>
#include <sstream>
#include <ctype.h>

#include "gc_globals.h"

namespace gs {
namespace ctr {

using std::vector;

  
/// Prints out all items of a vector (with iterator)
template<class T>
static void show_vector(std::vector<T> vec) {
  std::cout << "vector: size="<< vec.size() << std::endl;
  typename std::vector<T>::iterator iter;
  for( iter = vec.begin(); iter != vec.end(); iter++ ) {
    std::cout << *iter << std::endl;
  }
}

/// Prints the two vectors, and prints in nice readable format
template<class T, class P>
static void show_vector_gc_trace(std::vector<T> vec1, std::vector<P> vec2) {
  typename std::vector<T>::iterator iter1;
  typename std::vector<P>::iterator iter2;
  iter2 = vec2.begin();
  for( iter1 = vec1.begin() ; iter1 != vec1.end(); iter1++) {
    std::cout << *iter1 << " [" << *iter2 << "] ";
    iter2++;
  }
  std::cout << std::endl;
}

/// Prints out <size> comma separated items of a vector (with [] operator) with size information
template<class T>
static void show_vector(const std::vector<T> vec, const unsigned int size) {
  std::cout << "  vector: size="<< vec.size()<<", show "<< size<<" items" << std::endl << "  ";
  assert(vec.size() >= size);
  typename std::vector<T>::iterator iter;
  for (unsigned int i = 0; i<size; i++) {
    std::cout << vec[i] << ", ";
  }
  std::cout << std::endl;
}

/// Prints out <size> (not separated) items of a vector (with [] operator) with size information
template<class T>
static void show_vector_nosep(const std::vector<T> vec, const unsigned int size) {
  std::cout << "  vector: size="<< vec.size()<<", show "<< size<<" items" << std::endl << "  ";
  assert(vec.size() >= size);
  show_pure_vector(vec, size);
  std::cout << std::endl;
}

/// Prints out <size> items of a vector (with [] operator)
template<class T>
static void show_pure_vector(const std::vector<T> vec, const unsigned int size) {
  assert(vec.size() >= size);
  typename vector<T>::iterator iter;
  for (unsigned int i = 0; i<size; i++) {
    std::cout << vec[i] ;
  }
}

/// Class which objects return chars when calling next() in the order: 012...890ABC...XYZabc...xyz
class letter_digit_iterator
{
public:
  char count;

  letter_digit_iterator()
  {
    count = 122;
  }

  char next() {
    count++;
    if (count == 123) count = 48;
    else if (count == 91) count = 97;
    else if (count == 58) count = 65;
    return count;
  }

  void reset() {
    count = 122;
  }

};

/// Returns if there are sc_objects in the simulation tree of type template class T.
/**
 * @param node  Call with NULL to start search at root.
 * @param but_not_childs_of  But do not search underneath this object!
 * @return      If there is an sc_object of type T.
 */
template <class T>
static bool search_for_objects(sc_core::sc_object *node, const sc_core::sc_object* but_not_childs_of)
{
  // read out all childs and scan them
  {
    const std::vector<sc_core::sc_object *> *childs_stack = NULL;
    if (node) {
      // get child vector
#if SYSTEMC_API == 210
      // SystemC2.1 cannot get childs of an object, so only modules!
      sc_core::sc_module* node_as_module = dynamic_cast< sc_core::sc_module* >(node);
      if (node_as_module) {
        //sc_assert(node_as_module);
        if (node_as_module)
          childs_stack = &(node_as_module->get_child_objects());
      }
#else
      childs_stack = &node->get_child_objects();
#endif
    } else { // If NULL, then top level
      // get child vector of sim context
#if SYSTEMC_API == 210
      sc_core::sc_simcontext *sim; // deprecated with SystemC-2.2
      sim = sc_core::sc_get_curr_simcontext(); // deprecated with SystemC-2.2
      childs_stack = &(sim->get_child_objects());
#else
      childs_stack = &sc_core::sc_get_top_level_objects();
#endif
    }
    // If node can be casted to T, return true
    T *type = dynamic_cast<T*>(node);
    if (type) {
      return true;
    }
    
    // go through childs
    if (childs_stack) {
      for (unsigned int i = 0; i < childs_stack->size(); i++) {
        sc_core::sc_object *chnode = childs_stack->at(i);
        if (chnode != but_not_childs_of) { // do not search within but_not_childs_of
          if (search_for_objects<T>(chnode, but_not_childs_of))
            return true; // break search if one found
        }
      }
    }
  }
  return false;
}

  
  /// Converts each string to a SC name compatible string
  /**
   * Replaces all characters uneuqual
   *  - lower case letter a-z
   *  - upper case letters A-Z
   *  - decimal digits 0-9
   *  - underscore character _
   * with the underscore character _.
   *
   * @param name  Name to convert.
   * @return Converted SC compatible name.
   */
  inline std::string convert_to_sc_name(std::string name) {
    std::stringstream ssret;
    std::string::iterator my_iter;
    const std::string match = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for(my_iter = name.begin(); my_iter != name.end(); my_iter++)
    {
      size_t found = match.find_first_of(*my_iter);
      //if (isalnum(*my_iter) == 1)
      if (found != std::string::npos)
        ssret << *my_iter;
      else 
        ssret << "_";
    }
    return ssret.str();
  }
  
/*
// ////// Trim functions for std::string

#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
//#include <algorithm>
//#include <boost/algorithm/string.hpp>
     
/// Removes spaces at the front of a string
//static std::string TrimLeft(const std::string s)
//{
//  return std::string(std::find_if(s.begin(), s.end(), std::isgraph), s.end());
//}

/// Removes spaces at the back of a string
//static std::string TrimRight(const std::string s)
//{
//  return std::string(s.begin(), std::find_if(s.rbegin(), s.rend(), isgraph).base());
//}

/// Removes spaces at the front and back of a string
//static std::string TrimString(const std::string s)
//{
  //return boost::algorithm::trim_copy(s);
  //return TrimLeft(TrimRight(s));
//}
*/

} // end namespace ctr
} // end namespace gs

#endif
