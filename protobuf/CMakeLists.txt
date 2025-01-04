aux_source_directory(src SRC)
set(INCLUDE)
list(APPEND INCLUDE ${CMAKE_SOURCE_DIR}/protobuf/include)

add_library(protomsg SHARED ${SRC})
target_include_directories(protomsg PUBLIC ${INCLUDE})
target_link_libraries(protomsg protobuf utils znet)