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
//     Technical University of Braunschweig, Dept. E.I.S.
//     http://www.eis.cs.tu-bs.de
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
// 
// ENDLICENSETEXT


//
//  This header file can be included to keep the old namespace
//  system working. The following mappings will be done:
//  (new namespace -> former namespace)
//
//  gs::ctr  ->  tlm::gc
//  gs::cnf  ->  tlm::gc::config
//  gs::av  ->  tlm::gc::gav
//  

#ifndef __namespace_compatibility_h__
#define __namespace_compatibility_h__

#ifndef NO_DEPRECATED_WARNINGS
#ifndef _MSC_VER
#warning The header file namespace_compatibility.h is deprecated! \
Use the namespaces gs::* instead!
#else
#pragma message ( "The header file namespace_compatibility.h is deprecated! Use the namespaces gs::* instead!" )
#endif
#endif

// Define new namespace
namespace gs {
  namespace ctr { }
  namespace cnf { }
  namespace av  { }
}

// Make new namespaces available in old namespace hierarchy 
namespace tlm {
  namespace gc {
    using namespace gs::ctr;
    namespace config {
      using namespace gs::cnf;
    }
    namespace gav {
      using namespace gs::av;
    }
  }
}

#endif
