// LICENSETEXT
//
//   Copyright (C) 2009 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
//
//   Developed by:
//    Christian Schroeder <schroeder@eis.cs.tu-bs.de>,
//    Mark Burton, mark@greensocs.com
//
//
// The contents of this file are subject to the licensing terms specified
// in the file LICENSE. Please consult this file for restrictions and
// limitations that may apply.
//
// ENDLICENSETEXT


#include "cci_debug.h"
#include "cci_datatypes.h"
#include "cci_value.h"
#include "cci_report_handler.h"

cci::cnf::cci_value::cci_value()
{
	initialize_values();
	m_type = param_type_not_available;
}

cci::cnf::cci_value::cci_value(const char* value)
{
	initialize_values();
	m_type = param_type_string;
    m_value_string = value;

}

cci::cnf::cci_value::cci_value(const std::string& value)
{
	initialize_values();
	m_type = param_type_string;
    m_value_string = value;

}

cci::cnf::cci_value::cci_value(bool value)
{
	initialize_values();
	m_type = param_type_bool;
    m_value_bool = value;
}

cci::cnf::cci_value::cci_value(int value)
{
	initialize_values();
	m_type = param_type_number;
    m_value_number = value;
}

cci::cnf::cci_value::cci_value(sc_dt::int64 value)
{
	initialize_values();
	m_type = param_type_number;
    m_value_number = value;

}

cci::cnf::cci_value::cci_value(double value)
{
	initialize_values();
	m_type = param_type_real;
    m_value_real = value;
}

cci::cnf::cci_value::cci_value(const cci_value_list& value)
{
	initialize_values();
	m_type = param_type_list;
    m_value_list = value;
}

cci::cnf::cci_value::cci_value(const cci::cnf::cci_value& other) {
  operator=(other);
}

bool cci::cnf::cci_value::operator==(const cci::cnf::cci_value& lhs) const {
  if (lhs.type() != type()) return false;
  switch(lhs.type()) {
    case cci::cnf::param_type_not_available:
      return true;
      break;
    case cci::cnf::param_type_number:
      return (lhs.get_int64() == get_int64());
      break;
    case cci::cnf::param_type_real:
      return (lhs.get_real() == get_real());
      break;
    case cci::cnf::param_type_bool:
      return (lhs.get_bool() == get_bool());
      break;
    case cci::cnf::param_type_string:
      return (lhs.get_string() == get_string());
      break;
    case cci::cnf::param_type_list:
      return (lhs.get_list() == get_list());
      break;
    case cci::cnf::param_type_other:
      cci_report_handler::cci_value_failure("not implemented");
      break;
    default:
      cci_report_handler::cci_value_failure("not implemented");
      assert(false && "This should never happen!");
  }
  return false;
}

cci::cnf::cci_value& cci::cnf::cci_value::operator=(const cci::cnf::cci_value& lhs) {
  m_type = lhs.type();
  switch(lhs.type()) {
    case cci::cnf::param_type_not_available:
      break;
    case cci::cnf::param_type_number:
      m_value_number = lhs.get_int64();
      break;
    case cci::cnf::param_type_real:
      m_value_real = lhs.get_real();
      break;
    case cci::cnf::param_type_bool:
      m_value_bool = lhs.get_bool();
      break;
    case cci::cnf::param_type_string:
      m_value_string = lhs.get_string();
      break;
    case cci::cnf::param_type_list:
      m_value_list = lhs.get_list();
      break;
    case cci::cnf::param_type_other:
      cci_report_handler::cci_value_failure("Not implemented.");
      break;
    default:
      cci_report_handler::cci_value_failure("Not implemented.");
      assert(false && "This should never happen!");
  }
  return *this;
}

cci::cnf::basic_param_type cci::cnf::cci_value::type() const { return m_type; }

const std::string&      cci::cnf::cci_value::get_string()   const {
  if (m_type != cci::cnf::param_type_string)
    cci_report_handler::cci_value_failure("Wrong cci value type (no string).");
  return m_value_string;
}

const cci::cnf::cci_value_list&   cci::cnf::cci_value::get_list() const {
  if (m_type != cci::cnf::param_type_list)
    cci_report_handler::cci_value_failure("Wrong cci value type (no list).");
  return m_value_list;
}

bool cci::cnf::cci_value::get_bool()  const {
  if (m_type != cci::cnf::param_type_bool)
    cci_report_handler::cci_value_failure("Wrong cci value type (no bool).");
  return m_value_bool;
}

int cci::cnf::cci_value::get_int()   const {
  if (m_type != cci::cnf::param_type_number)
    cci_report_handler::cci_value_failure("Wrong cci value type (no number).");
  if (m_value_number > INT_MAX || m_value_number < INT_MIN) {
    //CCI_THROW_WARNING(cci::cnf::cci_report::cci_value_failure().get_type(), "Overflow cci value (number is larger than int can hold).");
    cci_report_handler::report(sc_core::SC_WARNING,"CCI_VALUE_FAILURE","Overflow cci value (number is larger than int can hold).",__FILE__,__LINE__);
  }
  return static_cast<int>(m_value_number);
}

sc_dt::int64 cci::cnf::cci_value::get_int64() const {
  if (m_type != cci::cnf::param_type_number)
    cci_report_handler::cci_value_failure("Wrong cci value type (no number).");
  return m_value_number;
}

double cci::cnf::cci_value::get_real()  const {
  if (m_type != cci::cnf::param_type_real)
    cci_report_handler::cci_value_failure("Wrong cci value type (no real).");
  return m_value_real;
}

void cci::cnf::cci_value::initialize_values()
{
    m_value_bool    = false;;
    m_value_number  = 0;
    m_value_real = 0;
}

// Borrowing this .cpp to provide a home for this value (creating cci_param.cpp
// solely for that purpose is overkill. @TODO - review home later in rework
const char* cci::cnf::PARAM_ORIGINATOR = "owning_param";

