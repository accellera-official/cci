//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2008 : GreenSocs Ltd
//    http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by :
//   
//   Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//   Robert Guenzel <guenzel@eis.cs.tu-bs.de>,
//   Gerrit Meier
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT

#ifndef __Calculator_h__
#define __Calculator_h__

#include "gav_globals.h"
#include "greencontrol/config.h"
#include "calc_if.h"

#include <iostream>
#include <sstream>
#include <deque>


namespace gs {
namespace av {
      
  /// GreenAV default calculator class.
  /**
   * Class for the StatCalc calculation part.
   * Implements <code>calc_if</code>.
   */
  template<typename T>
  class Calculator
  : public calc_if {

  protected:
    
    typedef T (*fpointer)(T,T,bool);

  public:
    /// Constructor with a user chosen name
    Calculator(const char* name)
    : //m_name(sc_gen_unique_name("Calculator")),
      m_name(name),
      got_result_param(false),
      got_input_params(false),
      sliding_window_size(0),
      sliding_window_result(NULL),
      m_sloppy(false),  // disable sloppy calculation. This means a division by zero will throw an SC_ERROR
      default_par(sc_gen_unique_name((std::string(name)+"_default").c_str()), "")
    {
      // get the right vector with function pointers and map with sign -> function id info
      Calculator<T>::initFuncs();
    }

    /// Destructor
    ~Calculator() {
      GAV_DUMP_N(name(), "Destruct Calculator.");      
      for (unsigned int i=0; i<m_results.size();i++) {
        delete m_results[i];
      }
      
      for (typename std::map<std::string, gs_param<T>* >::iterator itr=m_constmap.begin() ; itr != m_constmap.end() ; itr++) {
        delete itr->second;
      }
      m_constmap.clear();

      if (sliding_window_size > 0) {
        sliding_window_elements.clear();
      }
      if (sliding_window_result != NULL) {
        delete sliding_window_result; sliding_window_result = NULL;
      }
    }
  
    /// Function to realize input of constants (constants will be casted to the type of this calculator)
    /**
     * Important: When using constants it is not allowed to use the convenient operators. 
     * You must use these <code>calc</code> functions!
     *
     * @param func    Function ('operator') identifier
     * @param const_1 Constant that should be used within the formula - will be translated to a gs_param
     * @param second_comp   Second parameter, normal gs_param
     */
    gs_param_base& calc(const std::string func, const T const_1, const gs_param_base &second_comp) {
      if (got_result_param)
        SC_REPORT_WARNING(name(), "Do not modify the formula after having called get_result_param()!");
      std::ostringstream o;
      o << const_1;
      if (m_constmap.find(o.str()) == m_constmap.end()) {
        m_constmap[o.str()]=new gs_param<T>(generate_unique_name("const_temp_var"),const_1);
        m_constmap[o.str()]->add_param_attribute(cnf::param_attributes::temp);
      }
      return calc(func, *m_constmap[o.str()],second_comp);
    }

    /// Function to realize input of constants (constants will be casted to the type of this calculator)
    /**
     * @param func    Function ('operator') identifier
     * @param second_comp   First parameter, normal gs_param
     * @param const_2 Constant that should be used within the formula - will be translated to a gs_param
     */
    gs_param_base& calc(const std::string func, const gs_param_base &second_comp, const T const_2) {
      if (got_result_param)
        SC_REPORT_WARNING(name(), "Do not modify the formula after having called get_result_param()!");
      std::ostringstream o;
      o << const_2;
      if (m_constmap.find(o.str()) == m_constmap.end()) {
        m_constmap[o.str()]=new gs_param<T>(generate_unique_name("const_temp_var"),const_2);
        m_constmap[o.str()]->add_param_attribute(cnf::param_attributes::temp);
      }
      return calc(func,second_comp, *m_constmap[o.str()]);
    }
    
    /// Function to realize input of constants (constants will be casted to the type of this calculator)
    /**
     * You should better not use this function since it produces unneeded overhead during each calculation.
     *
     * @param func    Function ('operator') identifier
     * @param const_1 Constant that should be used within the formula - will be translated to a gs_param
     * @param const_2 Constant that should be used within the formula - will be translated to a gs_param
     */
    gs_param_base& calc(const std::string func, const T const_1, const T const_2) {
      if (got_result_param)
        SC_REPORT_WARNING(name(), "Do not modify the formula after having called get_result_param()!");
      std::ostringstream o1,o2;
      o1 << const_1;
      o2 << const_2;
      if (m_constmap.find(o1.str()) == m_constmap.end()) {
        m_constmap[o1.str()]=new gs_param<T>(generate_unique_name("const_temp_var"),const_1);
        m_constmap[o1.str()]->add_param_attribute(cnf::param_attributes::temp);
      }
      if (m_constmap.find(o2.str()) == m_constmap.end()) {
        m_constmap[o2.str()]=new gs_param<T>(generate_unique_name("const_temp_var"),const_2);
        m_constmap[o2.str()]->add_param_attribute(cnf::param_attributes::temp);
      }
      return calc(func,*m_constmap[o1.str()], *m_constmap[o2.str()]);
    }
    
    /// The basic function for initial building the formula.
    /**
     * Initially builds the formula.
     * Build the formula before creating the StatCalc (which calls
     * <code>get_used_params()</code>) and before calling 
     * <code>get_result_param()</code> (because formula building may
     * change the result parameter).
     * Does NO initial calculation!
     *
     * @param func   Operator string representation
     * @param first_comp  First component (parameter) for the operator
     * @param second_comp  Second component (parameter) for the operator
     * @return Result parameter updated each time the operator() is called for recalculation
     */
    gs_param_base& calc(const std::string& func, const gs_param_base &first_comp, const gs_param_base &second_comp) {
/*#ifdef GAV_VERBOSE
      cout << name() << " m_op_map:" << endl; 
      std::map<std::string, int>::iterator iterm;
      for (iterm = m_op_map.begin(); iterm != m_op_map.end(); iterm++) {
        cout << iterm->first <<"="<<iterm->second<< ",  "; 
      }
      cout << endl;
#endif*/
      // catch errors
      if (got_result_param)
        SC_REPORT_WARNING(name(), "Do not modify the formula after having called get_result_param()!");
      std::map<std::string,int>::iterator iter = m_op_map.find(func);
      if( iter == m_op_map.end() ) {
        GAV_DUMP_N(name(), "Calculator: operator '"<<func.c_str()<<"' unknown!");
        SC_REPORT_WARNING(name(), "Calculator: operator unknown!");
        return default_par;
      }
      // Flags to indicate if we have to register the parameter
      bool save_first=true;
      bool save_second=true;
      // create a new object for the result
      gs_param<T> *ans = new gs_param<T>(sc_gen_unique_name((m_name+"_result").c_str()));
      ans->add_param_attribute(cnf::param_attributes::output);
      // now do an initial calculating
      // *ans=Calculator<T>::m_func_vec[Calculator<T>::m_op_map[func]](convert(first_comp), convert(second_comp), m_sloppy);
      // save the result for callbacks and destructor
      m_results.push_back(ans);
      // ** Only register callback for a param once, because every registration will give us a callback
      // only register gs_params that are not results of a previous calculation
      for (unsigned int i=0; i<m_results.size();i++) {
        if (first_comp.getName()==m_results[i]->getName()){
          save_first=false;
        }
        if (second_comp.getName()==m_results[i]->getName()){
          save_second=false;
        }
      }
      if (first_comp.getName()==second_comp.getName()) {
        save_second=false;
      }
      // only register gs_params that are not used and registered before
      for (unsigned int p=0; p < m_params.size(); p++) {
        if (save_first) {
          if (first_comp.getName()==((m_params[p].first)->getName()) || first_comp.getName()==((m_params[p].second)->getName())) {
            save_first=false;
          }
        }
        if (save_second) {
          if (second_comp.getName()==((m_params[p].first)->getName()) || second_comp.getName()==((m_params[p].second)->getName())) {
            save_second=false;
          }
        }
      }
      // save the used gs_param_bases parameter
      m_params.push_back(std::make_pair(&first_comp,&second_comp));
      // and the used function
      m_functions.push_back(Calculator<T>::m_op_map[func]);
      // store the parameters to be given to the trigger for callbacks
      if (save_first) {
        //GC_REGISTER_PARAM_CALLBACK(&first_comp,statisticsCalculator,operator());
        input_parameters.push_back(&first_comp);
      }
      if (save_second) {
        //GC_REGISTER_PARAM_CALLBACK(&second_comp,statisticsCalculator,operator());
        input_parameters.push_back(&second_comp);
      }
      return *ans;
    }
    
    /// Wrapper that gets a triple which is converted to a <code>calc(...)</code>-call
    /**
     * @param trip  Triple that is returned by a gs_param_base when using 
     *              an operator (+, -, *, /).
     */
    gs_param_base& operator()(const gs::cnf::triple& trip) {
      return calc(trip.m_op, trip.m_p1, trip.m_p2);
    }
    
    /// implements gs::av::calc_if::operator()
    /**
     * Calculation function called by the trigger.
     * Before this may called the function get_used_params() must be 
     * called once! (This should be done by the StatCalc when
     * being instantiated with this calculator object).
     */
    void operator()() {
      
      // ////////// calculation /////////////////////////// //
      // recalc every function from 0..n
      for (unsigned int i = 0; i < m_results.size() ; i++) {
        *(m_results[i])=Calculator<T>::m_func_vec[m_functions[i]](convert(*m_params[i].first),convert(*m_params[i].second), m_sloppy);
#ifdef GAV_VERBOSE
        std::string sign;
        // search for operator sign:
        std::map<std::string, int>::iterator iter;
        for (iter = m_op_map.begin(); iter != m_op_map.end(); iter++) {
          if (iter->second == m_functions[i]) 
            sign = iter->first;
        }
        printf("@%s /%d (%s): ", sc_core::sc_time_stamp().to_string().c_str(), (unsigned)sc_core::sc_delta_count(), name());
        std::cout << "calculate: "<< m_params[i].first->getString().c_str() <<" "<< sign.c_str() <<" "<< m_params[i].second->getString().c_str();
        std::cout << " = " << m_results[i]->getString().c_str() << std::endl;
#endif
      }
      
      // if sliding window is enabled
      if (sliding_window_size > 0) {
        // recalculate sliding window result
        sliding_window_elements.push_back(m_results[m_results.size()-1]->getValue());
        if (sliding_window_elements.size() > sliding_window_size)
          sliding_window_elements.pop_front();
        T tmp;
        typename std::deque<T>::iterator iter;
        IF_GAV_VERBOSE( std::stringstream ss; ss << "current sliding window ("<<sliding_window_elements.size()<<" elements): "; )
        for( iter = sliding_window_elements.begin(); iter != sliding_window_elements.end(); iter++ ) {
          if (iter == sliding_window_elements.begin()) {
            tmp = *iter;
          } else {
            tmp = tmp + *iter;
            IF_GAV_VERBOSE( ss << " + "; )
          }
          IF_GAV_VERBOSE( ss << *iter; )
        }
        IF_GAV_VERBOSE( ss << " = " << tmp; )
        tmp = (tmp / sliding_window_elements.size());
        IF_GAV_VERBOSE( ss << " / " << sliding_window_elements.size() << " = " << tmp << std::endl << std::endl; std::cout << ss.str();)
        *sliding_window_result = tmp;
      }
      
    }
    
    /// implements gs::av::calc_if::get_result_param
    /**
     * Returns the result parameter of this calculator which is 
     * the result with the highest index (or sliding window) 
     * -> this is the result of the whole formula.
     *
     * This call has to be used to access the result of this StCalc.
     * Call this function after having set the formula completely
     * and after having enabled a sliding window (if needed) since
     * the activation of a sliding window will change the internal
     * result parameter.
     *
     * @returns Result parameter of this StCalc.
     */
    gs_param<T>* get_result_param() {
      if (!got_input_params) {
        SC_REPORT_WARNING(name(), "The function get_result_param() must not be called before get_used_params(). Add this Calculator to a StatCalc first!");
        return NULL;
      }
      got_result_param = true;
      if (sliding_window_size > 0) {
        return sliding_window_result;
      }
      if (m_results.empty()) {
        SC_REPORT_WARNING(name(), "get_result_param will return NULL because no formula was set for this Calculator!");
        return NULL;
      }
      return m_results[m_results.size()-1];
    }
    
    /// implements gs::av::calc_if::get_used_params
    std::vector<const gs_param_base*>& get_used_params() {
      got_input_params = true;
      return input_parameters;
    }
    
    /// Enables the sloppy behavior
    /**
     * If enabled, error during calculation do NOT result in an error but
     * only in a warning. For the concerning caluclation the default value
     * will be used.
     */
    void enable_sloppy() {
      m_sloppy = true;
    }
    
    /// Disables the sloppy behavior
    void disable_sloppy() {
      m_sloppy = false;
    }
    
    // //////////////////// Statistics ///////////////////////////// //
    
    /// Enable sliding window and set size (disable if size = 0)
    /**
     * This modifies the result parameter!
     * @param window_size  Size of the window, if 0: deactivate sliding window.
     */
    void enable_sliding_window(unsigned int window_size) {
      if (got_result_param && sliding_window_size == 0) 
        SC_REPORT_WARNING(name(), "You should not enable sliding window after calling get_result_param() - or call it again to get the actual result parameter!");
      // disable sliding window
      if (window_size == 0) {
        GAV_DUMP_N(name(), "Disable sliding window");
        sliding_window_elements.clear();
      }
      else {
        // enable sliding window
        if (sliding_window_size == 0) {
          GAV_DUMP_N(name(), "Enable sliding window (size = "<<window_size<<")");
          sliding_window_elements.clear();
          //std::cout << "sliding window elements: " << sliding_window_elements.size() << std::endl;
          if (sliding_window_result == NULL) {
            sliding_window_result = new gs_param<T>(string(name())+"_SlidingWindow");
            sliding_window_result->add_param_attribute(cnf::param_attributes::output);
          }
        }
        // resize sliding window
        else {
          GAV_DUMP_N(name(), "Resize sliding window to "<<window_size);
          sliding_window_elements.clear();
        }
      }
      sliding_window_size = window_size;
    }
    
    /// Add functions to the calculator
    /**
     * Can be used to add new 'operations' to the calculator.
     * This will add the function (only) to the Calculator class of the
     * specified template!
     *
     * @param func_point  Function pointer to the function that should be added.
     * @param sign        String that should be used as the identifier for this function.
     */
    static void addFunc(T (*func_point)(T, T, bool), std::string sign) {
      initFuncs();
      if (Calculator<T>::m_op_map.count(sign) <= 0) {
        Calculator<T>::m_func_vec.push_back(func_point);
        Calculator<T>::m_op_map[sign]=Calculator<T>::m_func_vec.size()-1;
      }
      else {
        std::cout << "WARNING: A function with this operator sign were already registered. Ignoring redefinition of \""<<sign<<"\"." << std::endl;
      }
    } 
   
  protected:
    
    // Returns the name of this
    const char* name() {
      return m_name.c_str();
    }
    
    /// Unique name of this Calculator.
    std::string m_name;
    
    /// If the user already has got the result parameter
    bool got_result_param;
    /// If the StatCalc already has got the input parameters
    bool got_input_params;

    // sliding window size (0 = disabled)
    unsigned int sliding_window_size;

    /// Vector containing all parameters that this Calculator uses as input
    std::vector<const gs_param_base*> input_parameters;
    
    /// Result parameter of the sliding window
    gs_param<T>* sliding_window_result;
    
    /// Sliding window intermediate results
    std::deque<T> sliding_window_elements;
    
    /// This is used for handling divisions by zero and other convert errors, standard set to false in constructor
    bool m_sloppy;
    /// Result vector
    std::vector<gs_param<T>* >m_results;
    /// Parameter list 
    std::vector<std::pair<const gs_param_base*,const gs_param_base*> > m_params;
    /// Vector of used functions
    std::vector<int> m_functions;
    /// Operator map to parse the string e.g. + -> addfunction_id
    static std::map<std::string, int> m_op_map;
    /// id to function vector
    static std::vector<fpointer> m_func_vec;
    /// If the static initFuncs() function has already been called
    static bool static_init_called;
    // Map for constant input
    std::map<std::string, gs_param<T>* > m_constmap;
    /// Default return value if cast fails
    gs_param<T> default_par;
    
  protected:
    
    /// Help function for SystemC 2.1 compatibility: generates a global unique name
    /**
     * SystemC 2.1 creates top-level parameters which then are
     * not unique named although they have been generated with
     * sc_gen_unique_name.
     *
     * @param nuname Name that should be made unique
     * @return Calculator-class-wide and sc_gen_unique-wide unique name
     */
    std::string generate_unique_name(const char* nuname) {
      static unsigned int unique_cnt = 0;
      std::ostringstream ret;
      ret << sc_gen_unique_name(nuname) << "_" << unique_cnt;
      unique_cnt++;
      return ret.str();
    }
    
  protected:
    
    /// Convert gs_params value to the right datatype if possible
    /**
     * Returns the default value if cast fails.
     *
     * @return Input parameter casted to the StCalc template type. (Or default if cast fails.)
     */
    const T convert(const gs_param_base &par){
      //prepare a return variable
      switch (par.getType()) 
      {
        case gs::cnf::PARTYPE_INT: 
        { const gs_param<int>* p = dynamic_cast<const gs_param<int>*>(&par);
          return (T) p->getValue();
        } break;
        case gs::cnf::PARTYPE_UINT:
        { const gs_param<unsigned int>* p = dynamic_cast<const gs_param<unsigned int>*>(&par);
          return (T) p->getValue();
        }  break;
        case gs::cnf::PARTYPE_BOOL:
        { const gs_param<bool>* p = dynamic_cast<const gs_param<bool>*>(&par);
          return (T) p->getValue();
        }  break;
        case gs::cnf::PARTYPE_DOUBLE:
        { const gs_param<double>* p = dynamic_cast<const gs_param<double>*>(&par);
          return (T) p->getValue();
        }  break;
        case gs::cnf::PARTYPE_FLOAT:
        { const gs_param<float>* p = dynamic_cast<const gs_param<float>*>(&par);
          return (T) p->getValue();
        }  break;
        case gs::cnf::PARTYPE_ULONGLONG:
        { const gs_param<unsigned long long>* p = dynamic_cast<const gs_param<unsigned long long>*>(&par);
          return (T) p->getValue();
        }  break;
        case gs::cnf::PARTYPE_UCHAR:
        { const gs_param<unsigned char>* p = dynamic_cast<const gs_param<unsigned char>*>(&par);
          return (T) p->getValue();
        }  break;
        case gs::cnf::PARTYPE_USHORT:
        { const gs_param<unsigned short>* p = dynamic_cast<const gs_param<unsigned short>*>(&par);
          return (T) p->getValue();
        }  break;
        case gs::cnf::PARTYPE_SC_INT_BASE:
        case gs::cnf::PARTYPE_SC_INT:
        { const void* pv = par.get_value_pointer();
          const sc_dt::sc_int_base* p = static_cast<const sc_dt::sc_int_base*>(pv);
          return (T) p->to_uint64();
        }  break;
        case gs::cnf::PARTYPE_SC_UINT_BASE:
        case gs::cnf::PARTYPE_SC_UINT:
        { const void* pv = par.get_value_pointer();
          const sc_dt::sc_uint_base* p = static_cast<const sc_dt::sc_uint_base*>(pv);
          return (T) p->to_uint64();
        }  break;
        case gs::cnf::PARTYPE_SC_SIGNED:
        case gs::cnf::PARTYPE_SC_BIGINT:
        { const void* pv = par.get_value_pointer();
          const sc_dt::sc_signed* p = static_cast<const sc_dt::sc_signed*>(pv);
          if (p->length() > 64)
            SC_REPORT_WARNING("StatisticsCalculator", "Size of sc_bigint greater than 64: Loose of precision.");
          return (T) p->to_uint64();
        }  break;
        case gs::cnf::PARTYPE_SC_UNSIGNED:
        case gs::cnf::PARTYPE_SC_BIGUINT:
        { const void* pv = par.get_value_pointer();
          const sc_dt::sc_unsigned* p = static_cast<const sc_dt::sc_unsigned*>(pv);
          if (p->length() > 64)
            SC_REPORT_WARNING("StatisticsCalculator", "Size of sc_biguint greater than 64: Loose of precision.");
          return (T) p->to_uint64();
        }  break;
        case gs::cnf::PARTYPE_SC_TIME:
        { const gs_param<sc_time>* p = dynamic_cast<const gs_param<sc_time>*>(&par);
          return (T) p->getValue().to_double();
        }  break;
        default:
        { std::string warn_msg="Unable to use this parameter. No fitting datatype: ";
          warn_msg+= par.getName();
          warn_msg+= " with type ";
          warn_msg+= par.getTypeString();
          SC_REPORT_WARNING("StatisticsCalculator", warn_msg.c_str());
        }
      }
      return default_par.getValue();
    }
    
    // /////////////// the calculation functions: //////////////////// //
    /// addition
    static T add_it(const T x, const T y, bool sloppy) {
      return x+y;
    }
    /// subtraction
    static T diff_it(const T x, const T y, bool sloppy) {
      return x-y;
    }
    /// diversion
    static T div_it(const T x, const T y, bool sloppy) {
      if (y==0) {
        if (!sloppy) {
          SC_REPORT_FATAL("StatisticsCalculator","Division by zero");
        }
        else {
          SC_REPORT_INFO("StatisticsCalculator","Division by zero");
          return 0;
        }
      }
      return x/y;
    }
    /// multiplication
    static T mult_it(const T x, const T y, bool sloppy) {
      return x*y;
    }
    
    /// equal
    static T equal(const T x, const T y, bool sloppy) {
      return (x == y);
    }
    /// not equal
    static T n_equal(const T x, const T y, bool sloppy) {
      return (x != y);
    }
    /// greater / equal than...
    static T gt_eq(const T x, const T y, bool sloppy) {
      return (x >= y);
    }
    /// lower / equal than...
    static T lt_eq(const T x, const T y, bool sloppy) {
      return (x <= y);
    }
    /// lower than
    static T lt(const T x, const T y, bool sloppy) {
      return (x < y);
    }
    /// greater than
    static T gt(const T x, const T y, bool sloppy) {
      return (x > y);
    }
    
    /// Prepare the right operation sign and function pointer/id
    static void initFuncs() {
      // if not yet added
      if (!static_init_called) {
        
        T (*func_point)(T, T, bool);
        
        func_point=&add_it;
        Calculator<T>::m_func_vec.push_back(func_point);
        Calculator<T>::m_op_map["+"]=Calculator<T>::m_func_vec.size()-1;
        
        func_point=&diff_it;
        Calculator<T>::m_func_vec.push_back(func_point);
        Calculator<T>::m_op_map["-"]=Calculator<T>::m_func_vec.size()-1;
        
        func_point=&div_it;
        Calculator<T>::m_func_vec.push_back(func_point);
        Calculator<T>::m_op_map["/"]=Calculator<T>::m_func_vec.size()-1;
        
        func_point=&mult_it;
        Calculator<T>::m_func_vec.push_back(func_point);
        Calculator<T>::m_op_map["*"]=Calculator<T>::m_func_vec.size()-1;
        
        func_point=&equal;
        Calculator<T>::m_func_vec.push_back(func_point);
        Calculator<T>::m_op_map["=="]=Calculator<T>::m_func_vec.size()-1;
        
        func_point=&n_equal;
        Calculator<T>::m_func_vec.push_back(func_point);
        Calculator<T>::m_op_map["!="]=Calculator<T>::m_func_vec.size()-1;
        
        func_point=&gt_eq;
        Calculator<T>::m_func_vec.push_back(func_point);
        Calculator<T>::m_op_map[">="]=Calculator<T>::m_func_vec.size()-1;
        
        func_point=&lt_eq;
        Calculator<T>::m_func_vec.push_back(func_point);
        Calculator<T>::m_op_map["<="]=Calculator<T>::m_func_vec.size()-1;
        
        func_point=&lt;
        Calculator<T>::m_func_vec.push_back(func_point);
        Calculator<T>::m_op_map["<"]=Calculator<T>::m_func_vec.size()-1;
        
        func_point=&gt;
        Calculator<T>::m_func_vec.push_back(func_point);
        Calculator<T>::m_op_map[">"]=Calculator<T>::m_func_vec.size()-1;

        static_init_called = true;
      }
    }
  
  }; // end of class 
  
  /// Initialize map of class Calculator<T>
  template<typename T> std::map<std::string, int> Calculator<T>::m_op_map;
  /// Initialize vector of class Calculator<T>
  template<typename T> std::vector<typename Calculator<T>::fpointer>  Calculator<T>::m_func_vec;
  /// Initialize to false;
  template<typename T> bool Calculator<T>::static_init_called = false;
  
  
} // end namespace av
} // end namespace gs

#endif // __Calculator_h__
