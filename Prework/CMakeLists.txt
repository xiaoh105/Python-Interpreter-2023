cmake_minimum_required(VERSION 3.22)
project(Prework)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(Prework
        calc.h
        test.cpp
        visitor.h)
