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

#include "lru_cache.h"
#include "greencontrol/core/helpfunctions.h"

void LRU_cache::create_cache_starter() {
  while (true)
  {
    wait(create_cache_event);
    LRU_TRACE("SC_THREAD create_cache_starter starts create_cache()!");
    create_cache(true);
  }
}

void LRU_cache::clear_up_cache() {
      LRU_TRACE("Clear up cache.");
      // Write back all lines (if needed)
      for (unsigned int i = 0; i < cache_lines; i++) {
        if (cache_mem[i]->dirty) {
          if (!writeLineBack(i))
            SC_REPORT_ERROR(name(), "Error writing back cache line");
        }
      }
}

void LRU_cache::create_cache(bool deleteFirst) {
  LRU_TRACE("create_cache: try to lock mutex create_cache_lock");
  create_cache_lock.lock();
  LRU_TRACE("create_cache: locked mutex create_cache_lock");

    if (deleteFirst) {
      // Write back all lines
      clear_up_cache();
      // delete cache lines if they exist
      LRU_TRACE("delete exiting cache lines");
      for (unsigned int i = 0; i < cache_lines; i++) {
        if (cache_mem[i] != NULL) {
          if (cache_mem[i]->data != NULL) {
            delete cache_mem[i]->data;
            cache_mem[i]->data = NULL;
          }
          delete cache_mem[i];
          cache_mem[i] = NULL;
        }
      }
    }
        
    // Check if size and #lines and line size matches
    //   if cache_size does not fit into lines
    //   or cache_size bigger than lines plus one line_size-1 
    if (param_cache_lines * param_cache_line_size < param_cache_size 
       || param_cache_lines * param_cache_line_size > param_cache_size + param_cache_line_size - 1) {
      SC_REPORT_ERROR(name(), "Cache sizes are wrong. Lines, line size and cache size doesn't match.");
    }
    
    // Set new sizes:
    cache_size = param_cache_size;
    cache_line_size = param_cache_line_size;
    cache_lines = param_cache_lines;
    
    // Create new cache lines
    LRU_TRACE("create_cache: Create new cache lines: %i", cache_lines);
    cache_mem.resize(cache_lines);
    for (unsigned int i = 0; i < cache_lines; i++) {
      std::vector<word_size> *d = new std::vector<word_size>;//[cache_line_size];
      d->resize(cache_line_size);
      CacheLine *l = new CacheLine();
      l->data = d;
      cache_mem[i] = l;
    }

    show_cache();

  create_cache_lock.unlock();
  LRU_TRACE("create_cache: unlocked mutex create_cache_lock");
}

bool LRU_cache::read(word_size &data, const addr_size addr) {
  LRU_TRACE("read: try to lock mutex create_cache_lock");
  create_cache_lock.lock();
  LRU_TRACE("read: locked mutex create_cache_lock");
  // check if word is in one of the (used!) lines
  int lineToUse = -1;
  for (unsigned int i = 0; i < cache_lines; i++) {
    if (isAddressInLine(i, addr) && cache_mem[i]->used) {
      lineToUse = i;
      break;
    }
  }
  LRU_TRACE("read: word is in line %i", lineToUse);
  wait(read_ws*clk_period, SC_NS);
  
  // if not, getLine
  bool ret = true;
  if (lineToUse < 0) {
    lineToUse = getLine(addr);
    if (lineToUse < 0)
      ret = false;
    LRU_TRACE("read: word is in line %i", lineToUse);
  }
  
  if (lineToUse > -1) {
    // read from line (with increment of used counter)
    data = cache_mem[lineToUse]->data->at( addr - cache_mem[lineToUse]->address );
    cache_mem[lineToUse]->used_cnt++;
  }
  create_cache_lock.unlock();
  LRU_TRACE("read: unlocked mutex create_cache_lock");
  return ret;
}
  
bool LRU_cache::write(const word_size &data, const addr_size addr) {
  LRU_TRACE("write(%u, addr=0x%x)", data, (unsigned int)addr);
  LRU_TRACE("write: try to lock mutex create_cache_lock");
  create_cache_lock.lock();
  LRU_TRACE("write: locked mutex create_cache_lock");
  wait(write_ws*clk_period, SC_NS);
  
  // check if word is in one of the (used!) lines
  int lineToUse = -1;
  for (unsigned int i = 0; i < cache_lines; i++) {
    if (isAddressInLine(i, addr) && cache_mem[i]->used) {
      lineToUse = i;
      break;
    }
  }
  LRU_TRACE("write: word is in line %i", lineToUse);
  
  // if not, getLine
  bool ret = true;
  if (lineToUse < 0) {
    lineToUse = getLine(addr);
    if (lineToUse < 0)
      ret = false;
  }
  
  if (lineToUse > -1) {
    LRU_TRACE("write: write word to address 0x%x in line %i (position in line: %u)", (unsigned int)addr, lineToUse, (unsigned int)(addr - cache_mem[lineToUse]->address));

    // write to that line (with increment of used counter and dirty tag)
    cache_mem[lineToUse]->data->at( addr - cache_mem[lineToUse]->address ) = data;
    cache_mem[lineToUse]->used_cnt++;
    cache_mem[lineToUse]->dirty = true;
    // If not write back, write through immediately
    if (!param_write_back) {
      LRU_TRACE("write: no write back -> write back immediately");
      return writeLineBack(lineToUse);
    }
  }
  create_cache_lock.unlock();
  LRU_TRACE("write: unlocked mutex create_cache_lock");
  return true;  
}



