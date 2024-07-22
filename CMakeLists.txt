cmake_minimum_required(VERSION 3.20)
project(elfrun)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE -Ofast)
set(CMAKE_EXE_LINKER_FLAGS -T${CMAKE_SOURCE_DIR}/loader_script.ld)

include_directories(.)

add_executable(run
    loader.cpp
    linkable_format.cpp
    linkable_format.h
    ehdr.h
)
