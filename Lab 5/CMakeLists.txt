cmake_minimum_required(VERSION 4.1)
project(Lab_5)

set(CMAKE_CXX_STANDARD 20)

add_executable(Lab_5 main.c
        io.h
        io.c
        string_analysis.h
        string_analysis.c)
