#include <cstdlib>
#include "Table.h"

using namespace std;

int main(int argc, char** argv) {
    Table table("puzzle5.txt");
    table.print();
    table.solve();
    table.print();
    return 0;
}

