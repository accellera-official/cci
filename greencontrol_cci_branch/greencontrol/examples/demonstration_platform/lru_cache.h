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

#ifndef __LRU_CACHE_H__
#define __LRU_CACHE_H__

#include "demo_globals.h"

// SimpleBus API
#include "greenbus/api/simplebusAPI.h"

// GreenControl and GreenConfig
#include "greencontrol/config.h"


/// default cache specifications (in 32bit words)
#define LRU_CACHE_SIZE      1024
#define LRU_CACHE_LINE_SIZE 128
#define LRU_CACHE_LINES     8

#define LRU_VERBOSE
#ifdef LRU_VERBOSE
  #define LRU_TRACE(msg, ...) { printf("@%s /%d (%s): ", sc_core::sc_time_stamp().to_string().c_str(), (unsigned)sc_core::sc_delta_count(), name()); printf(msg, ##__VA_ARGS__); printf("\n"); }
#else
  #define LRU_TRACE(msg, ...)
#endif

/// LRU (Least Recently Used) Cache with configurable parameters, word addressable.
/**
 * Cache with:
 * - write-through or write-back
 * - replacement of the least recently used cache line
 *
 * GC parameters (gs_param) for sizes are not used directly in the cache to allow 
 * runtime changes. If the cache size parameter changes during runtime, all lines are
 * written back and the cache is resetted and new sized. 
 * When editing size: first change cache_line_size and cache_lines, THEN cache_size.
 *
 * The parameter param_write_back is used directly. So a change takes directly 
 * effect on the behavior (write back or write through). 
 * If the behavior is changed from write back to write through the cache will reset
 * itself and write back dirty lines.
 */
class LRU_cache
: public sc_core::sc_module
{

  typedef gs_uint8   word_size;
  typedef unsigned long  addr_size;

  /// Cache line
  struct CacheLine
  {
    bool used;                  // marks if this line is in use (represents a memory line)
    addr_size address;
    std::vector<word_size> *data;
    unsigned int used_cnt;      // increment when read from or write to line
    bool dirty;                 // mark line as dirty
  };
    
private:
  /// Parameter for cache size in words (LRU_cache::word_size)
  gs::gs_param<unsigned int> param_cache_size;
  /// Parameter for cache-line-size in words (LRU_cache::word_size)
  gs::gs_param<unsigned int> param_cache_line_size;
  /// Parameter for number of cache-lines
  gs::gs_param<unsigned int> param_cache_lines;

  /// Parameter if to use write back (true) or write through (false)
  gs::gs_param<bool> param_write_back;

  /// Cache size in words (LRU_cache::word_size)
  unsigned int cache_size;
  /// Cache-line-size in words (LRU_cache::word_size)
  unsigned int cache_line_size;
  /// Number of cache-lines
  unsigned int cache_lines;

  /// Clock perios (ns)
  unsigned int clk_period;
  /// Read from cache wait states
  unsigned int read_ws;
  /// Write to cache wait states
  unsigned int write_ws;
  
public:
  /// Event which is notified by the callback function and starts the sc_thread create_cache_starter
  sc_event create_cache_event;
  
public:
  
  GC_HAS_CALLBACKS();
  
  SC_HAS_PROCESS(LRU_cache);
	
  /// Constructor with pointer to memory access
  /**
   * @param name  Name of the module.
   * @param memoryPort  Pointer of type simplebus_if which may be a 
   *                    SimpleBus master_port which connects to the memory.
   */
  LRU_cache(sc_core::sc_module_name name, tlm::simplebusMasterPort* memoryPort)
    : sc_core::sc_module(name),
      param_cache_size("param_cache_size", LRU_CACHE_SIZE),
      param_cache_line_size("param_cache_line_size", LRU_CACHE_LINE_SIZE),
      param_cache_lines("param_cache_lines", LRU_CACHE_LINES),
      param_write_back("param_write_back", true),
      clk_period(10),
      read_ws(5),
      write_ws(3)
  {
    mMemoryPort = memoryPort;
    create_cache(false);
    
    // register callback function for changes on size. When editing size: first change cache_line_size and cache_lines, THEN cache_size
    GC_REGISTER_PARAM_CALLBACK(&param_cache_size, LRU_cache, parameter_changed_callback);
    // register callback function for changes on behavior
    GC_REGISTER_PARAM_CALLBACK(&param_write_back, LRU_cache, parameter_changed_callback);
    
    // Thread to start the create_cache-call
    SC_THREAD(create_cache_starter);
  }
  
  ~LRU_cache() {
    GC_UNREGISTER_CALLBACKS();
  }
  
  /// Read one data word from cache.
  virtual bool read(word_size &data, const addr_size addr);
  
  /// Writes one data word to cache.
  virtual bool write(const word_size &data, const addr_size addr);
  
  /// Clears up the cache: writes back all dirty lines.
  void clear_up_cache();
  
  /// Callback functions for changed size-parameters
  /**
   * When editing size: first change cache_line_size and cache_lines, THEN cache_size
   * because only cache_size makes callback!
   */
  void parameter_changed_callback(gs::gs_param_base& par);
  
  /// (if true: Deletes existing cache lines,) creates new cache lines and checks the sizes.
  /**
   * @param deleteFirst  true:  First write back all dirty lines and delete the existing cache lines.
   *                     false: First call during construction.
   */
  void create_cache(bool deleteFirst);
  
  /// SC_THREAD which performs create_cache-call when cache is disabled or cache sizes change
  void create_cache_starter();

private:
  /// Reads the block out of underlying memory which has the address <address> in it.
  /**
   * LRU line is replaced.
   *
   * @return The number of the line which contains the read memory block.
   *         -1 if read failed!
   */
  int getLine(const unsigned long address);

  /// Writes back the line <wbline>.
  bool writeLineBack(const unsigned int wbline);
  
  /// Reads the memory address <mem_addr> to the line <rline>.
  bool readLine(const unsigned long mem_addr, const unsigned int rline);
  
  /// Returns the address of the Line where the address <addr> is inside.
  addr_size getLineAddress(const addr_size addr);
  
  /// Returns if the address <addr> is inside the line <line>.
  bool isAddressInLine(const unsigned int line, const addr_size addr);

  /// Debug function shows cache.
  void show_cache();

protected:
  /// Vector with pointers to the cache lines.
  std::vector<CacheLine*> cache_mem;
  
  /// Port to the memory.
  tlm::simplebusMasterPort *mMemoryPort;
  
private:
  sc_mutex create_cache_lock;
  
};

#endif
