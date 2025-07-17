add_library(cci-inspection cci/inspection/cci_inspection.cpp)
add_library(SystemC::cci-inspection ALIAS cci-inspection)
target_compile_features(cci-inspection PUBLIC cxx_std_17)
target_include_directories(cci-inspection
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
set_target_properties(cci-inspection
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      VERSION ${SystemCCCI_VERSION}
                      SOVERSION ${SystemCCCI_SOVERSION})

install(TARGETS cci-inspection
    EXPORT  SystemCCCITargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT lib)

install(DIRECTORY ./
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT dev
        FILES_MATCHING PATTERN "*.h")
install(FILES "cci_inspection"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT dev)
