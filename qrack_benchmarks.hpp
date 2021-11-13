//////////////////////////////////////////////////////////////////////////////////////
//
// (C) Daniel Strano and the Qrack contributors 2017-2019. All rights reserved.
//

#pragma once

#include <iostream>

#include "qrack/qfactory.hpp"

const int MAX_QUBITS = 16;
const int ITERATIONS = 100;
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
    bool hadamardRandomBits = false, bool logNormal = false, bool randQubits = false)
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
                if (logNormal) {
                    trialClocks[i] = log2(tClock * CLOCK_FACTOR);
                } else {
                    trialClocks[i] = tClock * CLOCK_FACTOR;
                }
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
            std::cout << formatTime(avgt, logNormal) << ","; /* Average Time (ms) */
            std::cout << formatTime(stdet, logNormal) << ","; /* Sample Std. Deviation (ms) */
            std::cout << formatTime(trialClocks[0], logNormal) << ","; /* Fastest (ms) */
            if (ITERATIONS % 4 == 0) {
                std::cout << formatTime((trialClocks[ITERATIONS / 4 - 1] + trialClocks[ITERATIONS / 4]) / 2, logNormal)
                          << ","; /* 1st Quartile (ms) */
            } else {
                std::cout << formatTime(trialClocks[ITERATIONS / 4 - 1] / 2, logNormal) << ","; /* 1st Quartile (ms) */
            }
            if (ITERATIONS % 2 == 0) {
                std::cout << formatTime((trialClocks[ITERATIONS / 2 - 1] + trialClocks[ITERATIONS / 2]) / 2, logNormal)
                          << ","; /* Median (ms) */
            } else {
                std::cout << formatTime(trialClocks[ITERATIONS / 2 - 1] / 2, logNormal) << ","; /* Median (ms) */
            }
            if (ITERATIONS % 4 == 0) {
                std::cout << formatTime(
                                 (trialClocks[(3 * ITERATIONS) / 4 - 1] + trialClocks[(3 * ITERATIONS) / 4]) / 2, logNormal)
                          << ","; /* 3rd Quartile (ms) */
            } else {
                std::cout << formatTime(trialClocks[(3 * ITERATIONS) / 4 - 1] / 2, logNormal)
                          << ","; /* 3rd Quartile (ms) */
            }
            std::cout << formatTime(trialClocks[ITERATIONS - 1], logNormal) << std::endl; /* Slowest (ms) */
        }
    }
    
    std::cout << std::endl;
}

void benchmarkLoop(std::function<void(Qrack::QInterfacePtr, int, int)> fn, int minDepth = 1, int maxDepth = 1, bool resetRandomPerm = true,
    bool hadamardRandomBits = false, bool logNormal = false, bool randQubits = false)
{
    benchmarkLoopVariable(fn, MAX_QUBITS, minDepth, maxDepth, resetRandomPerm, hadamardRandomBits, logNormal, randQubits);
}
