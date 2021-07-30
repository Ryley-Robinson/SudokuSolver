/**
* Author: Ryley Robinson
* 
* functions.cpp: Contains definitions for the functions declared in functions.h. Used in Sudoku.cpp.
*/

#include <vector>
#include <iostream>
#include "functions.h"

bool createBoard(int nDigitsToRemove_, int seedBoard_[9][9], int solvedBoard_[9][9], int unsolvedBoard_[9][9])
{
    /*
    * Randomize solved board:
    *
    * Swap blocks of three rows/columns to generate a new unique solution.
    * Swap single rows/ columns within row groups/column groups.
    */

    // debug
    std::cout << "\nCreating board..." << std::endl;

    // Decide where to place rows, columns, row groups, and column groups.
    std::vector<int> unassignedRowGroups;
    std::vector<int> unassignedColGroups;
    std::vector<int> unassignedRows;
    std::vector<int> unassignedCols;
    for (int n = 0; n < 3; n++)
    {
        unassignedRowGroups.push_back(n);
        unassignedColGroups.push_back(n);
        unassignedRows.push_back(n);
        unassignedCols.push_back(n);
    }

    int assignedRowGroups[3] = {};
    int assignedColGroups[3] = {};
    int assignedRows[3] = {};
    int assignedCols[3] = {};

    for (int n = 0; n < 3; n++)
    {
        int index = rand() % (3 - n);
        assignedRowGroups[n] = unassignedRowGroups[index];
        unassignedRowGroups.erase(unassignedRowGroups.begin() + (index));

        index = rand() % (3 - n);
        assignedColGroups[n] = unassignedColGroups[index];
        unassignedColGroups.erase(unassignedColGroups.begin() + (index));

        index = rand() % (3 - n);
        assignedRows[n] = unassignedRows[index];
        unassignedRows.erase(unassignedRows.begin() + (index));

        index = rand() % (3 - n);
        assignedCols[n] = unassignedCols[index];
        unassignedCols.erase(unassignedCols.begin() + (index));
    }

    // Create the solved and unsolved boards from the previous selections.
    for (int rowGroup = 0; rowGroup < 3; rowGroup++)
    {
        for (int colGroup = 0; colGroup < 3; colGroup++)
        {
            for (int row = 0; row < 3; row++)
            {
                for (int col = 0; col < 3; col++)
                {
                    solvedBoard_[row + 3 * rowGroup][col + 3 * colGroup] = seedBoard_[assignedRows[row] + 3 * assignedRowGroups[rowGroup]][assignedCols[col] + 3 * assignedColGroups[colGroup]];
                    unsolvedBoard_[row + 3 * rowGroup][col + 3 * colGroup] = seedBoard_[assignedRows[row] + 3 * assignedRowGroups[rowGroup]][assignedCols[col] + 3 * assignedColGroups[colGroup]];
                }
            }
        }
    }

    /*
    * Remove digits from solved board to create unsolved board:
    *
    * Pick an index at random
    * Check to see if removing it creates an unsolveable board
    * If so, remove that index from the list of indices available to remove
    * Otherwise, remove the digit at that index.
    * Repeat until enough digits have been removed.
    */

    std::vector<int> indicesAvailable;
    for (int n = 0; n < 81; n++) indicesAvailable.push_back(n);

    int tempBoard[9][9] = {};

    for (int i = 0; i < nDigitsToRemove_; i++)
    {
        bool digitFound = false;
        while (!digitFound)
        {
            // Pick an index at random
            int tempIndex, indAvailIndex;

            indAvailIndex = rand() % indicesAvailable.size();
            tempIndex = indicesAvailable[indAvailIndex];

            int tempRow = tempIndex / 9;
            int tempCol = tempIndex % 9;

            // Remember the old value in case removing it creates multiple solutions
            int oldValue = unsolvedBoard_[tempRow][tempCol];

            for (int n = 0; n < 81; n++) tempBoard[n / 9][n % 9] = unsolvedBoard_[n / 9][n % 9];

            // Counts how many solutions are possible if we remove the digit at tempRow, tempCol
            int multipleSolutions = 0;

            for (int n = 1; n < 10; n++)
            {
                tempBoard[tempRow][tempCol] = n;

                //Board must not be solveable in more than one way
                if (sanityCheck(tempBoard) &&
                    recursiveSolve(tempBoard))
                {
                    multipleSolutions++;
                }

                for (int k = 0; k < 81; k++) tempBoard[k / 9][k % 9] = unsolvedBoard_[k / 9][k % 9];
            }

            if (multipleSolutions == 1)
            {
                unsolvedBoard_[tempRow][tempCol] = 0;
                digitFound = true;
            }
            else if (multipleSolutions == 0)
            {
                // debug
                std::cout << "Something went wrong in board creation. No solution to current board." << std::endl;
                printBoard(unsolvedBoard_);
            }
            else
            {
                unsolvedBoard_[tempRow][tempCol] = oldValue;
            }

            if (indicesAvailable.size() == 1)
            {
                // debug
                std::cout << "No possible digits to remove. Trying board creation again." << std::endl;

                return false;
            }
            else
            {
                indicesAvailable.erase(indicesAvailable.begin() + indAvailIndex);
            }
        }

        printBoard(unsolvedBoard_);
    }

    return true;
}

