cmake_minimum_required(VERSION 3.2)

project(regex)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(REGEX_INSTALL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(REGEX_INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(REGEX_INSTALL_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

set(COMPILER_HEADERS_DIR ${PROJECT_SOURCE_DIR}/src/compiler)
set(VISUALIZER_HEADERS_DIR ${PROJECT_SOURCE_DIR}/src/visualizer)

include_directories(${REGEX_INSTALL_INCLUDE_DIR})
include_directories(${COMPILER_HEADERS_DIR})
include_directories(${VISUALIZER_HEADERS_DIR})

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
include(${CMAKE_ROOT}/Modules/CheckIncludeFileCXX.cmake)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
message(STATUS "CMAKE MODULE PATH ${CMAKE_MODULE_PATH}")
find_package(Graphviz REQUIRED)

include_directories(${graphviz_INCLUDE_DIRECTORIES})

add_subdirectory(src)
