aux_source_directory(test TEST)
aux_source_directory(src SRC)
set(INCLUDE)
list(APPEND INCLUDE ${CMAKE_SOURCE_DIR}/protobuf/include)
list(APPEND INCLUDE ${CMAKE_SOURCE_DIR}/ziface/include)
list(APPEND INCLUDE ${CMAKE_SOURCE_DIR}/core/include)
list(APPEND INCLUDE ${CMAKE_SOURCE_DIR}/api/include)

add_executable(api_test ${TEST} ${SRC})
target_include_directories(api_test PUBLIC ${INCLUDE})
target_link_libraries(api_test core)
add_custom_target(api_test_done COMMAND ./api_test DEPENDS api_test)

add_library(api SHARED ${SRC})
target_include_directories(api PUBLIC ${INCLUDE})
target_link_libraries(api PUBLIC core)
add_dependencies(api api_test_done)