void solveBoard(int board_[9][9])
{
    int operationsDone = 1;

    // The working board represents the board like usual, except each space can have more than one possible digit. Helpful for solving algorithm.
    std::vector<int> workingBoard[9][9];

    // These two for loops assign the known digits, and fill in the possible digits to spaces without known digits
    for (int i = 0; i < 81; i++)
    {
        if (board_[i / 9][i % 9] != 0)
        {
            workingBoard[i / 9][i % 9].push_back(board_[i / 9][i % 9]);
        }
    }

    for (int i = 0; i < 81; i++)
    {
        int row = i / 9, col = i % 9;

        // Find an empty space
        if (workingBoard[row][col].size() == 0)
        {
            // Add all possible digits
            for (int n = 1; n < 10; n++)
            {
                // Check to see if the digit can be added
                bool digitPossible = true;

                for (int j = 0; j < 9; j++)
                {
                    // Row
                    if (workingBoard[row][j].size() == 1)
                    {
                        if (workingBoard[row][j][0] == n)
                        {
                            digitPossible = false;
                        }
                    }

                    // Column
                    if (workingBoard[j][col].size() == 1)
                    {
                        if (workingBoard[j][col][0] == n)
                        {
                            digitPossible = false;
                        }
                    }

                    // Square
                    if (workingBoard[3 * (row / 3) + j / 3][3 * (col / 3) + j % 3].size() == 1)
                    {
                        if (workingBoard[3 * (row / 3) + j / 3][3 * (col / 3) + j % 3][0] == n)
                        {
                            digitPossible = false;
                        }
                    }
                }

                // If the digit has no apparent contradictions, add it
                if (digitPossible)
                {
                    workingBoard[row][col].push_back(n);
                }
            }
        }
    }

    // The main solveBoard loop. repeats until no further progress is made
    while (operationsDone > 0)
    {
        operationsDone = 0;

        // Any space with only one possible digit in workingBoard can be added to unsolvedBoard.
        for (int i = 0; i < 81; i++)
        {
            int row = i / 9, col = i % 9;

            if (board_[row][col] == 0 &&
                workingBoard[row][col].size() == 1)
            {
                board_[row][col] = workingBoard[row][col][0];
            }
        }

        printBoard(workingBoard);

        // Check each space and remove recorded possibilities that are no longer possible
        if (operationsDone == 0)
        {
            // debug
            std::cout << "\nRemoving ruled out possibilities." << std::endl;

            for (int i = 0; i < 81; i++)
            {
                int row = i / 9, col = i % 9;

                for (int n = workingBoard[row][col].size() - 1; n >= 0; n--)
                {
                    bool removeFlag = false;

                    // Find repeat digits
                    for (int j = 0; j < 9; j++)
                    {
                        // Row
                        if (workingBoard[row][j].size() == 1 &&
                            j != col)
                        {
                            if (workingBoard[row][col][n] == workingBoard[row][j][0])
                            {
                                removeFlag = true;
                            }
                        }

                        // Column
                        else if (workingBoard[j][col].size() == 1 &&
                            j != row)
                        {
                            if (workingBoard[row][col][n] == workingBoard[j][col][0])
                            {
                                removeFlag = true;
                            }
                        }

                        // Square
                        else if (workingBoard[3 * (row / 3) + j / 3][3 * (col / 3) + j % 3].size() == 1 &&
                            3 * (row / 3) + j / 3 != row &&
                            3 * (col / 3) + j % 3 != col)
                        {
                            if (workingBoard[row][col][n] == workingBoard[3 * (row / 3) + j / 3][3 * (col / 3) + j % 3][0])
                            {
                                removeFlag = true;
                            }
                        }
                    }

                    if (removeFlag &&
                        workingBoard[row][col].size() > 1)
                    {
                        // debug
                        std::cout << "Removing digit " << workingBoard[row][col][n] << " at " << row << " " << col << " from";
                        for (int k = 0; k < workingBoard[row][col].size(); k++) std::cout << " " << workingBoard[row][col][k];
                        std::cout << std::endl;

                        workingBoard[row][col].erase(workingBoard[row][col].begin() + n);
                        operationsDone++;
                    }
                }
            }
        }

        // Check each row, column, and square for digits that can only be placed in one space.
        if (operationsDone == 0)
        {

            for (int n = 1; n < 10; n++)
            {
                for (int i = 0; i < 9; i++)
                {
                    struct LastOccurrence
                    {
                        int row = -1, col = -1, squ = -1;
                    };

                    struct NumOccurrences
                    {
                        int row = 0, col = 0, squ = 0;
                    };

                    LastOccurrence lastOccurrence;
                    NumOccurrences numOccurrences;

                    for (int j = 0; j < 9; j++)
                    {
                        // Row occurrences
                        for (int k = 0; k < workingBoard[i][j].size(); k++)
                        {
                            if (workingBoard[i][j][k] == n)
                            {
                                lastOccurrence.row = j;
                                numOccurrences.row++;
                            }
                        }

                        // Column occurrences
                        for (int k = 0; k < workingBoard[j][i].size(); k++)
                        {
                            if (workingBoard[j][i][k] == n)
                            {
                                lastOccurrence.col = j;
                                numOccurrences.col++;
                            }
                        }

                        // Square occurrences
                        for (int k = 0; k < workingBoard[3 * (i / 3) + j / 3][3 * (i % 3) + j % 3].size(); k++)
                        {
                            if (workingBoard[3 * (i / 3) + j / 3][3 * (i % 3) + j % 3][k] == n)
                            {
                                lastOccurrence.squ = j;
                                numOccurrences.squ++;
                            }
                        }
                    }

                    // If there is only one occurrence of the number, remove the other possibilities from that space.
                    if (numOccurrences.row == 1 &&
                        workingBoard[i][lastOccurrence.row].size() > 1)
                    {
                        for (int l = 0; l < workingBoard[i][lastOccurrence.row].size(); l++) std::cout << " " << workingBoard[i][lastOccurrence.row][l];
                        std::cout << std::endl;

                        for (int k = workingBoard[i][lastOccurrence.row].size() - 1; k >= 0; k--)
                        {

                            if (workingBoard[i][lastOccurrence.row][k] != n)
                            {

                                workingBoard[i][lastOccurrence.row].erase(workingBoard[i][lastOccurrence.row].begin() + k);
                                operationsDone++;
                            }
                        }
                    }

                    else if (numOccurrences.col == 1 &&
                        workingBoard[lastOccurrence.col][i].size() > 1)
                    {
                        for (int l = 0; l < workingBoard[lastOccurrence.col][i].size(); l++) std::cout << " " << workingBoard[lastOccurrence.col][i][l];
                        std::cout << std::endl;

                        for (int k = workingBoard[lastOccurrence.col][i].size() - 1; k >= 0; k--)
                        {
                            if (workingBoard[lastOccurrence.col][i][k] != n)
                            {

                                workingBoard[lastOccurrence.col][i].erase(workingBoard[lastOccurrence.col][i].begin() + k);
                                operationsDone++;
                            }
                        }
                    }

                    else if (numOccurrences.squ == 1 &&
                        workingBoard[3 * (i / 3) + lastOccurrence.squ / 3][3 * (i % 3) + lastOccurrence.squ % 3].size() > 1)
                    {
                        for (int l = 0; l < workingBoard[3 * (i / 3) + lastOccurrence.squ / 3][3 * (i % 3) + lastOccurrence.squ % 3].size(); l++)
                            std::cout << " " << workingBoard[3 * (i / 3) + lastOccurrence.squ / 3][3 * (i % 3) + lastOccurrence.squ % 3][l];
                        std::cout << std::endl;

                        for (int k = workingBoard[3 * (i / 3) + lastOccurrence.squ / 3][3 * (i % 3) + lastOccurrence.squ % 3].size() - 1; k >= 0; k--)
                        {
                            if (workingBoard[3 * (i / 3) + lastOccurrence.squ / 3][3 * (i % 3) + lastOccurrence.squ % 3][k] != n)
                            {

                                workingBoard[3 * (i / 3) + lastOccurrence.squ / 3][3 * (i % 3) + lastOccurrence.squ % 3].erase(
                                    workingBoard[3 * (i / 3) + lastOccurrence.squ / 3][3 * (i % 3) + lastOccurrence.squ % 3].begin() + k);
                                operationsDone++;
                            }
                        }
                    }
                }
            }
        }

        // Identify naked pairs. Remove possible digits ruled out by that pair.
        if (operationsDone == 0)
        {
            for (int i = 0; i < 9; i++)
            {
                // Search for spaces with only the same two possibilities.
                for (int j = 0; j < 9; j++)
                {
                    struct PairIndices
                    {
                        int row[2], col[2], squ[2];
                    };

                    struct NakedPairFound
                    {
                        bool row = false, col = false, squ = false;
                    };

                    PairIndices pairIndices;
                    NakedPairFound nakedPairFound;

                    // Rows
                    if (workingBoard[i][j].size() == 2)
                    {
                        // If we find a space with only two possibilities, try to find another.
                        for (int k = 0; k < 9; k++)
                        {
                            if (workingBoard[i][k].size() == 2)
                            {
                                // If we find another with the same two possibilities, we've found a naked pair.
                                if (k != j &&
                                    workingBoard[i][k][0] == workingBoard[i][j][0] &&
                                    workingBoard[i][k][1] == workingBoard[i][j][1])
                                {
                                    pairIndices.row[0] = j, pairIndices.row[1] = k;
                                    nakedPairFound.row = true;
                                }
                            }
                        }
                    }

                    // Columns
                    if (workingBoard[j][i].size() == 2)
                    {
                        // If we find a space with only two possibilities, try to find another.
                        for (int k = 0; k < 9; k++)
                        {
                            if (workingBoard[k][i].size() == 2)
                            {
                                // If we find another with the same two possibilities, we've found a naked pair.
                                if (k != j &&
                                    workingBoard[k][i][0] == workingBoard[j][i][0] &&
                                    workingBoard[k][i][1] == workingBoard[j][i][1])
                                {
                                    pairIndices.col[0] = j, pairIndices.col[1] = k;
                                    nakedPairFound.col = true;
                                }
                            }
                        }
                    }

                    // Squares
                    if (workingBoard[3 * (i / 3) + j / 3][3 * (i % 3) + j % 3].size() == 2)
                    {
                        // If we find a space with only two possibilities, try to find another.
                        for (int k = 0; k < 9; k++)
                        {
                            if (workingBoard[3 * (i / 3) + k / 3][3 * (i % 3) + k % 3].size() == 2)
                            {
                                // If we find another with the same two possibilities, we've found a naked pair.
                                if (k != j &&
                                    workingBoard[3 * (i / 3) + k / 3][3 * (i % 3) + k % 3][0] == workingBoard[3 * (i / 3) + j / 3][3 * (i % 3) + j % 3][0] &&
                                    workingBoard[3 * (i / 3) + k / 3][3 * (i % 3) + k % 3][1] == workingBoard[3 * (i / 3) + j / 3][3 * (i % 3) + j % 3][1])
                                {
                                    pairIndices.squ[0] = j, pairIndices.squ[1] = k;
                                    nakedPairFound.squ = true;
                                }
                            }
                        }
                    }

                    // We can remove possibilities from this row that are eliminated by the naked pair
                    for (int k = 0; k < 9 && nakedPairFound.row; k++)
                    {
                        if (workingBoard[i][k].size() > 1 &&
                            k != pairIndices.row[0] &&
                            k != pairIndices.row[1])
                        {
                            for (int l = workingBoard[i][k].size() - 1; l >= 0; l--)
                            {
                                if (workingBoard[i][k][l] == workingBoard[i][pairIndices.row[0]][0] ||
                                    workingBoard[i][k][l] == workingBoard[i][pairIndices.row[0]][1])
                                {
                                    workingBoard[i][k].erase(workingBoard[i][k].begin() + l);
                                    operationsDone++;
                                }
                            }
                        }
                    }

                    // Same for columns
                    for (int k = 0; k < 9 && nakedPairFound.col; k++)
                    {
                        if (workingBoard[k][i].size() > 1 &&
                            k != pairIndices.col[0] &&
                            k != pairIndices.col[1])
                        {
                            for (int l = workingBoard[k][i].size() - 1; l >= 0; l--)
                            {
                                if (workingBoard[k][i][l] == workingBoard[pairIndices.col[0]][i][0] ||
                                    workingBoard[k][i][l] == workingBoard[pairIndices.col[0]][i][1])
                                {
                                    workingBoard[k][i].erase(workingBoard[k][i].begin() + l);
                                    operationsDone++;
                                }
                            }
                        }
                    }

                    // Same for squares
                    for (int k = 0; k < 9 && nakedPairFound.squ; k++)
                    {
                        if (workingBoard[3 * (i / 3) + k / 3][3 * (i % 3) + k % 3].size() > 1 &&
                            k != pairIndices.squ[0] &&
                            k != pairIndices.squ[1])
                        {
                            for (int l = workingBoard[3 * (i / 3) + k / 3][3 * (i % 3) + k % 3].size() - 1; l >= 0; l--)
                            {
                                if (workingBoard[3 * (i / 3) + k / 3][3 * (i % 3) + k % 3][l] == workingBoard[3 * (i / 3) + pairIndices.squ[0] / 3][3 * (i % 3) + pairIndices.squ[0] % 3][0] ||
                                    workingBoard[3 * (i / 3) + k / 3][3 * (i % 3) + k % 3][l] == workingBoard[3 * (i / 3) + pairIndices.squ[0] / 3][3 * (i % 3) + pairIndices.squ[0] % 3][1])
                                {
                                    workingBoard[3 * (i / 3) + k / 3][3 * (i % 3) + k % 3].erase(workingBoard[3 * (i / 3) + k / 3][3 * (i % 3) + k % 3].begin() + l);
                                    operationsDone++;
                                }
                            }
                        }
                    }
                }
            }
        }

        // Identify hidden pairs. Remove possible digits ruled out by that pair.
    }
}

