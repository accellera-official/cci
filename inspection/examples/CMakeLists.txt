function(add_example NAME)
    string(PREPEND EXAMPLE_NAME "inspection/example/" ${NAME})
    file(GLOB SRC_FILES ${NAME}/*.cpp)
    add_executable(${NAME} ${SRC_FILES})
    target_link_libraries(${NAME} cci-inspection)
    add_test(NAME ${EXAMPLE_NAME} COMMAND ${NAME})
endfunction()

file(GLOB EXAMPLES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ex*/)
foreach(example ${EXAMPLES})
    add_example(${example})
endforeach()

