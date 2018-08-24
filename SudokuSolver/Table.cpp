#include "Table.h"
#include "Cell.h"
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

Table::Table(std::string f) {
    std::ifstream infile;
    infile.open(f);
    for (int i = 0; i < NUM_ROWS; i++) {
        char data[NUM_COLUMNS * 2];
        infile >> data;
        int dataptr = 0;
        for (int j = 0; j < NUM_COLUMNS; j++) {
            cells[i][j].setValue(data[dataptr] - '0');
            dataptr += 2;
        }
    }
    infile.close();
}

Cell Table::getCell(int i, int j) {
    return cells[i][j];
}

/* Algorithm:
 * While puzzle is not solved:
 *  Update possible values of each cell based on rows, columns, and square
 *  If a cell only has one possible value, set the cell's value to it
 *  If a cell is the only one in its row, column, or square with a given possible value, set the cell's value to it
 */
void Table::solve() {
    while (!isSolved()) {
        bool anyValHasBeenChanged = false; //used to check if there are two passes with no change in values
        for (int i = 0; i < NUM_ROWS; i++) {
            for (int j = 0; j < NUM_COLUMNS; j++) {
                if (cells[i][j].getValue() == 0) {
                    //Update possible values of cells
                    for (int val = 1; val <= 9; val++) {
                        if (isPossibleValue(i, j, val)) {
                            cells[i][j].addPossValue(val);
                        }
                    }

                    //Check if a cell has one possible value
                    if (cells[i][j].getNumPossValues() == 1) {
                        anyValHasBeenChanged = true;
                        cells[i][j].setValue(cells[i][j].getPossValue(0));
                    }
                }
            }
        }

        //Check if each cell is the only one in its row, column, or square with a given possible value
        for (int i = 0; i < NUM_ROWS; i++) {
            for (int j = 0; j < NUM_COLUMNS; j++) {
                if (cells[i][j].getValue() == 0) {
                    for (int k = 0; k < cells[i][j].getNumPossValues(); k++) {
                        int val = cells[i][j].getPossValue(k);
                        bool onlyPossValueRow = true, onlyPossValueCol = true, onlyPossValueSq = true;
                        for (int row = 0; row < NUM_ROWS; row++) {
                            if (row != i && cells[row][j].isPossValue(val)) {
                                onlyPossValueRow = false;
                                break;
                            }
                        }
                        for (int col = 0; col < NUM_COLUMNS; col++) {
                            if (col != j && cells[i][col].isPossValue(val)) {
                                onlyPossValueCol = false;
                                break;
                            }
                        }
                        int sqrStart = getSquareRowStart(getSquare(i, j));
                        int sqcStart = getSquareColStart(getSquare(i, j));
                        for (int u = sqrStart; u <= sqrStart+2; u++) {
                            for (int v = sqcStart; v <= sqcStart+2; v++) {
                                if (!(u == i && v == j) && cells[u][v].isPossValue(val)) {
                                    onlyPossValueSq = false;
                                    break;
                                }
                            }
                        }
                        
                        if (onlyPossValueRow || onlyPossValueCol || onlyPossValueSq) {
                            anyValHasBeenChanged = true;
                            cells[i][j].setValue(val);
                            continue;
                        }
                    }
                }
            }
        }

        //reset the possible values
        for (int i = 0; i < NUM_ROWS; i++) {
            for (int j = 0; j < NUM_COLUMNS; j++) {
                cells[i][j].clearPossValues();
            }
        }

        if (!anyValHasBeenChanged) {
            std::cout << "Puzzle is not solvable." << std::endl;
            break;
        }
    }
}

//Formats and prints the table to console

void Table::print() {
    for (int i = 0; i < NUM_ROWS; i++) {
        if (i == 3 || i == 6) {
            std::cout << "----------------------" << std::endl;
        }
        for (int j = 0; j < NUM_COLUMNS; j++) {
            if (j == 3 || j == 6) {
                std::cout << "| ";
            }
            if (j != NUM_COLUMNS - 1) {
                std::cout << cells[i][j].getValue() << " ";
            } else {
                std::cout << cells[i][j].getValue() << std::endl;
            }
        }
    }
    std::cout << std::endl;
}

//Looks through cells for any 0 values

bool Table::isSolved() {
    for (int i = 0; i < NUM_COLUMNS; i++) {
        if (colContains(i, 0)) {
            return false;
        }
    }
    return true;
}

