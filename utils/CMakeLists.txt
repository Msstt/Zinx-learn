aux_source_directory(test TEST)
aux_source_directory(src SRC)
set(INCLUDE)
list(APPEND INCLUDE ${CMAKE_SOURCE_DIR}/utils/include)
list(APPEND INCLUDE ${CMAKE_SOURCE_DIR}/deps)

add_executable(utils_test ${TEST} ${SRC})
target_include_directories(utils_test PUBLIC ${INCLUDE})
add_custom_target(utils_test_done COMMAND ./utils_test DEPENDS utils_test)

add_library(utils SHARED ${SRC})
target_include_directories(utils PUBLIC ${INCLUDE})
add_dependencies(utils utils_test_done)
