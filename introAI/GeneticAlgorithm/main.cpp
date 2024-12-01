#include "normal.hpp"

int main(int argc, const char *argv[]) {
    int nInitPopulation = atoi(argv[1]);
    int nBit = atoi(argv[2]);
    int nPopulation = atoi(argv[3]);
    int cr = atoi(argv[4]);
    int mr = atoi(argv[5]);
    int max_iter = atoi(argv[6]);

    GA ga;
    ga.run(nInitPopulation, nBit, nPopulation, cr, mr, max_iter);
}
