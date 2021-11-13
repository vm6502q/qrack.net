//////////////////////////////////////////////////////////////////////////////////////
//
// (C) Daniel Strano and the Qrack contributors 2017-2019. All rights reserved.
//

#include "qrack_benchmarks.hpp"

using namespace Qrack;

bitLenInt pickRandomBit(QInterfacePtr qReg, std::set<bitLenInt>* unusedBitsPtr)
{
    std::set<bitLenInt>::iterator bitIterator = unusedBitsPtr->begin();
    bitLenInt bitRand = unusedBitsPtr->size() * qReg->Rand();
    if (bitRand >= unusedBitsPtr->size()) {
        bitRand = unusedBitsPtr->size() - 1U;
    }
    std::advance(bitIterator, bitRand);
    bitRand = *bitIterator;
    unusedBitsPtr->erase(bitIterator);
    return bitRand;
}

int main()
{

    // Random permutation basis eigenstate initialization
    std::cout<<">>>QFT, Random Permutation Basis Eigenstate Initialization:"<<std::endl;
    benchmarkLoop(
        [](QInterfacePtr qftReg, int n, int unused) {
            qftReg->QFT(0, n);
            qftReg->MAll();
        }, QINTERFACE_OPTIMAL, 1, 1, true, false, false);

    // Totally random, totally separable qubits, for initialization
    /*std::cout<<">>>Random Separable Bits:"<<std::endl;
    benchmarkLoop(
        [](Qrack::QInterfacePtr qftReg, int n, int unused) {
            qftReg->QFT(0, n);
            qftReg->MAll();
        }, 1, 1, false, false, true);*/
    
    const int GateCount1Qb = 4;
    const int GateCountMultiQb = 3;

    std::cout<<">>>Random stabilizer circuits:"<<std::endl;
    benchmarkLoop(
        [&](QInterfacePtr qReg, int n, int depth) {
            int d;
            bitLenInt i;
            real1_f gateRand;
            bitLenInt b1, b2;

            for (d = 0; d < depth; d++) {

                for (i = 0; i < n; i++) {
                    gateRand = qReg->Rand();
                    if (gateRand < (ONE_R1 / GateCount1Qb)) {
                        qReg->H(i);
                    } else if (gateRand < (2 * ONE_R1 / GateCount1Qb)) {
                        qReg->X(i);
                    } else if (gateRand < (3 * ONE_R1 / GateCount1Qb)) {
                        qReg->Y(i);
                    } else {
                        qReg->S(i);
                    }
                }

                std::set<bitLenInt> unusedBits;
                for (i = 0; i < n; i++) {
                    // In the past, "qReg->TrySeparate(i)" was also used, here, to attempt optimization. Be aware that
                    // the method can give performance advantages, under opportune conditions, but it does not, here.
                    unusedBits.insert(unusedBits.end(), i);
                }

                while (unusedBits.size() > 1) {
                    b1 = pickRandomBit(qReg, &unusedBits);
                    b2 = pickRandomBit(qReg, &unusedBits);

                    gateRand = GateCountMultiQb * qReg->Rand();

                    if (gateRand < ONE_R1) {
                        qReg->CNOT(b1, b2);
                    } else if (gateRand < (2 * ONE_R1)) {
                        qReg->CY(b1, b2);
                    } else {
                        qReg->CZ(b1, b2);
                    }
                }
            }

            qReg->MAll();
        }, QINTERFACE_STABILIZER_HYBRID, 10, 10, true, false, false);
}
