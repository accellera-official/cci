//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007-2010 : GreenSocs Ltd
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

// included by gs_param.h
// inside namespace gs::cnf


// included in gs_param.h
/*} } // close namespaces
  // needed for array parsing:
# include <boost/tokenizer.hpp>
namespace namespace gs { namespace cnf {*/


// define in config_globals.h #define GS_PARAM_ARRAY_VERBOSE
#ifndef GS_PARAM_ARRAY_DUMP
# ifdef GS_PARAM_ARRAY_VERBOSE
//#  define GS_PARAM_ARRAY_TRACE(msg, ...) { printf("%s: ", this->name()); printf(msg, ##__VA_ARGS__); printf("\n"); }
#  define GS_PARAM_ARRAY_DUMP(msg) { std::cout<<this->name()<<": " << msg << std::endl; }
# else
//#  define GS_PARAM_ARRAY_TRACE(msg, ...)
#  define GS_PARAM_ARRAY_DUMP(msg)
# endif
#endif


// /////////////////////////////////////////////////////////////////////////////// //
// /////////////////// gs_param< T* > /////////////////////////////////////////// //
// /////////////////////////////////////////////////////////////////////////////// //


// Default size for an uninitialized (unsized) array
#ifndef DEFAULT_ARRAY_SIZE
#  define DEFAULT_ARRAY_SIZE 0
#endif


/// Simple Parameter Array: template specialization for all pointer types.
/**
 * The Simple Parameter Array contains members each of the same type gs_param<T>.
 * They can be accessed by position (vector-like functions my_array.at(pos)
 * and the operator my_array[pos]. The member's parameter name is
 * <parent_array_name>.<pos>, e.g. "my_module.my_sub_module.my_integer_array.0".
 *
 * Each member is stored as an individual parameter in the database.
 * (This is needed to allow init values to be set e.g. by a config file with
 * the style IP1.my_array.0 1000.)
 *
 * Array size (with growing priority):
 * - The default array size DEFAULT_ARRAY_SIZE.
 * - The constructor array size specified during construction
 *   (overwrites default array size).
 * - Implicit parameter '<array_par_name>.init_size' in plugin database
 *   (overwrites default and constructor array size).
 * - Highest implicit member number (in database):
 *    overwrites default/constructor/implicit size only if the greatest
 *    member number is greater than that size.
 */
