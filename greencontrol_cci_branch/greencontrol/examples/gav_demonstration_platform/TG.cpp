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



void TG::action() {
  DEMO_TRACE(name(), "Demo action");
  show_parameters();

  if (loops == 0) return;
  
  static gs_uint32 running=0;
  running++;

  updateTarget();    

  updateWrite();
  
  updateRead();

  wait(SC_ZERO_TIME);
  
  while(loops>0) {
    DEMO_TRACE(name(), "loop %u", loops.getValue());
    updateTarget();    

    if (doWrite()) {
      write();
      updateWrite();
#ifdef DEMO_VERBOSE
      cout << endl;
#endif
    }
    
    if (doRead()) {
      read();
      updateRead();
#ifdef DEMO_VERBOSE
      cout << endl;
#endif
    }

  }
  running--;
  //if (running==0) sc_stop();
}


inline void TG::read() {
  //DEMO_TRACE(sc_core::sc_module::name(), "Reading %d bytes from address 0x%x", (gs_uint32)read_size, (gs_uint32)read_addr[target]);
#ifdef DEMO_VERBOSE
  tm.startMeasure();
#endif
  gs_uint64 address = 0;
  gs_uint32 tmp_read_size;
  for (gs_uint32 i=0; i<read_count; i++) {
#ifdef DEMO_VERBOSE
    cout << endl;
#endif
    tmp_read_size = read_size;
    // random address within the target
    if (read_addr_size[target] <= read_size) {
      tmp_read_size = read_addr_size[target];
      address = read_addr[target];
      //DEMO_TRACE(name(), "limited read size to %u", tmp_read_size);
    } else {
      address = getRandomAddress(read_addr[target], read_addr[target]+read_addr_size[target]-read_size);
    }
    read(tmp_read_size, address);
    sc_core::wait(getRandom(delay_max)*clk_period, SC_NS);     
    //sc_core::wait(delay_max*clk_period, SC_NS);
  }
#ifdef DEMO_VERBOSE
  cout << name() << ": read (# transactions, bytes, us, cycles), " << dec << (gs_uint32)read_count << ", " << dec << (gs_uint32)read_size << ", ";
  tm.stopMeasure();
#endif
}

inline void TG::read(gs_uint32 read_size, gs_uint64 addr) {
#ifdef DEMO_VERBOSE
  cout << endl;
  DEMO_TRACE(name(), "Reading %d bytes from address 0x%x", (gs_uint32)read_size, (gs_uint32)addr);
  cout << endl;
#endif
  if (mData.size() < read_size) {
    mData.resize(init_size, 42);
    sc_assert(read_size<=mData.size());
  }
  master_port.read(mData, addr, read_size);
  //gs::ctr::show_vector_nosep<gs_uint8>(mData, read_size);
  DEMO_TRACE(name(), "Reading finished");
}

/// Function performing write_count writes
/**
 * Calls write(size, addr) with a random addr matching into the
 * current target.
 */
inline void TG::write() {
  //DEMO_TRACE(sc_core::sc_module::name(), "Sending %d bytes to address 0x%x", (gs_uint32)write_size, (gs_uint32)write_addr[target]);
#ifdef DEMO_VERBOSE
  tm.startMeasure();
#endif
  gs_uint64 address = 0;
  gs_uint32 tmp_write_size;
  for (gs_uint32 i=0; i<write_count; i++) {
#ifdef DEMO_VERBOSE
    cout << endl;
#endif
    tmp_write_size = write_size;
    // random address within the target
    if (write_addr_size[target] <= write_size) {
      tmp_write_size = write_addr_size[target];
      address = write_addr[target];
      //DEMO_TRACE(name(), "limited write size to %u", tmp_write_size);
    } else {
      address = getRandomAddress(write_addr[target], write_addr[target]+write_addr_size[target]-write_size);
    }
    write(tmp_write_size, address);
    sc_core::wait(getRandom(delay_max)*clk_period, SC_NS);
    //sc_core::wait(delay_max*clk_period, SC_NS);
  }
  static gs_uint32 write_count_overall = 0;
  write_count_overall+=write_count;
#ifdef DEMO_VERBOSE
  cout << name() << ": write (# transactions, bytes, us, cycles), " << dec << (gs_uint32)write_count << "("<<write_count_overall<<"), " << dec << (gs_uint32)write_size << ", ";
  tm.stopMeasure();
#endif
}

/// Creates dummy write data and writes it to the master_port
inline void TG::write(gs_uint32 write_size, gs_uint64 addr) {
#ifdef DEMO_VERBOSE
  cout << endl;
  DEMO_TRACE(name(), "Sending %d bytes to address 0x%x", (gs_uint32)write_size, (gs_uint32)addr);
  cout << endl;
#endif
  if (mData.size() < write_size) {
    mData.resize(init_size, 42);
    sc_assert(write_size<=mData.size());
  }
  // Fill data with test values (from counter)
  for (unsigned int i = 0; i<write_size; i++) {
    mData[i] = letter_digit.next();
  }
  
  //gs::ctr::show_vector_nosep<gs_uint8>(mData, write_size);
  master_port.write(mData, addr, write_size);
  DEMO_TRACE(name(), "Sending finished");
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

/// get a random
inline gs_uint32 TG::getRandom(gs_uint32 max) {  
  return 1+ (gs_uint32)(max*(rand()/(RAND_MAX + 1.0)));
}

/// get a random address between min and max (containing min and max) 
inline gs_uint64 TG::getRandomAddress(gs_uint64 min, gs_uint64 max) {  
  assert(max >= min);
  gs_uint64 ret = min+ (gs_uint64)((max-min+1)*(rand()/(RAND_MAX + 1.0)));
  //cout << "getRandomAddress("<< min<<", "<<max<<")="<<ret<<endl;
  return ret;
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
  if (target >= target_names.size())
    target = 0;
#ifdef DEMO_VERBOSE
  cout << endl<<"("<<name()<<") Switch to target #"<< target<<" ("<<target_names.at(target)<<"):"<<endl;
#endif
}
