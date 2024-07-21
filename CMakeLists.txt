cmake_minimum_required(VERSION 3.29)
project(elfrun)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE -Ofast -static)
set(CMAKE_EXE_LINKER_FLAGS -T${CMAKE_SOURCE_DIR}/loader_script.x)

include_directories(.)

add_executable(run
    loader.cpp
    linkable_format.cpp
    linkable_format.h
    ehdr.h
)
