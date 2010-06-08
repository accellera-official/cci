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


#ifndef __gc_unevenpair_h__
#define __gc_unevenpair_h__


namespace gs {
namespace ctr {
  
  
  template<class _T1, class _T2>
  struct unevenpair: public std::pair<_T1, _T2>
  {
    inline bool same(const unevenpair<_T1, _T2>& __x) { 
      return __x.first == this->first ;
    }
    
    // this operator only compares the first element of the pair (for performance reasons)
    inline bool operator == (const unevenpair<_T1, _T2>& __x) {
      return __x.first == this->first; 
    }
    
    // this operator only compares the first element of the pair (for performance reasons)
    inline bool operator != (const unevenpair<_T1, _T2>& __x) {
      return __x.first != this->first; 
    }
    
    unevenpair(const _T1 &a, const _T2 &b) : std::pair<_T1, _T2>(a,b) {};
    unevenpair() : std::pair<_T1,_T2>() {};
    unevenpair(const unevenpair<_T1, _T2>& __p) : std::pair<_T1,_T2>(__p) {};
  };
  
  
} // end namespace ctr
} // end namespace gs


#endif
