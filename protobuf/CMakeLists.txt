execute_process(
    COMMAND /bin/bash ${CMAKE_SOURCE_DIR}/protobuf/msg.sh
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error_output
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/protobuf
)

if(NOT result EQUAL 0)
    message(FATAL_ERROR "msg.sh failed. Output: ${output}, Error: ${error_output}")
else()
    message(STATUS "msg.sh executed successfully.")
endif()

aux_source_directory(src SRC)
set(INCLUDE)
list(APPEND INCLUDE ${CMAKE_SOURCE_DIR}/protobuf/include)

add_library(protomsg SHARED ${SRC})
target_include_directories(protomsg PUBLIC ${INCLUDE})
target_link_libraries(protomsg protobuf utils)