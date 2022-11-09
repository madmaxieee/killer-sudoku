#include <iostream>
#include "KillerSudoku.h"

int main()
{
    KillerSudoku ks(3);
    ks.generateNewSolution();
    ks.printSolution();
    ks.generateCages();
    ks.printCages();
    return 0;
}