//Determines if a given value is possible at given coordinates

bool Table::isPossibleValue(int row, int col, int val) {
    return !rowContains(row, val) && !colContains(col, val) && !squareContains(getSquare(row, col), val);
}

bool Table::rowContains(int row, int value) {
    for (int i = 0; i < NUM_COLUMNS; i++) {
        if (cells[row][i].getValue() == value) {
            return true;
        }
    }
    return false;
}

bool Table::colContains(int col, int value) {
    for (int i = 0; i < NUM_ROWS; i++) {
        if (cells[i][col].getValue() == value) {
            return true;
        }
    }
    return false;
}

/* Squares are defined as:
 * 1 | 2 | 3
 * ---------
 * 4 | 5 | 6
 * ---------
 * 7 | 8 | 9
 */
bool Table::squareContains(int sq, int value) {
    int startCol = getSquareColStart(sq);
    int startRow = getSquareRowStart(sq);
    for (int i = startRow; i <= startRow + 2; i++) {
        for (int j = startCol; j <= startCol + 2; j++) {
            if (cells[i][j].getValue() == value) {
                return true;
            }
        }
    }
    return false;
}
//9+8+7+6+5+4+3+2+1 = 45

//int Table::checkForAlmostFilled(int row, int col) {
//    bool found1Zero = false, found2Zero = false;
//    int sum = 0;
//    //Check rows
//    for (int i = 0; i < NUM_COLUMNS; i++) {
//        sum += cells[row][i].getValue();
//        if (cells[row][i].getValue() == 0) {
//            if (!found1Zero) {
//                found1Zero = true;
//            } else {
//                found2Zero = true;
//            }
//        }
//    }
//    if (found1Zero && !found2Zero) {
//        std::cout << row << " " << col << ", " << 45 - sum << std::endl;
//        return 45 - sum;
//    }
//
//    //Check columns
//    found1Zero = false, found2Zero = false;
//    sum = 0;
//    for (int i = 0; i < NUM_ROWS; i++) {
//        sum += cells[i][col].getValue();
//        if (cells[i][col].getValue() == 0) {
//            if (!found1Zero) {
//                found1Zero = true;
//            } else {
//                found2Zero = true;
//            }
//        }
//    }
//    if (found1Zero && !found2Zero) {
//        std::cout << row << " " << col << ", " << 45 - sum << std::endl;
//        return 45 - sum;
//    }
//
//    //Check square
//    int rowStart = getSquareRowStart(getSquare(row, col));
//    int colStart = getSquareColStart(getSquare(row, col));
//    found1Zero = false, found2Zero = false;
//    sum = 0;
//    for (int r = rowStart; r <= rowStart + 2; r++) {
//        for (int c = colStart; c <= colStart + 2; c++) {
//            sum += cells[r][c].getValue();
//            if (cells[r][c].getValue() == 0) {
//                if (!found1Zero) {
//                    found1Zero = true;
//                } else {
//                    found2Zero = true;
//                }
//            }
//        }
//    }
//    if (found1Zero && !found2Zero) {
//        std::cout << row << " " << col << ", " << 45 - sum << " square" << std::endl;
//        return 45 - sum;
//    }
//
//    return 0;
//}

int Table::getSquare(int row, int col) {
    if (row >= 0 && row <= 2) {
        if (col >= 0 && col <= 2) {
            return 1;
        } else if (col >= 3 && col <= 5) {
            return 2;
        } else {
            return 3;
        }
    } else if (row >= 3 && row <= 5) {
        if (col >= 0 && col <= 2) {
            return 4;
        } else if (col >= 3 && col <= 5) {
            return 5;
        } else {
            return 6;
        }
    } else {
        if (col >= 0 && col <= 2) {
            return 7;
        } else if (col >= 3 && col <= 5) {
            return 8;
        } else {
            return 9;
        }
    }
}

//Gets the starting index of the first column of a square

int Table::getSquareColStart(int sq) {
    if (sq == 1 || sq == 4 || sq == 7) {
        return 0;
    } else if (sq == 2 || sq == 5 || sq == 8) {
        return 3;
    } else {
        return 6;
    }
}

//Gets the starting index of the first row of a square

int Table::getSquareRowStart(int sq) {
    if (sq == 1 || sq == 2 || sq == 3) {
        return 0;
    } else if (sq == 4 || sq == 5 || sq == 6) {
        return 3;
    } else {
        return 6;
    }
}