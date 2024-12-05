aux_source_directory(test TEST)
aux_source_directory(src SRC)
set(INCLUDE)
list(APPEND INCLUDE ${CMAKE_SOURCE_DIR}/tools/include)

add_executable(tools_test ${TEST} ${SRC})
target_include_directories(tools_test PUBLIC ${INCLUDE})
add_custom_target(tools_test_done COMMAND ./tools_test DEPENDS tools_test)

add_library(tools SHARED ${SRC})
target_include_directories(tools PUBLIC ${INCLUDE})
add_dependencies(tools tools_test_done)
