aux_source_directory(test TEST)
aux_source_directory(src SRC)
list(REMOVE_ITEM TEST test/client_test.cpp)
set(INCLUDE)
list(APPEND INCLUDE ${CMAKE_SOURCE_DIR}/tools/include)
list(APPEND INCLUDE ${CMAKE_SOURCE_DIR}/ziface/include)
list(APPEND INCLUDE ${CMAKE_SOURCE_DIR}/znet/include)

add_executable(znet_test ${TEST} ${SRC})
target_include_directories(znet_test PUBLIC ${INCLUDE})
add_custom_target(znet_test_done COMMAND ./znet_test DEPENDS znet_test)

add_executable(server_test test/client_test.cpp ${SRC})
target_include_directories(server_test PUBLIC ${INCLUDE})

add_library(znet SHARED ${SRC})
target_include_directories(znet PUBLIC ${INCLUDE})
add_dependencies(znet znet_test_done)
