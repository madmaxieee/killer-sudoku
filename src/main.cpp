#include <iostream>
#include "KillerSudoku.h"
#include "KSSolver.h"

int main()
{
    int size = 3;
    KillerSudoku ks(size);
    ks.generateNewSolution();
    ks.printSolution();
    ks.generateCages();
    ks.printCages();

    KSSolver solver(size);
    solver.createConstraints(ks.getCages(), ks.getBoxes());
    solver.printStats();
    solver.solve();
    solver.reportResult();

    return 0;
}