int LRU_cache::getLine(const addr_size address) {
  // find LRU line
  unsigned int lineToUse;
  unsigned int recentestUsage = 0xFFFFFFFF;
  for (unsigned int i = 0; i < cache_lines; i++) {
    if (!cache_mem[i]->used) { // use line if unused
      lineToUse = i;
      break;
    }
    if (cache_mem[i]->used_cnt < recentestUsage) {
      recentestUsage = cache_mem[i]->used_cnt;
      lineToUse = i;
    }
  }
  show_cache();
  LRU_TRACE("getLine: Find least recently used line: %i", lineToUse);
  
  // write back line if needed
  if (cache_mem[lineToUse]->dirty) {
    if (!writeLineBack(lineToUse))
      SC_REPORT_WARNING(name(), "Write back to memory failed!");
  }
  
  // get line out of memory
  if (!readLine(address, lineToUse))
    return -1;
  
  return lineToUse;
}

bool LRU_cache::writeLineBack(const unsigned int wbline) {
  LRU_TRACE("writeLineBack: write back line %u to memory and unset used, used_cnt, dirty.", wbline);
  cache_mem[wbline]->used = false;
  cache_mem[wbline]->used_cnt = 0;
  cache_mem[wbline]->dirty = false;
  bool succ = mMemoryPort->
          write(*(cache_mem[wbline]->data), 
                static_cast<gs_uint32>(cache_mem[wbline]->address), 
                cache_line_size);
  return succ;
}

bool LRU_cache::readLine(const addr_size mem_addr, const unsigned int rline) {
  LRU_TRACE("readLine");
  addr_size lineAddress = getLineAddress(mem_addr);
  cache_mem[rline]->used = true;
  cache_mem[rline]->address = lineAddress;
  LRU_TRACE("readLine: read line %i (%u words) for address 0x%x out of memory (at address 0x%x)", rline, cache_line_size, (unsigned int)mem_addr, (unsigned int)lineAddress);
  bool succ = mMemoryPort->
          read(*(cache_mem[rline]->data), 
               static_cast<gs_uint32>(lineAddress), cache_line_size);
  return succ;
}

LRU_cache::addr_size LRU_cache::getLineAddress(const addr_size addr) {
  addr_size lineAddress;
  lineAddress = addr / cache_line_size;
  lineAddress = lineAddress * cache_line_size;
  LRU_TRACE("getLineAddress: Line with address 0x%x begins at memory address 0x%x", (unsigned int)addr, (unsigned int)lineAddress);
  return lineAddress;
}

bool LRU_cache::isAddressInLine(const unsigned int line, const addr_size addr) {
  return ( 
            addr >= cache_mem[line]->address 
            && addr < cache_mem[line]->address + cache_line_size  
         );
}

void LRU_cache::parameter_changed_callback(gs::gs_param_base& par) {
  if (! par.is_destructing()) {
    LRU_TRACE("Size parameter callback function %s = %s", par.getName().c_str(), par.getString().c_str());
    create_cache_event.notify();
  } 
  else {
    DEMO_TRACE(sc_core::sc_module::name(), "parameter_changed_callback, %s param is destructing!", par.getName().c_str())
  }
}

void LRU_cache::show_cache() {
  #ifdef LRU_VERBOSE
    cout << "          LRU_Cache " << name() << ":"<<endl;
    for (unsigned int i = 0; i < cache_lines; i++) {
      cout << "            line #" << i << endl;
      cout << "              used:     " << cache_mem[i]->used     << endl;
      cout << "              addr:     0x" << (hex) << cache_mem[i]->address << (dec) << endl;
      cout << "              used_cnt: " << cache_mem[i]->used_cnt << endl;
      cout << "              dirty:    " << cache_mem[i]->dirty    << endl;
      cout << "              data:     <data>"; gs::ctr::show_pure_vector(*(cache_mem[i]->data), cache_line_size); cout << "</data>" <<endl;
    }
  #endif
}
