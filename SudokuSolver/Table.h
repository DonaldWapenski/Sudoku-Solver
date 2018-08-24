#ifndef TABLE_H
#define TABLE_H

#include "Cell.h"
#include <string>

class Table {
public:
    const static int NUM_ROWS = 9;
    const static int NUM_COLUMNS = 9;
    Table(std::string f);
    Cell getCell(int i, int j);
    void solve();
    void print();

private:
    Cell cells[NUM_ROWS][NUM_COLUMNS];
    bool isSolved();
    bool isPossibleValue(int row, int col, int val);
    bool rowContains(int row, int value);
    bool colContains(int col, int value);
    bool squareContains(int sq, int value);
    //int checkForAlmostFilled(int row, int col);
    int getSquare(int row, int col);
    int getSquareColStart(int sq);
    int getSquareRowStart(int sq);
};

#endif /* TABLE_H */

