#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;
typedef vector<char> c1d;
typedef vector<c1d> c2d;
typedef vector<c2d> c3d;
typedef vector<c3d> c4d;
typedef vector<int> i1d;
typedef vector<i1d> i2d;
typedef vector<i2d> i3d;
typedef vector<i3d> i4d;
typedef vector<double> d1d;
typedef vector<d1d> d2d;
typedef vector<d2d> d3d;
typedef vector<d3d> d4d;

class GA {
   public:
    i1d Run_result;

   public:
    void run(int nInitPopulation, int nBit, int nPopulation, int cr, int mr, int max_iter) {
        srand((unsigned)time(NULL));
        double START, END;

        START = clock();
        InitPopulation(nInitPopulation, nBit);
        for (int i = 0; i < max_iter; i++) {
            i2d newPopSet;
            Crossover(cr, newPopSet);
            Mutation(mr, newPopSet);
            Fitness(newPopSet);
            Selection(nPopulation, nBit, newPopSet);
            // Record the best fitness value of each iteration
            Run_result.push_back(fitness[0]);
        }
        END = clock();
        Output(max_iter, nInitPopulation, nBit, nPopulation, cr, mr, START, END);
    }

   private:
    i2d popSet;
    i1d fitness;

   private:
    inline int randomInt(int min, int max) {
        // [min, max]
        return (rand() % (max - min + 1) + min);
    }
    void InitPopulation(int nInitPopulation, int nBit) {
        for (int i = 0; i < nInitPopulation; i++) {
            i1d binNum;
            for (int j = 0; j < nBit; j++) {
                binNum.push_back(randomInt(0, 1));
            }
            popSet.push_back(binNum);
        }
    }
    void Crossover(int cr, i2d &newPopSet) {
        newPopSet = popSet;
        for (int i = 0; i < popSet.size(); i++) {
            for (int j = i + 1; j < popSet.size(); j++) {
                Crossover(cr, popSet[i], popSet[j], newPopSet[i], newPopSet[j]);
            }
        }
    }
    // Single-point Crossover
    void Crossover(int cr, i1d &chromesome1, i1d &chromesome2, i1d &newChromosome1, i1d &newChromosome2) {
        newChromosome1 = chromesome1;
        newChromosome2 = chromesome2;
        if (randomInt(0, 100) < cr) {
            int change_idx = randomInt(0, newChromosome1.size() - 1);
            for (int i = 0; i < newChromosome1.size(); i++) {
                if (i <= change_idx) {
                    swap(newChromosome1[i], newChromosome2[i]);
                } else
                    break;
            }
        }
    }

    void Mutation(int mr, i2d &newPopSet) {
        for (int i = 0; i < newPopSet.size(); i++) {
            Mutation(mr, newPopSet[i], newPopSet[i]);
        }
    }
    void Mutation(int mr, i1d &chromesome, i1d &newChromosome) {
        newChromosome = chromesome;
        if (randomInt(0, 100) < mr) {
            int change_idx = randomInt(0, chromesome.size() - 1);
            newChromosome[change_idx] = !chromesome[change_idx];
        }
    }

    void Fitness(i2d &newPopSet) {
        fitness.clear();
        for (int i = 0; i < newPopSet.size(); i++) {
            fitness.push_back(Fitness(newPopSet[i]));
        }
    }
    int Fitness(i1d &chromesome) {
        int sum = 0;
        for (int i = 0; i < chromesome.size(); i++) {
            if (i == chromesome.size() - 1)
                sum += chromesome[i];
            else
                sum += 1 << chromesome[i] * (chromesome.size() - i - 1);
        }
        return sum;
    }

    void Selection(int nPopulation, int nBit, i2d &newPopSet) {
        // Elitism Selection
        sort(newPopSet.begin(), newPopSet.end(), [&](i1d &a, i1d &b) { return Fitness(a) < Fitness(b); });
        popSet.assign(newPopSet.begin(), newPopSet.begin() + nPopulation);
    }

    void Output(int iteration, int nInitPopulation, int nBit, int nPopulation, int cr, int mr, double START, double END) {
        int runBest = 0;
        for (int i = 0; i < Run_result.size(); i++) {
            cout << "# Run Value : " << Run_result[i] << endl;

            if (Run_result[i] > runBest)
                runBest = Run_result[i];
        }

        cout << "# Iteration : " << iteration << endl;
        cout << "# Number of init population : " << nInitPopulation << endl;
        cout << "# Number of bit : " << nBit << endl;
        cout << "# Number of population : " << nPopulation << endl;
        cout << "# Crossover Rate : " << cr << endl;
        cout << "# Mutation Rate : " << mr << endl;
        cout << "# Best Objective : " << runBest << endl;
        cout << "# Execution Time : " << (END - START) / CLOCKS_PER_SEC << "(s)" << endl;
    }
};