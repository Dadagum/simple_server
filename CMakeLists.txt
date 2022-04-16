
cmake_minimum_required(VERSION 3.5)
project (simple_server)

set(PROJECT_SOURCE_DIR .)
set(CMAKE_BINARY_DIR ./build )
add_compile_options(-Wall -W -ggdb)
add_definitions(-DDEBUG)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/tcp DIR_TCP_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/utils DIR_UTILS_SRCS)

add_library(simple_server SHARED
    ${DIR_TCP_SRCS}    
    ${DIR_UTILS_SRCS}
)

target_include_directories(simple_server
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/src/include
) 

# test server and client
add_executable(test_simple_server
    ${PROJECT_SOURCE_DIR}/src/test/test_tcp_server.cpp
)

add_executable(test_simple_client
    ${PROJECT_SOURCE_DIR}/src/test/test_tcp_client.cpp
)

target_include_directories(test_simple_server
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/src/include
) 

target_link_libraries(test_simple_server
    PRIVATE
        simple_server
)

target_include_directories(test_simple_client
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/src/include
) 

target_link_libraries(test_simple_client
    PRIVATE
        simple_server
)