aux_source_directory(test TEST)
aux_source_directory(src SRC)
set(INCLUDE)
list(APPEND INCLUDE ${CMAKE_SOURCE_DIR}/protobuf/include)
list(APPEND INCLUDE ${CMAKE_SOURCE_DIR}/utils/include)
list(APPEND INCLUDE ${CMAKE_SOURCE_DIR}/znet/include)
list(APPEND INCLUDE ${CMAKE_SOURCE_DIR}/core/include)

add_executable(core_test ${TEST} ${SRC})
target_include_directories(core_test PUBLIC ${INCLUDE})
target_link_libraries(core_test protomsg utils znet)
add_custom_target(core_test_done COMMAND ./core_test DEPENDS core_test)

add_library(core SHARED ${SRC})
target_include_directories(core PUBLIC ${INCLUDE})
target_link_libraries(core PUBLIC protomsg utils znet)
add_dependencies(core core_test_done)
