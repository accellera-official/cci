// LICENSETEXT
//
//   Copyright (C) 2009-2011 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//    Mark Burton, mark@greensocs.com
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


#ifndef __CCI_PARAM_H__
#define __CCI_PARAM_H__


#ifndef __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
  #error "failed include check"
#else
  #undef __INCLUDE_ONLY_FROM_MAIN_INCLUDE_CHECK__
#endif


#include <string>
#include <iostream>
#include <sstream>

#include <boost/algorithm/string.hpp>


__OPEN_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__

  // Forward declaration
  template<typename T, cci::cnf::param_mutable_type TM>
  class gs_cci_param;

  /// Helper class: Needs to be specialized for all data types where cci_values shall work
  /**
   * This default implementation causes errors.
   */
  template<typename T, cci::cnf::param_mutable_type TM>
  struct cci_value_helper {
    typedef T my_type;
    /// Causes error by default; called by gs_cci_param::set_value
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_report_handler::cci_value_failure("Set cci value not implemented for not specialized parameter types.");
      // TODO: this could use a cci value's json representation to set the parameter
    }
    /// Causes error by default; called by gs_cci_param::get_value
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_report_handler::cci_value_failure("Get cci value not implemented for not specialized parameter types.");
      // TODO: this could use a cci value's json representation to get the parameter
      cci::cnf::cci_value val;
      return val;    
    }
  };

  // Helper class specializations
  // C++ data types for standard cci parameter types

  /// Helper class UNSIGNED INT template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<unsigned int, TM> {
    typedef unsigned int my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::param_type_number:
          // TODO one could implement further checks, e.g. overflows or implement conversions from type param_type_real
          param.set(val.get_int64());
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val((sc_dt::int64)param.get());
      return val;    
    }
  };

  /// Helper class INT template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<int, TM> {
    typedef int my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::param_type_number:
          // TODO one could implement further checks, e.g. overflows or implement conversions from type param_type_real
          param.set(val.get_int());
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get());
      return val;    
    }
  };

  /// Helper class BOOL template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<bool, TM> {
    typedef bool my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::param_type_bool:
          // TODO one could implement further checks or conversions from int etc.
          param.set(val.get_bool());
          break;
        case cci::cnf::param_type_number:
          if (val.get_int() > 0) param.set(true);
          else param.set(false);
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get());
      return val;    
    }
  };

  /// Helper class DOUBLE template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<double, TM> {
    typedef double my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::param_type_real:
          // TODO one could implement further checks or conversions from int etc.
          param.set(val.get_real());
          break;
        case cci::cnf::param_type_number:
          param.set(val.get_int());
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get());
      return val;    
    }
  };

  /// Helper class FLOAT template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<float, TM> {
    typedef float my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::param_type_real:
          // TODO one could implement further checks or conversions from int etc.
          param.set(val.get_real());
          break;
        case cci::cnf::param_type_number:
          param.set(val.get_int());
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get());
      return val;    
    }
  };

  /// Helper class STRING template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<std::string, TM> {
    typedef std::string my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::param_type_string:
          param.set(val.get_string());
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get());
      return val;    
    }
  };

  /// Helper class LONG LONG template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<long long, TM> {
    typedef long long my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::param_type_number:
          // TODO one could implement further checks or conversions from int etc.
          param.set(val.get_int64());
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get());
      return val;    
    }
  };


  /// Helper class UNSIGNED CHAR template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<unsigned char, TM> {
    typedef unsigned char my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::param_type_number:
          // TODO one could implement further checks or conversions from int etc.
          param.set(val.get_int());
          break;
        case cci::cnf::param_type_string:
          // TODO
          cci::cnf::cci_report_handler::cci_value_failure("not implemented");
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get());
      return val;    
    }
  };

  /// Helper class SIGNED CHAR template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<signed char, TM> {
    typedef signed char my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::param_type_number:
          // TODO one could implement further checks or conversions from int etc.
          param.set(val.get_int());
          break;
        case cci::cnf::param_type_string:
          // TODO
          cci::cnf::cci_report_handler::cci_value_failure("not implemented");
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get());
      return val;    
    }
  };


  /// Helper class CHAR template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<char, TM> {
    typedef char my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      std::string str;
      switch (val.type() ) {
        case cci::cnf::param_type_number:
          // TODO one could implement further checks or conversions from int etc.
          param.set(val.get_int());
          break;
        case cci::cnf::param_type_string:
          if (val.get_string().length() > 0) {
            param.set(val.get_string().at(0));
          } else {
            param.set(0);
          }
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get());
      return val;    
    }
  };

  /// Helper class UNSIGNED SHORT template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<unsigned short, TM> {
    typedef unsigned short my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::param_type_number:
          // TODO one could implement further checks or conversions from int etc.
          param.set(val.get_int());
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get());
      return val;    
    }
  };

  // TODO:
  /// Helper class VECTOR<STRING> template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<std::vector<std::string>, TM> {
    typedef std::vector<std::string> my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::param_type_list: {
          cci::cnf::cci_value_list lst_v = val.get_list();
          std::vector<std::string> l;
          for (cci::cnf::cci_value_list::iterator iter = lst_v.begin(); iter != lst_v.end(); iter++) {
            if (iter->type() == cci::cnf::param_type_string) {
              l.push_back(iter->get_string());
            } else {
              cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with malformed value type - not all items of list are strings.");              
              break;              
            }
          }
          param.set(l);
        }
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value_list lst_v;
      const std::vector<std::string>& vec = param.get();
      for (std::vector<std::string>::const_iterator iter = vec.begin(); iter != vec.end(); iter++) {
        cci::cnf::cci_value v(*iter);
        lst_v.push_back(v);
      }
      cci::cnf::cci_value val(lst_v);
      return val;    
    }
  };

  // SystemC data types for standard cci parameter types
  // TODO SystemC data types

  /// Helper class SC_INT_BASE template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<sc_dt::sc_int_base, TM> {
    typedef sc_dt::sc_int_base my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::param_type_number: {
            // TODO one could implement further checks or conversions
            sc_dt::sc_int<64> ib = val.get_int64();
            param.set(ib); 
          }
          break;
        case cci::cnf::param_type_string: 
        {
          sc_dt::sc_int_base ib(param.get().length());
          ib = val.get_string().c_str();
          param.set(ib); 
        }
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get());
      return val;    
    }
  };

  /// Helper class SC_UINT_BASE template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<sc_dt::sc_uint_base, TM> {
    typedef sc_dt::sc_uint_base my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::param_type_number: 
        {
          // TODO one could implement further checks or conversions
          sc_dt::sc_uint<64> ib = val.get_int64();
          param.set(ib); 
        }
          break;
        case cci::cnf::param_type_string: 
        {
          sc_dt::sc_uint_base ib(param.get().length());
          ib = val.get_string().c_str();
          param.set(ib); 
        }
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val((sc_dt::int64)param.get());
      return val;    
    }
  };

  /// Helper class SC_SIGNED template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<sc_dt::sc_signed, TM> {
    typedef sc_dt::sc_signed my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::param_type_string: {
          // TODO one could implement further checks or conversions
          sc_dt::sc_signed ib(param.get().length());
          ib = val.get_string().c_str();
          param.set(ib); 
        }
          break;
        case cci::cnf::param_type_number: 
        {
          // TODO one could implement further checks or conversions
          sc_dt::sc_bigint<64> ib = val.get_int64(); // note the limitation of number sizes in the cci_values does not fulfill this data type size! Use string instead.
          param.set(ib); 
        }
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get().to_string());
      return val;    
    }
  };

  /// Helper class SC_UNSIGNED template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<sc_dt::sc_unsigned, TM> {
    typedef sc_dt::sc_unsigned my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::param_type_string: {
          // TODO one could implement further checks or conversions
          sc_dt::sc_unsigned ib(param.get().length());
          ib = val.get_string().c_str();
          param.set(ib); 
        }
          break;
        case cci::cnf::param_type_number: 
        {
          // TODO one could implement further checks or conversions
          sc_dt::sc_biguint<64> ib = val.get_int64(); // note the limitation of number sizes in the cci_values does not fulfill this data type size! Use string instead.
          param.set(ib); 
        }
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get().to_string());
      return val;    
    }
  };

  /// Helper class SC_BIT template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<sc_dt::sc_bit, TM> {
    typedef sc_dt::sc_bit my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::param_type_number: 
          cci::cnf::cci_report_handler::cci_value_failure("not implemented because deprecated");
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get());
      return val;    
    }
  };

  /// Helper class SC_LOGIC template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<sc_dt::sc_logic, TM> {
    typedef sc_dt::sc_logic my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::param_type_string: { // valid are "0" "1" "Z" "z" "X" "x"
          if      (val.get_string().compare("0") == 0) param.set(sc_dt::Log_0);
          else if (val.get_string().compare("1") == 0) param.set(sc_dt::Log_1);
          else if (val.get_string().compare("Z") == 0
                   || val.get_string().compare("z") == 0) param.set(sc_dt::Log_Z);
          else if (val.get_string().compare("X") == 0
                   || val.get_string().compare("x") == 0) param.set(sc_dt::Log_X);
          else {
            cci::cnf::cci_report_handler::cci_value_failure("Unsupported cci_value string for sc_logic.");
          }
        }
          break;
        case cci::cnf::param_type_number: {
          sc_dt::sc_logic l(val.get_int()); // valid are 0 (Log_0), 1 (Log_1), 2 (Log_Z), 3 (Log_X)
          param.set(l);
        }
          break;
        case cci::cnf::param_type_bool: {
          sc_dt::sc_logic l(val.get_bool()); // valid are false (Log_0), true (Log_1)
          param.set(l);
        }
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      std::string str(1, param.get().to_char());
      cci::cnf::cci_value val(str);
      return val;    
    }
  };

  /// Helper class SC_TIME template specialization: to make cci_values work
  /**
   * allows following values:
   * - list of   param_type_real, param_type_string  :  time , sc_time_unit as string "SC_FS", "sc_fs", "SC_PC", "sc_pc" etc.
   * - list of   param_type_real, param_type_number  :  time , sc_time_unit as number according enum sc_core::sc_time_unit 
   * - param_type_string  :  string  TODO
   */
  template<cci::cnf::param_mutable_type TM>
  struct cci_value_helper<sc_core::sc_time, TM> {
    typedef sc_core::sc_time my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::param_type_list: {
          if (   val.get_list().at(0).type() == cci::cnf::param_type_real
              && val.get_list().at(1).type() == cci::cnf::param_type_string) {
            sc_core::sc_time tim;
            double      d = val.get_list().at(0).get_real();
            std::string s = val.get_list().at(1).get_string();
            boost::to_upper(s);
            if      (s.compare("SC_FS") == 0 || s.compare("fs") == 0)  tim = sc_core::sc_time(d, sc_core::SC_FS);
            else if (s.compare("SC_PS") == 0 || s.compare("ps") == 0)  tim = sc_core::sc_time(d, sc_core::SC_PS);
            else if (s.compare("SC_NS") == 0 || s.compare("ns") == 0)  tim = sc_core::sc_time(d, sc_core::SC_NS);
            else if (s.compare("SC_US") == 0 || s.compare("us") == 0)  tim = sc_core::sc_time(d, sc_core::SC_US);
            else if (s.compare("SC_MS") == 0 || s.compare("ms") == 0)  tim = sc_core::sc_time(d, sc_core::SC_MS);
            else if (s.compare("SC_SEC") == 0 || s.compare("sec") == 0) tim = sc_core::sc_time(d, sc_core::SC_SEC);
            param.set(tim);
          }
          else if (   val.get_list().at(0).type() == cci::cnf::param_type_real
                   && val.get_list().at(1).type() == cci::cnf::param_type_number) {
            sc_core::sc_time tim;
            double d = val.get_list().at(0).get_real();
            int    i = val.get_list().at(1).get_int();
            if      (i == 0)  tim = sc_core::sc_time(d, sc_core::SC_FS);
            else if (i == 1)  tim = sc_core::sc_time(d, sc_core::SC_PS);
            else if (i == 2)  tim = sc_core::sc_time(d, sc_core::SC_NS);
            else if (i == 3)  tim = sc_core::sc_time(d, sc_core::SC_US);
            else if (i == 4)  tim = sc_core::sc_time(d, sc_core::SC_MS);
            else if (i == 5)  tim = sc_core::sc_time(d, sc_core::SC_SEC);
            param.set(tim);
          }
          else {
            cci::cnf::cci_report_handler::cci_value_failure("Unsupported cci_value list for sc_time.");
          }
        }
          break;
        case cci::cnf::param_type_string: {
          cci::cnf::cci_report_handler::cci_value_failure("not implemented");
        }
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    /// Returns cci_value (param_type_list) with members param_type_double (time) and param_type_string (sc_time_unit)
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value_list sctime_v_lst;
      std::string strtime = param.get().to_string();
      std::size_t found_SC = strtime.find("SC_");
      std::size_t found_space = strtime.find_last_of(" ");
      if (found_SC != std::string::npos) {
        std::string strtimeunit = strtime.substr(found_SC, std::string::npos); // find and store time unit as string                                            
        sctime_v_lst.push_back(cci::cnf::cci_value(param.get().to_double()));
        sctime_v_lst.push_back(strtimeunit);
        cci::cnf::cci_value val(sctime_v_lst);
        return val;        
      }
      else if (found_space + 1 != std::string::npos) {
        std::string strtimeunit = strtime.substr(found_space+1, std::string::npos); // find and store time unit as string                                            
        sctime_v_lst.push_back(cci::cnf::cci_value(param.get().to_double()));
        sctime_v_lst.push_back(strtimeunit);
        cci::cnf::cci_value val(sctime_v_lst);
        return val;        
      }
      else {
        std::cout << strtime << std::endl;
        cci::cnf::cci_report_handler::cci_value_failure("Creating cci value for sc_time value failed!");
      }
      cci::cnf::cci_value val;
      return val;        
    }
  };

  /// Helper class SC_INT<> template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM, int W>
  struct cci_value_helper<sc_dt::sc_int<W>, TM> {
    typedef sc_dt::sc_int<W> my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::param_type_number: {
          // TODO one could implement further checks or conversions
          sc_dt::sc_int<W> ib = val.get_int64();
          param.set(ib); 
        }
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get());
      return val;    
    }
  };

  /// Helper class SC_UINT<> template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM, int W>
  struct cci_value_helper<sc_dt::sc_uint<W>, TM> {
    typedef sc_dt::sc_uint<W> my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::param_type_number: {
          // TODO one could implement further checks or conversions
          sc_dt::sc_uint<W> ib = val.get_int64();
          param.set(ib); 
        }
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val((sc_dt::int64)param.get());
      return val;    
    }
  };

  /// Helper class SC_BIGINT<> template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM, int W>
  struct cci_value_helper<sc_dt::sc_bigint<W>, TM> {
    typedef sc_dt::sc_bigint<W> my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::param_type_string: {
          // TODO one could implement further checks or conversions
          sc_dt::sc_bigint<W> ib = val.get_string().c_str();
          param.set(ib); 
        }
          break;
        // just for small numbers, default is string representation
        case cci::cnf::param_type_number: {
          // TODO one could implement further checks or conversions
          sc_dt::sc_bigint<W> ib = val.get_int64();
          param.set(ib); 
        }
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get().to_string());
      return val;    
    }
  };

  /// Helper class SC_BIGUINT<> template specialization: to make cci_values work
  template<cci::cnf::param_mutable_type TM, int W>
  struct cci_value_helper<sc_dt::sc_biguint<W>, TM> {
    typedef sc_dt::sc_biguint<W> my_type;
    static void from_value(const cci::cnf::cci_value& val, gs_cci_param<my_type, TM>& param) {
      switch (val.type() ) {
        case cci::cnf::param_type_string: {
          // TODO one could implement further checks or conversions
          sc_dt::sc_biguint<W> ib = val.get_string().c_str();
          param.set(ib); 
        }
          break;
          // just for small numbers, default is string representation
        case cci::cnf::param_type_number: {
          // TODO one could implement further checks or conversions
          sc_dt::sc_biguint<W> ib = val.get_int64();
          param.set(ib); 
        }
          break;
        default:
          cci::cnf::cci_report_handler::cci_value_failure("Set cci value called with wrong value type.");
      }
    }
    static cci::cnf::cci_value to_value(gs_cci_param<my_type, TM>& param) {
      cci::cnf::cci_value val(param.get().to_string());
      return val;    
    }
  };



  /// Prototype parameter implementation
  template<typename T, cci::cnf::param_mutable_type TM = cci::cnf::mutable_param>
  class gs_cci_param 
  : public gs_cci_param_t <T, TM>
  {
  public:

    /// Typedef for the value.
    typedef T value_type;
    /// Typedef for the param itself.
    typedef gs_cci_param<value_type, TM> my_type;
    /// Typedef for base type
    typedef gs_cci_param_t<value_type, TM> base_type;

    using base_type::return_string;
    //using base_type::my_value;
    using base_type::operator=;
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Construction / Destruction   ///////////////////// //

    gs_cci_param(cci::cnf::cci_param<value_type, TM>& owner_par, const std::string &nam, const char* val    , const bool is_top_level_name, cci::cnf::cci_cnf_broker_if* broker_accessor) : base_type(owner_par, nam, std::string(val), is_top_level_name, /*register_at_db=*/true                 , broker_accessor)   { /* TODO: Remove this check to allow empty default values!! This check is to ensure the framework internally uses the correct constructor*/ if (std::string(val).length()==0) assert(false && "This shall not happen, use other constructor!"); /*base_type::init(); Done with InitParam function*/ }
    gs_cci_param(cci::cnf::cci_param<value_type, TM>& owner_par, const std::string &nam, const value_type& val, const bool is_top_level_name, cci::cnf::cci_cnf_broker_if* broker_accessor) : base_type(owner_par, nam, val,              is_top_level_name, /*register_at_db=*/true, /*(dummy)*/true, broker_accessor)   { /*base_type::init(); Done with InitParam function*/ }
    gs_cci_param(cci::cnf::cci_param<value_type, TM>& owner_par, const std::string &nam,                      const bool is_top_level_name, cci::cnf::cci_cnf_broker_if* broker_accessor) : base_type(owner_par, nam,                   is_top_level_name, /*register_at_db=*/true                 , broker_accessor)   { /*base_type::init(); Done with InitParam function*/ }
  
    ~gs_cci_param() {
    }
    
    /*my_type& operator +=  (value_type);
    my_type& operator -=  (value_type);
    my_type& operator /=  (value_type);
    my_type& operator *=  (value_type);
    value_type operator +  (value_type);
    value_type operator -  (value_type);
    value_type operator /  (value_type);
    value_type operator *  (value_type);
    my_type& operator %=  (value_type);
    my_type& operator ^=  (value_type);
    my_type& operator &=  (value_type);
    my_type& operator |=  (value_type);
    my_type& operator <<= (value_type);
    my_type& operator >>= (value_type);
    
    my_type& operator -- ();    // prefix
    value_type operator -- (int); // postfix
    my_type& operator ++ ();    // prefix
    value_type operator ++ (int); // postfix */ 

    // //////////////// JSON (DE)SERIALIZE /////////////////////////// //

    std::string json_serialize() const {
      return_string = json_serialize(base_type::get());
      return return_string;
    }
    
    void json_deserialize(const std::string& str) {
      value_type t;
      json_deserialize(t, str);
      base_type::set(t);
    }

    std::string json_serialize(const value_type& val) const {
      // TODO: this is currently not a JSON but a GreenConfig specific string
      // TODO: throw exception on error
      return base_type::m_gs_param.serialize(val);
    }

    void json_deserialize(value_type& target_val, const std::string& str) {
      // TODO: this is currently not a JSON but a GreenConfig specific string
      if (!base_type::m_gs_param.deserialize(target_val, str))
        cci::cnf::cci_report_handler::set_param_failed("String conversion failed.");
    }

    // //////////////// CCI VALUE HANDLING /////////////////////////// //

    const value_type& get_default_value() {
      if (!base_type::m_gs_param.has_default_value())
        cci::cnf::cci_report_handler::get_param_failed("Param has no default value.");
      return base_type::m_gs_param.get_default_value();
    }

    // //////////////// CCI VALUE HANDLING /////////////////////////// //
    
    void set_value(const cci::cnf::cci_value& val) {
      cci_value_helper<value_type, TM>::from_value(val, *this);
    }

    cci::cnf::cci_value get_value() {
      return cci_value_helper<value_type, TM>::to_value(*this);
    }
    
  };
      
  

  /// Prototype parameter implementation, specialization for string
  template<cci::cnf::param_mutable_type TM>
  class gs_cci_param<std::string, TM>
  : public gs_cci_param_t<std::string, TM>
  {
  protected:
    /// Typedef for the value.
    typedef std::string value_type;
    /// Typedef for the param itself.
    typedef gs_cci_param<value_type, TM> my_type;
    /// Typedef for base type
    typedef gs_cci_param_t<value_type, TM> base_type;
    
  public:
    
    using base_type::return_string;
    //using base_type::my_value;
    using base_type::operator=;
    
    // //////////////////////////////////////////////////////////////////// //
    // ///////////////   Construction / Destruction   ///////////////////// //
    
    gs_cci_param(cci::cnf::cci_param<value_type, TM>& owner_par, const std::string &nam, const char* val    , const bool is_top_level_name, cci::cnf::cci_cnf_broker_if* broker_accessor) : base_type(owner_par, nam, std::string(val), is_top_level_name, true, (std::string(val).length()>0) ? true : false, broker_accessor) { /*base_type::init(); Done with InitParam function*/ }
    gs_cci_param(cci::cnf::cci_param<value_type, TM>& owner_par, const std::string &nam, const value_type& val, const bool is_top_level_name, cci::cnf::cci_cnf_broker_if* broker_accessor) : base_type(owner_par, nam, val, is_top_level_name, true, broker_accessor)   { /*base_type::init(); Done with InitParam function*/ }
    gs_cci_param(cci::cnf::cci_param<value_type, TM>& owner_par, const std::string &nam,                      const bool is_top_level_name, cci::cnf::cci_cnf_broker_if* broker_accessor) : base_type(owner_par, nam, std::string(""), is_top_level_name, true, false, broker_accessor) { /*base_type::init(); Done with InitParam function*/ }
    
    ~gs_cci_param() {
    }
    
    /*my_type& operator +=  (value_type);
     my_type& operator -=  (value_type);
     my_type& operator /=  (value_type);
     my_type& operator *=  (value_type);
     value_type operator +  (value_type);
     value_type operator -  (value_type);
     value_type operator /  (value_type);
     value_type operator *  (value_type);
     my_type& operator %=  (value_type);
     my_type& operator ^=  (value_type);
     my_type& operator &=  (value_type);
     my_type& operator |=  (value_type);
     my_type& operator <<= (value_type);
     my_type& operator >>= (value_type);
     
     my_type& operator -- ();    // prefix
     value_type operator -- (int); // postfix
     my_type& operator ++ ();    // prefix
     value_type operator ++ (int); // postfix */ 
    
    std::string json_serialize() const {
      return_string = json_serialize(base_type::get());
      return return_string;
    }
    
    void json_deserialize(const std::string& str) {
      value_type t;
      json_deserialize(t, str);
      base_type::set(t);
    }
    
    std::string json_serialize(const value_type& val) const {
      return base_type::m_gs_param.serialize(val);
    }
    
    void json_deserialize(value_type& target_val, const std::string& str) {
      base_type::m_gs_param.deserialize(target_val, str);
    }    
    
    const value_type& get_default_value() {
      if (!base_type::m_gs_param.has_default_value())
        cci::cnf::cci_report_handler::get_param_failed("Param has no default value.");
      return base_type::m_gs_param.get_default_value();
    }
    
    // TODO: check for update_latest_write_originator() for each successfull set (when adding new sets)
    void set_value(const cci::cnf::cci_value& val) {
      cci_value_helper<value_type, TM>::from_value(val, *this);
    }
    
    cci::cnf::cci_value get_value() {
      return cci_value_helper<value_type, TM>::to_value(*this);
    }
    
    const cci::cnf::basic_param_type get_basic_type() const { return cci::cnf::param_type_string; }

  };
  

__CLOSE_NAMESPACE_EXAMPLE_PARAM_IMPLEMENTATION__


#endif