bool recursiveSolve(int board_[9][9])
{
    /*
    * Algorithm pseudocode:
    * - Find an empty space
    * - Assign it a digit
    * - Check to see if this creates a contradiction
    *   - If yes, assign a new digit
    *     - If no more digits, return
    *   - If no, repeat function recursively
    */

    // Find an empty space on the board,
    int i = -1;
    do
    {
        i++;
        if (i > 80) return true;
    } while (board_[i / 9][i % 9] != 0);
    int row = i / 9, col = i % 9;

    // For each possible digit,
    for (int n = 1; n < 10; n++)
    {
        bool contradictionFound = false;

        // check for contradictions.
        for (int j = 0; j < 9 && !contradictionFound; j++)
        {
            if (board_[row][j] == n) contradictionFound = true;

            else if (board_[j][col] == n) contradictionFound = true;

            else if (board_[3 * (row / 3) + j / 3][3 * (col / 3) + j % 3] == n) contradictionFound = true;
        }

        // If it doesn't create a contradiction,
        if (!contradictionFound)
        {
            // place that digit,
            board_[row][col] = n;

            // otherwise find the next digit to place.
            if (recursiveSolve(board_))
            {
                return true;
            }
            else
            {
                board_[row][col] = 0;
            }
        }
    }

    // If there are no more digits to place, and the board isn't solved, something went wrong.
    return false;
}

