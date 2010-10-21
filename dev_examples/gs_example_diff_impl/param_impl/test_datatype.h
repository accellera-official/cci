// LICENSETEXT
//
//   Copyright (C) 2010 : GreenSocs Ltd
// 	 http://www.greensocs.com/ , email: info@greensocs.com
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


#ifndef __TEST_DATATYPE_H__
#define __TEST_DATATYPE_H__

#include <iostream>
#include <string.h> 

class test_datatype
{
public:
  
  friend std::istream& operator >>(std::istream &is,test_datatype &obj);
  friend std::ostream& operator <<(std::ostream &os,const test_datatype &obj);
  
  std::string strVal;
};

// implementation see library cpp gs_cci_symbols_b.cpp
/*std::istream& operator >>(std::istream &is,test_datatype &obj)
{
  is>>obj.strVal;
  return is;
}
std::ostream& operator <<(std::ostream &os,const test_datatype &obj)
{
  os<<obj.strVal;
  return os;
}
*/
#endif
