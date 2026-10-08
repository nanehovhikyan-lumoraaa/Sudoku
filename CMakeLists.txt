cmake_minimum_required(VERSION 3.16)

project(first LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(Sudoku
    sudoku.cpp
    sudoku.h
    main.cpp
    puzzles.h
)

file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(Sudoku PRIVATE Qt6::Widgets)