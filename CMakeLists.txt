cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall -g2 -ggdb -pthread")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -Wall -g2 -ggdb -pthread")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../../build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../build")
project(Rest_Threadpool)
add_executable(${PROJECT_NAME} "main.cpp" "thread_manager.h" "thread_manager.cc"
    "httpserver.h" "httpserver.cc" "define.h")
include_directories("../../cpprest/include")
link_directories("../../cpprest/lib")
link_libraries("libustore.so")
target_link_libraries(${PROJECT_NAME} libcpprest.so libcrypto.so libssl.so)
