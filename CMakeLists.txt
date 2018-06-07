cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(Chut CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -Os -std=c++17 -pthread")

include_directories("Chut")

FILE(GLOB SERVER_SOURCE ChutServer/*.cpp)
add_executable(ChutServer ${SERVER_SOURCE})
target_link_libraries(ChutServer sfml-network sfml-system)

FILE(GLOB CLIENT_SOURCE ChutClient/*.cpp)
add_executable(ChutClient ${CLIENT_SOURCE})
target_link_libraries(ChutClient sfml-network sfml-system)

