// LICENSETEXT
//
//   Copyright (C) 2011 : GreenSocs Ltd
//    http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
//
// ENDLICENSETEXT

/**
 * @file
 * @author Christian Schroeder, GreenSocs Ltd.
 * @author Mark Burton, GreenSocs Ltd.
 * @author Thomas Goodfellow, OFFIS
 * @brief Undef CCI internal macros (housekeeping)
 */

// Since this is solely for removing symbols it can be included mulitple times
// safely (which may even be useful, when hunting a symbol's origin)

#undef CCI_NOT_SUPPORTED_WRN
#undef CCI_REPORT_INFO
#undef CCI_REPORT_WARNING
#undef CCI_REPORT_ERROR
#undef CCI_REPORT_FATAL

/** Define this before including root "cci" to make use of naming and debugging
  * macros; nobody beyond API implementors should *need* this
  */
#ifndef CCI_CNF_SHARE_LIBRARY_MACROS

#undef __CCI_OWNER_ORIGINATOR_STRING__
#undef __CCI_DEFAULT_BROKER_STRING__
#undef __CCI_UNKNOWN_ORIGINATOR_STRING__
#undef __CCI_SC_REPORT_MSG_TYPE_PREFIX__

// May need these debugging macros split out to a separate undef header, so
// that the client can remove all macros except the debugging ones
#undef CCI_CNF_DUMP
#undef CCI_PARAM_DUMP
#undef CCI_SHOW_BROKER_STACK

#endif
