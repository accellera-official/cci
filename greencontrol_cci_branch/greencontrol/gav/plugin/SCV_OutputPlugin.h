//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2007-2009 : GreenSocs Ltd
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

#ifndef __SCV_OutputPlugin_h__
#define __SCV_OutputPlugin_h__

// ----------------- USER DEFINES ----------------------------------------- //

// ** Global scv behavior

/// use only one stream for all SCV_OutputPlugin instances, see class description
#define ONLY_ONE_GLOBAL_STREAM
/// use only one database instance (recommended)
#define ONLY_ONE_GLOBAL_DATABASE

// ** Transaction behavior

/// if to use transaction containing the correct types (default: false!!)
//  If false: ALL values will be presented as strings!!
#define USE_CORRECT_TYPE_TRANSACTIONS false

/// define if using a tool which cannot handle strings in transactions (e.g. ModelSim) (default: NOT defined)
//#define DO_NOT_SUPPORT_STRING_TRANSACTIONS

/// To choose if the transaction should contain a name field (type string), 
//  Automatically disabled when DO_NOT_SUPPORT_STRING_TRANSACTIONS is defined.
//  This is not needed because the transaction generator is already named with
//  the parameter name. Only needed if a tool does not show that name.
//  (default: not defined)
//#define USE_STRING_NAMES

// ** change defines for ModelSim
#ifdef MTI_SYSTEMC
# define DO_NOT_SUPPORT_STRING_TRANSACTIONS
# undef USE_CORRECT_TYPE_TRANSACTIONS
# define USE_CORRECT_TYPE_TRANSACTIONS true
#endif

// ** defaults for CoWare
#ifdef COWARE
#  ifdef USE_STRING_NAMES
#    undef USE_STRING_NAMES  // do not show string names: would be double information in MSC/Waveform View
#  endif
#endif
// ------------------------------------------------------------------------ //


#ifdef USE_STRING_NAMES
# ifndef DO_NOT_SUPPORT_STRING_TRANSACTIONS
#  define IF_USE_STRING_NAMES(arg) arg
# else
#  define IF_USE_STRING_NAMES(arg)
#  undef USE_STRING_NAMES
# endif
#else
# define IF_USE_STRING_NAMES(arg)
#endif


#include <time.h>
#include <stdio.h>
#include <map>
#include "OutputPlugin_base.h"
#include "gav_plugin.h"
#include <scv.h>
#ifdef COWARE
#  include <cwrScvInterface.h>
#endif


  namespace gs {  
    namespace av { 
      
      /// Structure which is the transaction data to be outputted by scv transaction generators
      template<typename T> 
      class param_struct { 
      public:              
        IF_USE_STRING_NAMES(std::string  nam;)  
        T val;             
      };
      
  } }

#define CREATE_SCV_TRANSACTION(second_type) \
  SCV_EXTENSIONS( ::gs::av::param_struct<second_type> ) {  \
    typedef ::gs::av::param_struct<second_type> type_name; \
  public:                               \
    IF_USE_STRING_NAMES(scv_extensions< std::string > nam;)  \
    scv_extensions< second_type > val;  \
                                        \
    SCV_EXTENSIONS_CTOR( type_name ) {      \
      IF_USE_STRING_NAMES(SCV_FIELD(nam);)  \
      SCV_FIELD(val);                       \
    }                                       \
  };

// TODO: all items of enum gs::cnf::Param_type
CREATE_SCV_TRANSACTION(int)
CREATE_SCV_TRANSACTION(unsigned int)
CREATE_SCV_TRANSACTION(bool)
CREATE_SCV_TRANSACTION(double)
CREATE_SCV_TRANSACTION(float)
CREATE_SCV_TRANSACTION(std::string)
CREATE_SCV_TRANSACTION(unsigned long long)
CREATE_SCV_TRANSACTION(unsigned char)
CREATE_SCV_TRANSACTION(sc_bit)
// TODO: arrays??

namespace gs {
namespace av {

