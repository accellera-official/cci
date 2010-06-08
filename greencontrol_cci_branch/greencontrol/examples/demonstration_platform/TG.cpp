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

#include "TG.h"

/*void TG::action() {

  wait(100, SC_NS);

  // Values for addressing
  unsigned int pciedevice_base_address;

  // convert base_address value from string to unsigned integer  
  std::istringstream ss( m_GCnf_Api->getParam("PCIeDevice2.base_address") );
  ss >> pciedevice_base_address;

  // set target address
  MAddr target_addr;

  // //////// Initial read and write tests //////////////////////

  // Read and write from Memory
  target_addr = 0x200;
  write(12, target_addr);

  target_addr = 0x220;
  write(12, target_addr);

  target_addr = 0x200;
  read(12, target_addr);


  // Read and write from PCIeDevice
  target_addr = 0x120;
  write(12, target_addr);

  target_addr = 0x120;
  write(100, target_addr);

  target_addr = 0x100;
  write(255, target_addr);
  target_addr = 0x100;
  read(255, target_addr);

  target_addr = 0x1FF;
  write(1, target_addr);
  read(1, target_addr);

  // //////// Random reads and writes //////////////////////

}*/


void TG::cache_test() {
  wait(100, SC_NS);

  // Values for addressing
  unsigned int pciedevice_base_address;

  // convert base_address value from string to unsigned integer  
  std::istringstream ss( m_GCnf_Api->getParam("PCIeDevice2.base_address") );
  ss >> pciedevice_base_address;

  // set target address
  tlm::MAddr target_addr;

  // //////// Initial read and write tests //////////////////////

  // Read and write from Memory
  target_addr = 0x200;
  write(12, target_addr);

  target_addr = 0x220;
  write(12, target_addr);

  target_addr = 0x200;
  read(12, target_addr);

  target_addr = 0x2FA;
  write(260, target_addr);


  // /// Read and write from PCIeDevice
  // use more than one cache line and not matching bounds
  target_addr = 0x120;
  write(100, target_addr);
  read (100, target_addr);

  target_addr = 0x120;
  write(100, target_addr);

  target_addr = 0x100;
  write(255, target_addr);
  read (255, target_addr);

  target_addr = 0x1FF;
  write(1, target_addr);
  read (1, target_addr);

  DEMO_TRACE(name(), "Change cache sizes!");
  m_GCnf_Api->getPar("Cache1.LRUCache.param_cache_lines")->setString("2");
  // old v0.2: m_GCnf_Api->setParam("Cache1.LRUCache.param_cache_lines", "2");
  m_GCnf_Api->getPar("Cache1.LRUCache.param_cache_line_size")->setString("64");
  // old v0.2: m_GCnf_Api->setParam("Cache1.LRUCache.param_cache_line_size", "64");

  // This change activates callback
  m_GCnf_Api->getPar("Cache1.LRUCache.param_cache_size")->setString("128");
  // old v0.2: m_GCnf_Api->setParam("Cache1.LRUCache.param_cache_size", "128");
  // The size changes will take some time!!

  DEMO_TRACE(name(), "Read from cache");
  // ////// read out of Memory which needs write-back
  target_addr = 0x2FA;
  read(260, target_addr);

  DEMO_TRACE(name(), "Disable cache!");
  // ////// disable cache and read out from memory
  m_GCnf_Api->getPar("Cache1.enable_cache")->setString("false");
  // old v0.2: m_GCnf_Api->setParam("Cache1.enable_cache", "false");
  // This will take some time to make write backs!
  
  wait(1, SC_MS);

  target_addr = 0x2FA;
  read(260, target_addr);

  // enable and read again
  DEMO_TRACE(name(), "Enable cache!");
  m_GCnf_Api->getPar("Cache1.enable_cache")->setString("true");
  // old v0.2: m_GCnf_Api->setParam("Cache1.enable_cache", "true");
  wait(1, SC_MS);

  read(260, target_addr);

}

void TG::action() {
  DEMO_TRACE(name(), "Demo action");
  show_parameters();
  
  // Make the cache tests
#ifdef MAKE_CACHE_TEST
  cache_test();
#endif

  static gs_uint32 running=0;
  running++;

  updateTarget();    

  updateWrite();
  
  updateRead();

  while(loops>0) {
    DEMO_TRACE(name(), "loop %u", loops.getValue());
    updateTarget();    

    if (doWrite()) {
      write();
      updateWrite();
      cout << endl;
    }
    
    if (doRead()) {
      read();
      updateRead();
      cout << endl;
    }

  }
  running--;
  //if (running==0) sc_stop();
}


