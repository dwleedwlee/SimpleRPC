cmake_minimum_required(VERSION 3.0)
project("SimpleRPC")
message("project name: ${CMAKE_PROJECT_NAME}")
message("install dir: ${CMAKE_INSTALL_PREFIX}")

set(CMAKE_CXX_COMPILER "C:/MinGW/bin/gcc")
set(CMAKE_BUILD_TYPE Release)
set(SOURCE_FILES ../src/rpc_main.c ../src/rpc_server.c ../src/rpc_client.c ../src/rpc_buffer.c)
message("set vars: ${SOURCE_FILES}")

add_executable(rpc ${SOURCE_FILES})
target_link_libraries(rpc PUBLIC pthread)