  GAV_REGISTER_PLUGIN(5, SCV_STREAM_OUT, SCV_OutputPlugin)

  /// Initializaltion static function returns the scv database
  /**
   * Dependent on <code>define ONLY_ONE_GLOBAL_DATABASE</code> returns one global
   * database for each call (defined) or returns one database instance
   * per call (not defined).
   */
  static scv_tr_db * init_scv_recording() { 
#ifdef ONLY_ONE_GLOBAL_DATABASE
    static scv_tr_db *txdb; 
    static bool initialized = false;
    if (!initialized) {
      GAV_DUMP_N("static init_scv_recording", "scv database not yet initialized, create global scv database.");
      initialized = true;
#else
      GAV_DUMP_N("static init_scv_recording", "Create local scv database.");
      scv_tr_db *txdb; 
#endif

#ifdef MTI_SYSTEMC // ModelSim
      /* Initialize SCV: */ 
      scv_startup(); 
      /* Tie databases to WLF: */ 
      scv_tr_wlf_init();
      /* Create the new DB: */ 
      txdb = new scv_tr_db("transaction_wlf_db"); 

#elif defined (COWARE) // CoWare
      /* Initialize SCV: */ 
      scv_tr_cwrdb_init();
      /* Create the new DB: */ 
      txdb = new scv_tr_db("transaction_cwr_db");

#else
      /* Tie databases to text */
      scv_tr_text_init();
      /* Create the new DB: */ 
      txdb = new scv_tr_db("transaction_text_db"); 
#endif

      if (txdb != NULL) 
        scv_tr_db::set_default_db(txdb); // make the database the default one
      else
        SC_REPORT_ERROR("scv init", "Error: no database!");
      return txdb; 
#ifdef ONLY_ONE_GLOBAL_DATABASE
    }
    else {
      GAV_DUMP_N("static init_scv_recording", "scv database already initialized, return that.");
      return txdb;
    }
#endif
  }

  /// A class to own and initialize the global stream pointer
  /**
   * When using <code>define ONLY_ONE_GLOBAL_DATABASE</code> a call to
   * <code>global_scvTransactionStream::get()</code> returns the reference to
   * the global SCV stream pointer.
   */
#ifdef ONLY_ONE_GLOBAL_STREAM
  struct global_scvTransactionStream {
    static scv_tr_stream *& get() {
      static scv_tr_stream * scvTransactionStream = new scv_tr_stream("GreenAVstream", "GreenAV_parameter");
      return scvTransactionStream;
    }
  };
#endif

