#ifndef CELL_H
#define CELL_H

#include <vector>

class Cell {
public:
    void setValue(int v);
    int getValue();
    void addPossValue(int value);
    bool isPossValue(int value);
    int getNumPossValues();
    int getPossValue(int index);
    void clearPossValues();
private:
    int val;
    std::vector<int> possVals;
};

#endif /* CELL_H */

