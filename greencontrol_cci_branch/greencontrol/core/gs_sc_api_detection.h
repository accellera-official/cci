// LICENSETEXT
// 
//   Copyright (C) 2007 : GreenSocs Ltd
//       http://www.greensocs.com/ , email: info@greensocs.com
// 
//   Developed by :
// 
//   Marcus Bartholomeu
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


////////////////////////////////////////////////////////
//
// Detect SystemC API and define the macro SYSTEMC_API
// (include detection for other SystemC vendors if necessary)
//
// 2008-10-02: Detect OSCI SystemC versions 2.1 and 2.2
//
////////////////////////////////////////////////////////


#ifndef GS_SC_API_DETECTION_H
#define GS_SC_API_DETECTION_H

#include <systemc>


// Respect if the macro is set externally (to 210 or 220 only)
#ifdef SYSTEMC_API

#if SYSTEMC_API != 210 && SYSTEMC_API != 220
#error The SYSTEMC_API macro is set to an unknown value. Accepted values are 210 or 220. Please, see the source code gs_sc_api_detection.h for more information.
#endif

// So it is not defined externally, detect it!
#else

// OSCI SystemC 2.1
#if SYSTEMC_VERSION == 20050714
#define SYSTEMC_API 210
#endif

// OSCI SystemC 2.2.0 and 2.2.05jun06
#if SYSTEMC_VERSION == 20070314 || SYSTEMC_VERSION == 20060505
#define SYSTEMC_API 220
#endif

// SystemC API to use must be defined at this point
// include the detection above for other SystemC vendors to
// define the SYSTEMC_API macro
#ifndef SYSTEMC_API
#error The SYSTEMC_API macro is not set and could not be detected. Please, see the source code gs_sc_api_detection.h for more information.
#endif

#endif // def SYSTEMC_API

#endif // ndef GS_SC_API_DETECTION_H