bool checkSolution(int solvedBoard_[9][9], int unsolvedBoard_[9][9])
{
    /*
    * Checks to see that the solution is correct.
    *
    * Go through each row and column and see if the digit matches the solvedBoard digit.
    * Go through each row, column, and square and see if there are any inconsistencies.
    */
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            if (solvedBoard_[row][col] != unsolvedBoard_[row][col])
            {
                return false;
            }
        }
    }
    return true;
}

bool sanityCheck(int board_[9][9])
{
    // Check for duplicates of each number in each row, column, and square. Does not consider 0 to be a duplicate.

    for (int i = 0; i < 9; i++)
    {
        for (int n = 1; n < 10; n++)
        {
            int count[3] = { 0, 0, 0 };

            for (int j = 0; j < 9; j++)
            {
                if (board_[i][j] == n)
                {
                    count[0]++;
                }
                if (board_[j][i] == n)
                {
                    count[1]++;
                }
                if (board_[3 * (i % 3) + j / 3][3 * (i / 3) + j % 3] == n)
                {
                    count[2]++;
                }
            }

            if (count[0] > 1 || count[1] > 1 || count[2] > 1)
            {
                return false;
            }
        }
    }

    return true;
}

void printBoard(int board_[9][9])
{
    std::cout << std::endl;
    for (int row = 0; row < 9; row++)
    {
        if (row == 3 || row == 6)
        {
            std::cout << " ------+-------+------" << std::endl;
        }
        for (int col = 0; col < 9; col++)
        {
            if (col == 3 || col == 6)
            {
                std::cout << " |";
            }
            if (board_[row][col] == 0)
            {
                std::cout << " -";
            }
            else
            {
                std::cout << " " << board_[row][col];
            }
        }

        std::cout << std::endl;
    }
}

void printBoard(std::vector<int> board_[9][9])
{
    std::cout << std::endl;
    for (int row = 0; row < 9; row++)
    {
        if (row == 3 || row == 6)
        {
            std::cout << " ------------------------------+------------------------------+------------------------------" << std::endl;
        }
        for (int col = 0; col < 9; col++)
        {
            if (col == 3 || col == 6)
            {
                std::cout << " |";
            }
            if (board_[row][col].size() == 0)
            {
                std::cout << " -  ";
            }
            else
            {
                std::cout << " ";
                for (int k = 0; k < 9; k++)
                {
                    if (k < board_[row][col].size()) std::cout << board_[row][col][k];

                    else std::cout << " ";
                }
            }
        }

        std::cout << std::endl;
    }
}

bool runAgainCheck()
{
    std::string userInput;
    std::cout << "\nEnter \"quit\" to end program, or anything else to run again." << std::endl;
    std::cin >> userInput;

    return userInput == "quit" ? false : true;
}