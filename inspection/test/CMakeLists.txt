add_subdirectory(Catch2)
target_compile_features(Catch2 PUBLIC cxx_std_17)

function(add_inspection_test NAME)
    string(PREPEND TEST_NAME "inspection/test/" ${NAME})
    file(GLOB SRC_FILES ${NAME}/*.cpp)
    add_executable(${NAME} ${SRC_FILES})
    target_link_libraries(${NAME} Catch2::Catch2WithMain cci-inspection)
    add_test(NAME ${TEST_NAME} COMMAND ${NAME})
endfunction()


#########################################################################
## Add the inspection tests.
#########################################################################
file(GLOB TESTS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test*/)
foreach(test ${TESTS})
    add_inspection_test(${test})
endforeach()
