cmake_minimum_required(VERSION 3.2)
project(compiler C CXX)

set(SOURCE_FILES
    compiler.h
    compiler.cpp
)

add_library(compiler SHARED STATIC ${SOURCE_FILES})

install(TARGETS compiler DESTINATION ${REGEX_INSTALL_LIB_DIR})
install(FILES compiler.h DESTINATION ${REGEX_INSTALL_INCLUDE_DIR})
