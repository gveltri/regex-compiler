cmake_minimum_required(VERSION 3.2)
project(compiler C CXX)

set(SOURCE_FILES
    visualizer.h
    visualizer.cpp
)

add_library(visualizer SHARED STATIC ${SOURCE_FILES})

install(TARGETS visualizer DESTINATION ${REGEX_INSTALL_LIB_DIR})
install(FILES visualizer.h DESTINATION ${REGEX_INSTALL_INCLUDE_DIR})
