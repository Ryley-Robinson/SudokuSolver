#pragma once
/**
* Author: Ryley Robinson
* 
* config.h: Contains variables used in the program.
*/

namespace Conf
{
    const int nDigitsToRemove = 55;

    // Seed board known to be sudoku-valid
    // Credit: https://www.researchgate.net/figure/A-Sudoku-with-17-clues-and-its-unique-solution_fig1_311250094
    int seedBoard[9][9] = {
        {2,3,7,8,4,1,5,6,9},
        {1,8,6,7,9,5,2,4,3},
        {5,9,4,3,2,6,7,1,8},
        {3,1,5,6,7,4,8,9,2},
        {4,6,9,5,8,2,1,3,7},
        {7,2,8,1,3,9,4,5,6},
        {6,4,2,9,1,8,3,7,5},
        {8,5,3,4,6,7,9,2,1},
        {9,7,1,2,5,3,6,8,4}
    };

    // Will hold the randomized but solved board.
    int solvedBoard[9][9] = {};

    // Unsolved board to be displayed to the user
    int unsolvedBoard[9][9] = {};
};