cmake_minimum_required(VERSION 3.2)
project(regex)

add_subdirectory(compiler)
add_subdirectory(visualizer)
set(SOURCE_FILES main.cpp)

add_executable(regex ${SOURCE_FILES})
target_link_libraries(regex compiler visualizer ${graphviz_LIBRARIES})
message(STATUS "linking regex, compiler, visualizer and ${graphviz_LIBRARIES}")

install(TARGETS regex DESTINATION ${REGEX_INSTALL_BIN_DIR})
