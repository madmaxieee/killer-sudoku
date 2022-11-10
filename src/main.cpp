#include <iostream>
#include "KillerSudoku.h"
#include "KSSolver.h"

int main()
{
    KillerSudoku ks(3);
    ks.generateNewSolution();
    ks.printSolution();
    ks.generateCages();
    ks.printCages();

    KSSolver solver(3);
    solver.createConstraints(ks.getCages(), ks.getBoxes());
    // solver.printClauses();
    solver.solve();
    solver.reportResult();

    return 0;
}
