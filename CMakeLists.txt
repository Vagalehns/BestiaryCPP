cmake_minimum_required(VERSION 4.0)
project(BestiaryCPP)

set(CMAKE_CXX_STANDARD 20)

add_executable(BestiaryCPP main.cpp
        info.h
        TUI_functions.cpp
        TUI_functions.h
        styling_functions.h
        BestiaryApp.cpp
        BestiaryApp.h)