template<class T>
class gs_param<T*>
: public gs_param_t<T*>
{
protected:

  /// Typedef for the value.
  typedef T* val_type;

  /// Internal vector which contains the pointers to the member parameters
  std::vector<gs_param<T>*> internal_ArrVec;

  /// Parameter to read out the init size of the array if it was set.
  gs_param<unsigned int>* m_init_size;

protected:
  /// Typedef for this specialized class.
  typedef gs_param<val_type> my_type;

  //using gs_param_t<val_type>::my_value;
  using gs_param_t<val_type>::m_api;
  using gs_param_t<val_type>::m_par_name;

  using gs_param_t<val_type>::convertStringToValue;

  using gs_param_t<val_type>::m_locked;
  using gs_param_t<val_type>::m_lock_pwd;

private:
  /// Not allowed constructor!
  explicit gs_param(const val_type &val) { sc_assert(false); }

public:
  // Explicit constructors to avoid implicit construction of parameters.

  /// Empty constructor (with optional parent array pointer). Name will be set in base
  explicit gs_param(gs_param_array* parent_array = NULL)
  : gs_param_t<val_type>(false, parent_array, true),
    m_init_size(NULL)
  {
    init_array(false, DEFAULT_ARRAY_SIZE, NULL);
    gs_param_t<val_type>::init();
  }
  /// Constructor with parent array reference. Name will be set in base
  explicit gs_param(gs_param_array& parent_array)
  : gs_param_t<val_type>(false, &parent_array, true),
    m_init_size(NULL)
  {
    init_array(false, DEFAULT_ARRAY_SIZE, NULL);
    gs_param_t<val_type>::init();
  }

  /// Constructor with (local) name (and optional parent array pointer).
  explicit gs_param(const std::string &nam, gs_param_array* parent_array = NULL)
  : gs_param_t<val_type>(nam, false, parent_array, true),
    m_init_size(NULL)
  {
    init_array(false, DEFAULT_ARRAY_SIZE, NULL);
    gs_param_t<val_type>::init();
  }
  /// Constructor with (local) name and parent array reference.
  explicit gs_param(const std::string &nam, gs_param_array& parent_array)
  : gs_param_t<val_type>(nam, false, &parent_array, true),
    m_init_size(NULL)
  {
    init_array(false, DEFAULT_ARRAY_SIZE, NULL);
    gs_param_t<val_type>::init();
  }

  /// Constructor with (local) name and default size (and optional parent array pointer).
  explicit gs_param(const std::string &nam, const unsigned int default_size, gs_param_array* parent_array = NULL)
  : gs_param_t<val_type>(nam, false, parent_array, true),
    m_init_size(NULL)
  {
    init_array(true, DEFAULT_ARRAY_SIZE, NULL);
    gs_param_t<val_type>::init();
  }
  /// Constructor with (local) name and default size and parent array reference.
  explicit gs_param(const std::string &nam, const unsigned int default_size, gs_param_array& parent_array)
  : gs_param_t<val_type>(nam, false, &parent_array, true),
    m_init_size(NULL)
  {
    init_array(true, default_size, NULL);
    gs_param_t<val_type>::init();
  }

  /// Constructor with (local) name (and optional parent array pointer).
  explicit gs_param(const char *nam, gs_param_array* parent_array = NULL)
  : gs_param_t<val_type>(string(nam), false, parent_array, true),
    m_init_size(NULL)
  {
    init_array(false, DEFAULT_ARRAY_SIZE, NULL);
    gs_param_t<val_type>::init();
  }
  /// Constructor with (local) name and parent array reference.
  explicit gs_param(const char *nam, gs_param_array& parent_array)
  : gs_param_t<val_type>(string(nam), false, &parent_array, true),
    m_init_size(NULL)
  {
    init_array(false, DEFAULT_ARRAY_SIZE, NULL);
    gs_param_t<val_type>::init();
  }

  /// Constructor with (local) name and default size (and optional parent array pointer).
  /**
   * @param default_size_low_priority  If set to true, the default size is of low priority and even overridden by implicit member in the db.
   */
  explicit gs_param(const char *nam, const unsigned int default_size, gs_param_array* parent_array = NULL, bool force_top_level_name = false, bool default_size_low_priority = false)
  : gs_param_t<val_type>(string(nam), force_top_level_name, parent_array, true),
    m_init_size(NULL)
  {
    init_array(!default_size_low_priority, default_size, NULL);
    gs_param_t<val_type>::init();
  }
  /// Constructor with (local) name and NO default size (and optional parent array pointer).
  explicit gs_param(const char *nam, gs_param_array* parent_array, bool force_top_level_name)
  : gs_param_t<val_type>(string(nam), force_top_level_name, parent_array, true),
    m_init_size(NULL)
  {
    init_array(false, DEFAULT_ARRAY_SIZE, NULL);
    gs_param_t<val_type>::init();
  }
  /// Constructor with (local) name and default size and parent array reference.
  explicit gs_param(const char *nam, const unsigned int default_size, gs_param_array& parent_array)
  : gs_param_t<val_type>(string(nam), false, &parent_array, true),
    m_init_size(NULL)
  {
    init_array(true, default_size, NULL);
    gs_param_t<val_type>::init();
  }
  
  /// Constructor with (local) name and initial value vector (and optional parent array pointer).
  explicit gs_param(const std::string &nam, const std::vector<T> default_vec, gs_param_array *parent_array = NULL)
  : gs_param_t<val_type>(nam, false, parent_array, true),
    m_init_size(NULL)
  {
    init_array(true, default_vec.size(), &default_vec);
    gs_param_t<val_type>::init();
  }
  /// Constructor with (local) name and initial value vector and parent array reference.
  explicit gs_param(const std::string &nam, const std::vector<T> default_vec, gs_param_array &parent_array)
  : gs_param_t<val_type>(nam, false, &parent_array, true),
    m_init_size(NULL)
  {
    init_array(true, default_vec.size(), &default_vec);
    gs_param_t<val_type>::init();
  }

  /// Constructor with (local) name and initial value vector (and optional parent array pointer).
  explicit gs_param(const char *nam, const std::vector<T> default_vec, gs_param_array *parent_array = NULL)
  : gs_param_t<val_type>(string(nam), false, parent_array, true),
    m_init_size(NULL)
  {
    init_array(true, default_vec.size(), &default_vec);
    gs_param_t<val_type>::init();
  }
  /// Constructor with (local) name and initial value vector and parent array reference.
  explicit gs_param(const char *nam, const std::vector<T> default_vec, gs_param_array &parent_array)
  : gs_param_t<val_type>(string(nam), false, &parent_array, true),
    m_init_size(NULL)
  {
    init_array(true, default_vec.size(), &default_vec);
    gs_param_t<val_type>::init();
  }


  /// Destructor
  virtual ~gs_param() {
    delete_array();
    gs_param_t<val_type>::destruct_gs_param();
    delete m_init_size;
  }

  /// To resolve the correct = operator
  // Do NOT use: using gs_param_t<val_type>::operator =;

  using gs_param_t<val_type>::gs_param_base::name;
  using gs_param_t<val_type>::setString;
  using gs_param_t<val_type>::getString;
  using gs_param_t<val_type>::setValue;
  using gs_param_t<val_type>::getValue;


  // ///////////////////////
  //  operators

  GC_SPECIALISATIONS_ARITHMETIC_OPERATORS;

  GC_SPECIALISATIONS_BINARY_OPERATORS;

  GC_SPECIALISATIONS_INCREMENT_OPERATORS;
  GC_SPECIALISATIONS_DECREMENT_OPERATORS;

  /// Overloads gs_param_base::getTypeString
  const std::string getTypeString() const {
    return string("Array");
  }

  /// Overloads gs_param_base::getType
  const Param_type getType() const {
    return PARTYPE_SMPL_ARRAY;
  }

  // //////////////////////////////////////////////////////////////////// //
  // ///////   set and get with value   ///////////////////////////////// //


  /// Set the member values of this parameter to the member values of another gs_param array.
  /**
   * @param v  Parameter where the values should be read from.
   * @return   Reference to this.
   */
  my_type& operator = (const my_type& v) {
#ifdef GCNF_ENABLE_GS_PARAM_LOCK
    // Check the lock!
    if (m_locked) {                                                
      GS_PARAM_DUMP("parameter is locked!");                                      
      SC_REPORT_ERROR(GCNF_SC_REPORTER(this->getName()), "parameter is locked!");
      return;                                                               
    }                                                                             
#endif
    this->delete_array();
    internal_ArrVec.resize(v.size());
    for (unsigned int i = 0; i < internal_ArrVec.size(); i++) {
      std::stringstream hierParName_ss;
      hierParName_ss << m_par_name << SC_NAME_DELIMITER << i;
      internal_ArrVec[i] = new gs_param<T>(hierParName_ss.str(), v[i].getValue(), true);
    }
    return *this;
  }

  /// Set the member values of this parameter to the values of a vector.
  /**
   * @param v  Vector of type T where the values should be read from.
   * @return   Reference to this.
   */
  my_type& operator = (const std::vector<T>& v) {
#ifdef GCNF_ENABLE_GS_PARAM_LOCK
    // Check the lock!
    if (m_locked) {                                                
      GS_PARAM_DUMP("parameter is locked!");                                      
      SC_REPORT_ERROR(GCNF_SC_REPORTER(this->getName()), "parameter is locked!");
      return *this;                                                               
    }                                                                             
#endif
    this->resize(v.size());
    for (unsigned int i = 0; i < internal_ArrVec.size(); i++) {
      internal_ArrVec[i]->setValue(v[i]);
    }
    return *this;
  }

  /* *** Do not enable this implementation!! Especially the () operator is the wrong one! ***
  /// NOT IMPLEMENTED! Set the value of this parameter.
  my_type& operator = (const val_type& v) {
    SC_REPORT_WARNING(this->name(), "operator = (value type) is not implemented for parameter arrays! Use operator = (gs_param<Array> type) instead!");
    return *this;
  }
  /// NOT IMPLEMENTED! Get the value of this parameter.
  operator const val_type& () { // const {
    SC_REPORT_WARNING(this->name(), "operator = (value type) is not implemented for parameter arrays! Use operator = (gs_param<Array> type) instead!");
    return getValue();
  }
  /// NOT IMPLEMENTED! Set the value of this parameter.
  void setValue(const val_type &val) {
    SC_REPORT_WARNING(this->name(), "setValue(val) is not implemented for parameter arrays! Use e.g. my_param[0].setValue(val) instead!");
  }
  /// NOT IMPLEMENTED! Returns the value of this parameter.
  const val_type& getValue() const {
    SC_REPORT_WARNING(this->name(), "getValue(val) is not implemented for parameter arrays! Use e.g. my_param[0].getValue(val) instead!");
    return this->my_value;
  }
   */

  // //////////////////////////////////////////////////////////////////// //
  // ///////   set and get with string representation   ///////////////// //

  /// EXPERIMENTAL: Overloads base method. Set the value of this parameter with a string. Special array version!
  /**
   * EXPERIMENTAL functionality! Use the lua config file parser or set the values
   * with operator=(vector) or set the members individually with strings or values.
   *
   * This cannot use the deserialze method because the target is not my_value.
   *
   * @param str  The new values for the array members, represented as a string, e.g. {"memberval" "memberval"}.
   * @return If setting was successful
   */
  bool setString(const std::string &str) {
    SC_REPORT_WARNING(this->name(), "setString for Simple Parameter Arrays is experimental!! Use the lua config file parser or set the values with operator=(vector) or set the members individually with strings or values.");
#ifdef GCNF_ENABLE_GS_PARAM_LOCK
    // Check the lock!
    if (m_locked) {                                                
      GS_PARAM_DUMP("parameter is locked!");                                      
      SC_REPORT_INFO(GCNF_SC_REPORTER(this->getName()), "parameter is locked!");
      return false;                                                               
    }                                                                             
#endif
    std::vector<std::string> vec;
    if (!parse(vec, str)) {
      SC_REPORT_WARNING(this->name(), "setString parsing failed!");
      return false;
    }
    unsigned int got_size = vec.size();

    // Resize this array if needed
    if (got_size != internal_ArrVec.size()) {
      GS_PARAM_ARRAY_DUMP("setString in array new size ("<<got_size<<") not equal existing size ("<<(unsigned int)internal_ArrVec.size()<<"): resize.");
      resize(got_size);
    }

    // Set values
    for (unsigned int i = 0; i < got_size; i++) {
      internal_ArrVec[i]->setString(vec[i]);
    }

    return true;
  }

  // //////////////////////////////////////////////////////////////////// //
  // ///////   virtual conversion methods string <-> value   //////////// //

  /// Overloads gs_param_t<T>::serialize
  std::string serialize(const val_type &val) const {
    std::ostringstream ss;
    ss << "{";
    for (unsigned int i = 0; i < internal_ArrVec.size(); i++) {
      ss << "\""<< internal_ArrVec[i]->getString() << "\"";
      if (i < internal_ArrVec.size()-1) ss << " ";
    }
    ss << "}";
    return ss.str();
  }

  /// Overloads gs_param_t<T>::deserialize in gs_param_t<T>
  /**
   * The deserialze method cannot be implemented because
   * the target is not a taget_val of type val_type.
   * Use setString(str) instead!
   */
  const bool deserialize(val_type &target_val, const std::string& str) {
    // Check the lock!
#ifdef GCNF_ENABLE_GS_PARAM_LOCK
    if (m_locked) {                                                
      GS_PARAM_DUMP("parameter is locked!");                                      
      SC_REPORT_INFO(GCNF_SC_REPORTER(this->getName()), "parameter is locked!");
      return false;                                                               
    }                                                                             
#endif
    return static_deserialize(target_val, str);
  }

  /// Static convertion function called by virtual deserialize and others (e.g. GCnf_API)
  inline static bool static_deserialize(val_type &target_val, const std::string& str) {
    if (str != "")
      SC_REPORT_WARNING("gs_param<Array>", "Deserialize is not implemented for parameter arrays! Please access the members individually!");
    return false;
  }


  // //////////////////////////////////////////////////////////////////// //
  // ///////   array methods   ////////////////////////////////////////// //

  /// Resizes the array and deletes members or creates members with default values.
  /**
   * If sz is smaller than the current array size, the content is reduced to its
   * first sz members, the rest being deleted.
   * If sz is greater than the current array size, the content is expanded by
   * inserting at the end as many default parameters as needed to reach a size
   * of sz elements.
   *
   * @param sz  Size of the array.
   * @return If resize was successfull (or not because e.g. read-only param)
   */
  bool resize(unsigned int sz) {
#ifdef GCNF_ENABLE_GS_PARAM_LOCK
    // Check the lock!
    if (m_locked) {
      GS_PARAM_DUMP("parameter is locked!");
      SC_REPORT_INFO(GCNF_SC_REPORTER(this->getName()), "parameter is locked!");
      return false;
    }                                                                             
#endif
    if (this->make_reject_write_callbacks() == return_value_change_rejected) {
      GS_PARAM_ARRAY_DUMP("resize: reject_write callback rejected resize!");
      return false;
    }
    this->make_pre_write_callbacks();
    // Delete unnecessary members if needed
    if (sz < internal_ArrVec.size()) {
      GS_PARAM_ARRAY_DUMP("resize: size ("<<sz<<") than existing size ("<<(unsigned int)internal_ArrVec.size()<<"): delete remaining members.");
      for (unsigned int i = sz; i < internal_ArrVec.size(); i++) {
        delete internal_ArrVec[i]; internal_ArrVec[i] = NULL;
      }
    }
    unsigned int old_array_size = internal_ArrVec.size();
    // resize the internal vector
    internal_ArrVec.resize(sz);
    // create new members if needed
    if (sz > old_array_size) {
      GS_PARAM_ARRAY_DUMP("resize: size ("<<sz<<") larger than existing size ("<<old_array_size<<"): create new members.");
      for (unsigned int i = old_array_size; i < sz; i++) {
        std::stringstream hierParName_ss;
        hierParName_ss << m_par_name << SC_NAME_DELIMITER << i;
        internal_ArrVec[i] = new gs_param<T>(hierParName_ss.str(), "", true);
        //cout << "create new parameter #"<<i<<" ("<<hierParName_ss.str()<<"), val="<<internal_ArrVec[i]->getString()<<endl;
      }
    }
    // callback for changed size
    this->make_post_write_callbacks();
    return true;
  }

  /// Returns the number of parameter members in this array
  unsigned int size() const {
    return internal_ArrVec.size();
  }

  /// Returns the member at position pos (safe: created if not existing)
  /**
   * Safe operator: If a member is accessed that is not existing
   * it will be created. All members in between the last and the
   * new highes member will be created with default values.
   * (Not in the case of TODO spare arrays.)
   */
  gs_param<T>& at(const unsigned int pos) {
    // create new member(s) if pos is outside the bounds
    if (internal_ArrVec.size() < pos+1) {
      resize(pos+1);
    }
    return *internal_ArrVec.at(pos);
  }

  /// Returns the member at position pos (unsafe)
  /**
   * Unsafe operator. Operator[] is unsafe and allows access only
   * to existing members (undefined behavior for others).
   */
  gs_param<T>& operator[] (const unsigned int pos) const {
    return *internal_ArrVec[pos];
  }

  /// Returns the member at position pos
  /* This resolves the ambiguous compiler error with built-in operator! */
  gs_param<T>& operator[] (const int pos) const {
    return *internal_ArrVec[(unsigned int)pos];
  }

protected:

  /// Initialize the array with default or (priority!) init size.
  /**
   * The init size is read out of the database parameter '<m_par_name>.init_size'.
   *
   * The array size is set to one of the following sizes (with growing priority):
   * - The default array size 10.
   *   Can be modified in the source file using the define DEFAULT_ARRAY_SIZE.
   * - Highest implicit member number array_par_name.member_number
   *   (in database): overwrites default array size. These members need not to be continuously
   *   (the user may set members .0 .1 .2 .100 .101 .2000 which will result in size = 2001).
   * - The constructor array size specified during construction
   *   (overwrites default array size as well as implicit members (except the .init_size).
   * - Implicit parameter array_par_name.init_size in plugin database
   *   (overwrites default and constructor array size).
   *
   * The init_vec values are set as long as the array is big enough.
   *
   * @param user_chosen_size If the size parameter is a user chosen one or the default size
   * @param size             Number of the members that have to be created.
   * @param init_vec         Vector of values that must have the size <size>.
   */
  void init_array(bool user_chosen_size, unsigned int size, const std::vector<T> *init_vec) {
    GS_PARAM_ARRAY_DUMP("init parameter array "<<m_par_name.c_str()<<": with size="<<size<<" (user_chosen="<<user_chosen_size<<")");
    unsigned int new_size;
    // set array size if given e.g. by config file
    bool init_size_set_in_db = false;
    std::string size_par_name = m_par_name + ".init_size";
    if (m_api->existsParam(size_par_name)) {
      m_init_size = new gs_param<unsigned int>(size_par_name, size, true); // value will be set automatically to init size if there is one
      GS_PARAM_ARRAY_DUMP("  init array size in database = "<< m_init_size->getString().c_str());
      new_size = *m_init_size;
      init_size_set_in_db = true;
    }
    // if not given by database set array size to constructor/default size
    else {
      GS_PARAM_ARRAY_DUMP("  no init array size in database, use "<< size);
      new_size = size;
    }
    // higher priority: if implicit members existing, set array size
    // to the maximum member (if less than current array size)
    std::vector<std::string> members = m_api->getParamList(m_par_name, true); // getParamList with childs of childs
    //GS_PARAM_ARRAY_DUMP("  check existing implicit array members");
    unsigned int highest_impl_member = 0;
    for (std::vector<std::string>::iterator iter = members.begin(); iter != members.end(); iter++) {
      std::string no;
      size_t found;
      // name of this (name):                           mymod.mySPA
      // orig string:                                   mymod.mySPA.0.submember.subsubmember
      // 1/ cut off leading name + delimiter:                       0.submember.subsubmember
      // 2/ cut off appended hierarchical names:                    0

      if (iter->length() > m_par_name.length()) { // if the iter can contain the par name (appended name possible)
        // 1/
        found = (*iter).find(m_par_name);
        assert(found != string::npos && found == 0); // getParamList should have returned only params belonging to this array
        no = (*iter).substr(found+1 + m_par_name.length());
        // 2/
        found = no.find(SC_NAME_DELIMITER); // find first delimiter
        if (found != string::npos)
          no = no.substr(0, found);
        //found = (*iter).find_last_of(SC_NAME_DELIMITER);
        //if (found != string::npos) {
          //no = (*iter).substr(found+1);
          if (no.find_last_not_of("01234567890") == string::npos) { // check if number
            unsigned int tmp;
            istringstream ss(no);
            ss >> tmp;
            if (!ss.fail()) {
              //if (tmp > new_size) {
                if (tmp+1 > highest_impl_member) highest_impl_member = tmp+1;
              //}
            }
            else {
              GS_PARAM_ARRAY_DUMP("Failed to convert array member name '"<<no.c_str()<<"' to member number!");
              //SC_REPORT_INFO(name(), "Failed to convert array member name to member number!");
            }
          } // end if number
        //}
      } // end if appended name
    }
    GS_PARAM_ARRAY_DUMP("  Found "<<highest_impl_member<<" implicit array members in database");
    // if more implicit members than new size, resize only if not user chosen size
    if (highest_impl_member > 0 && highest_impl_member != new_size) {
      if (user_chosen_size) {
        GS_PARAM_ARRAY_DUMP("  Warning: different amount of implicit members ("<<highest_impl_member<<") than user chosen size ("<<new_size<<"): not change array size");
      }
      else if (init_size_set_in_db) {
        GS_PARAM_ARRAY_DUMP("  Warning: different amount of implicit members ("<<highest_impl_member<<") than given by init_size in database ("<<new_size<<"): not change array size");
      } else {
        GS_PARAM_ARRAY_DUMP("  Different amount of implicit members ("<<highest_impl_member<<") than given default size ("<<new_size<<"), change array size to "<<highest_impl_member);
        new_size = highest_impl_member;
      }
    }
    // resize and fill array vector
    // (implicit params will automatically overwrite constructor default values)
    internal_ArrVec.resize(new_size);
    this->m_is_simple_array = true;
    for (unsigned int i = 0; i < new_size; i++) {
      std::stringstream hierParName_ss;
      hierParName_ss << m_par_name << SC_NAME_DELIMITER << i;
      if (init_vec != NULL) {
        if (size > i) // set vector init value as long as there is one
          internal_ArrVec[i] = new gs_param<T>(hierParName_ss.str(), init_vec->at(i), true); // gs_param with forced top-level name
        else
          internal_ArrVec[i] = new gs_param<T>(hierParName_ss.str(), "", true); // gs_param with forced top-level name
      } else
        internal_ArrVec[i] = new gs_param<T>(hierParName_ss.str(), "", true); // gs_param with forced top-level name
    }
    GS_PARAM_ARRAY_DUMP("init array of parameter "<<m_par_name.c_str()<<" done\n");
  }

  /// Delete all members and the internal array
  void delete_array() {
    typename std::vector<gs_param<T>*>::iterator it;
    for (it = internal_ArrVec.begin();
         it != internal_ArrVec.end(); it++) {
      delete *it;
    }
    internal_ArrVec.clear();
  }

  /// Parse the string and put the member strings into the vector <vec>
  /**
   * @param  vec  Reference to an empty (!) string vector where the tokens
   *              are put to.
   * @param  str  String that has to be parsed.
   * @return      If parsing succeeded.
   */
  const bool parse(std::vector<std::string> &vec, const std::string &str) const {
    // TODO: EXPERIMENTAL does not work for e.g. floats
    //       Use the lua config file parser or set the values with operator=(vector)
    //       or set the members individually with strings or values.

    //typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    //boost::char_separator<char> sep(" ");
    //tokenizer tok(str, sep);

    typedef boost::tokenizer<> tokenizer;
    tokenizer tok(str);
    for(tokenizer::iterator beg=tok.begin(); beg!=tok.end();++beg){
      vec.push_back(*beg);
      //cout << "Token :"<<*beg<<endl;
    }
    return true;
  }

};



