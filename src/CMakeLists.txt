cmake_minimum_required(VERSION 3.2)
project(regex)

add_subdirectory(compiler)
set(SOURCE_FILES main.cpp)

add_executable(regex ${SOURCE_FILES})
target_link_libraries(regex compiler ${graphviz_LIBRARIES})
message(STATUS "linking regex and compiler and ${graphviz_LIBRARIES}")

install(TARGETS regex DESTINATION ${REGEX_INSTALL_BIN_DIR})
