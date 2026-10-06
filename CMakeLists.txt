cmake_minimum_required(VERSION 3.16)

project(first LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(first
    sudoku.cpp
    sudoku.h
    main.cpp
)

target_link_libraries(first PRIVATE Qt6::Widgets)