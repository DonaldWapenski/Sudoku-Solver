#include "Cell.h"

#include <algorithm>

void Cell::setValue(int v) {
    val = v;
}

int Cell::getValue() {
    return val;
}

void Cell::addPossValue(int value) {
    if (!isPossValue(value)) {
        possVals.push_back(value);
    }
}

bool Cell::isPossValue(int value) {
    return std::any_of(possVals.begin(), possVals.end(), [&value](const int& i){return i == value;});
}

int Cell::getNumPossValues() {
    return possVals.size();
}

int Cell::getPossValue(int index) {
    return possVals.at(index);
}

void Cell::clearPossValues() {
    if (!possVals.empty()) {
        possVals.erase(possVals.begin(), possVals.end());
    }
}