inline void TG::read(gs_uint32 read_size, gs_uint64 addr) {
  DEMO_TRACE(name(), "Reading %d bytes from address 0x%x", (gs_uint32)read_size, (gs_uint32)addr);
  if (mData.size() < read_size) {
    mData.resize(init_size, 42);
    sc_assert(read_size<=mData.size());
  }
  master_port.read(mData, addr, read_size);
  gs::ctr::show_vector_nosep<gs_uint8>(mData, read_size);
  DEMO_TRACE(name(), "Reading finished");
}

inline void TG::write(gs_uint32 write_size, gs_uint64 addr) {
  DEMO_TRACE(name(), "Sending %d bytes to address 0x%x", (gs_uint32)write_size, (gs_uint32)addr);
  if (mData.size() < write_size) {
    mData.resize(init_size, 42);
    sc_assert(write_size<=mData.size());
  }
  // Fill data with test values (from counter)
  for (unsigned int i = 0; i<write_size; i++) {
    mData[i] = letter_digit.next();
  }
  
  gs::ctr::show_vector_nosep<gs_uint8>(mData, write_size);
  master_port.write(mData, addr, write_size);
  DEMO_TRACE(name(), "Sending finished");
}



inline void TG::read() {
  //DEMO_TRACE(sc_core::sc_module::name(), "Reading %d bytes from address 0x%x", (gs_uint32)read_size, (gs_uint32)read_addr[target]);
  if (do_time_measure) tm.startMeasure();
  for (gs_uint32 i=0; i<read_count; i++) {
    read(read_size, read_addr[target]);
    sc_core::wait(getRandom(delay_max)*clk_period, SC_NS);     
    //sc_core::wait(delay_max*clk_period, SC_NS);
  }
  cout << name() << ": read (# transactions, bytes, us, cycles), " << dec << (gs_uint32)read_count << ", " << dec << (gs_uint32)read_size << ", ";
  if (do_time_measure) tm.stopMeasure();
  else cout << "omitted";
}

inline void TG::write() {
  //DEMO_TRACE(sc_core::sc_module::name(), "Sending %d bytes to address 0x%x", (gs_uint32)write_size, (gs_uint32)write_addr[target]);
  if (do_time_measure) tm.startMeasure();
  for (gs_uint32 i=0; i<write_count; i++) {
    write(write_size, write_addr[target]);
    sc_core::wait(getRandom(delay_max)*clk_period, SC_NS);
    //sc_core::wait(delay_max*clk_period, SC_NS);
  }
  static gs_uint32 write_count_overall = 0;
  write_count_overall+=write_count;
  cout << name() << ": write (# transactions, bytes, us, cycles), " << dec << (gs_uint32)write_count << "("<<write_count_overall<<"), " << dec << (gs_uint32)write_size << ", ";
  if (do_time_measure) tm.stopMeasure();
  else cout << "omitted";
}

inline bool TG::doWrite() {
  return write_count>0;
}

inline bool TG::doRead() {  
  return read_count>0;
}

/// update parameter values after write
inline void TG::updateWrite() {
  loops-=1; // global transaction counter
  write_size+=write_inc;
  if (write_count_rnd==0) {
    write_count+=write_count_inc;  
  } else {
    write_count=getRandom(write_count_rnd);
  }
} 

/// update parameter values after read
inline void TG::updateRead() {
  loops-=1;  // global transaction counter
  read_size+=read_inc;
  if (read_count_rnd==0) {
    read_count+=read_count_inc;
  } else {
    read_count=getRandom(read_count_rnd);
  }
}

/// get a random delay (configure range with parameter delay_max)
inline gs_uint32 TG::getRandom(gs_uint32 max) {  
  return 1+ (gs_uint32)(max*(rand()/(RAND_MAX + 1.0)));
}

void TG::end_of_simulation() {
  //cout << name() << ": Transaction count="<<tcount;
}

void TG::show_parameters() {
  cout << "parameters:" << endl
       << "  init_size  = " << init_size << endl
       << "  write_size = " << write_size << endl
       << "  read_size  = " << read_size << endl
    //<< "  write_addr = " << write_addr << endl
    //<< "  read_addr  = " << read_addr << endl
       << "  clk_period = " << clk_period << endl
       << "  write_inc  = " << write_inc << endl
       << "  read_inc   = " << read_inc << endl
       << "  loops      = " << loops << endl
       << "  read_count      = " << read_count << endl
       << "  write_count     = " << write_count << endl
       << "  write_count_inc = " << write_count_inc << endl
       << "  read_count_inc  = " << read_count_inc << endl
       << "  write_count_rnd = " << write_count_rnd << endl
       << "  read_count_rnd  = " << read_count_rnd << endl
       << "  delay_max       = " << delay_max << endl<<flush;
}

void TG::updateTarget() {
  target++;
  if (target >= sizeof(write_addr)/sizeof(write_addr[0])) target = 0;
  cout << endl<<"Switch to target #"<< target<<":"<<endl;
}
