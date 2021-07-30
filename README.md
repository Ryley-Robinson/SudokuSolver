# SudokuSolver
This simple, fast command line program generates well-posed sudoku puzzles, and then solves them.

# Board Generation
SudokuSolver generates sudoku puzzles by applying transformations to a "seed" board, which is known to be a valid solution. Two transformations are applied: swapping rows/columns within 3-row/column groups, and swapping groups of 3 rows/columns. These transformations preserve the validity of the solution (one of each number per row, column, and square). The program then removes digits at random. After each digit is removed, the puzzle is solved using a backtracking algorithm in such a way as to test whether removing that digit would create a puzzle that isn't well-posed. If it does, that digit is not removed. The process is repeated until a specified number of digits is removed.

Board creation is usually quick, but can slow down significantly if one wants to create a puzzle with few given digits. This is because of the backtracking algorithm, which scales poorly - something like O(n^n), where n is the number of digits missing. Something notable is that attempting to create a puzzle with less than seventeen digits given will result in this program running indefinitely - there has been no well-posed puzzle with less than seventeen digits given, so the program will search indefinitely for one.

# Puzzle solving
Once the puzzle is generated, the program attempts to solve it using human methods:
1) Mark each square with all of the digits that can't be immediately ruled out.
2) If a digit is only possible in one space of a row, column, or square, eliminate all other possible digits in that space.
3) Identify naked pairs - if two digits are only possible in two spaces of a rwo, column, or square, eliminate all other possible digits in that row, column, or square.
4) Remove digits from each space that are no longer possible.
5) Repeat from 2) until no progress is made or the board is solved.

Using this fairly simple method, the program is able to solve almost all puzzles with 26 digits given, and roughly half of all puzzles with 21 digits given. Since this method will not "guess" digits to place in spaces, it is not always able to solve the puzzle.

# Possible improvements
- Board creation could be faster if each time a digit was removed, the "human method" algorithm ran first, and if it fails to solve the puzzle, the backtracking algorithm finished it off. The human method has a much better time complexity, since it is looking for specific patterns, as opposed to the backtracking algorithm, which solves via brute-force. If the human method can place even one digit, checking (n-1)^(n-1) combinations is a vast improvement to checking n^n combinations. I chose not to do this because I wanted the creation algorithm and solving algorithms to be completely separate.
- Many additions could be added to the human method solving algorithm to improve its ability to solve puzzles, such as looking for "hidden pairs", "X-wings", "Y-wings", and "swordfish".

# Terminology
- A "space" here refers to one of the 81 positions of the sudoku.
- A sudoku board has nine rows, nine columns, and nine squares. A square is a 3x3 arrangement of spaces. It is standard for the squares of a sudoku puzzle to have a thicker outline than the individual spaces.
- A "well-posed" sudoku is a puzzle with only one solution. This is not a strict rule of sudoku, but is always desireable.
- A "valid" solution is one with digits 1 - 9 appearing once in each row, column, and square of the puzzle.
