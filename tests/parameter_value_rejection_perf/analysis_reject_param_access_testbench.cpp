//   CCI
//
// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//   Mark Burton <mark@greensocs.com>
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
//   This program is free software.
// 
//   If you have no applicable agreement with GreenSocs Ltd, this software
//   is licensed to you, and you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   If you have a applicable agreement with GreenSocs Ltd, the terms of that
//   agreement prevail.
// 
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
//   02110-1301  USA 
// 
// ENDLICENSETEXT

//#define OPERATORS_THROWING

//#include <systemc>
#include <stdio.h>
#include <iostream>
#include <assert.h>

using namespace std;

bool global_bool_checker;

enum callback_return_type {
  return_nothing,
  return_value_change_rejected, // may only be used in pre_write callbacks
  return_read_rejected_because_private,
  return_other_error
};

template<class T>
class param {
  typedef T val_type;
  typedef param<T> my_type;
  
  val_type my_value;
  val_type dummy_value;
public:
  param() {
  }
  
  my_type& operator = (const val_type& v) 
#ifdef OPERATORS_THROWING
    throw (exception)
#endif
  { 
    callback_return_type result = set(v);
#ifdef OPERATORS_THROWING
    if (result != return_nothing && !global_bool_checker) // this never is true, but the compiler cannot know!
      throw exception();
#endif
    return *this;
  }

  ////////////////////////////////////////////////////////

  callback_return_type set(const val_type& val) {
    callback_return_type result = do_pre_write_callback();
    if (result == return_nothing) {
      my_value = val;
      do_post_write_callback();
    }
    return result;
  }

  
  callback_return_type set_throwing(const val_type& val) throw (exception) {
    callback_return_type result = set(val);
    if (result != return_nothing)
      throw exception();
    return result;
  }

  callback_return_type set_not_throwing(const val_type& val) throw (exception) {
    callback_return_type result = set(val);
    if (result != return_nothing)
      return result;
    if (!global_bool_checker) throw exception();
    return result;
  }
  
  ////////////////////////////////////////////////////////
  
  operator const val_type& () 
#ifdef OPERATORS_THROWING
    throw (exception)
#endif
  { 
    callback_return_type result = get(my_value); // not clean :-) 
#ifdef OPERATORS_THROWING
    if (result != return_nothing && !global_bool_checker) // this never is true, but the compiler cannot know!
      throw exception();
#endif
    return my_value;
  }
  
  const val_type& get() {
    callback_return_type result = do_pre_read_callback();
    if (result != return_read_rejected_because_private) {
      do_post_read_callback();
      return my_value;
    }
    return dummy_value;
  }

  const callback_return_type get(val_type& val) {
    callback_return_type result = do_pre_read_callback();
    if (result != return_read_rejected_because_private) {
      do_post_read_callback();
      val = my_value;
    }
    return result;
  }
  
  const val_type& get_not_throwing() throw (exception) {
    callback_return_type result = do_pre_read_callback();
    if (result != return_read_rejected_because_private) {
      do_post_read_callback();
      return my_value;
    }
    if (!global_bool_checker) throw exception();
    return dummy_value;
  }
  
  const val_type& get_throwing() throw (exception) {
    callback_return_type result = do_pre_read_callback();
    if (result != return_read_rejected_because_private) {
      do_post_read_callback();
      return my_value;
    }
    throw exception();
    return dummy_value;
  }
  
  ////////////////////////////////////////////////////////

protected:
  callback_return_type do_pre_write_callback() {
    return return_value_change_rejected; // reject all
  }
  callback_return_type do_post_write_callback() {
    return return_nothing;
  }
  callback_return_type do_pre_read_callback() {
    return return_read_rejected_because_private; // reject all
  }
  callback_return_type do_post_read_callback() {
    return return_nothing;
  }

};


