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
//   This program is free software.
// 
//   If you have no applicable agreement with GreenSocs Ltd, this software
//   is licensed to you, and you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
// 
//   If you have a applicable agreement with GreenSocs Ltd, the terms of that
//   agreement prevail.
// 
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
// 
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
//   02110-1301  USA 
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
