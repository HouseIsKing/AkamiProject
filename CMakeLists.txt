cmake_minimum_required(VERSION 3.20)
project(AkamiProject)

set(CMAKE_CXX_STANDARD 23)
include_directories(include)
add_executable(AkamiProject main.cpp)
target_link_directories(AkamiProject PRIVATE library)
target_link_libraries(AkamiProject LINK_PRIVATE restbed)