  /// Output Plugin which observes GreenConfig gs_param parameters and exports changes to scv streams
  /**
   * This is an output plugin to the GreenAV analysis plugin which exports the registered
   * parameter changes to an SCV stream named '<code>GreenAVstream_<OutputPluginName></code>'. Each
   * parameter gets its own transaction generator. When a parameter changes the previous
   * transaction is ended and a new one with the new value is started. Accordingly a 
   * transaction represents the time where the value of a parameter remains unchanged.
   *
   * The database that should be written to can be chosen in the static function 
   * <code>init_scv_recording()</code>. The default is a text database written to the
   * file transaction_text_db. Add <code>define</code> areas to use other databases (like ModelSim).
   *
   * If a tool is only able to handle one stream, use the <code>#define ONLY_ONE_GLOBAL_STREAM</code>.
   * Then all SCV_OutputPlugin instances write to the same stream.
   *
   * The OutputPlugin is able to create transactions with values of the type 'string' (default)
   * The user may 
   * <code>#define USE_CORRECT_TYPE_TRANSACTIONS true</code>
   * to enable transactions containing
   * values of the correct type (as long as the type is listed in the enumeration
   * gs::cnf::Param_type and is implemented here). This is slower because of 
   * some additional switch statements and casts. 
   * @todo: check if using correct data types is really slower or if the not used string output speeds up.
   *
   * By default this plugin will begin logging and writing at the moment the parameter is added for observation.
   * A constructor option allows to start on the automatic end_of_elaboration call.
   * See OutputPlugin_base for details on how to handle and influence running behavior.
   */ 
  class SCV_OutputPlugin 
  : public OutputPlugin_base 
  {
    /// Typedef for value in scvTrGenMap
    typedef std::pair<scv_tr_generator_base*, scv_tr_handle*> scv_gen_handle_pair_type;
    /// Typedef for scvTrGenMap
    typedef std::map<std::string, scv_gen_handle_pair_type > scv_gen_map_type;
    
  public:
    
    /// Constructor with name
    /**
     * Does NOT create the stream! See init() function.
     *
     * @param opname Name of the output plugin.
     * @param ev_listn  Pointer to event listener that may be used by this.
     * @param start_running  If this output plugin (start_running=true, default) starts running immediately on first param adding or (start_running=false:) begins to output on end_of_elaboreation 
     */
    SCV_OutputPlugin(const char* opname, event_listener<OutputPlugin_base> *ev_listn, bool start_running = true)
    : OutputPlugin_base(opname, ev_listn, SCV_STREAM_OUT, start_running)
    , txdb(NULL)
    , m_scv_initialized(false)
    { }
    
  protected:
    /// Init function called on first output and (internally) on first action (which is earlier than the fist output), not called during construction!
    /**
     * Creates the stream etc.
     */
    virtual void init() {
      if (m_scv_initialized) return;
      m_scv_initialized = true;
      GAV_DUMP_N(name(), "Init SCV Output Plugin:");
      txdb = init_scv_recording();
      // globalTrGen("trGen", scvTransactionStream) 
#ifdef ONLY_ONE_GLOBAL_STREAM
      scvTransactionStream = global_scvTransactionStream::get();
#else
      scvTransactionStream = new scv_tr_stream(std::string(std::string("GreenAVstream_").append(constructor_param)).c_str(), 
                                               "GreenAV_parameter");
#endif
      
      GAV_DUMP_N(name(), constructor_param.c_str());
      
      // if to enable the output of correct typed transaction instead of string transactions
      is_type_output_enabled = USE_CORRECT_TYPE_TRANSACTIONS;
      
      GAV_DUMP_N(name(), "Used SCV database: "<<txdb->get_name());
      // start global transaction, all params will be sub-transactions
      /*param_struct ps;
       ps.nam = "glob"; ps.val = 0;
       global_txh = globalTrGen.begin_transaction(ps);*/
      //cnt = 0; cnt2 = 0;
    }

  public:
    
    /// Destructor
    ~SCV_OutputPlugin() {
      if (m_scv_initialized) {
        // end all running transactions!
        scv_gen_handle_pair_type *parpair;
        scv_tr_generator_base *trGenBase;
        scv_tr_handle *trh;
        for (scv_gen_map_type::iterator
             iter = scvTrGenMap.begin(); iter != scvTrGenMap.end(); iter++) {
          parpair = &(iter->second);
          trGenBase = parpair->first;
          trh = parpair->second;
          if (trh != NULL) {
            trh->end_transaction();
            delete trh;
          }
          delete trGenBase;
        }
        //globalTrGen.end_transaction(global_txh);
#ifndef ONLY_ONE_GLOBAL_STREAM
        delete scvTransactionStream;
#endif
      }
    }

    /// Enables the usage of transactions containing correct type for the value field
    /*void enable_type_output() {
      if (scvTrGenMap.size() == 0) {
        is_type_output_enabled = true;
      }
    }*/
    
/* // cast parameter
   gs_param<int> *cpar = static_cast<gs_param<int>* >(&par);
   // set data to be transmitted via scv
   param_struct<int> pst;
   pst.nam = cpar->getName();
   pst.val = cpar->getValue();
   // read pointers out of map for this parameter
   scv_gen_handle_pair_type *parpair 
   = &((scvTrGenMap.find(cpar->getName()))->second);
   scv_tr_generator<param_struct<int> >* trGen =
   dynamic_cast<scv_tr_generator<param_struct<int> >* >(parpair->first);
   scv_tr_handle *trh = parpair->second;
   // end old transaction if there is one
   if (trh != NULL) trGen->end_transaction(*trh);
   else parpair->second = new scv_tr_handle();
   // begin new transaction
   *(parpair->second) = trGen->begin_transaction(pst);//, "param", global_txh);
*/
#define DO_TRANSACTION(typ)       \
  gs_param<typ> *cpar = static_cast<gs_param<typ>* >(&par);  \
  param_struct<typ> pst;          \
  IF_USE_STRING_NAMES(pst.nam = cpar->getName();)      \
  pst.val = cpar->getValue();                        \
  scv_gen_handle_pair_type *parpair                  \
   = &((scvTrGenMap.find(cpar->getName()))->second); \
  scv_tr_generator<param_struct<typ> >* trGen =      \
  dynamic_cast<scv_tr_generator<param_struct<typ> >* >(parpair->first);  \
  assert(trGen != NULL && "This is an scv failure!?"); \
  scv_tr_handle *trh = parpair->second;              \
  if (trh != NULL) trGen->end_transaction(*trh);     \
  else parpair->second = new scv_tr_handle();        \
  *(parpair->second) = trGen->begin_transaction(pst);

    /// Callback function
    void config_callback(gs_param_base &par) {
      // check for destructing parameters: remove them from callback list if removed
      if (par.is_destructing()) {
        GAV_DUMP_N(name(), "callback "<<par.getName().c_str()<<" is destructing. Delete from scv output plugin!");
        remove(par);
      }
      // only perform output action when not paused (or stopped)
      else if (is_running) 
      {
        if (!is_type_output_enabled) {
          GAV_DUMP_N(name(), "callback "<<par.getName().c_str()<<" = "<<par.getString().c_str()<<", write string based to scv transaction generator");
          // set data to be transmitted via scv
          param_struct<std::string> pst;
          IF_USE_STRING_NAMES(pst.nam = par.getName();)
          pst.val = par.getString();
          // read pointers out of map for this parameter
          scv_gen_handle_pair_type *parpair 
                                 = &((scvTrGenMap.find(par.getName()))->second);
          scv_tr_generator<param_struct<std::string> >* trGen =
            dynamic_cast<scv_tr_generator<param_struct<std::string> >* >(parpair->first);
          assert(trGen != NULL && "This is an scv failure!?");
          scv_tr_handle *trh = parpair->second;
          // end old transaction if there is one
          if (trh != NULL) trGen->end_transaction(*trh);
          else parpair->second = new scv_tr_handle();
          // begin new transaction
          *(parpair->second) = trGen->begin_transaction(pst);//, "param", global_txh);
        } // end if (!is_type_output_enabled) 
        else {
          GAV_DUMP_N(name(), "callback "<<par.getName().c_str()<<" = "<<par.getString().c_str()<<", write type based ('"<<par.getTypeString().c_str()<<"') to scv transaction generator");
          // TODO
          switch (par.getType()) {
            case ::gs::cnf::PARTYPE_INT: {
              DO_TRANSACTION(int);
            } break;
            case ::gs::cnf::PARTYPE_UINT: {
              DO_TRANSACTION(unsigned int);
            } break;
            case ::gs::cnf::PARTYPE_BOOL: {
              DO_TRANSACTION(bool);
            } break;
            case ::gs::cnf::PARTYPE_DOUBLE: {
              DO_TRANSACTION(double);
            } break;
            case ::gs::cnf::PARTYPE_FLOAT: {
              DO_TRANSACTION(float);
            } break;
            case ::gs::cnf::PARTYPE_ULONGLONG: {
              DO_TRANSACTION(unsigned long long);
            } break;
            case ::gs::cnf::PARTYPE_UCHAR: {
              DO_TRANSACTION(unsigned char);
            } break;
            case ::gs::cnf::PARTYPE_USHORT: {
              DO_TRANSACTION(unsigned int); // unsigned short not supported by scv!!
            } break;
            case ::gs::cnf::PARTYPE_SC_BIT: {
              DO_TRANSACTION(sc_bit);
            } break;
            case ::gs::cnf::PARTYPE_STRING:
            default: {
#ifndef DO_NOT_SUPPORT_STRING_TRANSACTIONS
              DO_TRANSACTION(std::string);
#else
              SC_REPORT_WARNING(name(), "String in transactions are not allowed because of defined DO_NOT_SUPPORT_STRING_TRANSACTIONS.");
#endif
            }
          }
        } // end if (!is_type_output_enabled) else

      
      }
    }
    
    /// Implements base class function to catch all observed parameters
    virtual bool observe(gs_param_base& par) {
      init();
      if (OutputPlugin_base::observed_param_cb_adapters.find(par.getName()) == OutputPlugin_base::observed_param_cb_adapters.end()) {
        // create transaction generator for this parameter
        if (!is_type_output_enabled) {
          // create transaction generators of value type string
          GAV_DUMP_N(name(), "Create scv transaction generator for parameter '"<<par.getName().c_str()<<"' (value type string)");
          scv_tr_generator<param_struct<std::string> > *trGen = 
            new scv_tr_generator<param_struct<std::string> >(par.getName().c_str(), *scvTransactionStream);
          scvTrGenMap.insert(
             make_pair(  par.getName(), 
                         scv_gen_handle_pair_type(trGen, NULL)  )  );
        } else {
          // create transaction generators of correct value type
          GAV_DUMP_N(name(), "Create scv transaction generator for parameter '"<<par.getName().c_str()<<"' (value type "<<par.getTypeString().c_str()<<")");
          // TODO: one case statement for each item of enum gs::cnf::Param_type!
          switch (par.getType()) {
            case ::gs::cnf::PARTYPE_INT: {
              scv_tr_generator<param_struct<int> > *trGen =
              new scv_tr_generator<param_struct<int> >(par.getName().c_str(), *scvTransactionStream);
              scvTrGenMap.insert(make_pair(  par.getName(), 
                                           scv_gen_handle_pair_type(trGen, NULL)  )  );
            } break;
            case ::gs::cnf::PARTYPE_UINT: {
              scv_tr_generator<param_struct<unsigned int> > *trGen =
              new scv_tr_generator<param_struct<unsigned int> >(par.getName().c_str(), *scvTransactionStream);
              scvTrGenMap.insert(make_pair(  par.getName(), 
                                           scv_gen_handle_pair_type(trGen, NULL)  )  );
            } break;
            case ::gs::cnf::PARTYPE_BOOL: {
              scv_tr_generator<param_struct<bool> > *trGen =
              new scv_tr_generator<param_struct<bool> >(par.getName().c_str(), *scvTransactionStream);
              scvTrGenMap.insert(make_pair(  par.getName(), 
                                           scv_gen_handle_pair_type(trGen, NULL)  )  );
            } break;
            case ::gs::cnf::PARTYPE_DOUBLE: {
              scv_tr_generator<param_struct<double> > *trGen =
              new scv_tr_generator<param_struct<double> >(par.getName().c_str(), *scvTransactionStream);
              scvTrGenMap.insert(make_pair(  par.getName(), 
                                           scv_gen_handle_pair_type(trGen, NULL)  )  );
            } break;
            case ::gs::cnf::PARTYPE_FLOAT: {
              scv_tr_generator<param_struct<float> > *trGen =
              new scv_tr_generator<param_struct<float> >(par.getName().c_str(), *scvTransactionStream);
              scvTrGenMap.insert(make_pair(  par.getName(), 
                                           scv_gen_handle_pair_type(trGen, NULL)  )  );
            } break;
            case ::gs::cnf::PARTYPE_STRING: {
              scv_tr_generator<param_struct<std::string> > *trGen =
              new scv_tr_generator<param_struct<std::string> >(par.getName().c_str(), *scvTransactionStream);
              scvTrGenMap.insert(make_pair(  par.getName(), 
                                           scv_gen_handle_pair_type(trGen, NULL)  )  );
            } break;
            case ::gs::cnf::PARTYPE_ULONGLONG: {
              scv_tr_generator<param_struct<unsigned long long> > *trGen =
              new scv_tr_generator<param_struct<unsigned long long> >(par.getName().c_str(), *scvTransactionStream);
              scvTrGenMap.insert(make_pair(  par.getName(), 
                                           scv_gen_handle_pair_type(trGen, NULL)  )  );
            } break;
            case ::gs::cnf::PARTYPE_UCHAR: {
              scv_tr_generator<param_struct<unsigned char> > *trGen =
              new scv_tr_generator<param_struct<unsigned char> >(par.getName().c_str(), *scvTransactionStream);
              scvTrGenMap.insert(make_pair(  par.getName(), 
                                           scv_gen_handle_pair_type(trGen, NULL)  )  );
            } break;
            case ::gs::cnf::PARTYPE_USHORT: {
              scv_tr_generator<param_struct<unsigned int> > *trGen = // unsigned short not supported by scv!!
              new scv_tr_generator<param_struct<unsigned int> >(par.getName().c_str(), *scvTransactionStream);
              scvTrGenMap.insert(make_pair(  par.getName(), 
                                           scv_gen_handle_pair_type(trGen, NULL)  )  );
            } break;
            case ::gs::cnf::PARTYPE_SC_BIT: {
              scv_tr_generator<param_struct<sc_bit> > *trGen =
              new scv_tr_generator<param_struct<sc_bit> >(par.getName().c_str(), *scvTransactionStream);
              scvTrGenMap.insert(make_pair(  par.getName(), 
                                           scv_gen_handle_pair_type(trGen, NULL)  )  );
            } break;
            default: {
              SC_REPORT_WARNING(name(), "You asked for a not supported parameter type to output. The string representation will be used!");
              scv_tr_generator<param_struct<std::string> > *trGen =
              new scv_tr_generator<param_struct<std::string> >(par.getName().c_str(), *scvTransactionStream);
              scvTrGenMap.insert(make_pair(  par.getName(), 
                                           scv_gen_handle_pair_type(trGen, NULL)  )  );
            }
          }
        }
        // call base class function
        return OutputPlugin_base::observe(par);
      }
      return false;
    }

    virtual bool remove(gs_param_base& par) {
      init();
      GAV_DUMP_N(name(), "Delete param '"<<par.getName().c_str()<<"' from scv output plugin.");
      // call base funtion
      if (OutputPlugin_base::remove(par)) {
        // end transaction and delete param transaction generator and handle
        scv_gen_handle_pair_type *parpair 
                                  = &((scvTrGenMap.find(par.getName()))->second);
        scv_tr_generator_base* trGenBase = parpair->first;
        scv_tr_handle *trh = parpair->second;
        if (trh != NULL) {
          GAV_DUMP_N(name(), "     end transaction due to removal from output plugin");
          trh->end_transaction();
          delete trh;
        }
        delete trGenBase;
          
        // delete tr gen
        scvTrGenMap.erase(scvTrGenMap.find(par.getName()));
        return true;
      }
      return false;
    }
    
  protected:
        
    unsigned int cnt, cnt2; 

    scv_tr_db *txdb;
    scv_tr_stream *scvTransactionStream;

    // Transaction that includes all other parameter transactions as sub-transactions
    /*scv_tr_generator<param_struct> globalTrGen;
    scv_tr_handle global_txh;*/
    scv_tr_handle global_txh;

    // Key: parameter name
    // Value: pair: - first:  transaction generator for the parameter
    //              - second: transaction handle to be able to end a running transaction. May be NULL if no one running
    /// Map containing a transaction generator and the potential running transaction handle for each parameter
    scv_gen_map_type scvTrGenMap; 
    
    /// if to enable the output of correct typed transaction instead of string transactions
    bool is_type_output_enabled;

    /// If the my_init function has been called
    bool m_scv_initialized;
  };
  
} // end namespace av
} // end namespace gs

#endif