/// Testbench for the performance test for Parameter value rejection throwing
int main(int argc, char *argv[]) {
  //sc_core::sc_report_handler::set_actions(sc_core::SC_WARNING, sc_core::SC_ABORT);

  //cout << "argc="<<argc<<endl;
  assert (argc == 2 && "Please specify command line argument A or B");
  
  param<int> p;
  unsigned int u = 0;
  callback_return_type result;
  int tmp_val;
  global_bool_checker = true; // unpredictable for compiler

  //cout << "argv[1]="<< argv[1] << endl;
  
  // Test Operator= without throwing (ignoring the result)
  if (strcmp(argv[1], "A1") == 0) {
    cout << "A1: Test Operator= without throwing (ignoring the result)" << endl;
#ifdef OPERATORS_THROWING
    assert(false && "You must compile without define OPERATORS_THROWING for this test");
#endif
    for(unsigned int i = 0; i < 90000000; i++) {
      p = i;
    }
  }
  
  // Test Operator= with throwing (but no error case)
  else if (strcmp(argv[1], "A2") == 0) {
    cout << "A2: Test Operator= with throwing (but no error case)" << endl;
#ifndef OPERATORS_THROWING
    assert(false && "You must compile with define OPERATORS_THROWING for this test");
#endif
    for(unsigned int i = 0; i < 90000000; i++) {
      try {
        p = i;
      } catch (exception e) {   u++;   }
    }
  }
  
  // Test set without throwing (returning the result)
  else if (strcmp(argv[1], "A3") == 0) {
    cout << "A3: Test set without throwing (returning the result)" << endl;
    for(unsigned int i = 0; i < 90000000; i++) {
      result = p.set(i);
      if (result != return_nothing) u++;
    }
  }
  
  // Test set with throwing (but no error case)
  else if (strcmp(argv[1], "A4") == 0) {
    cout << "A4: Test set with throwing (but no error case)" << endl;
    for(unsigned int i = 0; i < 90000000; i++) {
      try {
        p.set_not_throwing(i);
      } catch (exception e) {   u++;   }
    }
  }
  
  // Test set with throwing (and throwing each time) NOTE: 10 times smaller loop!
  else if (strcmp(argv[1], "A5") == 0) {
    cout << "A5: Test set with throwing (and throwing each time) NOTE: 10 times smaller loop!" << endl;
    for(unsigned int i = 0; i < 9000000; i++) {
      try {
        p.set_throwing(i);
      } catch (exception e) {   u++;   }
    }
  }
  
  
  ///////////////////////////////////////////////////////////////
  
  
  // Test operator() without throwing (ignoring the result)
  else if (strcmp(argv[1], "B1") == 0) {
    cout << "B1: Test operator() without throwing (ignoring the result)" << endl;
#ifdef OPERATORS_THROWING
    assert(false && "You must compile without define OPERATORS_THROWING for this test");
#endif
    for(unsigned int i = 0; i < 90000000; i++) {
      tmp_val = p;
    }
  }

  // Test operator() with throwing (but with no error case)
  else if (strcmp(argv[1], "B2") == 0) {
    cout << "B2: Test operator() with throwing (but with no error case)" << endl;
#ifndef OPERATORS_THROWING
    assert(false && "You must compile with define OPERATORS_THROWING for this test");
#endif
    for(unsigned int i = 0; i < 90000000; i++) {
      try {
        tmp_val = p;
      } catch (exception e) {   u++;   }
    }
  }

  // Test get() (ignoring the result)
  else if (strcmp(argv[1], "B3") == 0) {
    cout << "B3: Test get() (ignoring the result)" << endl;
    for(unsigned int i = 0; i < 90000000; i++) {
      tmp_val = p.get();
    }
  }
  
  // Test get(val) (returning the result)
  else if (strcmp(argv[1], "B4") == 0) {
    cout << "B4: Test get(val) (returning the result)" << endl;
    for(unsigned int i = 0; i < 90000000; i++) {
      result = p.get(tmp_val);
      if (result != return_nothing) u++;
    }
  }

  // Test get() with throwing (but with no error case)
  else if (strcmp(argv[1], "B5") == 0) {
    cout << "B5: Test get() with throwing (but with no error case)" << endl;
    for(unsigned int i = 0; i < 90000000; i++) {
      try {
        tmp_val = p.get_not_throwing();
      } catch (exception e) {   u++;   }
    }
  }

  // Test get() with throwing (and throwing each time) NOTE: 10 times smaller loop!
  else if (strcmp(argv[1], "B6") == 0) {
    cout << "B6: Test get() with throwing (and throwing each time) NOTE: 10 times smaller loop!" << endl;
    for(unsigned int i = 0; i < 9000000; i++) {
      try {
        tmp_val = p.get_throwing();
      } catch (exception e) {   u++;   }
    }
  }
  
  else {
    global_bool_checker = false;
    assert(false && "You have chosen an invalid scenario!");
  }
  
  //sc_core::sc_start();
  
  //return EXIT_SUCCESS; 
}
