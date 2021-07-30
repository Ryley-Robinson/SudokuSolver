cmake_minimum_required(VERSION 3.1...3.21)
project(
  SudokuSolver 
  VERSION 3.0 
  LANGUAGES CXX)

add_library(Functions src/functions.cpp include/functions.h)

add_executable(SudokuSolver src/sudoku.cpp)

target_link_libraries(Functions PRIVATE FunctionsLib)