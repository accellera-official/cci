// LICENSETEXT
// 
//   Copyright (C) 2007 : GreenSocs Ltd
//       http://www.greensocs.com/ , email: info@greensocs.com
// 
//   Developed by :
// 
//   Wolfgang Klingauf, Robert Guenzel
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
// 
//   Mark Burton, Marcus Bartholomeu
//     GreenSocs Ltd
// 
// 
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


// The following code is a copy of:
//  #include "greenbus/utils/gs_datatypes.h"
// <copy>

#ifndef __gs_datatypes_h__
#ifndef __gs_gs_datatypes__
#define __gs_datatypes_h__

/** 
 * \file gs_datatypes.h Default POD datatypes for GreenBus models
 */

namespace tlm {
  
  typedef char                 gs_int8;
  typedef short                gs_int16;
  typedef int                  gs_int32;
  typedef long long            gs_int64;
  
  typedef unsigned char        gs_uint8;
  typedef unsigned short       gs_uint16;
  typedef unsigned int         gs_uint32;
  typedef unsigned long long   gs_uint64;
  typedef long                 gs_handle;
  
} // end namespace tlm

#endif
#endif

// </copy>


#ifndef __gc_datatypes_h__
#define __gc_datatypes_h__
namespace gs {
#ifdef __gs_datatypes_h__
  using tlm::gs_int8;
  using tlm::gs_int16;
  using tlm::gs_int32;
  using tlm::gs_int64;
  using tlm::gs_uint8;
  using tlm::gs_uint16;
  using tlm::gs_uint32;
  using tlm::gs_uint64;
#endif
} // end namespace gs
#endif
