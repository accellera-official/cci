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

#ifndef DDR_RAM_H_
#define DDR_RAM_H_

#include "demo_globals.h"

// SimpleBus API
#include "greenbus/api/simplebusAPI.h"

// GreenControl and GreenConfig
#include "greencontrol/config.h"


/// default timing specification
#define DDR_CLK_PERIOD         10  /* default 100 MHz */
#define DDR_WRITE_WAIT_STATES  7   /* initial wait staits for write access */
#define DDR_READ_WAIT_STATES   11  /* initial wait staits for read access */
#define DDR_SIZE               0x200000 /* initial memory size */
#define DDR_SIMULATION_MODE    0   /* default BA */

/// Simplified transaction level model of a DDR RAM controller
/**
 * SystemC transaction level model of a double data rate random access memory
 * (DDR RAM). This model implements the opb_slave_if interface.
 * @see opb_slave_if
 */
class ddr 
: public sc_core::sc_module, 
  public virtual tlm::simplebus_if
{

public:
  /// clock period for this ddr memory in nanoseconds
  gs::gs_param<gs_uint32> clk_period;
  /// size of this ddr memory in byte
  gs::gs_param<tlm::MBurstLength> size;
  /// write wait states for this ddr memory
  gs::gs_param<gs_uint32> write_ws;
  /// read wait states for this ddr memory
  gs::gs_param<gs_uint32> read_ws;
  /// CT mode behaviour
  gs::gs_param<gs_uint32> simulation_mode; // 0: BA, 1: CT
  
  SC_HAS_PROCESS(ddr);
 
  /**
   * Create a DDR RAM component. 
   * Use the configuration framework to specify the size 
   * of the memory (size), its clock frequency (clk_period),
   * its write wait states (write_ws), and its read wait states 
   * (read_ws). The base address of the memory block always is 0.
   * A burst read takes read_ws*clk_period+burstlength*clk_period ns.
   * A burst write takes write_ws*clk_period+burstlength*clk_period ns.
   */
  ddr(sc_core::sc_module_name name);

  ~ddr();

  /// Read data from memory (@see simplebus_if)
  virtual bool read(std::vector<gs_uint8> &data, const gs_uint32 addr, const gs_uint32 length);
  
  /// Write data to memory (@see simplebus_if)
  virtual bool write(const std::vector<gs_uint8> &data, const gs_uint32 addr, const gs_uint32 length);
  
protected:
  std::vector<gs_uint8> memory;
};


#endif // DDR_RAM_H_
