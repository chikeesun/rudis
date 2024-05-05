cmake_minimum_required(VERSION 3.13)
project(redis-starter-cpp)
set(CMAKE_CXX_STANDARD 20) # Enable the C++20 standard
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SOURCE_FILES src/Server.cpp src/TimePriorityQueue.cpp)

add_executable(server ${SOURCE_FILES})
target_include_directories(server PRIVATE inc)
target_link_libraries(server pthread)