cmake_minimum_required (VERSION 3.5)
cmake_policy(SET CMP0001 NEW)

project (SystemCCCI CXX C)

option(CCI_ENABLE_CFG "Build SystemCCI configuration library" ON)
option(SYSTEMCCCI_BUILD_TESTS "Build tests & examples" ON)
option(CCI_ENABLE_INSPECTION "Build SystemCCI inspection library" ON)
option(BUILD_SOURCE_DOCUMENTATION "Build source documentation with Doxygen." OFF)

set(SystemCCCI_VERSION_FILE "${PROJECT_SOURCE_DIR}/cci/core/cci_version.h")
if(EXISTS ${SystemCCCI_VERSION_FILE})
    file(READ ${SystemCCCI_VERSION_FILE} SYSTEMCCCI_VERSION_FILE_CONTENT)
    string(REGEX MATCH "CCI_VERSION_MAJOR[ \t]+([0-9]+)" SystemCCCI_VERSION_MAJOR ${SYSTEMCCCI_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemCCCI_VERSION_MAJOR ${SystemCCCI_VERSION_MAJOR})
    string(REGEX MATCH "CCI_VERSION_MINOR[ \t]+([0-9]+)" SystemCCCI_VERSION_MINOR ${SYSTEMCCCI_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemCCCI_VERSION_MINOR ${SystemCCCI_VERSION_MINOR})
    string(REGEX MATCH "CCI_VERSION_PATCH[ \t]+([0-9]+)" SystemCCCI_VERSION_PATCH ${SYSTEMCCCI_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemCCCI_VERSION_PATCH ${SystemCCCI_VERSION_PATCH})
    string(REGEX MATCH "CCI_IS_PRERELEASE[ \t]+([0-9]+)" SystemCCCI_VERSION_IS_PRERELEASE ${SYSTEMCCCI_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemCCCI_VERSION_IS_PRERELEASE ${SystemCCCI_VERSION_IS_PRERELEASE})
    string(REGEX MATCH "CCI_VERSION_PRERELEASE[ \t]+\"([^\".]+)\"" SystemCCCI_VERSION_PRERELEASE ${SYSTEMCCCI_VERSION_FILE_CONTENT})
    string(REGEX REPLACE "(.*)\"(.*)\"" "\\2" SystemCCCI_VERSION_PRERELEASE ${SystemCCCI_VERSION_PRERELEASE})
    string(REGEX MATCH "CCI_SHORT_RELEASE_DATE[ \t]+([0-9]+)" SystemCCCI_VERSION_RELEASE_DATE ${SYSTEMCCCI_VERSION_FILE_CONTENT})
    string(REGEX MATCH "([0-9]+)" SystemCCCI_VERSION_RELEASE_DATE ${SystemCCCI_VERSION_RELEASE_DATE})
else(EXISTS ${SystemCCCI_VERSION_FILE})
    message (FATAL_ERROR "Unable to read CCI version file")
endif(EXISTS ${SystemCCCI_VERSION_FILE})

set (SystemCCCI_VERSION "${SystemCCCI_VERSION_MAJOR}.${SystemCCCI_VERSION_MINOR}.${SystemCCCI_VERSION_PATCH}")
if (SystemCCCI_VERSION_IS_PRERELEASE)
  set (SystemCCCI_VERSION "${SystemCCCI_VERSION}_${SystemCCCI_VERSION_PRERELEASE}_${SystemCCCI_VERSION_RELEASE_DATE}")
endif (SystemCCCI_VERSION_IS_PRERELEASE)
set (SystemCCCI_SOVERSION "${SystemCCCI_VERSION_MAJOR}.${SystemCCCI_VERSION_MINOR}")

if(CCI_ENABLE_CFG)
    add_subdirectory(configuration)
endif()
if(CCI_ENABLE_INSPECTION)
    add_subdirectory(inspection)
endif()
if(BUILD_SOURCE_DOCUMENTATION)
    add_subdirectory (docs)
endif()

###############################################################################
# Install README files
###############################################################################

install (FILES INSTALL.md
               cmake/INSTALL_USING_CMAKE
               LICENSE
               NOTICE
               README.md
               RELEASENOTES.md
         DESTINATION ${CMAKE_INSTALL_DOCDIR}
         COMPONENT doc)


###############################################################################
# Provide package of the SystemC library to other programs using CMake
###############################################################################

# Export the cci library target for usage by other programs
# ... to local build tree
export (TARGETS cci-config
        NAMESPACE SystemC::
        FILE SystemCCCITargets.cmake)
# ... to installation tree
install (EXPORT SystemCCCITargets
         NAMESPACE SystemC::
         DESTINATION ${SystemCCCI_INSTALL_CMAKEDIR}
         COMPONENT dev)

# Register the SystemCCCI packages for use from the
# build tree. (This registers the build tree with the global CMake-registry.)
export (PACKAGE SystemCCCI)

# Create the <Package>Config.cmake and <Package>ConfigVersion files
include(CMakePackageConfigHelpers)
configure_package_config_file(cmake/SystemCCCIConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/SystemCCCIConfig.cmake
  INSTALL_DESTINATION ${SystemCCCI_INSTALL_CMAKEDIR})
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/SystemCCCIConfigVersion.cmake"
  VERSION ${SystemCCCI_VERSION_MAJOR}.${SystemCCCI_VERSION_MINOR}.${SystemCCCI_VERSION_PATCH}.${SystemCCCI_VERSION_RELEASE_DATE}
  COMPATIBILITY AnyNewerVersion
)

# Install the <Package>Config.cmake and <Package>ConfigVersion.cmake
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/SystemCCCIConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/SystemCCCIConfigVersion.cmake"
        DESTINATION "${SystemCCCI_INSTALL_CMAKEDIR}"
        COMPONENT dev)
