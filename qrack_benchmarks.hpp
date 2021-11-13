//////////////////////////////////////////////////////////////////////////////////////
//
// (C) Daniel Strano and the Qrack contributors 2017-2019. All rights reserved.
//

#pragma once

#include <iostream>

#include "qrack/qfactory.hpp"

const int MAX_QUBITS = 8;
const int ITERATIONS = 10;
const double CLOCK_FACTOR = 1000.0 / CLOCKS_PER_SEC; // Report in ms

double formatTime(double t, bool logNormal)
{
    if (logNormal) {
        return pow(2.0, t);
    } else {
        return t;
    }
}

Qrack::QInterfacePtr MakeRandQubit()
{
    Qrack::QInterfacePtr qubit = Qrack::CreateQuantumInterface(Qrack::QINTERFACE_OPTIMAL, 1U, 0);

    Qrack::real1 theta = 4 * M_PI * qubit->Rand();
    Qrack::real1 phi = 2 * M_PI * qubit->Rand();
    Qrack::real1 lambda = 2 * M_PI * qubit->Rand();

    qubit->U(0, theta, phi, lambda);

    return qubit;
}

void benchmarkLoopVariable(std::function<void(Qrack::QInterfacePtr, int, int)> fn, bitLenInt mxQbts, int minDepth = 1, int maxDepth = 1, bool resetRandomPerm = true,
    bool hadamardRandomBits = false, bool randQubits = false)
{
    Qrack::QInterfacePtr qftReg;

    std::cout << ITERATIONS << " iterations" << std::endl;
    std::cout << "# of Qubits, ";
    std::cout << "Depth, ";
    std::cout << "Average Time (ms), ";
    std::cout << "Sample Std. Deviation (ms), ";
    std::cout << "Fastest (ms), ";
    std::cout << "1st Quartile (ms), ";
    std::cout << "Median (ms), ";
    std::cout << "3rd Quartile (ms), ";
    std::cout << "Slowest (ms)," << std::endl;

    clock_t tClock, iterClock;
    Qrack::real1 trialClocks[ITERATIONS];

    bitLenInt i, j, numBits, depth;

    double avgt, stdet;

    for (numBits = 4; numBits <= MAX_QUBITS; numBits++) {
        qftReg = Qrack::CreateQuantumInterface(Qrack::QINTERFACE_OPTIMAL, numBits, 0);
        for (depth = minDepth; depth <= maxDepth; depth++) {
            avgt = 0.0;

            for (i = 0; i < ITERATIONS; i++) {

                if (randQubits) {
                    for (bitLenInt b = 0; b < numBits; b++) {
                        if (b == 0) {
                            qftReg = MakeRandQubit();
                        } else {
                            qftReg->Compose(MakeRandQubit());
                        }
                    }
                } else if (resetRandomPerm) {
                    qftReg->SetPermutation(qftReg->Rand() * qftReg->GetMaxQPower());
                } else {
                    qftReg->SetPermutation(0);
                }

                if (hadamardRandomBits) {
                    for (j = 0; j < numBits; j++) {
                        if (qftReg->Rand() >= ONE_R1 / 2) {
                            qftReg->H(j);
                        }
                    }
                }

                qftReg->Finish();

                iterClock = clock();

                // Run loop body
                fn(qftReg, numBits, depth);

                qftReg->Finish();

                // Collect interval data
                tClock = clock() - iterClock;
                trialClocks[i] = tClock * CLOCK_FACTOR;
                avgt += trialClocks[i];
            }
            avgt /= ITERATIONS;

            stdet = 0.0;
            for (i = 0; i < ITERATIONS; i++) {
                stdet += (trialClocks[i] - avgt) * (trialClocks[i] - avgt);
            }
            stdet = sqrt(stdet / ITERATIONS);

            std::sort(trialClocks, trialClocks + ITERATIONS);

            std::cout << (int)numBits << ", "; /* # of Qubits */
            std::cout << (int)depth << ", "; /* Depth */
            std::cout << avgt << ","; /* Average Time (ms) */
            std::cout << stdet << ","; /* Sample Std. Deviation (ms) */
            std::cout << trialClocks[0] << ","; /* Fastest (ms) */
            if (ITERATIONS % 4 == 0) {
                std::cout << ((trialClocks[ITERATIONS / 4 - 1] + trialClocks[ITERATIONS / 4]) / 2)
                          << ","; /* 1st Quartile (ms) */
            } else {
                std::cout << (trialClocks[ITERATIONS / 4 - 1] / 2) << ","; /* 1st Quartile (ms) */
            }
            if (ITERATIONS % 2 == 0) {
                std::cout << ((trialClocks[ITERATIONS / 2 - 1] + trialClocks[ITERATIONS / 2]) / 2)
                          << ","; /* Median (ms) */
            } else {
                std::cout << (trialClocks[ITERATIONS / 2 - 1] / 2) << ","; /* Median (ms) */
            }
            if (ITERATIONS % 4 == 0) {
                std::cout << ((trialClocks[(3 * ITERATIONS) / 4 - 1] + trialClocks[(3 * ITERATIONS) / 4]) / 2)
                          << ","; /* 3rd Quartile (ms) */
            } else {
                std::cout << (trialClocks[(3 * ITERATIONS) / 4 - 1] / 2)
                          << ","; /* 3rd Quartile (ms) */
            }
            std::cout << trialClocks[ITERATIONS - 1] << std::endl; /* Slowest (ms) */
        }
    }
    
    std::cout << std::endl;
}

void benchmarkLoop(std::function<void(Qrack::QInterfacePtr, int, int)> fn, int minDepth = 1, int maxDepth = 1, bool resetRandomPerm = true,
    bool hadamardRandomBits = false, bool randQubits = false)
{
    benchmarkLoopVariable(fn, MAX_QUBITS, minDepth, maxDepth, resetRandomPerm, hadamardRandomBits, randQubits);
}
