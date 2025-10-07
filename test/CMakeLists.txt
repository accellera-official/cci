cmake_minimum_required(VERSION 3.5)
project(CCI-Example CXX)

find_package(SystemCLanguage CONFIG REQUIRED)
find_package(SystemCCCI CONFIG REQUIRED)

# Workaround for when SystemC is not installed as a system library
get_target_property(SYSTEMC_LOCATION SystemC::systemc LOCATION)
get_filename_component(SYSTEMC_LIB_DIR ${SYSTEMC_LOCATION} DIRECTORY)
link_directories(${SYSTEMC_LIB_DIR})

add_executable(cci-example src/example.cpp)
target_link_libraries(cci-example PUBLIC SystemC::cci-config SystemC::cci-inspection SystemC::systemc)
target_include_directories(cci-example PRIVATE src)
