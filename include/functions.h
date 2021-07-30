#pragma once

/**
* Author: Ryley Robinson
* 
* functions.h: Includes all function declarations that will be used in the program.
*/

// Randomizes the solved board, unassigns digits from the unsolved board
bool createBoard(int nDigitsToRemove_, int seedBoard_[9][9], int solvedBoard_[9][9], int unsolvedBoard_[9][9]);

// Contains all of the algorithms to solve the sudoku
void solveBoard(int board_[9][9]);

// Uses a backtracking algorithm to brute-force solve the sudoku. Returns true if solved.
bool recursiveSolve(int board_[9][9]);

// Checks to make sure that the solution is valid
bool checkSolution(int solvedBoard_[9][9], int unsolvedBoard_[9][9]);

// Returns false if there's repeat digits in a row/column/square.
bool sanityCheck(int board_[9][9]);

// Prints the board in a nice format to the console
void printBoard(int board_[9][9]);

// Only prints the first possibility for each number in the working board
void printBoard(std::vector<int> board_[9][9]);

bool runAgainCheck();
