//   GreenControl framework
//
// LICENSETEXT
//
//   Copyright (C) 2010 : GreenSocs Ltd
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

#ifndef __gc_service_h__
#define __gc_service_h__

#include <map>

namespace gs {
namespace ctr {

  /// Control Service ID typedef to be used when Service should be stored (independently from basic or extended service)
  typedef unsigned int ControlService;
  
  /// Control service specifier.
  /**
   * Control service to route the transactions in the GreenControl Core 
   * and announce supported service from API or plugin to the Core.
   */
  enum ControlService_enum {
    /// No service.
    NO_SERVICE=0,
    // Config Service, provided by ConfigPlugin
    CONFIG_SERVICE=1,
    // Analysis and Visibility Service
    AV_SERVICE=2,
    // Logging Service
    LOG_SERVICE=3,
    // Other Service, see extension mechanism (reserved)
    EXTENDED_SERVICE=4
  };


  
  /// INTERNAL method to organize the string representation of the extended ControlServices.
  /**
   * User shall use the gs::ctr::getControlServiceString(ControlService) function!
   *
   * @param cs        Extended ControlService ID.
   * @param id_string Optional parameter to set the string.
   * @return          String representation of cs.
   */
  inline const std::string getExtendedControlServiceString(ControlService cs, const char* id_string = NULL) {
    static std::map<gs::ctr::ControlService, std::string> gc_service_ID_extension_strings;
    if (id_string != NULL) {
      if (gc_service_ID_extension_strings.find(cs) != gc_service_ID_extension_strings.end()) {
        assert((gc_service_ID_extension_strings.find(cs)->second).compare(id_string) == 0 && "A same Service Extension must not add different ID strings!");
      }
      gc_service_ID_extension_strings.insert(std::pair<ControlService, std::string>(cs, id_string));
    }
    std::map<ControlService, std::string>::iterator ret = gc_service_ID_extension_strings.find(cs);
    assert(ret != gc_service_ID_extension_strings.end() && "ID string not available. Registration of Service ID string must have failed!");
    return ret->second;
  }
  
  
  /// Method to get the string representation of the ControlService ID (for debug purpose).
  /**
   * This works for standard and extension service IDs
   *
   * @param cs  ControlService ID.
   * @return    String representation of cs.
   */
  inline const std::string getControlServiceString(ControlService cs) {
    switch (cs) {
      case NO_SERVICE:
        return string("NO_SERVICE");
      case CONFIG_SERVICE:
        return string("CONFIG_SERVICE");
      case AV_SERVICE:
        return string("AV_SERVICE");
      case LOG_SERVICE:
        return string("LOG_SERVICE");
      case EXTENDED_SERVICE:
        return string("EXTENDED_SERVICE (this is an error, this must not be used for any service!)");
      default:
        return getExtendedControlServiceString(cs);
    }
  }
  

  // //////////////////////////////////////////////////////////////////////////////////// //
  // /////////////////////////////  EXTENSION MECHANISM  //////////////////////////////// //
  // //////////////////////////////////////////////////////////////////////////////////// //
  
  
  /// Helper function managing the maximum number of services extensions existing (including the ControlService numbers)
  /**
   * This function counts the global IDs for service extensions available in the system.
   * The returned number is the number the next created service would get, beginning with 
   * ControlService_enum::EXTENDED_SERVICE + 1. Note NO_SERVICE (=0) and EXTENDED_SERVICE 
   * are reserved and are not used for any service ID.
   * This is incremented each time a new class of gc_service_ID_extension is define.
   * Hence it is changed during static initialization.
   */
  inline unsigned int max_num_gc_service_ID_extensions(bool increment=false)
  {
    static unsigned int max_num = EXTENDED_SERVICE + 1;
    if (increment) ++max_num;
    return max_num;
  }
  
  /// Internal class to store extensions in the gc_service_ID_extensions
  class gc_service_ID_extension_base
  {
  public:
    /// Returns the Service ID
    virtual unsigned int get_Service_ID() const = 0;
    /// Returns the Service ID string (for debug)
    virtual std::string get_Service_ID_string() const = 0;
  protected:
    virtual ~gc_service_ID_extension_base() {}
    static unsigned int register_extension()
    {
      return (max_num_gc_service_ID_extensions(true) - 1);
    };
  };
  
  /// Base class to derive service extensions from (to provide them e.g. to the gc_port)
  /**
   * Derive your service extension class in the following way:
   * \verbatim
     struct test_service_ID
     : public gs::ctr::gc_service_ID_extension<test_service_ID>
     {
       test_service_ID() : gs::ctr::gc_service_ID_extension<test_service_ID>("TEST_SERVICE") { }
     }; \endverbatim
   * The extension constructor defines the debug and log string connected to the service ID.
   *
   * This triggers proper service extension registration during C++ static
   * contruction time. my_extension::ID will hold the unique service index.
   */
  template <typename T>
  class gc_service_ID_extension : public gc_service_ID_extension_base
  {
  public:
    gc_service_ID_extension(const char* debug_service_ID_string) : mIDstring(debug_service_ID_string) { getExtendedControlServiceString(get_Service_ID(), debug_service_ID_string); }
    virtual ~gc_service_ID_extension() {}
    /// Returns the Service ID
    virtual unsigned int get_Service_ID() const { return ID; }
    /// Returns the Service ID string (for debug)
    virtual std::string get_Service_ID_string() const { return mIDstring; }
  protected:
    const static unsigned int ID;
    const std::string mIDstring;
  };
  
  template <typename T>
  const unsigned int gc_service_ID_extension<T>::ID = gc_service_ID_extension_base::register_extension();
  
  

} // end namespace ctr
} // end namespace gs   

#endif
