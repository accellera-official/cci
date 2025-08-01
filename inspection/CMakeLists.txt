add_subdirectory(src)

install (FILES README.md
         DESTINATION ${CMAKE_INSTALL_DOCDIR}/inspection
         COMPONENT doc)

if(SYSTEMCCCI_BUILD_TESTS)
  enable_testing()
  add_subdirectory(examples)
  add_subdirectory(test)
endif()
